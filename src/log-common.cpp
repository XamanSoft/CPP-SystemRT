#include <CppSystemRT.hpp>

namespace CppSystemRT {
	
std::unique_ptr<Log> systemlog{nullptr};

Log* Log::sysLog() {
	return systemlog.get();
}

Log* Log::init(char const* log_name) {
	systemlog = std::unique_ptr<Log>(new Log(log_name));

	return systemlog.get();
}

void Log::report(LogType type, char const* message, ...) {
	va_list args;
	va_start (args, message);
	do_report(type, message, args);
	va_end (args);
}

void Log::success(char const* message, ...) {
	va_list args;
	va_start (args, message);
	systemlog->do_report(L_SUCCESS, message, args);
	va_end (args);
}

void Log::info(char const* message, ...) {
	va_list args;
	va_start (args, message);
	systemlog->do_report(L_INFO, message, args);
	va_end (args);
}

void Log::warning(char const* message, ...) {
	va_list args;
	va_start (args, message);
	systemlog->do_report(L_WARNING, message, args);
	va_end (args);
}

void Log::error(char const* message, ...) {
	va_list args;
	va_start (args, message);
	systemlog->do_report(L_ERROR, message, args);
	va_end (args);
}

}
