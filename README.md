# CPP-SystemRT

The aim of this project is to create a Multiplataform System Runtime Library for Windows and Linux in C++.

## Library Support

* Daemon / Service
* Config File (INI Format)
* Object Oriented Threads
* TCP/UDP Sockets
* Named Pipes
* System Logging

### Comming Soon

* Library Documentation (After final code revision)

## Examples

Examples can be found on [test folder](test).

### Daemon Example

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
