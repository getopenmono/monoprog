#if !defined(__IPROGRAMMER_H)
#define __IPROGRAMMER_H

class OutputCollector;

enum ProgramStatus
{
	ProgrammerSuccess,
	ProgrammerNoConnectionToMonoDevice,
	ProgrammerCorruptProgram,
	ProgrammerUnsupportedMetaData,
	ProgrammerFailed
};

struct IProgrammer
{
	virtual ~IProgrammer() {};
	virtual ProgramStatus program () = 0;
	virtual void setOutput (OutputCollector * output) = 0;
};

#endif // __IPROGRAMMER_H
