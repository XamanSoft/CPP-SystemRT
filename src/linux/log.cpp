#include <CppSystemRT.hpp>

namespace CppSystemRT {

Log::Log(char const* log_name) {
	openlog(log_name, LOG_PID|LOG_CONS, LOG_DAEMON);
}

Log::~Log() {
	closelog();
}

void Log::do_report(LogType type, char const* message, va_list args) {
	vsyslog(type == L_SUCCESS? LOG_NOTICE : (type == L_WARNING ? LOG_WARNING : (type == L_INFO ? LOG_INFO : LOG_ERR)), message, args);
}

}
