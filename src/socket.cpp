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
inline bool _resolve(int& sockfd, std::string const& host, std::map<std::string,std::string> const& accessInfo) {
	addrinfo hints;
	addrinfo *result, *rp;
	
	::memset(&hints, 0, sizeof(addrinfo));
	hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
	hints.ai_socktype = (accessInfo.count("proto") && accessInfo.at("proto") == "udp") ? SOCK_DGRAM : SOCK_STREAM; /* Datagram socket */
	hints.ai_flags = 0;
	hints.ai_protocol = (accessInfo.count("proto") && accessInfo.at("proto") == "udp") ? IPPROTO_UDP : IPPROTO_TCP;
	int err= 0;

	if ((err=::getaddrinfo(host.c_str(), (accessInfo.count("port") ? accessInfo.at("port").c_str() : "http"), &hints, &result)) != 0) {
		return false;
	}
	char errmsg[256];
    memset(errmsg, 0, 256);
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sockfd = ::socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

		if (function(sockfd, rp->ai_addr, rp->ai_addrlen) != SOCKET_ERROR)
			break;

#ifdef _WIN32
		::closesocket(sockfd);
#else
		::close(sockfd);
#endif
	}
	
	::freeaddrinfo(result);

	return sockfd != INVALID_SOCKET;
}

}

Socket::Socket(): is_open(false), sockfd(0) {
	WSOCK;
}

Socket::Socket(int sockfd): is_open(false), sockfd(sockfd) {
	WSOCK;
}

Socket::Socket(std::string const& name, std::map<std::string,std::string> const& accessInfo): is_open(false) {
	WSOCK;
	is_open=open(name, accessInfo);
}

Socket::~Socket() {
	close();
}

bool Socket::open(std::string const& name, std::map<std::string,std::string> const& accessInfo) {
	if (accessInfo.count("bind") && accessInfo.at("bind") == "true") {
		return is_open = (_resolve<::bind>(sockfd, name, accessInfo) && ::listen(sockfd, SOMAXCONN) != SOCKET_ERROR);
	}

	return is_open = _resolve<::connect>(sockfd, name, accessInfo);
}

Socket* Socket::accept() {
	int income_sock = ::accept(sockfd, NULL, NULL);
	
	return income_sock != INVALID_SOCKET ? (new Socket(income_sock)) : nullptr;
}

bool Socket::isOpen() const {
	return is_open;
}

int Socket::read(char* s, unsigned int n) {
	return ::recv(sockfd, s, n, 0);
}

int Socket::write(const char* s, unsigned int n) const {
	return ::send(sockfd, s, n, 0);
}

unsigned int Socket::available() const {
	unsigned long avail = 0;
#ifdef _WIN32
	if (::ioctlsocket(sockfd, FIONREAD, &avail) != NO_ERROR)
		return 0;
#else
	if (!::ioctl(sockfd, FIONREAD, &avail))
		return 0;
#endif
	return avail;
}

void Socket::close() const {
#ifdef _WIN32
	::closesocket(sockfd);
#else
	::close(sockfd);
#endif
}

}