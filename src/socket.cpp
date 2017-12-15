#include <CppSystemRT.hpp>

namespace CppSystemRT {

namespace {
	
#ifdef _WIN32
template <BYTE major, BYTE minor>
class WinSock {
	WSADATA wsaData;

	WinSock() {
		if (WSAStartup(MAKEWORD(major, minor), &wsaData) != 0) {
			return;
		}
	}

public:
	~WinSock() {
		WSACleanup();
	}
	
	static void init() { static WinSock<major,minor> wsock; }
};

#define WSOCK WinSock<2,2>::init();

typedef int (*function_call_t)(_In_ SOCKET sockfd, _In_ const struct sockaddr *addr, _In_ int addrlen);

#else
#define WSOCK
typedef int (*function_call_t)(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#endif

template <function_call_t function>
inline bool _resolve(int& sockfd, std::string const& host, std::string const& port, Socket::Protocol proto) {
	addrinfo hints;
	addrinfo *result, *rp;
	
	::memset(&hints, 0, sizeof(addrinfo));
	hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
	hints.ai_socktype = proto == Socket::TCP ? SOCK_STREAM : SOCK_DGRAM; /* Datagram socket */
	hints.ai_flags = 0;
	hints.ai_protocol = proto == Socket::TCP ? IPPROTO_TCP : IPPROTO_UDP;
	int err= 0;
	if ((err=::getaddrinfo(host.c_str(), port.c_str(), &hints, &result)) != 0) {
		std::cout << "aqui " << err << std::endl;
		return false;
	}
	
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sockfd = ::socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		
		if (sockfd == SOCKET_ERROR)
			continue;

		if (function(sockfd, rp->ai_addr, rp->ai_addrlen) != SOCKET_ERROR)
			break;

#ifdef _WIN32
		::closesocket(sockfd);
#else
		::close(sockfd);
#endif
	}
	
	::freeaddrinfo(result);
	
	return true;
}

}

Socket::Socket(): is_open(false), sockfd(0) {
	WSOCK;
}

Socket::Socket(int sockfd): is_open(false), sockfd(sockfd) {
	WSOCK;
}

Socket::Socket(std::string const& host, std::string const& port, Protocol proto): is_open(false) {
	WSOCK;
	is_open=open(host, port, proto);
}

Socket::~Socket() {
	close();
}

bool Socket::open(std::string const& host, std::string const& port, Protocol proto) {
	return is_open = _resolve<::connect>(sockfd, host, port, proto);
}

bool Socket::bind(std::string const& host, std::string const& port, Protocol proto) {
	return is_open = (_resolve<::bind>(sockfd, host, port, proto) && ::listen(sockfd, SOMAXCONN) != SOCKET_ERROR);
}

Socket* Socket::accept() {
	int income_sock = ::accept(sockfd, NULL, NULL);
	
	return income_sock != INVALID_SOCKET ? (new Socket(income_sock)) : nullptr;
}

bool Socket::isOpen() {
	return is_open;
}

int Socket::read(char* s, std::streamsize n) {
	return ::recv(sockfd, s, n, 0);
}

int Socket::write(const char* s, std::streamsize n) {
	return ::send(sockfd, s, n, 0);
}

void Socket::close() {
#ifdef _WIN32
	::closesocket(sockfd);
#else
	::close(sockfd);
#endif
}

}