#include <CppSystemRT.hpp>

using namespace CppSystemRT;

class DaemonThread: public Thread {
public:
	void run() {
		// Daemon code
	}
};

int main(int argc, char *argv[]) {
	Log::init(argv[0]);
	Log::info("DaemonEntry point");
	
	return Daemon::exec<DaemonThread>();
}
