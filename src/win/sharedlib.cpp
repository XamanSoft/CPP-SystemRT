#include <CppSystemRT.hpp>

namespace CppSystemRT {
	
SharedLib::SharedLib(std::string const& name): module(::LoadLibrary(name.c_str())) {}

SharedLib::~SharedLib() {
	if (module != nullptr)
		::FreeLibrary(module);
}

void* SharedLib::sym(std::string const& name) {
	if (module == nullptr)
		return nullptr;
	
	return ::GetProcAddress(module, name.c_str());
}

}
