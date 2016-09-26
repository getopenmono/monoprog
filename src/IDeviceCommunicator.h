#if !defined(__IDEVICECOMMUNICATOR_H)
#define __IDEVICECOMMUNICATOR_H
#include "IPsocCommunicator.h"
#include "ISerialCommunicator.h"

struct IDeviceCommunicator
:
	public IPsocCommunicator,
	public ISerialCommunicator
{
	virtual ~IDeviceCommunicator () {}
};

#endif // __IDEVICECOMMUNICATOR_H
