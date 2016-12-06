#if !defined(__HIDDEVICE_H)
#define __HIDDEVICE_H
#include "IDeviceCommunicator.h"
#include "OutputCollector.h"
#include "hidapi.h"
#include <cstdint>
#include <QtSerialPort/QSerialPortInfo>

namespace {
	enum HidDeviceStatus { DeviceNotFound, AccessDenied, ConnectedToDevice };
}

class HidDevice : public IDeviceCommunicator
{
public:
	HidDevice (OutputCollector & output);
	virtual int openConnection (uint32_t msTimeout);
	virtual int closeConnection ();
	virtual int writeData (uint8_t * data, int bytesToWrite);
	virtual int readData (uint8_t * buffer, int bytesToRead);
	virtual int unsigned getBufferSize ();
	virtual SerialStatus serialOpen ();
	virtual SerialStatus serialSendReset ();
	virtual ~HidDevice ();
private:
	OutputCollector & output;
	HidDeviceStatus connectRealDev (uint32_t msTimeout);
	bool matchingSerialDetectedAndSetup (QSerialPortInfo const & serialPortInfo);
	hid_device * usbDevice;
	QString serialDevice;
};

#endif // __HIDDEVICE_H
