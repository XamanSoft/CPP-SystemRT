#include <CppSystemRT.hpp>

namespace CppSystemRT {
	
Thread::Thread(): running(false), thread(nullptr) {
}

void Thread::start() {
	running = true;
	thread = new std::thread(run,this);
}

void Thread::stop() {
	running = false;
}

void Thread::run(Thread *thread) {
	while (thread->running) {
		thread->exec();
		
#ifdef _WIN32
		Sleep(1000);
#else
		sleep(1);
#endif
	}
	
	delete thread->thread;
}

}