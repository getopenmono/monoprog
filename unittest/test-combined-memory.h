#include "autotest.h"
#include "CombinedMemory.h"
#include "IMemorySection.h"
#include "MemorySection.h"
//#include "OutputCollector.h"

class TestCombinedMemory : public QObject
{
	Q_OBJECT
private slots:
	void combiningTwoSectionsGivesUnionOfAddressSpace ()
	{
		// Arrange
		uint8_t one[] = {0x01,0x02};
		std::unique_ptr<IMemorySection> low(new MemorySection(0x100,(char const *)one,2));
		uint8_t two[] = {0x04,0x05};
		std::unique_ptr<IMemorySection> high(new MemorySection(0x103,(char const *)two,2));
		uint8_t layout[] = {0x01,0x02,0x00,0x04,0x05};
		// Act
		std::unique_ptr<IMemorySection> sut(new CombinedMemory(std::move(high),std::move(low)));
		// Assert
		QCOMPARE(sut->address(),(size_t)0x100);
		QCOMPARE(sut->size(),sizeof(layout));
		for (size_t i = 0; i < sizeof(layout); ++i)
		{
			QCOMPARE((*sut)[i+sut->address()],layout[i]);
		}
		QCOMPARE((*sut)[sut->address()-1],(uint8_t)0x00);
	}
	void overlappingAddressSpacesGivesException ()
	{
		// Arrange
		uint8_t one[] = {0x01,0x02};
		std::unique_ptr<IMemorySection> low(new MemorySection(0x10000,(char const *)one,2));
		uint8_t two[] = {0x04,0x05};
		std::unique_ptr<IMemorySection> high(new MemorySection(0x10001,(char const *)two,2));
		// Assert
		QVERIFY_EXCEPTION_THROWN(new CombinedMemory(std::move(low),std::move(high)),char*);
	}
	void relocatingMemoryMovesBothSections ()
	{
		// Arrange
		uint8_t one[] = {0x01,0x02};
		std::unique_ptr<IMemorySection> low(new MemorySection(0x100,(char const *)one,2));
		uint8_t two[] = {0x04,0x05};
		std::unique_ptr<IMemorySection> high(new MemorySection(0x103,(char const *)two,2));
		uint8_t layout[] = {0x01,0x02,0x00,0x04,0x05};
		std::unique_ptr<IMemorySection> sut(new CombinedMemory(std::move(high),std::move(low)));
		// Act
		sut->relocateTo(0x200);
		// Assert
		QCOMPARE(sut->address(),(size_t)0x200);
		QCOMPARE(sut->size(),sizeof(layout));
		for (size_t i = 0; i < sizeof(layout); ++i)
		{
			QCOMPARE((*sut)[i+sut->address()],layout[i]);
		}
		QCOMPARE((*sut)[sut->address()-1],(uint8_t)0x00);
	}
};

DECLARE_TEST(TestCombinedMemory)
