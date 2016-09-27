#if !defined(__NOTCONNECTEDMOCKDEVICE_H)
#define __NOTCONNECTEDMOCKDEVICE_H
#include "IDeviceCommunicator.h"
#include "OutputCollector.h"
#include "cybtldr_api.h"

#define OUTPUT(level) OUTPUTCOLLECTOR_LINE((output),level)

class NotConnectedMockDevice:
	public IDeviceCommunicator
{
public:
	NotConnectedMockDevice (OutputCollector & output_)
	: output(output_)
	{
		OUTPUT(1) << "Using connected mock device.";
	}
	virtual int openConnection ()
	{
		output.error() << "Mono device not detected on USB ports.";
		return -1;
	}
	virtual int closeConnection ()
	{
		return CYRET_SUCCESS;
	}
	virtual int writeData (char unsigned * /*data*/, int /*bytesToWrite*/)
	{
		return -1;
	}
	virtual int readData (char unsigned * /*buffer*/, int /*bytesToRead*/)
	{
		return -1;
	}
	virtual void progressUpdate (char unsigned /*arrayId*/, int unsigned short /*rowNr*/)
	{
	}
	virtual int unsigned getBufferSize ()
	{
		return 64;
	}
	virtual SerialStatus detect ()
	{
		return NoSerialDetected;
	}
	virtual SerialStatus open ()
	{
		return SerialPortCouldNotBeOpened;
	}
	virtual SerialStatus sendReset ()
	{
		return NoSerialDetected;
	}
	virtual int getAvailableBytes (uint8_t *, size_t)
	{
		return -1;
	}
	virtual ~NotConnectedMockDevice ()
	{
	}
private:
	OutputCollector & output;
};

#undef OUTPUT

#endif // __NOTCONNECTEDMOCKDEVICE_H
