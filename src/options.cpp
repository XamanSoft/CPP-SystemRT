#include <CppSystemRT.hpp>

namespace CppSystemRT {
	
void Options::Parse(int const& argc, char** argv, std::function<void(std::string const&, std::string const&)> f_arg) {
	int current_argc = 0;

	while (current_argc < argc) {
		char* arg = argv[current_argc++];
		char* ch = arg;
		std::string option;
		std::string value;

		if (*ch == '-') {
			ch++;

			if (*ch == '-') {
				option = ++ch;
			} else if (*ch && isalnum(*ch)) {
				option = *ch;
				ch++;
				
				if (*ch) {
					value = ch;
				}
			}
		} else
			current_argc--;

		if (value.empty() && (current_argc < argc)) {
			char* c = argv[current_argc];
			if (*c != '-') {
				value = c;
				current_argc++;
			}
		}

		if (!option.empty() || !value.empty())
			f_arg(option, value);
	}
}

}
