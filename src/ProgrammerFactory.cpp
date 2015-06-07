#include "ProgrammerFactory.h"
#include "IProgrammer.h"
#include "CyacdProgrammer.h"
#include "ElfProgrammer.h"
#include <QString>
#include <QFileInfo>

struct IDeviceCommunicator;

IProgrammer * ProgrammerFactory::createProgrammer (QFileInfo & file, IDeviceCommunicator * device)
{
	if (QString::compare("cyacd",file.suffix(),Qt::CaseInsensitive) == 0)
		return new CyacdProgrammer(file,device);
	else if (QString::compare("elf",file.suffix(),Qt::CaseInsensitive) == 0)
		return new ElfProgrammer(file,device);
	else
		return 0;
}
