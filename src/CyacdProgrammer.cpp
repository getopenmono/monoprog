#include "CyacdProgrammer.h"
#include "IProgrammer.h"
#include "IDeviceCommunicator.h"
#include "OutputCollector.h"
#include "cybtldr_api.h"
#include "cybtldr_api2.h"

#define OUTPUT(level) OUTPUTCOLLECTOR_LINE((*output),level)

CyacdProgrammer::CyacdProgrammer
(
	QFileInfo & file,
	IDeviceCommunicator * device,
	IProgressUpdater * updater,
	uint32_t msTimeout
)
: ProgrammerBase(file,device,updater,msTimeout)
{
}

size_t CyacdProgrammer::getProgramSize ()
{
	checkSetup();
	return 0;
}

ProgramStatus CyacdProgrammer::program ()
{
	checkSetup();
	std::string const filePath = file.filePath().toStdString();
	char const * path = filePath.c_str();
	OUTPUT(5) << "CyacdProgrammer " << path;
	CyBtldr_CommunicationsData cyComms = getCybtldrCommPack();
	CyBtldr_ProgressUpdate * updater = getCybtldrProgressUpdate();
	int result = CyBtldr_Program(path,&cyComms,updater);
	switch (result)
	{
		case CYRET_SUCCESS:
			return ProgrammerSuccess;
		case CYRET_ERR_DATA:
		case CYRET_ERR_LENGTH:
			return  ProgrammerCorruptProgram;
		case -1:
			return ProgrammerNoConnectionToMonoDevice;
		default:
			OUTPUT(2) << "Unknown CYRES code: " << result;
			return ProgrammerFailed;
	}
}
