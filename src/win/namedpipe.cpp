#include <CppSystemRT.hpp>

namespace CppSystemRT {
	
#define BUFSIZE 512
	
NamedPipe::NamedPipe(): is_open(false), is_bind(false), pipefd(INVALID_HANDLE_VALUE) {
	
}

NamedPipe::NamedPipe(std::string const& pipeName): is_open(false), is_bind(false), pipefd(INVALID_HANDLE_VALUE) {
	open(pipeName);
}

NamedPipe::~NamedPipe() {
	close();
}

bool NamedPipe::open(std::string const& pipeName) {
	pName = pipeName;
	
	if (pipeName.find("\\") != 0)
		pName = std::string("\\\\.\\pipe\\") + pipeName;

	while (true) {
		pipefd = CreateFile( 
			pName.c_str(),   // pipe name 
			GENERIC_READ |  // read and write access 
			GENERIC_WRITE,
			0,              // no sharing 
			NULL,           // default security attributes
			OPEN_EXISTING,  // opens existing pipe 
			0,              // default attributes 
			NULL);          // no template file
		
		if (pipefd != INVALID_HANDLE_VALUE) {
			is_open = true;
			break;
		}
		
		if (GetLastError() != ERROR_PIPE_BUSY) 
		{
			return is_open = false;
		}
		
		if (!WaitNamedPipe(pName.c_str(), 20000)) 
		{ 
			return is_open = false;
		}
	}
	
	DWORD dwMode = PIPE_READMODE_MESSAGE; 
	is_open = SetNamedPipeHandleState( 
		pipefd,    // pipe handle 
		&dwMode,  // new pipe mode 
		NULL,     // don't set maximum bytes 
		NULL);    // don't set maximum time 
	
	return is_open;
}

bool NamedPipe::bind(std::string const& pipeName) {
	pName = pipeName;
	
	if (pipeName.find("\\") != 0)
		pName = std::string("\\\\.\\pipe\\") + pipeName;

	pipefd = CreateNamedPipe( 
		pName.c_str(),             // pipe name 
		PIPE_ACCESS_DUPLEX,       // read/write access 
		PIPE_TYPE_MESSAGE |       // message type pipe 
		PIPE_READMODE_MESSAGE |   // message-read mode 
		PIPE_WAIT,                // blocking mode 
		PIPE_UNLIMITED_INSTANCES, // max. instances  
		BUFSIZE,                  // output buffer size 
		BUFSIZE,                  // input buffer size 
		0,                        // client time-out 
		NULL);                    // default security attribute 
		
	if (pipefd == INVALID_HANDLE_VALUE) 
	{
		return false;
	}
	
	return is_bind = is_open = ConnectNamedPipe(pipefd, NULL) ? true : (GetLastError() == ERROR_PIPE_CONNECTED); 
}

bool NamedPipe::isOpen() {
	return is_open;
}

int NamedPipe::read(char* s, std::streamsize n) {
	DWORD cbBytesRead = 0;
	bool fSuccess = false;
	
	if (n > BUFSIZE) n = BUFSIZE;
	
	// Read client requests from the pipe. This simplistic code only allows messages
	// up to BUFSIZE characters in length.
	fSuccess = ReadFile( 
		pipefd,        // handle to pipe 
		s,    // buffer to receive data 
		n, // size of buffer 
		&cbBytesRead, // number of bytes read 
		NULL);        // not overlapped I/O 

	if (!fSuccess)
	{   
		return -(int)GetLastError(); //GetLastError() == ERROR_BROKEN_PIPE - disconnected
	}

	return cbBytesRead;
}

int NamedPipe::write(const char* s, std::streamsize n) {
	DWORD cbWritten = 0;
	bool fSuccess = false;
	
	if (n > BUFSIZE) n = BUFSIZE;

	fSuccess = WriteFile( 
		pipefd,        // handle to pipe 
		s,     // buffer to write from 
		n, // number of bytes to write 
		&cbWritten,   // number of bytes written 
		NULL);        // not overlapped I/O 

	if (!fSuccess || n != cbWritten)
	{   
		return -(int)GetLastError();
	}
	
	return cbWritten;
}

void NamedPipe::close() {
	FlushFileBuffers(pipefd); 
	if (is_bind) {
		DisconnectNamedPipe(pipefd); 
	}
	CloseHandle(pipefd);
	pName.clear();
	is_open = is_bind = false;
	pipefd = INVALID_HANDLE_VALUE;
}

}
