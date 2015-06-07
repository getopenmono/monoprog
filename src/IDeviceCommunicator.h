#if !defined(__IDEVICECOMMUNICATOR_H)
#define __IDEVICECOMMUNICATOR_H

struct IDeviceCommunicator
{
	virtual int openConnection () = 0;
	virtual int closeConnection () = 0;
	virtual int writeData (char unsigned * data, int bytesToWrite) = 0;
	virtual int readData (char unsigned * buffer, int bytesToRead) = 0;
	virtual void progressUpdate (char unsigned arrayId, int unsigned short rowNr) = 0;
	virtual int unsigned getBufferSize () = 0;
	virtual ~IDeviceCommunicator () {}
};

#endif // __IDEVICECOMMUNICATOR_H
