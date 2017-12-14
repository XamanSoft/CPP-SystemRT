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

private:
	static void run(Thread *thread);

	bool running;
	std::thread* thread;
};

}

#endif
