#include <CppSystemRT.hpp>

namespace CppSystemRT {
	
Error::Error(): _code(GetLastError()) {}

char const* Error::what() const noexcept {
	char err[512] = {0};
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, _code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), err, 511, NULL);
	msg = err;
	return msg.c_str();
}

}
