#include "HidDevice.h"
#include "cybtldr_api.h"
#include <QtSerialPort/QSerialPort>
#if defined(WIN32)
#	include <windows.h>
#else
#	include <unistd.h>
#endif

#define OUTPUT(level) OUTPUTCOLLECTOR_LINE((output),level)
#define PROGRESS(level) OUTPUTCOLLECTOR_PROGRESS((output),level)

namespace {
enum Ids
{
	VendorId = 0x4b4,
	ProductIdSerial = 0xf232,
};
} // namespace

namespace CypressPsoc {
enum Settings
{
	VendorIdUsb = VendorId,
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
	switch (connectRealDev())
	{
		default:
		case DeviceNotFound:
			output.error() << "Mono device not detected on USB ports.";
			return -1;
		case AccessDenied:
			output.error() << "Could not get access to USB device, maybe you need to be administrator?";
			return -1;
		case ConnectedToDevice:
			return CYRET_SUCCESS;
	}
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
	for (int i = 0; i < bytesToWrite; ++i) buffer[i+1] = data[i];
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

HidDeviceStatus HidDevice::connectRealDev ()
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
			OUTPUT(2) << "Found Mono.";
			// 0 == Accept any serial number.
			usbDevice = hid_open(CypressPsoc::VendorIdUsb,CypressPsoc::ProductIdUsb,0);
			if (0 == usbDevice) return AccessDenied;
			return ConnectedToDevice;
		}
		PROGRESS(1);
		// Poll in 100ms intervals.
#		if defined(WIN32)
			Sleep(100);
#		else
			usleep(100000);
#		endif
	}
	return DeviceNotFound;
}

int unsigned HidDevice::getBufferSize ()
{
	return CypressPsoc::BufferSize;
}

SerialStatus HidDevice::serialOpen ()
{
	QList<QSerialPortInfo> const serialPortInfos = QSerialPortInfo::availablePorts();
	OUTPUT(3) << "Total number of ports available: " << serialPortInfos.count();
	for (QList<QSerialPortInfo>::const_iterator i = serialPortInfos.begin(); i != serialPortInfos.end(); ++i)
		if (matchingSerialDetectedAndSetup(*i))
			return SerialDetected;
	return NoSerialDetected;
}

SerialStatus HidDevice::serialSendReset ()
{
	if (serialDevice.length() == 0) return NoSerialDetected;
	QSerialPort serialPort;
	serialPort.setPortName(serialDevice);
	if (! serialPort.open(QIODevice::WriteOnly))
	{
		output.error() << "Failed to open serial port " << serialDevice.toStdString()
			<< ": " << serialPort.errorString().toStdString();
		return SerialPortCouldNotBeOpened;
	}
	serialPort.setDataTerminalReady(true);
#	if defined(WIN32)
		Sleep(100);
#	else
		usleep(100000);
#	endif
	serialPort.setDataTerminalReady(false);
	serialPort.close();
	return SerialResetSent;
}

bool HidDevice::matchingSerialDetectedAndSetup (QSerialPortInfo const & serialPortInfo)
{
	QString const blankString = "N/A";
	QString description = serialPortInfo.description();
	QString manufacturer = serialPortInfo.manufacturer();
	//QString serialNumber = serialPortInfo.serialNumber();
	OUTPUT(3) << "Port: " << serialPortInfo.portName().toStdString();
	OUTPUT(3) << "Location: " << serialPortInfo.systemLocation().toStdString();
	OUTPUT(3) << "Description: " << (!description.isEmpty() ? description : blankString).toStdString();
	OUTPUT(3) << "Manufacturer: " << (!manufacturer.isEmpty() ? manufacturer : blankString).toStdString();
	//OUTPUT(3) << "Serial number: " << (!serialNumber.isEmpty() ? serialNumber : blankString).toStdString();
	OUTPUT(3) << "Vendor Identifier: " << (serialPortInfo.hasVendorIdentifier() ? QByteArray::number(serialPortInfo.vendorIdentifier(), 16) : blankString).toStdString();
	OUTPUT(3) << "Product Identifier: " << (serialPortInfo.hasProductIdentifier() ? QByteArray::number(serialPortInfo.productIdentifier(), 16) : blankString).toStdString();
	OUTPUT(3) << "Busy: " << (serialPortInfo.isBusy() ? "Yes" : "No");
	if (serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier())
	{
		if (serialPortInfo.vendorIdentifier() == VendorId &&
			serialPortInfo.productIdentifier() == ProductIdSerial)
		{
			OUTPUT(2) << "Mono detected on serial bus.";
			serialDevice = serialPortInfo.systemLocation();
			return true;
		}
	}
	return false;
}
