#include <CppSystemRT.hpp>

using namespace CppSystemRT;

int main(int argc, char *argv[]) {
	Log::init(argv[0]);
	Log::info("DaemonEntry point");
	
	return DaemonEntry();
}
