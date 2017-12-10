#include <CppSystemRT.hpp>

namespace CppSystemRT {

Log::Log(char const* log_name): event_log(RegisterEventSource(NULL, log_name)) {
	
}

Log::~Log() {
	DeregisterEventSource(event_log);
}

void Log::do_report(LogType type, char const* message, va_list args) {
	char buffer[256];
	LPCTSTR buf = static_cast<LPCTSTR>(buffer);
	vsnprintf (buffer, 256, message, args);
	if (event_log)
		ReportEvent(event_log, type == L_SUCCESS? EVENTLOG_SUCCESS : (type == L_WARNING ? EVENTLOG_WARNING_TYPE : (type == L_INFO ? EVENTLOG_INFORMATION_TYPE : EVENTLOG_ERROR_TYPE))
						, 0, 0, NULL, 1, 0, &buf, NULL);
}

}
