#include <CppSystemRT.hpp>

using namespace CppSystemRT;

int main(int argc, const char* argv[]) {
	char buff[500] = {0};
	const char response[] = "It works!\n";
	NamedPipe test;
	
	if (argc > 1 && std::string(argv[1]) == "server") {
		if (test.open("testpipe", {{"bind", "true"}}))
		{
			int count = test.read(buff, sizeof(buff));
			std::cout << "Message: \n";
			if (count > 0)
				std::cout.write(buff, count);
			test.write(response, sizeof(response));
		}
	} else {
		if (test.open("testpipe"))
		{
			test.write(response, sizeof(response));
			int count = test.read(buff, sizeof(buff));
			std::cout << "Message: \n";
			if (count > 0)
				std::cout.write(buff, count);
		}
	}

	return 0;
}
