#if !defined(__RESPONSIVEAPPMOCKDEVICE_H)
#define __RESPONSIVEAPPMOCKDEVICE_H
#include "cybtldr_api.h"
#include "IDeviceCommunicator.h"
#include "InBootloaderMockDevice.h"
#include "OutputCollector.h"

#define OUTPUT(level) OUTPUTCOLLECTOR_LINE((output),level)

class ResponsiveAppMockDevice : public InBootloaderMockDevice
{
public:
	ResponsiveAppMockDevice (OutputCollector & output_, uint32_t siliconId_)
	: InBootloaderMockDevice(output_,siliconId_)
	, runningApp(true)
	{
		OUTPUT(1) << "Using connected mock device with responsive app.";
	}
	virtual int openConnection ()
	{
		if (runningApp)
		{
			output.error() << "Mono device not detected on USB ports.";
			return -1;
		}
		return InBootloaderMockDevice::openConnection();
	}
	virtual int closeConnection ()
	{
		if (runningApp) return CYRET_SUCCESS;
		return InBootloaderMockDevice::closeConnection();
	}
	virtual int writeData (char unsigned * data, int bytesToWrite)
	{
		if (runningApp) return -1;
		return InBootloaderMockDevice::writeData(data,bytesToWrite);
	}
	virtual int readData (char unsigned * buffer, int bytesToRead)
	{
		if (runningApp) return -1;
		return InBootloaderMockDevice::readData(buffer,bytesToRead);
	}
	virtual void progressUpdate (char unsigned arrayId, int unsigned short rowNr)
	{
		if (!runningApp) InBootloaderMockDevice::progressUpdate(arrayId,rowNr);
	}
	virtual int unsigned getBufferSize ()
	{
		return 64;
	}
	virtual SerialStatus serialOpen ()
	{
		if (runningApp) return SerialDetected;
		return NoSerialDetected;
	}
	virtual SerialStatus serialSendReset ()
	{
		// TODO:
		return NoSerialDetected;
	}
	virtual ~ResponsiveAppMockDevice ()
	{
	}
private:
	bool runningApp;
};

#undef OUTPUT

#endif // __RESPONSIVEAPPMOCKDEVICE_H
