#include "AutoTest.h"

class TestQString : public QObject
{
	Q_OBJECT
private slots:
	void initTestCase()
	{
		// Called before the first test.
	}
	void init()
	{
		// Called before every test.
	}
	void toUpper()
	{
		QString str = "Hello";
		QCOMPARE(str.toUpper(),QString("HELLO"));
	}
	void cleanup()
	{
		// Called after every test.
	}
	void cleanupTestCase()
	{
		// Called after the last test.
	}
};

DECLARE_TEST(TestQString)
