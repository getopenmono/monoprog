#include "sleep.h"

#ifdef _WIN32
#	include <windows.h>
#else
#	include <unistd.h>
#endif

void msSleep (uint32_t ms)
{
#	ifdef WIN32
		Sleep(ms);
#	else
		usleep(ms*1000);
#	endif
}
