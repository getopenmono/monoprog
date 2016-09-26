#if !defined(__APPLICATION_H)
#define __APPLICATION_H
#include <ostream>
struct IProgrammer;

enum StatusCode
{
	Success = 0,
	Usage = 1,
	NoConnectionToMonoDevice = 2,
	ProgrammingDeviceFailed = 3,
	FileDoesNotExist = 4,
	ProgramTypeNotRecognised = 5,
	MissingElfSection = 6,
	WrongSiliconId = 7,
	CouldNotResetMono = 8,
	UnknownError
};

class Arguments;
class QCoreApplication;
class OutputCollector;
class QString;
class QFileInfo;
struct IDeviceCommunicator;

class Application
{
public:
	Application (QCoreApplication * qtApp, std::ostream & out, std::ostream & error);
	~Application ();
	StatusCode run ();
private:
	void setupOutput (std::ostream & out, std::ostream & error);
	IDeviceCommunicator * createDeviceCommunication () const;
	void setupApplicationConstants ();
	StatusCode displayVersion ();
	StatusCode displayLicenses ();
	StatusCode detectDevice ();
	StatusCode echoSerial ();
	StatusCode programDevice (QString const & appPath);
	StatusCode programDeviceInBootloader (QString const & appPath, IProgrammer * programmer);
	bool fileExists (QFileInfo const & file);
	QCoreApplication * qtApp;
	Arguments * arguments;
	std::ostream & stdout;
	OutputCollector * output;
};

#endif // __APPLICATION_H
