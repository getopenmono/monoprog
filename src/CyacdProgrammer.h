#if !defined(__CYACDPROGRAMMER_H)
#define __CYACDPROGRAMMER_H
#include "ProgrammerBase.h"
#include <QFileInfo>

struct IDeviceCommunicator;

class CyacdProgrammer : public ProgrammerBase
{
public:
	CyacdProgrammer
	(
		QFileInfo & file,
		IDeviceCommunicator * device,
		IProgressUpdater * updater,
		uint32_t msTimeout
	);
	virtual size_t getProgramSize ();
	virtual ProgramStatus program ();
};

#endif // __CYACDPROGRAMMER_H
