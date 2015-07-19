#if !defined(__ELFREADER_H)
#define __ELFREADER_H
#include <memory>
#include <string>
#include "IMemorySection.h"
namespace ELFIO
{
	class elfio;
	class section;
}

class ElfReader
{
public:
	ElfReader (ELFIO::elfio * elfio);
	size_t getEntryAddress() const;
	IMemorySection * getSection (std::string const & sectionName) const;
private:
	ELFIO::section const * findSection (std::string const & sectionName) const;
	std::unique_ptr<ELFIO::elfio> elfio;
};

#endif // __ELFREADER_H
