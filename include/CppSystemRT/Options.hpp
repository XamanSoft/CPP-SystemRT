#ifndef _CPPSYSTEMRT_OPTIONS_HPP
#define _CPPSYSTEMRT_OPTIONS_HPP

namespace CppSystemRT {
	
namespace Options {
	void Parse(int const& argc, char** argv, std::function<void(std::string const&, std::string const&)> f_arg);
};

}

#endif
