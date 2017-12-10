#ifndef _CPPSYSTEMRT_LOG_HPP
#define _CPPSYSTEMRT_LOG_HPP

namespace CppSystemRT {
	
class Log {
public:
	virtual ~Log();
	
	enum LogType { L_SUCCESS, L_INFO, L_WARNING, L_ERROR };
	
	void report(LogType type, char const* message, ...);
	
	static Log* sysLog();
	static Log* init(char const* log_name);
	
	static void success(char const* message, ...);
	static void info(char const* message, ...);
	static void warning(char const* message, ...);
	static void error(char const* message, ...);

private:
#if _WIN32
	HANDLE event_log;
#endif

	void do_report(LogType type, char const* message, va_list arg);

	Log(char const* log_name);
};
	
}

#endif
