#include <CppSystemRT.hpp>

namespace CppSystemRT {
	
ConfigFile::ConfigFile(): line(1) {
	
}
	
ConfigFile::ConfigFile(std::string filename): line(1), file(filename) {
	parse();
}

ConfigFile::~ConfigFile() {
	
}

bool ConfigFile::open(std::string filename) {
	if (file.is_open()) file.close();
	file.open(filename);
	return parse();
}

bool ConfigFile::isOpen() {
	return file.is_open();
}

bool ConfigFile::hasError() {
	return !errorStr.empty();
}

std::string ConfigFile::error() {
	return errorStr;
}

void ConfigFile::close() {
	file.close();
}

std::map<std::string, std::string> const& ConfigFile::values() const {
	return keys;
}

bool ConfigFile::hasKey(std::string const& key) {
	return keys.count(key);
}

ConfigFileValue ConfigFile::operator[](std::string const& key){
	return keys[key];
}

ConfigFile::operator bool() {
	return file.good();
}

void ConfigFile::setError(std::string const& errStr) {
	errorStr.clear();
	std::stringstream err;
	err << "line " << line << ": " << errStr;
	errorStr = err.str();
}

bool ConfigFile::parse() {
	std::string section;
	int inputChar = 0;
	line = 1;
	
	while (file.good()) { // until EOF
		skipWS();
		inputChar = file.peek();
		
		if (inputChar == ';' || inputChar == '#') {	// comment - ignore until end-of-line
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			line++;
			continue;
		}

		if (inputChar == '[') { // Section
			file.ignore(1);
			section.clear();
			skipWS();
			inputChar = file.peek();
			if (inputChar != '_' && !std::isalpha(inputChar)) { setError("invalid section name"); return false; }
			section += getName();
			while ((inputChar = file.peek()) == '/') {
				section += file.get();
				inputChar = file.peek();
				if (inputChar != '_' && !std::isalpha(inputChar)) { setError("invalid section name"); return false; }
				section += getName();
			}
			skipWS();
			if (file.get() != ']') {
				setError("wrong end of section");
				return false;
			}

			continue;
		}
		
		if (inputChar == '_' || std::isalpha(inputChar)) { // key
			std::string key = getName();
			skipWS(false);

			if (file.get() != '=') {
				setError("must be equal symbol");
				return false;
			}

			std::string value = getValue();
			if (section.empty())
				keys[key] = value;
			else
				keys[section+'/'+key] = value;

			continue;
		}

		setError("invalid symbol");
		return false;
	}
	
	return true;
}

std::string ConfigFile::getName() {
	std::string name;
	int inputChar = file.peek();

	if (inputChar == '_' || std::isalpha(inputChar))
		name += file.get();

	while (file.good() && (std::isalnum(inputChar = file.peek()) || inputChar == '_'))
		name += file.get();
	
	return name;
}

std::string ConfigFile::getValue() {
	std::string value;
	
	while (file.good() && file.peek() != '\n')
		value += file.get();
	line++;
	file.ignore(1); // remove \n

	return value;
}

void ConfigFile::skipWS(bool skipNewLine) {
	int inputChar;
	
	while (file.good() && std::isspace(inputChar = file.peek())) {
		if (inputChar == '\n') {
			if (skipNewLine)
				line++;
			else
				break;
		}
		
		file.ignore(1);
	}
}

}
