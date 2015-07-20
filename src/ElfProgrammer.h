#if !defined(__ELFPROGRAMMER_H)
#define __ELFPROGRAMMER_H
#include "ProgrammerBase.h"
#include <cstdint>
#include <memory>
#include <QFileInfo>
class ElfReader;
struct IMemorySection;
struct IDeviceCommunicator;

class ElfProgrammer : public ProgrammerBase
{
public:
	ElfProgrammer (QFileInfo & file, IDeviceCommunicator * device);
	virtual ~ElfProgrammer();
	virtual ProgramStatus program ();
private:
	bool setUpElfReader ();
	void setupProgramMemory ();
	void setupConfigMemory ();
	void setupMetadataMemory ();
	void setupSiliconId ();
	size_t applicationAndConfigDataToWrite () const;
	void useInvertedSummationOfAllBytesChecksum ();
	ProgramStatus startBootloader ();
	bool transferProgramToBooloader ();
	size_t flashRowsToWrite () const;
	uint8_t getChecksumOfCode () const;
	std::unique_ptr<ElfReader> reader;
	std::unique_ptr<IMemorySection> code;
	std::unique_ptr<IMemorySection> config;
	std::unique_ptr<IMemorySection> metadata;
	long unsigned bootloaderVersion;
	long unsigned siliconId;
	CyBtldr_CommunicationsData cyComms;
	bool bootloaderFound;
};

#endif // __ELFPROGRAMMER_H
