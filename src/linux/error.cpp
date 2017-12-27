#include <CppSystemRT.hpp>

namespace CppSystemRT {
	
Error::Error(): _code((unsigned)errno) {}

char const* Error::what() const noexcept {
	char err[512] = {0};
	(void) strerror_r((int)_code, err, 511);
	msg = err;
	return msg.c_str();
}

}
