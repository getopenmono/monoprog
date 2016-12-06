#if !defined(__IPROGRESSUPDATER_H)
#define __IPROGRESSUPDATER_H

struct IProgressUpdater
{
	virtual void progressUpdate (char unsigned arrayId, int unsigned short rowNr) = 0;
	virtual void setAppSize (size_t bytes) = 0;
};

#endif // __IPROGRESSUPDATER_H
