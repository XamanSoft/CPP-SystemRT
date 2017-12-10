#ifndef _CPPSYSTEMRT_THREAD_HPP
#define _CPPSYSTEMRT_THREAD_HPP

namespace CppSystemRT {
	
class Thread {
public:
	virtual ~Thread() {};

	virtual void run() =0;
};
	
}

#endif
