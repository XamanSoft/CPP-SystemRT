#ifndef _CPPFASTCGI_DAEMON_HPP
#define _CPPFASTCGI_DAEMON_HPP

namespace CppSystemRT {
	
namespace Daemon {

typedef Thread* (*thread_create_t)();

int exec(thread_create_t thread_create);

template <typename Tp>
static Thread* thread_create() {
	return new Tp();
}

template <>
Thread* thread_create<void>() {
	return nullptr;
}

template <typename Tp>
inline int exec() { return exec(thread_create<Tp>); }

}

}

#endif
