#include <CppSystemRT.hpp>

using namespace CppSystemRT;

int main() {
	char buff[500] = {0};
	const char request[] = "GET /index.html HTTP/1.1\nHost: google.com\n\n";
	Socket test;
	
	if (test.open("google.com", "http"))
	{
		std::cout << "Socket open:\n";
		test.write(request, sizeof(request));
		test.read(buff, 500);
		std::cout.write(buff, 500);
		std::cout << std::endl;
	}

	return 0;
}
