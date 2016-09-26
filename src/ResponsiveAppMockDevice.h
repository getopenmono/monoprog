#if !defined(__RESPONSIVEAPPMOCKDEVICE_H)
#define __RESPONSIVEAPPMOCKDEVICE_H
#include "cybtldr_api.h"
#include "InBootloaderMockDevice.h"
#include "OutputCollector.h"
#include <cstring>

#define OUTPUT(level) OUTPUTCOLLECTOR_LINE((output),level)

enum DeviceState { AppRunning, ResetNotYetDetected, Bootloader };

class ResponsiveAppMockDevice:
	public InBootloaderMockDevice
{
public:
	ResponsiveAppMockDevice (OutputCollector & output_, uint32_t siliconId_)
	: InBootloaderMockDevice(output_,siliconId_)
	, state(AppRunning)
	{
		OUTPUT(1) << "Using connected mock device with responsive app.";
	}
	virtual int openConnection ()
	{
		if (ResetNotYetDetected == state)
		{
			state = Bootloader;
		}
		if (Bootloader != state)
		{
			OUTPUT(2) << "Mono device not in bootloader.";
		}
		return InBootloaderMockDevice::openConnection();
	}
	virtual int closeConnection ()
	{
		if (Bootloader != state) return CYRET_SUCCESS;
		return InBootloaderMockDevice::closeConnection();
	}
	virtual int writeData (char unsigned * data, int bytesToWrite)
	{
		if (Bootloader != state) return -1;
		return InBootloaderMockDevice::writeData(data,bytesToWrite);
	}
	virtual int readData (char unsigned * buffer, int bytesToRead)
	{
		if (Bootloader != state) return -1;
		return InBootloaderMockDevice::readData(buffer,bytesToRead);
	}
	virtual void progressUpdate (char unsigned arrayId, int unsigned short rowNr)
	{
		if (Bootloader == state) InBootloaderMockDevice::progressUpdate(arrayId,rowNr);
	}
	virtual int unsigned getBufferSize ()
	{
		return 64;
	}
	virtual SerialStatus detect ()
	{
		if (Bootloader != state)
		{
			if (ResetNotYetDetected == state) state = Bootloader;
			return SerialDetected;
		}
		return NoSerialDetected;
	}
	virtual SerialStatus sendReset ()
	{
		if (AppRunning == state)
		{
			state = ResetNotYetDetected;
			return SerialResetSent;
		}
		if (ResetNotYetDetected == state)
		{
			state = Bootloader;
			return SerialResetSent;
		}
		return NoSerialDetected;
	}
	virtual int getAvailableBytes (uint8_t * buffer, size_t size)
	{
		static char const * data = "Test data from Mono...\r\n";
		strncpy((char*)buffer,data,size);
		return strlen(data);
	}
	virtual ~ResponsiveAppMockDevice ()
	{
	}
private:
	DeviceState state;
};

#undef OUTPUT

#endif // __RESPONSIVEAPPMOCKDEVICE_H
