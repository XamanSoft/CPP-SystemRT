#ifndef _CPPSYSTEMRT_ERROR_HPP
#define _CPPSYSTEMRT_ERROR_HPP

namespace CppSystemRT {
	
class Error: public std::exception {
public:
	Error();
	Error(unsigned int _code): _code(_code) { }
	virtual ~Error() {}

	unsigned int code() { return _code; }
	char const* what();

private:
	unsigned int _code;
	std::string msg;
};

}

#endif
