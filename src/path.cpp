#include <CppSystemRT.hpp>

#ifdef _WIN32
#define getcwd		_getcwd
#define PATH_MAX	_MAX_PATH
#endif

namespace {

class PathParser {
public:
	PathParser() {
		char cwd[PATH_MAX];
		if (getcwd(cwd, sizeof(cwd)) != NULL)
			vars["cwd"] = cwd;
	}
	
	inline std::string var(std::string const& name, std::string const& value = std::string()) {
		if (!value.empty())
			vars[name] = value;
		
		return vars[name];
	}
	
	inline std::string	parse(std::string const& path) {
		std::stringstream ipath(path);
		std::stringstream ppath;
		
		while(ipath.peek() == '$' || ipath.get(*ppath.rdbuf(), '$').good()) {
			std::string var;
			ipath.ignore();

			if (ipath.peek() == '{') {
				ipath.ignore();
				while (ipath.peek() != '}') {
					var += ipath.get();
				}
				ipath.ignore();
			} else if (isalnum(ipath.peek())) {
				while (isalnum(ipath.peek())) {
					var += ipath.get();
				}
			} else {
				var += ipath.get();
			}
			
			if (vars.count(var))
				ppath << vars[var];
			else
				ppath << var;
		}

		#ifdef _WIN32
		std::string result(ppath.str());
		std::replace(result.begin(), result.end(), '/', '\\');
		ppath.str(result);
		#endif
		return ppath.str();
	}

private:
	std::map<std::string, std::string> vars;
};

PathParser pp_istance;
	
}

namespace CppSystemRT {
namespace Path {

std::string var(std::string const& name, std::string const& value) {
	return pp_istance.var(name, value);
}

std::string	parse(std::string const& path) {
	return pp_istance.parse(path);
}

}
}
