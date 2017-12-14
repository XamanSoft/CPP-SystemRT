#include <CppSystemRT.hpp>

namespace CppSystemRT {
	
Thread::Thread(): running(false), retCode(0) {
}

Thread::~Thread() {}

int Thread::exec(bool waitFinish) {
	if (running) return -1;

	retCode = 0;
	running = true;
	thread.reset(new std::thread(thread_run,this));
	
	if (waitFinish && thread->joinable())
	{
		thread->join();
	}
	
	return retCode;
}

int Thread::wait() {
	if (thread->joinable())
	{
		thread->join();
	}

	return retCode;
}

void Thread::exit(int code) {
	retCode = code;
	running = false;
}

void Thread::thread_run(Thread *thread) {
	while (thread && thread->running) {
		thread->run();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

}