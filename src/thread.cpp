#include <CppSystemRT.hpp>

namespace CppSystemRT {
	
Thread::Thread(): running(false), retCode(0), thread(nullptr) {
}

void Thread::start() {
	if (running) return;
	running = true;
	thread = new std::thread(run,this);
}

void Thread::stop() {
	running = false;
}

int Thread::exitCode() {
	return retCode;
}

void Thread::run(Thread *thread) {
	while (thread && thread->running) {
		if (thread->retCode = thread->exec()) {
			thread->running = false;
			break;
		}
		
#ifdef _WIN32
		Sleep(1000);
#else
		sleep(1);
#endif
	}
	
	delete thread->thread;
	thread->thread = nullptr;
}

}