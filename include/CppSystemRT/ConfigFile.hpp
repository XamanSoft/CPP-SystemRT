#ifndef _CPPSYSTEMRT_CONFIGFILE_HPP
#define _CPPSYSTEMRT_CONFIGFILE_HPP

namespace CppSystemRT {

class ConfigFileValue {
public:
	template <typename Tp> ConfigFileValue(Tp val) { value << val; }
	ConfigFileValue(std::string val): value(val) {}
	ConfigFileValue(ConfigFileValue const& cfv): value(cfv.value.str()) {}

	virtual ~ConfigFileValue() {}
	
	template <typename Tp>
	ConfigFileValue& operator>> (Tp& val) { value >> val; return *this; }
	
	template <typename Tp>
	operator Tp () { Tp val; value >> val; return val; }
	
	std::string str() { return value.str(); }
	
	ConfigFileValue& operator= (const ConfigFileValue &cfv) { value.str(cfv.value.str()); return *this; }

private:
	std::stringstream value;
};

class ConfigFile {
public:
	ConfigFile();
	ConfigFile(std::string filename);
	virtual ~ConfigFile();
	
	bool open(std::string filename);
	bool isOpen();
	bool hasError();
	std::string error();
	void close();
	
	std::map<std::string, std::string> const& values() const;

	bool hasKey(std::string const& key);
	ConfigFileValue operator[](std::string const& key); // [Section[/Subsection]*/]Value
	
	operator bool();

private:
	void setError(std::string const& errStr);
	bool parse();
	std::string parseName();
	std::string parseValue();
	void skipWS(bool skipNewLine = true);

	int line;
	std::string errorStr;
	std::ifstream file;
	std::map<std::string, std::string> keys;
};

}

#endif
