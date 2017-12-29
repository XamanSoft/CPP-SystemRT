#ifndef _CPPSYSTEMRT_SHAREDLIB_HPP
#define _CPPSYSTEMRT_SHAREDLIB_HPP

namespace CppSystemRT {
	
#ifndef _WIN32
#define HMODULE void*
#endif
	
class SharedLib {
public:
	SharedLib(std::string const& name);
	virtual ~SharedLib();
	
	template <typename Tp>
	inline Tp symbol(std::string const& name) { return static_cast<Tp>(sym(name)); }

private:
	HMODULE module;
	
	void* sym(std::string const& name);
};

}

#endif
