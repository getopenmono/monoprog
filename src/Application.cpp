#include "Application.h"
#include "IDeviceCommunicator.h"
#include "HidDevice.h"
#include "IProgrammer.h"
#include "ProgrammerFactory.h"
#include "NotConnectedMockDevice.h"
#include "ConnectedMockDevice.h"
#include "OutputCollector.h"
#include "cybtldr_utils.h"
#include <QCommandLineParser>
#include <QFile>
#include <QFileInfo>
#include <memory>

#define OUTPUT(level) OUTPUTCOLLECTOR_LINE((*output),level)
#define PROGRESS(level) OUTPUTCOLLECTOR_PROGRESS((*output),level)

namespace {

enum OperationMode
{
	Unknown,
	UnknownMockDevice,
	Detect,
	License,
	Program
};

enum DeviceType
{
	RealDevice,
	MockNotConnected,
	MockDevBoard,
	MockMonoBoard
};

} // namespace

class Arguments
{
public:
	static QStringList deviceType;
	Arguments (QCoreApplication const & qtApp, OutputCollector & output)
	{
		setupStandardOptions();
		setupLicenseOptions();
		setupDetectOptions();
		setupDeviceSimulationOptions();
		setupProgramOptions();
		setupDebugOptions();
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
		return parser.value(*debugOption).toInt();
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
		parser.addVersionOption();
	}
	void setupDeviceSimulationOptions ()
	{
		simulatedDeviceType = new QCommandLineOption
		(
			"mock",
			"Simulates device to be in <type>.",
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
			"Transfers <app> to Mono.",
			"app"
		);
		parser.addOption(*programOption);
	}
	void setupDebugOptions()
	{
		debugOption = new QCommandLineOption
		(
			QStringList() << "verbose",
			"Set output verbosity level (default is 0).",
			"level",
			"0"
		);
		parser.addOption(*debugOption);
	}
	void decideDeviceType (OutputCollector & output)
	{
		QString type = parser.value(*simulatedDeviceType);
		if (type.size() == 0) device = RealDevice;
		else if (type == deviceType.at(0)) device = MockNotConnected;
		else if (type == deviceType.at(1)) device = MockDevBoard;
		else if (type == deviceType.at(2)) device = MockMonoBoard;
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
		else if (parser.isSet(*detectOption)) mode = Detect;
		else if (parser.isSet(*programOption))
		{
			mode = Program;
			programPath = parser.value(*programOption);
		}
		else mode = Unknown;
	}
	QCommandLineParser parser;
	QCommandLineOption * simulatedDeviceType;
	QCommandLineOption * licenseOption;
	QCommandLineOption * detectOption;
	QCommandLineOption * programOption;
	QCommandLineOption * debugOption;
	OperationMode mode;
	DeviceType device;
	QString programPath;
};

QStringList Arguments::deviceType = QStringList()
	<< "disconnected" // 0
	<< "devboard" // 1
	<< "monoboard" // 2
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
	QCoreApplication::setApplicationVersion("0.4");
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
			return new ConnectedMockDevice(*output,0x2e123069);
		case MockMonoBoard:
			return new ConnectedMockDevice(*output,0x2e16a069);
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
		OUTPUT(0) << textFile.readAll().toStdString();
	}
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
	std::unique_ptr<IProgrammer> programmer
	(
		ProgrammerFactory::createProgrammer(file,createDeviceCommunication())
	);
	if (!programmer)
	{
		output->error() << "Unknown program type: " << file.suffix().toStdString();
		return ProgramTypeNotRecognised;
	}
	programmer->setOutput(output);
	std::string const filePath = appPath.toStdString();
	char const * path = filePath.c_str();
	OUTPUT(1) << "Programming Mono device with " << path;
	switch (programmer->program())
	{
		case ProgrammerSuccess:
			return Success;
		case ProgrammerCorruptProgram:
			output->error() << "Corrupt program " << file.filePath().toStdString();
			return ProgrammingDeviceFailed;
		case ProgrammerUnsupportedMetaData:
			output->error() << "Metadata section too large in  " << file.filePath().toStdString();
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
	if (CYRET_SUCCESS != psocDevice->openConnection())
	{
		return NoConnectionToMonoDevice;
	}
	else
	{
		OUTPUT(0) << "Mono device detected.";
		psocDevice->closeConnection();
		return Success;
	}
}
