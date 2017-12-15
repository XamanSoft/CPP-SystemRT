#ifndef _CPPSYSTEMRT_SOCKET_HPP
#define _CPPSYSTEMRT_SOCKET_HPP

namespace CppSystemRT {
	
class Socket {
public:
	enum Protocol { UDP, TCP };
	
	Socket();
	Socket(std::string const& host, std::string const& port, Protocol proto = TCP); // client mode

	virtual ~Socket();
	
	bool open(std::string const& host, std::string const& port, Protocol proto = TCP);
	bool bind(std::string const& host, std::string const& port, Protocol proto = TCP);
	Socket* accept();
	bool isOpen();
	int read(char* s, std::streamsize n);
	int write(const char* s, std::streamsize n);
	void close();

private:
	Socket(int sockfd); // accept constructor

	bool is_open;
	int sockfd;
};

}

#endif
