#if !defined(__HEADLESSPROGRESSUPDATER_H)
#define __HEADLESSPROGRESSUPDATER_H
#include "IProgressUpdater.h"
#include "OutputCollector.h"

#define OUTPUT(level) OUTPUTCOLLECTOR_LINE((output),level)

class HeadlessProgressUpdater : public IProgressUpdater
{
public:
	HeadlessProgressUpdater (OutputCollector & output_)
	: output(output_)
	, bytesToWrite(1)
	, bytesWritten(0)
	{
	}
	virtual void progressUpdate (char unsigned, int unsigned short)
	{
		bytesWritten += 0x100 + 0x20;
		OUTPUT(0) << (int) ((bytesWritten * 100) / bytesToWrite) << "%";
	}
	virtual void setAppSize (size_t bytes)
	{
		bytesToWrite = bytes;
	}
private:
	OutputCollector & output;
	size_t bytesToWrite;
	size_t bytesWritten;
};

#endif // __HEADLESSPROGRESSUPDATER_H
