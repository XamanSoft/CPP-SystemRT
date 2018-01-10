#ifndef _CPPSYSTEMRT_PATH_HPP
#define _CPPSYSTEMRT_PATH_HPP

namespace CppSystemRT {
	
namespace Path {

std::string var(std::string const& name, std::string const& value = std::string());
std::string	parse(std::string const& path);

}

}

#endif
