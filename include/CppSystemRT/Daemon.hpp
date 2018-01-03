#ifndef _CPPSYSTEMRT_DAEMON_HPP
#define _CPPSYSTEMRT_DAEMON_HPP

namespace CppSystemRT {
	
namespace Daemon {
	
class ThreadCreator {
public:
	virtual ~ThreadCreator() {}

	virtual Thread* create() =0;
};

template <std::size_t... Indices>
struct indices {
	using next = indices<Indices..., sizeof...(Indices)>;
};
template <std::size_t N>
struct build_indices {
	using type = typename build_indices<N-1>::type::next;
};
template <>
struct build_indices<0> {
	using type = indices<>;
};
template <std::size_t N>
using BuildIndices = typename build_indices<N>::type;

template <typename Tp, typename... params>
class ThreadCreatorTp: public ThreadCreator {	
	template <size_t... I>
	Thread* create(indices<I...>)
	{
		return new Tp(std::get<I>(data)...);
	}

public:
	ThreadCreatorTp(params... param): data(param...) {}
	
	Thread* create() {
		return create(BuildIndices<sizeof...(params)>{});
	}
	
private:
	std::tuple<params...> data;
};

template <>
class ThreadCreatorTp<void>: public ThreadCreator {
public:
	ThreadCreatorTp() {}
	
	Thread* create() {
		return nullptr;
	}
};

int exec(ThreadCreator* tc);

template <typename Tp, typename... params>
inline int exec(params... param) { std::unique_ptr<ThreadCreator> tc(new ThreadCreatorTp<Tp,params...>(param...)); return exec(tc.get()); }
bool config(std::string const& param, std::string const& value);
bool stop();

}

}

#endif
