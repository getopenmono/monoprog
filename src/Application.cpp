#include "Application.h"
#include "cybtldr_utils.h"
#include "HidDevice.h"
#include "IDeviceCommunicator.h"
#include "InBootloaderMockDevice.h"
#include "IProgrammer.h"
#include "NotConnectedMockDevice.h"
#include "OutputCollector.h"
#include "ProgrammerFactory.h"
#include "ResponsiveAppMockDevice.h"
#include "UnresponsiveAppMockDevice.h"
#include <memory>
#include <QCommandLineParser>
#include <QFile>
#include <QFileInfo>

#define OUTPUT(level) OUTPUTCOLLECTOR_LINE((*output),level)

namespace {

enum OperationMode
{
	Unknown,
	UnknownMockDevice,
	Version,
	Detect,
	License,
	Program
};

enum DeviceType
{
	// The default: A real Mono.
	RealDevice,
	// Emulate different kinds of devices in various states.
	MockNotConnected,
	MockDevBoard,
	MockMonoBoard,
	MockResponsiveApp,
	MockUnresponsiveApp
};

enum SiliconId
{
	DevBoard = 0x2e123069,
	MonoBoard = 0x2e16a069
};

} // namespace

class Arguments
{
public:
	static QStringList deviceType;
	Arguments (QCoreApplication const & qtApp, OutputCollector & output)
	{
		setupStandardOptions();
		setupVersionOptions();
		setupLicenseOptions();
		setupDetectOptions();
		setupDeviceSimulationOptions();
		setupProgramOptions();
		setupDebugOptions();
		setupSilentOptions();
		parser.process(qtApp);
		decideOperationModeFromArguments();
		decideDeviceType(output);
	}
	~Arguments ()
	{
		delete simulatedDeviceType;
		delete detectOption;
	}
	OperationMode getOperationMode () const
	{
		return mode;
	}
	DeviceType getDevice () const
	{
		return device;
	}
	QString const & getAppPath () const
	{
		return programPath;
	}
	int getVerbosity () const
	{
		if (parser.isSet(*silentOption)) return 0;
		else return parser.value(*debugOption).toInt();
	}
	QCommandLineParser & getParser ()
	{
		return parser;
	}
private:
	void setupStandardOptions ()
	{
		parser.setApplicationDescription
		(
			"Bootloadable Programmer for Mono board."
		);
		parser.addHelpOption();
	}
	void setupVersionOptions ()
	{
		versionOption = new QCommandLineOption
		(
			QStringList() << "V" << "version",
			"Displays version information."
		);
		parser.addOption(*versionOption);
	}

	void setupDeviceSimulationOptions ()
	{
		simulatedDeviceType = new QCommandLineOption
		(
			"mock",
			"Simulates device to be of <type>.",
			"type"
		);
		parser.addOption(*simulatedDeviceType);
	}
	void setupLicenseOptions ()
	{
		licenseOption = new QCommandLineOption
		(
			QStringList() << "license",
			"Displays licenses of software components."
		);
		parser.addOption(*licenseOption);
	}
	void setupDetectOptions ()
	{
		detectOption = new QCommandLineOption
		(
			QStringList() << "d" << "detect",
			"Detects whether a Mono is connected via USB."
		);
		parser.addOption(*detectOption);
	}
	void setupProgramOptions()
	{
		programOption = new QCommandLineOption
		(
			QStringList() << "p" << "program",
			"Transfers <app> to a connected Mono.",
			"app"
		);
		parser.addOption(*programOption);
	}
	void setupDebugOptions()
	{
		debugOption = new QCommandLineOption
		(
			QStringList() << "v" << "verbose",
			"Set output verbosity level (default is 1).",
			"level",
			"1"
		);
		parser.addOption(*debugOption);
	}
	void setupSilentOptions()
	{
		silentOption = new QCommandLineOption
		(
			QStringList() << "q" << "quiet" << "silent",
			"Set output verbosity level to 0."
		);
		parser.addOption(*silentOption);
	}
	void decideDeviceType (OutputCollector & output)
	{
		QString type = parser.value(*simulatedDeviceType);
		if (type.size() == 0) device = RealDevice;
		else if (type == deviceType.at(0)) device = MockNotConnected;
		else if (type == deviceType.at(1)) device = MockDevBoard;
		else if (type == deviceType.at(2)) device = MockMonoBoard;
		else if (type == deviceType.at(3)) device = MockResponsiveApp;
		else if (type == deviceType.at(4)) device = MockUnresponsiveApp;
		else
		{
			device = MockNotConnected;
			mode = UnknownMockDevice;
			output.error() << "Unknown mock device type requested: "
				<< type.toStdString();
			QString mock;
			foreach (mock, deviceType)
			{
				output.error() << " '" << mock.toStdString()
					<< "' is a legal mock device.";
			}
			output.error();
		}
	}
	void decideOperationModeFromArguments ()
	{
		if (parser.isSet(*licenseOption)) mode = License;
		else if (parser.isSet(*versionOption)) mode = Version;
		else if (parser.isSet(*detectOption)) mode = Detect;
		else if (parser.isSet(*programOption))
		{
			mode = Program;
			programPath = parser.value(*programOption);
		}
		else mode = Unknown;
	}
	QCommandLineParser parser;
	QCommandLineOption * versionOption;
	QCommandLineOption * simulatedDeviceType;
	QCommandLineOption * licenseOption;
	QCommandLineOption * detectOption;
	QCommandLineOption * programOption;
	QCommandLineOption * debugOption;
	QCommandLineOption * silentOption;
	OperationMode mode;
	DeviceType device;
	QString programPath;
};

QStringList Arguments::deviceType = QStringList()
	<< "disconnected" // 0
	<< "devboard" // 1
	<< "monoboard" // 2
	<< "responsive" // 3
	<< "unresponsive" // 4
	;

Application::Application (QCoreApplication * qtApp_, std::ostream & out, std::ostream & error)
: qtApp(qtApp_)
{
	setupApplicationConstants();
	output = new OutputCollector(out,error);
	arguments = new Arguments(*qtApp,*output);
	output->setVerbosity(arguments->getVerbosity());
}

Application::~Application ()
{
	delete arguments;
	delete output;
}

void Application::setupApplicationConstants ()
{
	QCoreApplication::setApplicationName("monoprog");
	QCoreApplication::setOrganizationName("Monolit ApS");
	QCoreApplication::setApplicationVersion("0.8.0");
}

IDeviceCommunicator * Application::createDeviceCommunication () const
{
	switch (arguments->getDevice())
	{
		case RealDevice:
			return new HidDevice(*output);
		case MockNotConnected:
			return new NotConnectedMockDevice(*output);
		case MockDevBoard:
			return new InBootloaderMockDevice(*output,DevBoard);
		case MockMonoBoard:
			return new InBootloaderMockDevice(*output,MonoBoard);
		case MockResponsiveApp:
			return new ResponsiveAppMockDevice(*output,MonoBoard);
		case MockUnresponsiveApp:
			return new UnresponsiveAppMockDevice(*output);
		default:
			throw "Unknown device";
			return 0;
	}
}

StatusCode Application::run ()
{
	switch (arguments->getOperationMode())
	{
		case License:
			return displayLicenses();
		case Version:
			return displayVersion();
		case Detect:
			return detectDevice();
		case Program:
		 	return programDevice(arguments->getAppPath());
		case Unknown:
		case UnknownMockDevice:
			arguments->getParser().showHelp(Usage);
		default:
			return UnknownError;
	}
}

StatusCode Application::displayLicenses ()
{
	QStringList licenses = QStringList()
		<< "LICENSE.txt"
		<< "cybootloaderutils/LICENSE.txt"
		<< "hidapi/LICENSE-bsd.txt"
		<< "elfio/COPYING"
		;
	QString license;
	foreach (license, licenses)
	{
		QFile textFile(":/"+license);
		if (!textFile.open(QIODevice::ReadOnly|QIODevice::Text)) return UnknownError;
		OUTPUT(0) << "----[" << license.toStdString() << "]----" << std::endl;
		OUTPUT(0) << QString(textFile.readAll()).toStdString();
	}
	return Success;
}

StatusCode Application::displayVersion ()
{
	OUTPUT(0) << "monoprog 0.8.0";
	return Success;
}

bool Application::fileExists (QFileInfo const & file)
{
	if (!file.exists())
	{
		output->error() << "File does not exist: " << file.filePath().toStdString();
		return false;
	}
	return true;
}

StatusCode Application::programDevice (QString const & appPath)
{
	QFileInfo file(appPath);
	if (!fileExists(file)) return FileDoesNotExist;
	std::unique_ptr<IDeviceCommunicator> psocDevice(createDeviceCommunication());
	// TODO:
	if (SerialDetected == psocDevice->serialOpen())
	{
		OUTPUT(1) << "Resetting Mono device to go to bootloader";
		if (SerialResetSent != psocDevice->serialSendReset())
		{
			return CouldNotResetMono;
		}
		// TODO: wait for bootloader?
	}
	IProgrammer * programmer = ProgrammerFactory::createProgrammer(file,psocDevice.release());
	if (!programmer)
	{
		output->error() << "Unknown program type: " << file.suffix().toStdString();
		return ProgramTypeNotRecognised;
	}
	return programDeviceInBootloader(appPath,programmer);
}

StatusCode Application::programDeviceInBootloader (QString const & appPath, IProgrammer * programmer_)
{
	std::unique_ptr<IProgrammer> programmer(programmer_);
	programmer->setOutput(output);
	std::string const filePath = appPath.toStdString();
	char const * path = filePath.c_str();
	OUTPUT(1) << "Programming Mono device with " << path;
	switch (programmer->program())
	{
		case ProgrammerSuccess:
			return Success;
		case ProgrammerCorruptProgram:
			output->error() << "Corrupt program " << appPath.toStdString();
			return ProgrammingDeviceFailed;
		case ProgrammerUnsupportedMetaData:
			output->error() << "Metadata section too large in  " << appPath.toStdString();
			return ProgrammingDeviceFailed;
		case ProgrammerNoConnectionToMonoDevice:
			output->error() << "No connection to Mono device.";
			return NoConnectionToMonoDevice;
		case ProgrammerWrongSiliconId:
			output->error() << "Silicon ID for device does not match program.";
			return WrongSiliconId;
		default:
			output->error() << "Programming failed for unknown reason.";
			return ProgrammingDeviceFailed;
	}
}

StatusCode Application::detectDevice ()
{
	std::unique_ptr<IDeviceCommunicator> psocDevice(createDeviceCommunication());
	if (SerialDetected == psocDevice->serialOpen())
	{
		OUTPUT(0) << "Mono device running app detected.";
		return Success;
	}
	if (CYRET_SUCCESS == psocDevice->openConnection())
	{
		OUTPUT(0) << "Mono device in bootloader detected.";
		psocDevice->closeConnection();
		return Success;
	}
	return NoConnectionToMonoDevice;
}
