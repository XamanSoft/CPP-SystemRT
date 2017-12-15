#include <CppSystemRT.hpp>

namespace CppSystemRT {
	
#define BUFSIZE 512
	
NamedPipe::NamedPipe(): is_open(false), is_bind(false), pipefd(0) {
	
}

NamedPipe::NamedPipe(std::string const& pipeName): is_open(false), is_bind(false), pipefd(0) {
	open(pipeName);
}

NamedPipe::~NamedPipe() {
	close();
}

bool NamedPipe::open(std::string const& pipeName) {
	pName = pipeName;
	
	if (pipeName.find("/") != 0)
		pName = std::string("/tmp/") + pipeName;

    pipefd = ::open(pName.c_str(), O_RDWR);

	return is_open = pipefd > -1;
}

bool NamedPipe::bind(std::string const& pipeName) {
	pName = pipeName;
	
	if (pipeName.find("/") != 0)
		pName = std::string("/tmp/") + pipeName;
	
	/* create the FIFO (named pipe) */
    mkfifo(pName.c_str(), 0666);
	
	pipefd = ::open(pName.c_str(), O_RDWR);
	
	return is_bind = is_open = pipefd > -1;
}

bool NamedPipe::isOpen() {
	return is_open;
}

int NamedPipe::read(char* s, std::streamsize n) {
	return ::read(pipefd, s, n);
}

int NamedPipe::write(const char* s, std::streamsize n) {
	return ::write(pipefd, s, n);
}

void NamedPipe::close() {
	::fdatasync(pipefd);
	::close(pipefd);
	if (is_bind)
		::unlink(pName.c_str());
	is_bind = is_open = false;
	pipefd=0;
}

}
