#ifndef _CPPSYSTEMRT_SOCKET_HPP
#define _CPPSYSTEMRT_SOCKET_HPP

namespace CppSystemRT {
	
class Socket: public File {
public:
	Socket();
	Socket(std::string const& name, std::map<std::string,std::string> const& accessInfo = {});

	virtual ~Socket();
	
	bool open(std::string const& name, std::map<std::string,std::string> const& accessInfo = {});
	bool select();
	Socket* accept();
	bool isOpen() const;
	int read(char* s, unsigned int n);
	int write(const char* s, unsigned int n) const;
	void close() const;
	
	unsigned int available() const;

private:
	Socket(int sockfd); // accept constructor

	bool is_open;
	int sockfd;
};

}

#endif
