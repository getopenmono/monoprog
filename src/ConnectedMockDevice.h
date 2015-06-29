#if !defined(__CONNECTEDMOCKDEVICE_H)
#define __CONNECTEDMOCKDEVICE_H
#include "IDeviceCommunicator.h"
#include "BootloaderPacket.h"
#include "OutputCollector.h"
#include "cybtldr_api.h"
#include "cybtldr_command.h"

#define OUTPUT(level) OUTPUTCOLLECTOR_LINE((output),level)
#define PROGRESS(level) OUTPUTCOLLECTOR_PROGRESS((output),level)

class ConnectedMockDevice : public IDeviceCommunicator
{
public:
	static int const BUFFERSIZE = 64;
	ConnectedMockDevice (OutputCollector & output_)
	: output(output_)
	, returnType(Unknown)
	{
		OUTPUT(1) << "Using connected mock device.";
	}
	virtual int openConnection ()
	{
		return CYRET_SUCCESS;
	}
	virtual int closeConnection ()
	{
		return CYRET_SUCCESS;
	}
	virtual int writeData (uint8_t * data, int bytesToWrite)
	{
		OUTPUT(3) << "Writing " << bytesToWrite << " bytes to mock device.";
		output.outputHex(4,(char const *)data,bytesToWrite,16);
		BootloaderPacket packet(data,bytesToWrite);
		switch (packet.getCommand())
		{
			case CMD_ENTER_BOOTLOADER:
			{
				returnType = EnterBootLoader;
				return CYRET_SUCCESS;
			}
			case CMD_GET_FLASH_SIZE:
			{
				returnType = GetFlashSize;
				return CYRET_SUCCESS;
			}
			case CMD_SEND_DATA:
			{
				std::vector<uint8_t> const & data = packet.getData();
				for (size_t i = 0; i < data.size(); ++i) dataReceived.push_back(data[i]);
				returnType = SendData;
				return CYRET_SUCCESS;
			}
			case CMD_PROGRAM_ROW:
			{
				std::vector<uint8_t> const & data = packet.getData();
				arrayId = data[0];
				rowNr = data[1] + (data[2] << 8);
				OUTPUT(5) << "array id " << arrayId << " row " << rowNr;
				for (size_t i = 3; i < data.size(); ++i) dataReceived.push_back(data[i]);
				returnType = ProgramRow;
				return CYRET_SUCCESS;
			}
			case CMD_VERIFY_ROW:
			{
				returnType = VerifyRow;
				return CYRET_SUCCESS;
			}
			case CMD_EXIT_BOOTLOADER:
			{
				returnType = NoReturn;
				return CYRET_SUCCESS;
			}

		}
		returnType = Unknown;
		return -1;
	}
	virtual int readData (uint8_t * buffer, int bytesToRead)
	{
		OUTPUT(3) << "Reading " << bytesToRead << " bytes from mock device.";
		switch (returnType)
		{
			case EnterBootLoader:
			{
				// Emulate the PSoC development board:
				// siliconId=0x2e123069, siliconRev=0, bootloaderversion=0x01011e
				int data[] = {0x69,0x30,0x12,0x2E,0x00,0x1E,0x01,0x01};
				fillBuffer(buffer,data,sizeof(data)/sizeof(data[0]));
				break;
			}
			case GetFlashSize:
			{
				int data[] = {0x3C,0x00,0xFF,0x00};
				fillBuffer(buffer,data,sizeof(data)/sizeof(data[0]));
				break;
			}
			case SendData:
			{
				fillBuffer(buffer,0,0);
				break;
			}
			case ProgramRow:
			{
				fillBuffer(buffer,0,0);
				break;
			}
			case VerifyRow:
			{
				int checksum = calculateRowCheckSum();
				cleanup();
				OUTPUT(5) << "checksum=" << checksum;
				int data[] = {checksum};
				fillBuffer(buffer,data,sizeof(data)/sizeof(data[0]));
				break;
			}
			case NoReturn:
			{
				return CYRET_SUCCESS;
			}
			default:
			{
				return -1;
			}
		}
		output.outputHex(4,(char const *)buffer,bytesToRead,16);
		return CYRET_SUCCESS;
	}
	virtual void progressUpdate (uint8_t arrayId, int unsigned short rowNr)
	{
		OUTPUT(2) << "Completed array '" << (int) arrayId << "' row " << rowNr;
		PROGRESS(1);
	}
	virtual int unsigned getBufferSize ()
	{
		return BUFFERSIZE;
	}
	virtual ~ConnectedMockDevice ()
	{
	}
private:
	void fillBuffer (uint8_t * buffer, int const * data, size_t size)
	{
		buffer[0] = CMD_START;
		buffer[1] = CYRET_SUCCESS;
		buffer[2] = size & 0xFF;
		buffer[3] = size >> 8;
		size_t i = 4;
		for (; i-4 < size; ++i) buffer[i] = data[i-4];
		size_t checkSum = calculatePacketCheckSum(buffer,4+size);
		buffer[i] = checkSum & 0xFF;
		buffer[i+1] = checkSum >> 8;
		buffer[i+2] = CMD_STOP;
	}
	size_t calculatePacketCheckSum (uint8_t * buffer, size_t size)
	{
		short unsigned sum = 0;
		while (size-- > 0)
		{
			sum += *buffer;
			buffer++;
		}
		return (1 + ~sum);
	}
	size_t calculateRowCheckSum ()
	{
		size_t size = dataReceived.size();
		OUTPUT(5) << "array id " << arrayId << " row " << rowNr;
		OUTPUT(5) << "bytes received: " << size;
		std::vector<char> buffer(size+1+2+2);
		//char buffer[size+1+2+2];
		buffer[0] = arrayId;
		buffer[1] = rowNr & 0xFF;
		buffer[2] = rowNr >> 8;
		buffer[3] = size & 0xFF;
		buffer[4] = size >> 8;
		for (size_t i = 0; i < size; ++i) buffer[i+5] = dataReceived[i];
		output.outputHex(6,buffer.data(),5,32);
		output.outputHex(5,buffer.data()+5,size,32);
		short unsigned sum = 0;
		for (size_t j = 0; j < size+5; ++j) sum += buffer[j];
		uint8_t checksum = 1 + ~sum;
		uint8_t checksum2 = (uint8_t)(checksum + arrayId + rowNr + (rowNr >> 8) + size + (size >> 8));
		return checksum2;
	}
	void cleanup ()
	{
		arrayId = 0;
		rowNr = 0;
		dataReceived.clear();
	}
 	OutputCollector & output;
	enum ReturnType
	{
		EnterBootLoader,
		GetFlashSize,
		SendData,
		ProgramRow,
		VerifyRow,
		NoReturn,
		Unknown
	} returnType;
	std::vector<uint8_t> dataReceived;
	int arrayId;
	int rowNr;
};

#undef OUTPUT
#undef PROGRESS

#endif // __CONNECTEDMOCKDEVICE_H
