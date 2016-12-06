#if !defined(__NOTCONNECTEDMOCKDEVICE_H)
#define __NOTCONNECTEDMOCKDEVICE_H
#include "IDeviceCommunicator.h"
#include "OutputCollector.h"
#include "cybtldr_api.h"
#if defined(WIN32)
#	include <windows.h>
#else
#	include <unistd.h>
#endif

#define OUTPUT(level) OUTPUTCOLLECTOR_LINE((output),level)

class NotConnectedMockDevice : public IDeviceCommunicator
{
public:
	NotConnectedMockDevice (OutputCollector & output_)
	: output(output_)
	{
		OUTPUT(1) << "Using connected mock device.";
	}
	virtual int openConnection (uint32_t msTimeout)
	{
#		if defined(WIN32)
			Sleep(msTimeout);
#		else
			usleep(msTimeout * 1000);
#		endif
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
	virtual int unsigned getBufferSize ()
	{
		return 64;
	}
	virtual SerialStatus serialOpen ()
	{
		return NoSerialDetected;
	}
	virtual SerialStatus serialSendReset ()
	{
		return NoSerialDetected;
	}
	virtual ~NotConnectedMockDevice ()
	{
	}
private:
	OutputCollector & output;
};

#undef OUTPUT

#endif // __NOTCONNECTEDMOCKDEVICE_H
