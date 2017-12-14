#include <CppSystemRT.hpp>

using namespace CppSystemRT;

class DaemonThread: public Thread {
public:
	int exec() {
		return 0;
	}
};

int main(int argc, char *argv[]) {
	Log::init(argv[0]);
	Log::info("DaemonEntry point");
	
	return Daemon::exec<DaemonThread>();
}
