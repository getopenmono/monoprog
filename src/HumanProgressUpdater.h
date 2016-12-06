#if !defined(__HUMANPROGRESSUPDATER_H)
#define __HUMANPROGRESSUPDATER_H
#include "IProgressUpdater.h"
#include "OutputCollector.h"

#define OUTPUT(level) OUTPUTCOLLECTOR_LINE((output),level)
#define PROGRESS(level) OUTPUTCOLLECTOR_PROGRESS((output),level)

class HumanProgressUpdater : public IProgressUpdater
{
public:
	HumanProgressUpdater (OutputCollector & output_)
	: output(output_)
	{
	}
	virtual void progressUpdate (char unsigned arrayId, int unsigned short rowNr)
	{
		OUTPUT(3) << "Completed array '" << (int) arrayId << "' row " << rowNr;
		PROGRESS(1);
	}
	virtual void setAppSize (size_t bytes)
	{
		bytesToWrite = bytes;
	}
private:
	OutputCollector & output;
	size_t bytesToWrite;
};

#endif // __HUMANPROGRESSUPDATER_H
