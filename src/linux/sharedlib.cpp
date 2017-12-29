#include <CppSystemRT.hpp>

namespace CppSystemRT {
	
SharedLib::SharedLib(std::string const& name): module(::dlopen(name.c_str(), RTLD_LAZY)) {
}

SharedLib::~SharedLib() {
	if (module != nullptr)
		::dlclose(module);
}

void* SharedLib::sym(std::string const& name) {
	if (module == nullptr)
		return nullptr;
	
	return ::dlsym(module, name.c_str());
}

}
