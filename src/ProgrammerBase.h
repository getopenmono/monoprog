#if !defined(__PROGRAMMERBASE_H)
#define __PROGRAMMERBASE_H
#include "IProgrammer.h"
#include "cybtldr_api.h"
#include "cybtldr_api2.h"
#include <QFileInfo>
#include <memory>
struct IDeviceCommunicator;

class ProgrammerBase : public IProgrammer
{
public:
	ProgrammerBase (QFileInfo & file, IDeviceCommunicator * device, uint32_t msTimeout);
	virtual void setOutput (OutputCollector * output);
protected:
	CyBtldr_CommunicationsData getCybtldrCommPack () const;
	CyBtldr_ProgressUpdate * getCybtldrProgressUpdate () const;
	OutputCollector * output;
	uint32_t msTimeout;
	QFileInfo & file;
private:
	std::unique_ptr<IDeviceCommunicator> device;
};

#endif // __PROGRAMMERBASE_H
