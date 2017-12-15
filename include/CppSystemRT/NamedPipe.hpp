#ifndef _CPPSYSTEMRT_NAMEDPIPE_HPP
#define _CPPSYSTEMRT_NAMEDPIPE_HPP

namespace CppSystemRT {
	
class NamedPipe {
public:
	NamedPipe();
	NamedPipe(std::string const& pipeName);

	virtual ~NamedPipe();
	
	bool open(std::string const& pipeName);
	bool bind(std::string const& pipeName);
	bool isOpen();
	int read(char* s, std::streamsize n);
	int write(const char* s, std::streamsize n);
	void close();

private:
	bool is_open;
	bool is_bind;
	std::string pName;
	HANDLE pipefd;
};

}

#endif
