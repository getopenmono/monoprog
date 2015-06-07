#if !defined(__BOOTLOADERPACKET_H)
#define __BOOTLOADERPACKET_H
#include <vector>

class BootloaderPacket
{
public:
	BootloaderPacket (char unsigned * packet, size_t size)
	: code(0)
	{
		size_t i = 0;
		// See PSoC "Bootloader and Bootloadable" (Cypress 001-92648).
		// First byte must be 0x01.
		if (size <= i || packet[i] != 0x01) return;
		++i;
		// Second byte is the Command or Status code:
		if (size <= i) return;
		code =  packet[i];
		++i;
		// The third & fourth bytes are the packet length, LSB order.
		if (size <= i+1) return;
		size_t length = packet[i] + (packet[i+1] << 8);
		i += 2;
		// The following bytes are the data.
		if (size <= i+length) return;
		data = std::vector<char unsigned>(packet+i,packet+i+length);
		i += length;
		// The last two bytes before the last byte are the checksum, LSB order.
		if (size <= i+1) return;
		//size_t checksum = packet[i] + (packet[i+1] << 8);
		i += 2;
		// Last byte must be 0x17.
		if (size <= i || packet[i] != 0x17) return;
	}
	int getCommand () const
	{
		return code;
	}
	std::vector<char unsigned> const & getData () const
	{
		return data;
	}
private:
	int code;
	std::vector<char unsigned> data;
};

#endif // __BOOTLOADERPACKET_H
