#ifndef _CPPSYSTEMRT_THREAD_HPP
#define _CPPSYSTEMRT_THREAD_HPP

namespace CppSystemRT {
	
class Thread {
public:
	Thread();
	virtual ~Thread() {}

	virtual void start();
	virtual void stop();
	virtual int exec() =0;
	virtual int exitCode();

private:
	static void run(Thread *thread);

	std::atomic<bool> running;
	std::atomic<int> retCode;
	std::atomic<std::thread*> thread;
};

}

#endif
