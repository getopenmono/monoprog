#if !defined(__HIDDEVICE_H)
#define __HIDDEVICE_H
#include "IDeviceCommunicator.h"
#include "OutputCollector.h"
#include "hidapi.h"

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
	virtual ~HidDevice ();
private:
	OutputCollector & output;
	void connectRealDev ();
	hid_device * usbDevice;
};

#endif // __HIDDEVICE_H
