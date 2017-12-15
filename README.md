# CPP-SystemRT

The aim of this project is to create a System Runtime library that support Threads, Daemons(linux)/Services(Windows), Log, etc.

## Daemon Example

```cpp
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
```
