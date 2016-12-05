#if !defined(__PROGRAMMERFACTORY_H)
#define __PROGRAMMERFACTORY_H
#include <cstdint>

class QFileInfo;
struct IProgrammer;
struct IDeviceCommunicator;

struct ProgrammerFactory
{
	static IProgrammer * createProgrammer (QFileInfo & file, IDeviceCommunicator * device, uint32_t msTimeout);
};

#endif // __PROGRAMMERFACTORY_H
