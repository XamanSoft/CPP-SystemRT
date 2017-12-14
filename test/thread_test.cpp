#include <CppSystemRT.hpp>

using namespace CppSystemRT;

class TestThread: public Thread {
	std::atomic<int> i{0};
public:
	void run() {
		std::cout << "Test " << (i++) << std::endl;
		if (i > 10)
			exit(0);
	}
};

int main() {
	TestThread test;

	return test.exec(true);
}
