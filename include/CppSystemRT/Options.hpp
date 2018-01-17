#ifndef _CPPSYSTEMRT_OPTIONS_HPP
#define _CPPSYSTEMRT_OPTIONS_HPP

namespace CppSystemRT {
	
namespace Options {
	void Parse(int const& argc, char** argv, std::function<void(std::string const&, std::function<std::string()>)> f_arg);
};

}

#endif
