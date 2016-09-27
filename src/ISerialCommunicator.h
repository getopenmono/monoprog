#if !defined(__ISERIALOMMUNICATOR_H)
#define __ISERIALOMMUNICATOR_H
#include <cstdint>

enum SerialStatus
{
	SerialDetected,
	NoSerialDetected,
	SerialPortOpen,
	SerialPortCouldNotBeOpened,
	SerialResetSent
};

struct ISerialCommunicator
{
	virtual SerialStatus detect () = 0;
	virtual SerialStatus sendReset () = 0;
	virtual SerialStatus open () = 0;
	/**
	 * Retrieve avaiable bytes from the serial port, blocking.
	 * @param  buffer pointer to where to store the bytes.
	 * @param  size   size of buffer.
	 * @return        number of bytes read, or -1 on error.
	 */
	virtual int getAvailableBytes (uint8_t * buffer, size_t size) = 0;
};

#endif // __ISERIALOMMUNICATOR_H
