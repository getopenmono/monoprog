#include "ProgrammerFactory.h"
#include "CyacdProgrammer.h"
#include "ElfProgrammer.h"
#include "IProgrammer.h"
#include "IProgressUpdater.h"
#include <QString>
#include <QFileInfo>

struct IDeviceCommunicator;

IProgrammer * ProgrammerFactory::createProgrammer
(
	QFileInfo & file,
	IDeviceCommunicator * device,
	IProgressUpdater * updater,
	uint32_t msTimeout
) {
	if (QString::compare("cyacd",file.suffix(),Qt::CaseInsensitive) == 0)
		return new CyacdProgrammer(file,device,updater,msTimeout);
	else if (QString::compare("elf",file.suffix(),Qt::CaseInsensitive) == 0)
		return new ElfProgrammer(file,device,updater,msTimeout);
	else
		return 0;
}
