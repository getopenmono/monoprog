#if !defined(__PROGRAMMERFACTORY_H)
#define __PROGRAMMERFACTORY_H

class QFileInfo;
struct IProgrammer;
struct IDeviceCommunicator;

struct ProgrammerFactory
{
	static IProgrammer * createProgrammer (QFileInfo & file, IDeviceCommunicator * device);
};

#endif // __PROGRAMMERFACTORY_H
