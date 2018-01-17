#include <CppSystemRT.hpp>

using namespace CppSystemRT;

int main(int const argc, char** argv) {
	Options::Parse(argc-1, &argv[1], [] (std::string const& param, std::function<std::string()> value) { 
		std::cout << param << " -> " << value() << std::endl;
	});
	
	return 0;
}
