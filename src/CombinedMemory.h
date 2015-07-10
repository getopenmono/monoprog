#if !defined(__COMBINEDMEMORY_H)
#define __COMBINEDMEMORY_H
#include "IMemorySection.h"
#include <memory>

class CombinedMemory : public IMemorySection
{
public:
	CombinedMemory (std::unique_ptr<IMemorySection> x, std::unique_ptr<IMemorySection> y)
	{
		if (x->address() < y->address())
		{
			low.swap(x);
			high.swap(y);
		}
		else
		{
			low.swap(y);
			high.swap(x);
		}
		if (low->indexInRange(high->address())) throw "Overlapping memory sections";
	}
	virtual size_t address () const
	{
		return low->address();
	}
	virtual size_t size () const
	{
		return (high->address() + high->size() - low->address());
	}
	virtual bool indexInRange (size_t i) const
	{
		return (low->indexInRange(i) || high->indexInRange(i));
	}
	virtual uint8_t operator[] (size_t i) const
	{
		if (low->indexInRange(i)) return (*low)[i];
		else return (*high)[i];
	}
	virtual void relocateTo (size_t targetAddress)
	{
		size_t oldAddress = low->address();
		low->relocateTo(targetAddress);
		high->relocateTo(targetAddress+high->address()-oldAddress);
	}
private:
	std::unique_ptr<IMemorySection> low;
	std::unique_ptr<IMemorySection> high;
};

#endif // __COMBINEDMEMORY_H
