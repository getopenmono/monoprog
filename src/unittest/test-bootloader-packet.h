#include "autotest.h"
#include "BootloaderPacket.h"
#include "cybtldr_command.h"

#define CONSTRUCTPACKET(buffer,var) \
	BootloaderPacket var(buffer,sizeof(buffer)/sizeof(buffer[0]))

class TestBootloaderPacket : public QObject
{
	Q_OBJECT
private slots:
	void enterBootloaderPacketGivesEnterBootloaderCommand ()
	{
		// Arrange
		uint8_t buffer[] = {CMD_START,CMD_ENTER_BOOTLOADER,0x00,0x00,0xC7,0xFF,CMD_STOP};
		CONSTRUCTPACKET(buffer,packet);
		// Assert
		QCOMPARE(packet.getCommand(),CMD_ENTER_BOOTLOADER);
	}
	void getFlashSizePacketGivesFlashSizeCommandAndData ()
	{
		// Arrange
		const uint8_t arrayId = 0x03;
		uint8_t buffer[] = {CMD_START,CMD_GET_FLASH_SIZE,0x01,0x00,arrayId,0xC9,0xFF,CMD_STOP};
		CONSTRUCTPACKET(buffer,packet);
		// Assert
		QCOMPARE(packet.getCommand(),CMD_GET_FLASH_SIZE);
		long unsigned size = packet.getData().size();
		QCOMPARE(size,1UL);
		QCOMPARE(packet.getData()[0],arrayId);
	}
};

DECLARE_TEST(TestBootloaderPacket)
