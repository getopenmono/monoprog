#include "ElfReader.h"
#include "memorySection.h"
#include "elfio.hpp"

ElfReader::ElfReader (ELFIO::elfio * elfio_)
: elfio(elfio_)
{
}

IMemorySection * ElfReader::getSection (std::string const & sectionName) const
{
	ELFIO::section const * section = findSection(sectionName);
	if (0 == section) return 0;
	return new MemorySection
	(
		section->get_address(),section->get_data(),section->get_size()
	);
}

ELFIO::section const * ElfReader::findSection (std::string const & sectionName) const
{
	for (ELFIO::Elf_Half i = 0; i < elfio->sections.size(); ++i)
	{
		ELFIO::section const * section = elfio->sections[i];
		if (sectionName.compare(section->get_name()) == 0) return section;
	}
	return 0;
}
