#include "loader.hpp"

// see
// http://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf
std::istream& safeGetLine(std::istream& is, std::string& str)
{
	str.clear();

	// The characters in the stream are read one-by-one using a std::streambuf.
	// That is faster than reading them one-by-one using the std::istream.
	// Code that uses streambuf this way must be guarded by a sentry object.
	// The sentry object performs various tasks,
	// such as thread synchronization and updating the stream state.

	std::istream::sentry se(is, true);
	std::streambuf* sb = is.rdbuf();

	for (;;) {
		int c = sb->sbumpc();
		switch (c) {
			case '\n':
				return is;
			case '\r':
				if (sb->sgetc() == '\n') {
					sb->sbumpc();
				}
				return is;
			case EOF:
				// Also handle the case when the last line has no line ending
				if (str.empty()) {
					is.setstate(std::ios::eofbit);
				}
				return is;
			default:
				str += static_cast<char>(c);
		}
	}
}
