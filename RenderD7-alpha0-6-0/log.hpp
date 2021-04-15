
#include <fstream>
#include <stdarg.h>
#include <string>
#include <time.h>
#include <unistd.h>

class Log
{
	public:
		Log();
		~Log();
		void Init(const char *filename);
		void Write(std::string debug_text);
		std::string logDate(void);
		std::string format(const std::string& fmt_str, ...);
	private:
		std::string filename;
};