#ifndef _CPPSYSTEMRT_FILESTREAMBUF_HPP
#define _CPPSYSTEMRT_FILESTREAMBUF_HPP

namespace CppSystemRT {
	
class IFileStreamBuf: public std::streambuf {
public:	
	IFileStreamBuf(File& file, std::streamsize bufSz = 1);
	virtual ~IFileStreamBuf();

protected:
	// Input
    int_type underflow() override;
    int_type uflow() override;
    int_type pbackfail(int_type ch) override;
    std::streamsize showmanyc() override;

private:
	File& file;
	std::streamsize _inputBufferSz;
	std::unique_ptr<char> _inputBuffer;
};

class OFileStreamBuf: public std::streambuf {
public:	
	OFileStreamBuf(File& file);
	virtual ~OFileStreamBuf();

protected:	
	// Output
	std::streamsize xsputn(const char_type* s, std::streamsize n) override;
	int_type overflow(int_type ch) override;
	
private:
	File& file;
};

}

#endif
