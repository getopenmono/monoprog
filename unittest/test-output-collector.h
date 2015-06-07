#include "AutoTest.h"
#include "OutputCollector.h"
#include <ostream>
#include <sstream>

#define OUTPUT(level) OUTPUTCOLLECTOR_LINE(sut,level)
#define PROGRESS(level) OUTPUTCOLLECTOR_PROGRESS(sut,level)

class TestOutputCollector : public QObject
{
	Q_OBJECT
private slots:
	void verbosityDefaultsToZero ()
	{
		// Arrange
		std::ostringstream output;
		std::ostringstream error;
		OutputCollector sut (output,error);
		// Assert
		QCOMPARE(sut.getVerbosity(),0);
	}
	void verbosityCanBeSet ()
	{
		// Arrange
		std::ostringstream output;
		std::ostringstream error;
		OutputCollector sut (output,error);
		// Act
		sut.setVerbosity(2);
		// Assert
		QCOMPARE(sut.getVerbosity(),2);
	}
	void inputForMatchingVerbosityBecomesOutput ()
	{
		// Arrange
		std::ostringstream output;
		std::ostringstream error;
		OutputCollector sut (output,error);
		// Act
		OUTPUT(0) << "My " << 2 << " friends";
		// Assert
		QCOMPARE(output.str(),std::string("My 2 friends"));
	}
	void inputForHigherLevelGetsDiscarded ()
	{
		// Arrange
		std::ostringstream output;
		std::ostringstream error;
		OutputCollector sut (output,error);
		// Act
		OUTPUT(1) << "DEBUG";
		// Assert
		QCOMPARE(output.str(),std::string(""));
	}
	void mixedLevelInputDiscardsTooHighLevels ()
	{
		// Arrange
		std::ostringstream output;
		std::ostringstream error;
		OutputCollector sut (output,error);
		// Act
		OUTPUT(0) << "line " << 1;
		OUTPUT(1) << "DEBUG";
		OUTPUT(0) << "present";
		// Assert
		QCOMPARE(output.str(),std::string("line 1\npresent"));
	}
	void progressDefaultsToDots ()
	{
		// Arrange
		std::ostringstream output;
		std::ostringstream error;
		OutputCollector sut (output,error);
		// Act
		PROGRESS(0);
		PROGRESS(0);
		// Assert
		QCOMPARE(output.str(),std::string(".."));
	}
	void progressRespectsVerbosity ()
	{
		// Arrange
		std::ostringstream output;
		std::ostringstream error;
		OutputCollector sut (output,error);
		sut.setVerbosity(1);
		// Act
		PROGRESS(0);
		PROGRESS(2);
		PROGRESS(1);
		PROGRESS(3);
		PROGRESS(0);
		// Assert
		QCOMPARE(output.str(),std::string("..."));
	}
	void progressContinuesPreviousInput ()
	{
		// Arrange
		std::ostringstream output;
		std::ostringstream error;
		OutputCollector sut (output,error);
		// Act
		OUTPUT(0) << "Testing";
		PROGRESS(0);
		PROGRESS(0);
		OUTPUT(0) << "OK";
		// Assert
		QCOMPARE(output.str(),std::string("Testing..\nOK"));
		QCOMPARE(error.str(),std::string(""));
	}
	void endingNoLineGivesNoLineFeed ()
	{
		// Arrange
		std::ostringstream output;
		std::ostringstream error;
		{
			// Act
			OutputCollector sut (output,error);
		}
		// Assert
		QCOMPARE(output.str(),std::string(""));
		QCOMPARE(error.str(),std::string(""));
	}
	void endingOpenLineGivesLineFeed ()
	{
		// Arrange
		std::ostringstream output;
		std::ostringstream error;
		{
			// Act
			OutputCollector sut (output,error);
			OUTPUT(0) << "Testing";
		}
		// Assert
		QCOMPARE(output.str(),std::string("Testing\n"));
		QCOMPARE(error.str(),std::string(""));
	}
	void multipleEndlinesGivesOneLineFeed ()
	{
		// Arrange
		std::ostringstream output;
		std::ostringstream error;
		{
			// Act
			OutputCollector sut (output,error);
			OUTPUT(0) << "Testing";
			sut.endLastLineIfNeeded();
		}
		// Assert
		QCOMPARE(output.str(),std::string("Testing\n"));
		QCOMPARE(error.str(),std::string(""));
	}
	void errorLinesAreTerminatedByLineFeed ()
	{
		// Arrange
		std::ostringstream output;
		std::ostringstream error;
		{
			// Act
			OutputCollector sut (output,error);
			sut.error() << "Something's rotten";
		}
		// Assert
		QCOMPARE(error.str(),std::string("Something's rotten\n"));
		QCOMPARE(output.str(),std::string(""));
	}
	void interspersedLinesAreTerminatedByLineFeed ()
	{
		// Arrange
		std::ostringstream output;
		std::ostringstream error;
		{
			// Act
			OutputCollector sut (output,error);
			OUTPUT(0) << "info1";
			sut.error() << "error2";
			OUTPUT(0) << "info3";
			OUTPUT(0) << "info4";
			sut.error() << "error5";
		}
		// Assert
		QCOMPARE(output.str(),std::string("info1\ninfo3\ninfo4\n"));
		QCOMPARE(error.str(),std::string("error2\nerror5\n"));
	}
	void byteArraysCanBeOutputAsHexRows ()
	{
		// Arrange
		std::ostringstream output;
		std::ostringstream error;
		char const * buffer = "ABCDE\1[]!\t\n";
		// Act
		OutputCollector sut (output,error);
		sut.outputHex(0,buffer,strlen(buffer),5);
		// Assert
		QCOMPARE(output.str(),std::string("41 42 43 44 45\n01 5B 5D 21 09\n0A\n"));
		QCOMPARE(error.str(),std::string(""));
	}
	void manipulationFlagsCanBeSetAndRestored ()
	{
		// Arrange
		std::ostringstream output;
		std::ostringstream error;
		{
			OutputCollector sut (output,error);
			// Act
			std::ios::fmtflags flags(sut.getFlags());
			OUTPUT(0) << std::hex << std::setfill('0') << std::uppercase << std::setw(2) << 12;
			sut.setFlags(flags);
			OUTPUT(0) << 12 << "a";
		}
		// Assert
		QCOMPARE(output.str(),std::string("0C\n12a\n"));
	}
};

DECLARE_TEST(TestOutputCollector)
