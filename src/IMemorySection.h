#if !defined(__IMEMORYSECTION_H)
#define __IMEMORYSECTION_H

#include <cstdint>

struct IMemorySection
{
	virtual ~IMemorySection () {};
	virtual size_t address () const = 0;
	virtual size_t size () const = 0;
	virtual bool indexInRange (size_t i) const = 0;
	virtual uint8_t operator[] (size_t i) const = 0;
	virtual void relocateTo (size_t targetAddress) = 0;
};

#endif // __IMEMORYSECTION_H
