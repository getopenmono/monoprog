#include "ProgrammerBase.h"
#include "IDeviceCommunicator.h"
#include "OutputCollector.h"
#include "IProgressUpdater.h"

namespace {

/**
 * Functions passed to Cypress Bootloader API.
 * Need to be global variables so that the Read and Write C functions can see
 * the open USB device and the output collector.
 */
IDeviceCommunicator * psocDevice;
IProgressUpdater * psocUpdater;
uint32_t msTimeout;
extern "C" int openConnection ()
{
	return psocDevice->openConnection(msTimeout);
}
extern "C" int closeConnection ()
{
	return psocDevice->closeConnection();
}
extern "C" int readData (char unsigned * buffer, int length)
{
	return psocDevice->readData(buffer,length);
}
extern "C" int writeData (char unsigned * buffer, int length)
{
	return psocDevice->writeData(buffer,length);
}
extern "C" void progressUpdate (char unsigned arrayId, int unsigned short rowNr)
{
	psocUpdater->progressUpdate(arrayId,rowNr);
}

} // namespace {

ProgrammerBase::ProgrammerBase
(
	QFileInfo & fileInfo,
	IDeviceCommunicator * device_,
	IProgressUpdater * updater_,
	uint32_t ms
)
: file(fileInfo)
, device(device_)
, updater(updater_)
{
	psocDevice = device.get();
	psocUpdater = updater.get();
	msTimeout = ms;
}

void ProgrammerBase::setOutput (OutputCollector * output)
{
	this->output = output;
}

CyBtldr_CommunicationsData ProgrammerBase::getCybtldrCommPack () const
{
	static CyBtldr_CommunicationsData bundle;
	bundle.OpenConnection = openConnection;
	bundle.CloseConnection = closeConnection;
	bundle.ReadData = readData;
	bundle.WriteData = writeData;
	bundle.MaxTransferSize = psocDevice->getBufferSize();
	return bundle;
}

CyBtldr_ProgressUpdate * ProgrammerBase::getCybtldrProgressUpdate () const
{
	return progressUpdate;
}

void ProgrammerBase::checkSetup () const
{
	if (!output) throw std::logic_error("Programmer has no output attached");
}
