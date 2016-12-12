#include "Application.h"
#include "cybtldr_utils.h"
#include "HeadlessProgressUpdater.h"
#include "HidDevice.h"
#include "HumanProgressUpdater.h"
#include "IDeviceCommunicator.h"
#include "InBootloaderMockDevice.h"
#include "IProgrammer.h"
#include "IProgressUpdater.h"
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

namespace monolit {

enum OperationMode
{
	Unknown,
	UnknownMockDevice,
	Version,
	Detect,
	Info,
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
		setupTimeoutOptions();
		setupHeadlessOptions();
		setupInfoOptions();
		parser.process(qtApp);
		decideOperationModeFromArguments();
		decideDeviceType(output);
	}
	~Arguments ()
	{
		delete simulatedDeviceType;
		delete detectOption;
	}
	monolit::OperationMode getOperationMode () const
	{
		return mode;
	}
	monolit::DeviceType getDevice () const
	{
		return device;
	}
	QString const & getAppPath () const
	{
		return programPath;
	}
	int getVerbosity () const
	{
		if (parser.isSet(*silentOption) || parser.isSet(*headlessOption)) return 0;
		else return parser.value(*debugOption).toInt();
	}
	int getMsTimeout () const
	{
		return parser.value(*timeoutOption).toInt();
	}
	bool getHeadless () const
	{
		return parser.isSet(*headlessOption);
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
	void setupProgramOptions ()
	{
		programOption = new QCommandLineOption
		(
			QStringList() << "p" << "program",
			"Transfers <app> to a connected Mono.",
			"app"
		);
		parser.addOption(*programOption);
	}
	void setupDebugOptions ()
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
	void setupSilentOptions ()
	{
		silentOption = new QCommandLineOption
		(
			QStringList() << "q" << "quiet" << "silent",
			"Set output verbosity level to 0."
		);
		parser.addOption(*silentOption);
	}
	void setupTimeoutOptions ()
	{
		timeoutOption = new QCommandLineOption
		(
			QStringList() << "t" << "timeout",
			"Set device detection timeout (default is 500 ms).",
			"ms",
			"500"
		);
		parser.addOption(*timeoutOption);
	}
	void setupHeadlessOptions ()
	{
		headlessOption = new QCommandLineOption
		(
			QStringList() << "H" << "headless",
			"Make output more friendly for other programs."
		);
		parser.addOption(*headlessOption);
	}
	void setupInfoOptions ()
	{
		infoOption = new QCommandLineOption
		(
			QStringList() << "i" << "info",
			"Display meta information from <app>.",
			"app"
		);
		parser.addOption(*infoOption);
	}
	void decideDeviceType (OutputCollector & output)
	{
		QString type = parser.value(*simulatedDeviceType);
		if (type.size() == 0) device = monolit::RealDevice;
		else if (type == deviceType.at(0)) device = monolit::MockNotConnected;
		else if (type == deviceType.at(1)) device = monolit::MockDevBoard;
		else if (type == deviceType.at(2)) device = monolit::MockMonoBoard;
		else if (type == deviceType.at(3)) device = monolit::MockResponsiveApp;
		else if (type == deviceType.at(4)) device = monolit::MockUnresponsiveApp;
		else
		{
			device = monolit::MockNotConnected;
			mode = monolit::UnknownMockDevice;
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
		if (parser.isSet(*licenseOption)) mode = monolit::License;
		else if (parser.isSet(*versionOption)) mode = monolit::Version;
		else if (parser.isSet(*detectOption)) mode = monolit::Detect;
		else if (parser.isSet(*infoOption))
		{
			mode = monolit::Info;
			programPath = parser.value(*infoOption);
		}
		else if (parser.isSet(*programOption))
		{
			mode = monolit::Program;
			programPath = parser.value(*programOption);
		}
		else mode = monolit::Unknown;
	}
	QCommandLineParser parser;
	QCommandLineOption * versionOption;
	QCommandLineOption * simulatedDeviceType;
	QCommandLineOption * licenseOption;
	QCommandLineOption * detectOption;
	QCommandLineOption * programOption;
	QCommandLineOption * debugOption;
	QCommandLineOption * silentOption;
	QCommandLineOption * timeoutOption;
	QCommandLineOption * headlessOption;
	QCommandLineOption * infoOption;
	monolit::OperationMode mode;
	monolit::DeviceType device;
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
	QCoreApplication::setApplicationVersion("0.9.3");
}

IDeviceCommunicator * Application::createDeviceCommunication () const
{
	switch (arguments->getDevice())
	{
		case monolit::RealDevice:
			return new HidDevice(*output);
		case monolit::MockNotConnected:
			return new NotConnectedMockDevice(*output);
		case monolit::MockDevBoard:
			return new InBootloaderMockDevice(*output,monolit::DevBoard);
		case monolit::MockMonoBoard:
			return new InBootloaderMockDevice(*output,monolit::MonoBoard);
		case monolit::MockResponsiveApp:
			return new ResponsiveAppMockDevice(*output,monolit::MonoBoard);
		case monolit::MockUnresponsiveApp:
			return new UnresponsiveAppMockDevice(*output);
	}
}

StatusCode Application::run ()
{
	switch (arguments->getOperationMode())
	{
		case monolit::License:
			return displayLicenses();
		case monolit::Version:
			return displayVersion();
		case monolit::Detect:
			return detectDevice();
		case monolit::Info:
			return displayInfo(arguments->getAppPath());
		case monolit::Program:
		 	return programDevice(arguments->getAppPath());
		case monolit::Unknown:
		case monolit::UnknownMockDevice:
			arguments->getParser().showHelp(Usage);
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
	OUTPUT(0)
		<< QCoreApplication::applicationName().toStdString()
		<< " "
		<< QCoreApplication::applicationVersion().toStdString();
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

IProgressUpdater * Application::createProgressUpdater () const
{
	if (arguments->getHeadless())
	{
		return new HeadlessProgressUpdater(*output);
	}
	else
	{
		return new HumanProgressUpdater(*output);
	}
}

StatusCode Application::displayInfo (QString const & appPath)
{
	QFileInfo file(appPath);
	if (!fileExists(file)) return FileDoesNotExist;
	// TODO: ATM using a null device, but ProgrammerBase should
	// be detached from the actual program so we can get meta info from the
	// program without having a device.
	IProgrammer * programmer = ProgrammerFactory::createProgrammer(file,0,0,0);
	if (!programmer)
	{
		output->error() << "Unknown program type: " << file.suffix().toStdString();
		return ProgramTypeNotRecognised;
	}
	programmer->setOutput(output);
	size_t flashBytes = programmer->getProgramSize();
	if (flashBytes == 0)
	{
		output->error() << "File type not supported: " << file.suffix().toStdString();
		return UnsupportedFileType;
	}
	if (arguments->getHeadless())
	{
		OUTPUT(0) << flashBytes;
	}
	else
	{
		OUTPUT(0) << "Flash bytes used: " << flashBytes;
	}
	return Success;
}

StatusCode Application::programDevice (QString const & appPath)
{
	QFileInfo file(appPath);
	if (!fileExists(file)) return FileDoesNotExist;
	std::unique_ptr<IDeviceCommunicator> psocDevice(createDeviceCommunication());
	if (SerialDetected == psocDevice->serialOpen())
	{
		OUTPUT(1) << "Resetting Mono device to start bootloader";
		if (SerialResetSent != psocDevice->serialSendReset())
		{
			return CouldNotResetMono;
		}
	}
	IProgressUpdater * updater = createProgressUpdater();
	IProgrammer * programmer = ProgrammerFactory::createProgrammer
	(
		file,
		psocDevice.release(),
		updater,
		arguments->getMsTimeout()
	);
	if (!programmer)
	{
		output->error() << "Unknown program type: " << file.suffix().toStdString();
		return ProgramTypeNotRecognised;
	}
	programmer->setOutput(output);
	size_t flashBytes = programmer->getProgramSize();
	updater->setAppSize(flashBytes);
	return programDeviceInBootloader(appPath,programmer);
}

StatusCode Application::programDeviceInBootloader (QString const & appPath, IProgrammer * programmer_)
{
	std::unique_ptr<IProgrammer> programmer(programmer_);
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
		if (arguments->getHeadless())
		{
			OUTPUT(0) << "app";
		}
		else
		{
			OUTPUT(0) << "Mono device running app detected.";
		}
		return Success;
	}
	if (CYRET_SUCCESS == psocDevice->openConnection(arguments->getMsTimeout()))
	{
		if (arguments->getHeadless())
		{
			OUTPUT(0) << "bootloader";
		}
		else
		{
			OUTPUT(0) << "Mono device in bootloader detected.";
		}
		psocDevice->closeConnection();
		return Success;
	}
	return NoConnectionToMonoDevice;
}
