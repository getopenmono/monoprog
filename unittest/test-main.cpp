#include "AutoTest.h"
#include <QDebug>

#if 0
TEST_MAIN
#else
int main(int argc, char *argv[])
{
	int failures = AutoTest::run(argc, argv);
	if (failures == 0)
	{
    	qDebug() << "ALL TESTS PASSED";
	}
	else
	{
    	qDebug() << failures << " TESTS FAILED!";
	}
	return failures;
}
#endif
