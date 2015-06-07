#include "AutoTest.h"
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
		char unsigned one[] = {0x01,0x02};
		std::unique_ptr<IMemorySection> low(new MemorySection(0x100,(char const *)one,2));
		char unsigned two[] = {0x04,0x05};
		std::unique_ptr<IMemorySection> high(new MemorySection(0x103,(char const *)two,2));
		char unsigned layout[] = {0x01,0x02,0x00,0x04,0x05};
		// Act
		std::unique_ptr<IMemorySection> sut(new CombinedMemory(std::move(high),std::move(low)));
		// Assert
		QCOMPARE(sut->address(),(size_t)0x100);
		QCOMPARE(sut->size(),sizeof(layout));
		for (size_t i = 0; i < sizeof(layout); ++i)
		{
			QCOMPARE((*sut)[i+sut->address()],(char)layout[i]);
		}
		QCOMPARE((*sut)[sut->address()-1],(char)0x00);
	}
};

DECLARE_TEST(TestCombinedMemory)
