#if !defined(__IMEMORYSECTION_H)
#define __IMEMORYSECTION_H

struct IMemorySection
{
	virtual ~IMemorySection () {};
	virtual size_t address () const = 0;
	virtual size_t size () const = 0;
	virtual bool indexInRange (size_t i) const = 0;
	virtual char operator[] (size_t i) const = 0;
};

#endif // __IMEMORYSECTION_H
