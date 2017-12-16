#include <CppSystemRT.hpp>

using namespace CppSystemRT;

int main(int argc, char const* argv[]) {
	ConfigFile configFile;
	
	if (argc > 1) {
		if (configFile.open(argv[1])) {
			for (auto& kv : configFile.values()) {
				std::cout << "["<< kv.first << "]=" << kv.second << std::endl;
			}
		} else {
			if (configFile.hasError()) {
				std::cout << "error " << configFile.error() << std::endl;
			} else {
				std::cout << "File couldn't be open!" << std::endl;
			}
		}
	}

	return 0;
}
