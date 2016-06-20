#if !defined(__HIDDEVICE_H)
#define __HIDDEVICE_H
#include "IDeviceCommunicator.h"
#include "OutputCollector.h"
#include "hidapi.h"
#include <cstdint>
#include <QtSerialPort/QSerialPortInfo>

namespace { enum HidDeviceStatus {DeviceNotFound,AccessDenied,ConnectedToDevice}; }

class HidDevice : public IDeviceCommunicator
{
public:
	HidDevice (OutputCollector & output);
	virtual int openConnection ();
	virtual int closeConnection ();
	virtual int writeData (uint8_t * data, int bytesToWrite);
	virtual int readData (uint8_t * buffer, int bytesToRead);
	virtual void progressUpdate (uint8_t arrayId, int unsigned short rowNr);
	virtual int unsigned getBufferSize ();
	virtual SerialStatus serialOpen ();
	virtual SerialStatus serialSendReset ();
	virtual ~HidDevice ();
private:
	OutputCollector & output;
	HidDeviceStatus connectRealDev ();
	bool matchingSerialDetectedAndSetup (QSerialPortInfo const & serialPortInfo);
	hid_device * usbDevice;
	QString serialDevice;
};

#endif // __HIDDEVICE_H
