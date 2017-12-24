#ifndef _CPPSYSTEMRT_FILE_HPP
#define _CPPSYSTEMRT_FILE_HPP

namespace CppSystemRT {
	
class File {
public:	
	virtual bool open(std::string const& name, std::map<std::string,std::string> const& accessInfo) =0;
	virtual bool isOpen() const =0;
	virtual int read(char* s, unsigned int n) =0;
	virtual int write(const char* s, unsigned int n) const =0;
	virtual void close() const =0;
	
	virtual ~File() {}
};

}

#endif
