#if !defined(__PROGRAMMERBASE_H)
#define __PROGRAMMERBASE_H
#include "IProgrammer.h"
#include "IProgressUpdater.h"
#include "cybtldr_api.h"
#include "cybtldr_api2.h"
#include <QFileInfo>
#include <memory>
struct IDeviceCommunicator;

class ProgrammerBase : public IProgrammer
{
public:
	ProgrammerBase
	(
		QFileInfo & file,
		IDeviceCommunicator * device,
		IProgressUpdater * updater,
		uint32_t msTimeout
	);
	virtual void setOutput (OutputCollector * output);
protected:
	void checkSetup () const;
	CyBtldr_CommunicationsData getCybtldrCommPack () const;
	CyBtldr_ProgressUpdate * getCybtldrProgressUpdate () const;
	OutputCollector * output;
	QFileInfo & file;
private:
	std::unique_ptr<IDeviceCommunicator> device;
	std::unique_ptr<IProgressUpdater> updater;
};

#endif // __PROGRAMMERBASE_H
