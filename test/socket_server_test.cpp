#include <CppSystemRT.hpp>

using namespace CppSystemRT;

int main() {
	char buff[1500] = {0};
	const char response[] = "HTTP/1.1 200 OK\nContent-Length: 44\nContent-Type: text/html\nConnection: Closed\n\n<html><body><h1>It works!</h1></body></html>";
	Socket test;
	
	if (test.open("localhost", {{"port", "3000"}, {"bind", "true"}}))
	{
		Socket* sock = test.accept();

		if (sock) {
			int count = sock->read(buff, sizeof(buff));
			std::cout << "Message: \n";
			if (count > 0)
				std::cout.write(buff, count);
			sock->write(response, sizeof(response));
		}
		
		delete sock;
	}

	return 0;
}
