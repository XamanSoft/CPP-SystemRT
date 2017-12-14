#ifndef _CPPSYSTEMRT_THREAD_HPP
#define _CPPSYSTEMRT_THREAD_HPP

namespace CppSystemRT {
	
class Thread {
public:
	Thread();
	virtual ~Thread();

	virtual int exec(bool waitFinish = false);
	virtual int wait();
	virtual void exit(int code);
	virtual void run() =0;

private:
	static void thread_run(Thread *thread);

	std::atomic<bool> running;
	std::atomic<int> retCode;
	std::unique_ptr<std::thread> thread;
};

}

#endif
