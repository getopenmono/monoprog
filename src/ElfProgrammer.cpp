#include "CombinedMemory.h"
#include "cybtldr_api.h"
#include "cybtldr_command.h"
#include "elfio.hpp"
#include "ElfProgrammer.h"
#include "ElfReader.h"
#include "IDeviceCommunicator.h"
#include "IProgrammer.h"
#include "MemorySection.h"
#include "OutputCollector.h"
#include <cmath>

namespace {

size_t max (size_t x, size_t y)
{
	if (x > y) return x;
	else return y;
}

size_t divideAndCeil (size_t dividend, size_t divisor)
{
	double quotient = (double)dividend / (double)divisor;
	return ceil(quotient);
}

/**
 Each write of a flash row needs to be accompanied with configuration part,
 which is what ConfiguredFlashRow accomplishes.
 */
struct ConfiguredFlashRow
{
	static const size_t BUFSIZE = 0x120;
	char buffer[BUFSIZE];
	size_t arrayId;
	size_t rowNum;
	ConfiguredFlashRow (size_t index, IMemorySection & program, IMemorySection & config)
	{
		// Memory must be alligned on Flash line.
		size_t address = program.address();
		if ((address & 0xFF) != 0x00) throw "Misaligned program";
		// Calculate the right part of the program according to index.
		address += index * 0x100;
		for (size_t i = 0; i < 0x100; ++i) buffer[i] = program[address+i];
		size_t highAddress = address >> 8;
		arrayId = highAddress >> 8;
		rowNum = highAddress & 0xFF;
		size_t configAddress = config.address() + index * 0x20;
		for (size_t i = 0; i < 0x20; ++i) buffer[i+0x100] = config[configAddress+i];
	}
	ConfiguredFlashRow (size_t address, IMemorySection & metadata)
	{
		// Memory must be alligned on Flash line.
		if ((address & 0xFF) != 0x00) throw "Misaligned metadata";
		for (size_t i = 0; i < 0x100; ++i) buffer[i] = metadata[address+i];
		size_t highAddress = address >> 8;
		arrayId = highAddress >> 8;
		rowNum = highAddress & 0xFF;
		for (size_t i = 0; i < 0x20; ++i) buffer[i+0x100] = 0;
	}
	int writeToDevice () const
	{
		/* TODO: test whether it speeds up programming to do an extra VerifyRow
		   and then only optionally do a ProgramRow.  This should be guarded by
		   a command-line switch because VerifyRow only uses a simple checksum.
		*/
		int err = CyBtldr_ProgramRow(arrayId,rowNum,(uint8_t *)&buffer,BUFSIZE);
		if (CYRET_SUCCESS != err) return err;
		uint8_t checksum = CyBtldr_ComputeChecksum((uint8_t *)&buffer,BUFSIZE);
		err = CyBtldr_VerifyRow(arrayId,rowNum,checksum);
		return err;
	}
};

} // namespace

#define OUTPUT(level) OUTPUTCOLLECTOR_LINE((*output),level)

ElfProgrammer::ElfProgrammer (QFileInfo & file, IDeviceCommunicator * device)
: ProgrammerBase(file,device)
, bootloaderVersion(0)
, bootloaderFound(false)
{
}

ElfProgrammer::~ElfProgrammer ()
{
	if (bootloaderFound) CyBtldr_SafeEndBootloadOperation();
}

ProgramStatus ElfProgrammer::program ()
{
	std::string const stringPath = file.filePath().toStdString();
	char const * path = stringPath.c_str();
	OUTPUT(5) << "ElfProgrammer " << path;
	if (! setUpElfReader()) return ProgrammerCorruptProgram;
	setupProgramMemory();
	setupConfigMemory();
	setupMetadataMemory();
	setupSiliconId();
	if (!code || !config || !metadata) return ProgrammerCorruptProgram;
	std::ios::fmtflags flags(output->getFlags());
	OUTPUT(2) << "Program: " << std::hex << code->address() << "-" << code->address()+code->size();
	OUTPUT(2) << "Config: " << std::hex << config->address() << "-" << config->address()+config->size();
	OUTPUT(2) << "Meta: " << std::hex << metadata->address() << "-" << metadata->address()+metadata->size();
	output->setFlags(flags);
	useInvertedSummationOfAllBytesChecksum();
	ProgramStatus status = startBootloader();
	if (ProgrammerSuccess != status) return status;
	if (! transferProgramToBooloader()) return ProgrammerFailed;
	return ProgrammerSuccess;
}

bool ElfProgrammer::transferProgramToBooloader ()
{
	OUTPUT(2) << "Programming";
	CyBtldr_ProgressUpdate * updater = getCybtldrProgressUpdate();
	// Memory must be alligned on Flash line.
	size_t address = code->address();
	if ((address & 0xFF) != 0x00) throw "Misaligned program";
	size_t rows = flashRowsToWrite();
	for (size_t i = 0; i < rows; ++i)
	{
		ConfiguredFlashRow row(i,*code,*config);
		OUTPUT(6) << "arrayId=" << row.arrayId;
		OUTPUT(6) << "rowNum=" << row.rowNum;
		output->outputHex(6,row.buffer,row.BUFSIZE,32);
		int res = row.writeToDevice();
		if (CYRET_SUCCESS != res)
		{
			output->error() << "Programming array " << row.arrayId << " row " << row.rowNum << " failed: " << res;
			return false;
		}
		updater(row.arrayId,row.rowNum);
	}
	// Align row start for Metadata.
	size_t metaRowStartAddress = metadata->address() & 0xFFFFFF00;
	size_t metaRowSize = metadata->address()+metadata->size()-metaRowStartAddress;
	// The empirical data we have only has short meta data sections, so we do
	// not know what the layout would be if the meta data section was longer
	// than one flash row.  Maybe the last 0x20 bytes are just zero padded?
	if (metaRowSize > 0x100)
	{
		output->error() << "Metadata section too large";
		return false;
	}
	ConfiguredFlashRow row(metaRowStartAddress,*metadata);
	OUTPUT(6) << "arrayId=" << row.arrayId;
	OUTPUT(6) << "rowNum=" << row.rowNum;
	output->outputHex(6,row.buffer,row.BUFSIZE,32);
	if (CYRET_SUCCESS != row.writeToDevice()) return false;
	updater(row.arrayId,row.rowNum);
	const unsigned long BL_VER_SUPPORT_VERIFY = 0x010214;
	if (bootloaderVersion >= BL_VER_SUPPORT_VERIFY)
	{
		if (CYRET_SUCCESS != CyBtldr_VerifyApplication()) return false;
	}
	return true;
}

size_t ElfProgrammer::flashRowsToWrite () const
{
	// The code and config sections are written interleaved, with 256 bytes code
	// followed by 32 bytes config.
	return max(divideAndCeil(code->size(),0x100),divideAndCeil(config->size(),0x20));
}

void ElfProgrammer::useInvertedSummationOfAllBytesChecksum ()
{
	CyBtldr_SetCheckSumType(SUM_CHECKSUM);
}

ProgramStatus ElfProgrammer::startBootloader ()
{
	const uint8_t siliconRev = 0;
	cyComms = getCybtldrCommPack();
	int result = CyBtldr_StartBootloadOperation(&cyComms,siliconId,siliconRev,&bootloaderVersion);
	switch (result)
	{
		case CYRET_SUCCESS:
			OUTPUT(2) << "Bootloader started";
			bootloaderFound = true;
			return ProgrammerSuccess;
		case CYRET_ERR_DEVICE:
			return ProgrammerWrongSiliconId;
		case -1:
			return ProgrammerNoConnectionToMonoDevice;
		default:
			output->error() << "Could not start bootloader, CYRET=" << result;
			return ProgrammerFailed;
	}
}

bool ElfProgrammer::setUpElfReader ()
{
	ELFIO::elfio * elfio = new ELFIO::elfio();
	if (! elfio->load(file.filePath().toStdString()))
	{
		output->error() << "Could not load ELF program " << file.filePath().toStdString();
		return false;
	}
	reader = std::unique_ptr<ElfReader>(new ElfReader(elfio));
	return true;
}

void ElfProgrammer::setupProgramMemory ()
{
	std::unique_ptr<IMemorySection> text(reader->getSection(".text"));
	if (! text)
	{
		output->error() << "No section .text in ELF program " << file.filePath().toStdString();
		return;
	}
	std::unique_ptr<IMemorySection> rodata(reader->getSection(".rodata"));
	if (! rodata)
	{
		output->error() << "No section .rodata in ELF program " << file.filePath().toStdString();
		return;
	}
	std::unique_ptr<IMemorySection> realcode = std::unique_ptr<IMemorySection>
	(
		new CombinedMemory(std::move(text),std::move(rodata))
	);
	std::unique_ptr<IMemorySection> data(reader->getSection(".data"));
	if (! data)
	{
		output->error() << "No section .data in ELF program " << file.filePath().toStdString();
		return;
	}
	data->relocateTo(realcode->address()+realcode->size());
	code = std::unique_ptr<IMemorySection>
	(
		new CombinedMemory(std::move(realcode),std::move(data))
	);
}

void ElfProgrammer::setupConfigMemory ()
{
	config = std::unique_ptr<IMemorySection>(reader->getSection(".cyconfigecc"));
	if (! config)
	{
		output->error() << "No section .cyconfigecc in ELF program " << file.filePath().toStdString();
	}
}

void ElfProgrammer::setupMetadataMemory ()
{
	// For the loayout of the metadata section, see pages 20-22 of
	// [Bootloader and bootloadable](http://www.cypress.com/file/140321/download).
	size_t const METADATASIZE = 0x40;
	uint8_t * buffer = new uint8_t[METADATASIZE];
	// Application checksum.
	uint8_t checksum = getChecksumOfCode();
	buffer[0x00] = checksum;
	// Application start routine address.
	uint32_t const entry = reader->getEntryAddress();
	buffer[0x01] = entry & 0xFF;
	buffer[0x02] = (entry >> 8) & 0xFF;
	buffer[0x03] = (entry >> 16) & 0xFF;
	buffer[0x04] = (entry >> 24) & 0xFF;
	// Last bootloader flash row.
	buffer[0x05] = ((entry >> 8) & 0xFF) - 1;
	buffer[0x06] = 0x00;
	// Reserved.
	buffer[0x07] = 0x00;
	buffer[0x08] = 0x00;
	// Application length.
	uint32_t const roundedAppSize = applicationAndConfigDataToWrite();
	buffer[0x09] = roundedAppSize & 0xFF;
	buffer[0x0A] = (roundedAppSize >> 8) & 0xFF;
	buffer[0x0B] = (roundedAppSize >> 16) & 0xFF;
	buffer[0x0C] = (roundedAppSize >> 24) & 0xFF;
	// Reserved.
	buffer[0x0D] = 0x00;
	buffer[0x0E] = 0x00;
	buffer[0x0F] = 0x00;
	// Active application (we only support one).
	buffer[0x10] = 0x00;
	// Application verification status.
	buffer[0x11] = 0x00;
	// Bootloader application version.
	buffer[0x12] = 0x00;
	buffer[0x13] = 0x00;
	// Bootloader ID.
	buffer[0x14] = 0x00;
	buffer[0x15] = 0x00;
	// Application version.
	buffer[0x16] = 0x00;
	buffer[0x17] = 0x00;
	// Application custom ID.
	buffer[0x18] = 0x00;
	buffer[0x19] = 0x00;
	buffer[0x1A] = 0x00;
	buffer[0x1B] = 0x00;
	// Pad reset with zeros.
	for (size_t i = 0x1C; i < METADATASIZE; ++i) buffer[i] = 0;
	// Get the address of the metadata section.
	std::unique_ptr<IMemorySection> precomputed(reader->getSection(".cyloadablemeta"));
	metadata = std::unique_ptr<IMemorySection>(new MemorySection(precomputed->address(),buffer,METADATASIZE));
}

uint8_t ElfProgrammer::getChecksumOfCode () const
{
	uint8_t checksum = 0;
	size_t rows = flashRowsToWrite();
	for (size_t i = 0; i < rows; ++i)
	{
		ConfiguredFlashRow row(i,*code,*config);
		checksum += CyBtldr_ComputeChecksum((char unsigned *)row.buffer,row.BUFSIZE);
	}
	return checksum;
}

size_t ElfProgrammer::applicationAndConfigDataToWrite () const
{
	// Each flash row gets 32 bytes config (in addition to the 256 code bytes).
	size_t configFlashRows = config->size() >> 5;
	// Apparently Cypress calculated the the total size of the flash writes wrt.
	// the config section as seen below, and NOT as one would expect:
	//     size_t configFlashRowBytes = (config->size() & 0x1F) + (configFlashRows << 8);
	size_t configFlashRowBytes = ((config->size() & 0x1F) << 3) + (configFlashRows << 8);
	if (configFlashRowBytes > code->size()) return configFlashRowBytes;
	else return code->size();
}

void ElfProgrammer::setupSiliconId ()
{
	std::unique_ptr<IMemorySection> cymeta(reader->getSection(".cymeta"));
	if (! cymeta)
	{
		output->error() << "No section .cymeta in ELF program " << file.filePath().toStdString();
	}
	IMemorySection & mem = *cymeta;
	size_t startAddress = mem.address() + 2;
	uint8_t byte0 = mem[startAddress+0];
	uint8_t byte1 = mem[startAddress+1];
	uint8_t byte2 = mem[startAddress+2];
	uint8_t byte3 = mem[startAddress+3];
	siliconId =
		(byte0<<24) +
		(byte1<<16) +
		(byte2<<8) +
		byte3;
	std::ios::fmtflags flags(output->getFlags());
	OUTPUT(2) << "SiliconId: " << std::hex << siliconId;
	output->setFlags(flags);
}
