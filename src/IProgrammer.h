#if !defined(__IPROGRAMMER_H)
#define __IPROGRAMMER_H
#include  <cstddef>

class OutputCollector;

enum ProgramStatus
{
	ProgrammerSuccess,
	ProgrammerNoConnectionToMonoDevice,
	ProgrammerCorruptProgram,
	ProgrammerUnsupportedMetaData,
	ProgrammerWrongSiliconId,
	ProgrammerFailed
};

struct IProgrammer
{
	virtual ~IProgrammer() {};
	virtual size_t getProgramSize () = 0;
	virtual ProgramStatus program () = 0;
	virtual void setOutput (OutputCollector * output) = 0;
};

#endif // __IPROGRAMMER_H
