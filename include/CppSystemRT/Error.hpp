#ifndef _CPPSYSTEMRT_ERROR_HPP
#define _CPPSYSTEMRT_ERROR_HPP

namespace CppSystemRT {
	
class Error: public std::exception {
public:
	Error();
	Error(unsigned int _code): _code(_code) { }
	virtual ~Error() {}

	unsigned int code() const noexcept { return _code; }
	char const* what() const noexcept;

private:
	unsigned int _code;
	mutable std::string msg;
};

}

#endif
