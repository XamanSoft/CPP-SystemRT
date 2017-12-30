#ifndef _CPPSYSTEMRT_NAMEDPIPE_HPP
#define _CPPSYSTEMRT_NAMEDPIPE_HPP

namespace CppSystemRT {
	
class NamedPipe: public File {
public:
	NamedPipe();
	NamedPipe(std::string const& pipeName, std::map<std::string,std::string> const& accessInfo = {});

	virtual ~NamedPipe();
	
	bool open(std::string const& pipeName, std::map<std::string,std::string> const& accessInfo = {});
	bool isOpen() const;
	int read(char* s, unsigned int n);
	unsigned int available() const;
	int write(const char* s, unsigned int n) const;
	void close() const;

private:
	mutable bool is_open;
	mutable bool is_bind;
	mutable std::string pName;
	mutable HANDLE pipefd;
};

}

#endif
