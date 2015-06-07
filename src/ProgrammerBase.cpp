#include "ProgrammerBase.h"
#include "IDeviceCommunicator.h"
#include "OutputCollector.h"

namespace {

/**
 * Functions passed to Cypress Bootloader API.
 * Need to be global variables so that the Read and Write C functions can see
 * the open USB device and the verbosity.
 */
IDeviceCommunicator * psocDevice;
extern "C" int openConnection ()
{
	return psocDevice->openConnection();
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
	psocDevice->progressUpdate(arrayId,rowNr);
}

} // namespace {

ProgrammerBase::ProgrammerBase (QFileInfo & fileInfo, IDeviceCommunicator * device_)
: file(fileInfo)
, device(device_)
{
	psocDevice = device.get();
}

void ProgrammerBase::setOutput (OutputCollector * output)
{
	this->output = output;
}

CyBtldr_CommunicationsData ProgrammerBase::getCybtldrCommPack () const
{
	return
	{
		openConnection,
		closeConnection,
		readData,
		writeData,
		psocDevice->getBufferSize()
	};
}

CyBtldr_ProgressUpdate * ProgrammerBase::getCybtldrProgressUpdate () const
{
	return progressUpdate;
}
