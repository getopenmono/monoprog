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
	UnsupportedFileType = 9,
	UnknownError
};

class Arguments;
class QCoreApplication;
class OutputCollector;
class QString;
class QFileInfo;
struct IDeviceCommunicator;
struct IProgressUpdater;

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
	IProgressUpdater * createProgressUpdater () const;
	StatusCode displayVersion ();
	StatusCode displayLicenses ();
	StatusCode detectDevice ();
	StatusCode programDevice (QString const & appPath);
	StatusCode displayInfo (QString const & appPath);
	StatusCode programDeviceInBootloader (QString const & appPath, IProgrammer * programmer);
	bool fileExists (QFileInfo const & file);
	QCoreApplication * qtApp;
	Arguments * arguments;
	OutputCollector * output;
};

#endif // __APPLICATION_H
