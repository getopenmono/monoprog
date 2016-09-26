#if !defined(__HIDDEVICE_H)
#define __HIDDEVICE_H
#include "IDeviceCommunicator.h"
#include "OutputCollector.h"
#include "hidapi.h"
#include <cstdint>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>

namespace { enum HidDeviceStatus {DeviceNotFound,AccessDenied,ConnectedToDevice}; }

class HidDevice
#include "ISerialCommunicator.h"
:
	public IDeviceCommunicator
{
public:
	HidDevice (OutputCollector & output);
	virtual int openConnection ();
	virtual int closeConnection ();
	virtual int writeData (uint8_t * data, int bytesToWrite);
	virtual int readData (uint8_t * buffer, int bytesToRead);
	virtual void progressUpdate (uint8_t arrayId, int unsigned short rowNr);
	virtual int unsigned getBufferSize ();
	virtual SerialStatus detect ();
	virtual SerialStatus sendReset ();
	virtual int getAvailableBytes (uint8_t * buffer, size_t size);
	virtual ~HidDevice ();
private:
	OutputCollector & output;
	HidDeviceStatus connectRealDev ();
	bool matchingSerialDetectedAndSetup (QSerialPortInfo const & serialPortInfo);
	SerialStatus openSerialPort ();
	hid_device * usbDevice;
	QSerialPortInfo serialInfo;
	QSerialPort serialPort;
};

#endif // __HIDDEVICE_H
