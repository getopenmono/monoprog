#include "ElfReader.h"
#include "MemorySection.h"
#include "elfio.hpp"

ElfReader::ElfReader (ELFIO::elfio * elfio_)
: elfio(elfio_)
{
}

size_t ElfReader::getEntryAddress() const
{
	return elfio->get_entry();
}

IMemorySection * ElfReader::getSection (std::string const & sectionName) const
{
	ELFIO::section const * section = findSection(sectionName);
	if (0 == section) return 0;
	return new MemorySection
	(
		section->get_address(),
		reinterpret_cast<uint8_t const *>(section->get_data()),
		section->get_size()
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
