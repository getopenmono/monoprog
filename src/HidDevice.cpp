#include "HidDevice.h"
#include "cybtldr_api.h"
#if defined(WIN32)
#	include <windows.h>
#else
#	include <unistd.h>
#endif

#define OUTPUT(level) OUTPUTCOLLECTOR_LINE((output),level)
#define PROGRESS(level) OUTPUTCOLLECTOR_PROGRESS((output),level)

namespace CypressPsoc {
enum Settings
{
	VendorIdUsb = 0x4b4,
	ProductIdUsb = 0xb71d,
	//SerialNumber = 0x7fc120606400,
	BufferSize = 0x40
};
} // CypressPsoc

HidDevice::HidDevice (OutputCollector & output_)
: output(output_)
, usbDevice(0)
{
}

HidDevice::~HidDevice ()
{
}

int HidDevice::openConnection ()
{
	connectRealDev();
	if (0 == usbDevice)
	{
		output.error() << "Mono device not detected on USB ports.";
		output.error() << "Make sure that Mono's bootloader is waiting for Host communication.";
		return -1;
	}
	else return CYRET_SUCCESS;
}

int HidDevice::closeConnection ()
{
	OUTPUT(2) << "Closing real device.";
	if (0 != usbDevice) hid_close(usbDevice);
	return CYRET_SUCCESS;
}

int HidDevice::writeData (uint8_t * data, int bytesToWrite)
{
	OUTPUT(3) << "Writing " << bytesToWrite << " bytes to real device.";
	output.outputHex(4,(char const *)data,bytesToWrite,16);
	uint8_t buffer [CypressPsoc::BufferSize+1];
	// The first byte needs to be zero, do not know why.
	buffer[0] = 0;
	for (int i = 0; i < bytesToWrite; ++i)
	{
		buffer[i+1] = data[i];
	}
	int result = hid_write(usbDevice,buffer,bytesToWrite+1);
	// Allow for hid_write() on Windows to write more than what is needed.
	if (bytesToWrite+1 <= result) return CYRET_SUCCESS;
	else return -1;
}

int HidDevice::readData (uint8_t * buffer, int bytesToRead)
{
	OUTPUT(5) << "Reading " << bytesToRead << " bytes from real device.";
	int bytesRead = hid_read(usbDevice,buffer,bytesToRead);
	OUTPUT(3) << "Read " << bytesRead << " bytes from real device.";
	output.outputHex(4,(char const *)buffer,bytesRead,16);
	if (bytesToRead == bytesRead) return CYRET_SUCCESS;
	else return -1;
}

void HidDevice::progressUpdate (uint8_t arrayId, int unsigned short rowNr)
{
	OUTPUT(3) << "Completed array '" << (int) arrayId << "' row " << rowNr;
	PROGRESS(1);
}

void HidDevice::connectRealDev ()
{
	OUTPUT(2) << "Looking for Mono as a USB device.";
	hid_device_info * device = 0;
	int tries = 10;
	while (tries-- >= 0)
	{
		device = hid_enumerate(CypressPsoc::VendorIdUsb,CypressPsoc::ProductIdUsb);
		if (0 != device)
		{
			hid_free_enumeration(device);
			// 0 == Accept any serial number.
			usbDevice = hid_open(CypressPsoc::VendorIdUsb,CypressPsoc::ProductIdUsb,0);
			return;
		}
		PROGRESS(1);
		// Poll in 100ms intervals.
#		if defined(WIN32)
			//SetLastError(0);
			Sleep(100);
#		else
			usleep(100000);
#		endif
	}
}

int unsigned HidDevice::getBufferSize ()
{
	return CypressPsoc::BufferSize;
}
