#if !defined(__MEMORYSECTION_H)
#define __MEMORYSECTION_H
#include "IMemorySection.h"
#include <sstream>

class MemorySection : public IMemorySection
{
public:
	MemorySection (size_t address_, char const * data_, size_t size_)
	: _address(address_)
	, _data(data_)
	, _size(size_)
	{
	}
	virtual size_t address () const
	{
		return _address;
	}
	virtual size_t size () const
	{
		return _size;
	}
	virtual bool indexInRange (size_t i) const
	{
		return (i >= address() && i < address()+size());
	}
	virtual uint8_t operator[] (size_t i) const
	{
		if (indexInRange(i)) return _data[i-address()];
		else return 0;
	}
	virtual void relocateTo (size_t targetAddress)
	{
		_address = targetAddress;
	}
private:
	size_t _address;
	char const * _data;
	size_t _size;
};

#endif // __MEMORYSECTION_H
