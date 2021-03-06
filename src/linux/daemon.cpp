#include <CppSystemRT.hpp>

namespace CppSystemRT {
	
namespace Daemon {

namespace {

int pid_fd{-1};	
std::string pid_file_name;
bool running{false};

int read_pid() {
	int pid;
	std::ifstream pidfile(pid_file_name);
	
	if (!pidfile)
		return 0;
	
	pidfile >> pid;
	
	return pid;
}

}

void process(ThreadCreator* tc);

void _handleSignal(int sig);

int exec(ThreadCreator* tc) {
	// Process ID and Session ID
	pid_t pid, sid;
	int fd;
	
	/* Daemon will handle two signals */
	signal(SIGINT, _handleSignal);
	signal(SIGHUP, _handleSignal);

	// Forking the parent process
	// fork return the process id of the child process
	pid = fork();
	if (pid < 0) {
		return EXIT_FAILURE;
	}
	
	/* Success: Let the parent terminate */
	if (pid > 0) {
		return EXIT_SUCCESS;
	}

	// setting the umask to 0 in order to have full access to the files generated by the daemon
	umask(0);

	// Creating a new SID for the child process
	// The child process must get a unique SID from the kernel in order to operate
	sid = setsid();
	if (sid < 0) {
		return EXIT_FAILURE;
	}

	// Changing the current working directory
	if ((chdir("/")) < 0) {
		return EXIT_FAILURE;
	}
	
	// Close all open file descriptors
	for (fd = sysconf(_SC_OPEN_MAX); fd > 0; fd--) {
		close(fd);
	}

	// Reopen stdin (fd = 0), stdout (fd = 1), stderr (fd = 2)
	stdin = fopen("/dev/null", "r");
	stdout = fopen("/dev/null", "w+");
	stderr = fopen("/dev/null", "w+");
	
	if (!pid_file_name.empty())
	{
		std::stringstream str;
		pid_fd = open(pid_file_name.c_str(), O_RDWR|O_CREAT, 0640);
		if (pid_fd < 0) {
			/* Can't open lockfile */
			exit(EXIT_FAILURE);
		}
		if (lockf(pid_fd, F_TLOCK, 0) < 0) {
			/* Can't lock file */
			exit(EXIT_FAILURE);
		}
		/* Get current PID */
		str << getpid();

		/* Write PID to lockfile */
		if (write(pid_fd, str.str().c_str(), str.str().size())); // gcc ignore warning
	}

	// calling process function
	running = true;
	process(tc);

	return EXIT_SUCCESS;
}

void process(ThreadCreator* tc) {
	if (tc == nullptr) return;
	std::unique_ptr<Thread> worker(tc->create());

	while (worker && running) {		
		worker->run();
		usleep(1000);
	}
}

bool config(std::string const& param, std::string const& value) {
	if (param == "pidfile") {
		pid_file_name = value;
		return true;
	}
	
	return false;
}

bool stop()
{
	int pid = read_pid();

	if (pid)
		return kill(pid,SIGINT) == 0;
	
	return false;
}

void _handleSignal(int sig)
{
	switch (sig) {
		case SIGINT: {
			//fprintf(log_stream, "Debug: stopping daemon ...\n");
			/* Unlock and close lockfile */
			if (pid_fd != -1) {
				lockf(pid_fd, F_ULOCK, 0);
				close(pid_fd);
			}
			/* Try to delete lockfile */
			if (!pid_file_name.empty()) {
				unlink(pid_file_name.c_str());
			}
			running = false;
			/* Reset signal handling to default behavior */
			signal(SIGINT, SIG_DFL);
		} 
		break;

		case SIGHUP: {
			//fprintf(log_stream, "Debug: reloading daemon config file ...\n");
			//read_conf_file(1);
		}
		break;

		case SIGCHLD: {
			//fprintf(log_stream, "Debug: received SIGCHLD signal\n");
		}
		break;
	}
}

}

}
