#include <CppSystemRT.hpp>

#ifdef _WIN32
#define getcwd		_getcwd
#define PATH_SEP	'\\'
#define GetExecPath(path, pathsz) ::GetModuleFileName(GetModuleHandle(NULL), path, pathsz)
#else
#define PATH_SEP	'/'
#define GetExecPath(path, pathsz) ::readlink("/proc/self/exe", path, pathsz)
#endif

namespace CppSystemRT {
namespace Path {

namespace {

class PathParser {
public:
	PathParser() {
		char path[Path::MAX_SZ]{0};
		if (getcwd(path, sizeof(path)) != NULL)
			vars["CWD"] = path;

		if (GetExecPath(path, Path::MAX_SZ) > 0) {
			vars["EWD"] = path;
			vars["EWD"] = vars["EWD"].substr(0, vars["EWD"].find_last_of(PATH_SEP));
		}
#ifdef _WIN32
		vars["SYSNAME"] = "win";
#else
		vars["SYSNAME"] = "linux";
#endif
		vars["EXE"] = Path::EXE_EXT;
		vars["SOBJ"] = Path::SOBJ_EXT;
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

std::string Var(std::string const& name, std::string const& value) {
	return pp_istance.var(name, value);
}

std::string	Parse(std::string const& path) {
	return pp_istance.parse(path);
}

}
}
