#include "autotest.h"
#include "ElfReader.h"
#include "IMemorySection.h"
#include "CombinedMemory.h"
#include "elfio.hpp"

class TestElfReader : public QObject
{
	Q_OBJECT
private slots:
	void readerGivesErrorWhenSectionMissing ()
	{
		// Arrange
		ElfReader reader = setUpTestReader();
		// Act
		IMemorySection * sut = reader.getSection(".donotexist");
		// Assert
		QCOMPARE(sut,(IMemorySection *)0);
	}
	void readerReturnsDataFromExistingSection ()
	{
		// Arrange
		uint8_t metadata[] =
		{
			0x99,0x11,0x3d,0x00,0x00,0x3c,0x00,0x00,0x00,0x40,0x73,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
		};
		ElfReader reader = setUpTestReader();
		// Act
		std::unique_ptr<IMemorySection> sut(reader.getSection(".cyloadablemeta"));
		// Assert
		QVERIFY((bool)sut);
		QCOMPARE(sut->address(),(size_t)0x3ffc0);
		QCOMPARE(sut->size(),(size_t)0x40);
		for (size_t i = 0; i < sut->size(); ++i)
		{
			QCOMPARE((*sut)[i+sut->address()],(uint8_t)metadata[i]);
		}
	}
	void memorySectionsCanBeCombined ()
	{
		// Arrange
		ElfReader reader = setUpTestReader();
		std::unique_ptr<IMemorySection> text(reader.getSection(".text"));
		QVERIFY((bool)text);
		QCOMPARE(text->address(),(size_t)0x3d00);
		QCOMPARE(text->size(),(size_t)0x546);
		std::unique_ptr<IMemorySection> rodata(reader.getSection(".rodata"));
		QVERIFY((bool)rodata);
		QCOMPARE(rodata->address(),(size_t)0x424c);
		QCOMPARE(rodata->size(),(size_t)0x6c);
		// Act
		std::unique_ptr<IMemorySection> sut
		(
			new CombinedMemory(std::move(text),std::move(rodata))
		);
		// Assert
		QCOMPARE(sut->address(),(size_t)0x3d00);
		QCOMPARE(sut->size(),(size_t)0x5b8);
		QCOMPARE((*sut)[0x3d08],(uint8_t)0x75);
		QCOMPARE((*sut)[0x424b],(uint8_t)0x00);
		QCOMPARE((*sut)[0x424d],(uint8_t)0x51);
	}
	void readerHasEntryPoint ()
	{
		// Arrange
		ElfReader reader = setUpTestReader();
		// Assert
		QCOMPARE(reader.getEntryAddress(),(size_t)0x3D11);
	}
private:
	ElfReader setUpTestReader ()
	{
		ELFIO::elfio * elfio = new ELFIO::elfio();
		elfio->load("../../src/features/data/ledBlinker.elf");
		Q_ASSERT(elfio);
		return ElfReader(elfio);
	}
};

DECLARE_TEST(TestElfReader)
