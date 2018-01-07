#ifndef _CPPSYSTEMRT_FILESTREAM_HPP
#define _CPPSYSTEMRT_FILESTREAM_HPP

namespace CppSystemRT {
	
class IFileStreamBuf: public std::streambuf {
public:	
	IFileStreamBuf(File& file, std::streamsize bufSz = 1);
	virtual ~IFileStreamBuf();
	
	IFileStreamBuf(const IFileStreamBuf&) = delete;
    IFileStreamBuf(IFileStreamBuf&&) = default;
    IFileStreamBuf& operator=(const IFileStreamBuf&) = delete;
    IFileStreamBuf& operator=(IFileStreamBuf&&) = default;

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
	
	OFileStreamBuf(const OFileStreamBuf &) = delete;
    OFileStreamBuf(OFileStreamBuf &&) = default;
    OFileStreamBuf& operator=(const OFileStreamBuf&) = delete;
    OFileStreamBuf& operator=(OFileStreamBuf&&) = default;

protected:	
	// Output
	std::streamsize xsputn(const char_type* s, std::streamsize n) override;
	int_type overflow(int_type ch) override;
	
private:
	File& file;
};

class IFileStream
    : public std::istream
{
public:
    IFileStream(File& file)
        : std::istream(new IFileStreamBuf(file))
    {
        exceptions(std::ios_base::badbit);
    }
    explicit IFileStream(File* file)
        : std::istream(new IFileStreamBuf(*file))
    {
        exceptions(std::ios_base::badbit);
    }
    virtual ~IFileStream()
    {
        delete rdbuf();
    }
};

class OFileStream
    : public std::ostream
{
public:
    OFileStream(File& file)
        : std::ostream(new OFileStreamBuf(file))
    {
        exceptions(std::ios_base::badbit);
    }
    explicit OFileStream(File* file)
        : std::ostream(new OFileStreamBuf(*file))
    {
        exceptions(std::ios_base::badbit);
    }
    virtual ~OFileStream()
    {
        delete rdbuf();
    }
};

}

#endif
