/**
 * Use like this:
 *   #define OUTPUT(level) OUTPUTCOLLECTOR_LINE(output,level)
 *   #define PROGRESS(level) OUTPUTCOLLECTOR_PROGRESS(output,level)
 *   class MyCmdLineApp {
 *     OutputCollector * output = new OutputCollector(std::cout,std::cerr);
 *     void doStuff() {
 *	      OUTPUT(2) << "Internal value=" << value;
 *        PROGRESS(2);
 *	      OUTPUT(0) << "All is good";
 *        output.error() << "Something's wrong";
 *        ...
 *        delete output;
 *     }
 *   }
 */

#if !defined(__OUTPUTCOLLECTOR_H)
#define __OUTPUTCOLLECTOR_H
#include <ostream>
#include <iomanip>

class OutputCollector
{
public:
	OutputCollector (std::ostream & output, std::ostream & error)
	: verbosity(0)
	, outputStream(output)
	, errorStream(error)
	, pendingOutputNewline(false)
	, pendingErrorNewline(false)
	{
	}
	~OutputCollector ()
	{
		endLastLineIfNeeded();
	}
	int getVerbosity () const
	{
		return verbosity;
	}
	OutputCollector & setVerbosity (int verbosity_)
	{
		verbosity = verbosity_;
		return *this;
	}
	std::ostream & getOutputStream ()
	{
		endLastLineIfNeeded();
		pendingOutputNewline = true;
		return outputStream;
	}
	std::ostream & error ()
	{
		endLastLineIfNeeded();
		pendingErrorNewline = true;
		return errorStream;
	}
	OutputCollector & showProgress ()
	{
		pendingOutputNewline = true;
		outputStream << "." << std::flush;
		return *this;
	}
	OutputCollector & endLastLineIfNeeded ()
	{
		if (pendingOutputNewline) outputStream << std::endl << std::flush;
		pendingOutputNewline = false;
		if (pendingErrorNewline) errorStream << std::endl << std::flush;
		pendingErrorNewline = false;
		return *this;
	}
	std::ios::fmtflags getFlags () const
	{
		return outputStream.flags();
	}
	OutputCollector & setFlags (std::ios::fmtflags const & flags)
	{
		outputStream.flags(flags);
		return *this;
	}
	OutputCollector & outputHex (int level, char const * buffer, std::size_t count, std::size_t bytesPerLine)
	{
		if (level > getVerbosity()) return *this;
		endLastLineIfNeeded();
		// Save flags before manipulation.
		std::ios::fmtflags flags(outputStream.flags());
		outputStream << std::hex << std::setfill('0');
		outputStream.setf(std::ios::uppercase);
		for (std::size_t i = 0; i != count; ++i)
		{
			unsigned int currentByte = static_cast<unsigned int>(static_cast<unsigned char>(buffer[i]));
			outputStream << std::setw(2) << currentByte;
			bool isEndOfLine = (bytesPerLine != 0) && ((i + 1 == count) || ((i + 1) % bytesPerLine == 0));
			if (isEndOfLine) outputStream << std::endl;
			else outputStream << ' ';
		}
		outputStream.flush();
		// Restore original flags.
		outputStream.flags(flags);
		return *this;
	}

private:
	int verbosity;
	std::ostream & outputStream;
	std::ostream & errorStream;
	bool pendingOutputNewline;
	bool pendingErrorNewline;
};

#define OUTPUTCOLLECTOR_LINE(instance,level) \
	if (instance.getVerbosity()>=level) instance.getOutputStream()
#define OUTPUTCOLLECTOR_PROGRESS(instance,level) \
	if (instance.getVerbosity()>=level) instance.showProgress()

#endif // __OUTPUTCOLLECTOR_H
