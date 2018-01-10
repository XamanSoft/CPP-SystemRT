#ifndef _CPPSYSTEMRT_PATH_HPP
#define _CPPSYSTEMRT_PATH_HPP

namespace CppSystemRT {
	
namespace Path {
	
#ifdef _WIN32
constexpr const int		MAX_SZ		= _MAX_PATH;
constexpr const char*	EXE_EXT		= ".exe";
constexpr const char*	SOBJ_EXT	= ".dll";
#else
constexpr const int 	MAX_SZ 		= PATH_MAX;
constexpr const char*	EXE_EXT		= "";
constexpr const char*	SOBJ_EXT	= ".so";
#endif

std::string Var(std::string const& name, std::string const& value = std::string());
std::string	Parse(std::string const& path);
inline std::string	Exec(std::string const& path) { return Parse(path+EXE_EXT); }
inline std::string	SharedObject(std::string const& path) { return Parse(path+SOBJ_EXT); }

}

}

#endif
