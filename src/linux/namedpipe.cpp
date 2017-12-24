#include <CppSystemRT.hpp>

namespace CppSystemRT {
	
#define BUFSIZE 512
	
NamedPipe::NamedPipe(): is_open(false), is_bind(false), pipefd(0) {
	
}

NamedPipe::NamedPipe(std::string const& pipeName, std::map<std::string,std::string> const& accessInfo): is_open(false), is_bind(false), pipefd(0) {
	open(pipeName, accessInfo);
}

NamedPipe::~NamedPipe() {
	close();
}

bool NamedPipe::open(std::string const& pipeName, std::map<std::string,std::string> const& accessInfo) {
	pName = pipeName;
	
	if (pipeName.find("/") != 0)
		pName = std::string("/tmp/") + pipeName;

	if (accessInfo.count("bind") && accessInfo.at("bind") == "true") {
		/* create the FIFO (named pipe) */
		is_bind = mkfifo(pName.c_str(), 0666) == 0;
	}

    pipefd = ::open(pName.c_str(), O_RDWR);

	return is_open = pipefd > -1;
}

bool NamedPipe::isOpen() const {
	return is_open;
}

int NamedPipe::read(char* s, unsigned int n) {
	return ::read(pipefd, s, n);
}

int NamedPipe::write(const char* s, unsigned int n) const {
	return ::write(pipefd, s, n);
}

void NamedPipe::close() const {
	::fdatasync(pipefd);
	::close(pipefd);
	if (is_bind)
		::unlink(pName.c_str());
	is_bind = is_open = false;
	pipefd=0;
}

}
