#include <CppSystemRT.hpp>

namespace CppSystemRT {
	
IFileStreamBuf::IFileStreamBuf(File& file, std::streamsize bufSz): file(file), _inputBufferSz(!bufSz ? 1 : bufSz), _inputBuffer(new char[_inputBufferSz]) {
	setg(_inputBuffer.get(), _inputBuffer.get(), _inputBuffer.get()); // cause a underflow
}

IFileStreamBuf::~IFileStreamBuf() {
	
}

IFileStreamBuf::int_type IFileStreamBuf::underflow() {
	if (gptr() == egptr()) {
		// read to buff and reset it
		int sz = 0;
		if ((sz = file.read(_inputBuffer.get(), _inputBufferSz)) <= 0) {
			return traits_type::eof();
		}
		
		setg(_inputBuffer.get(), _inputBuffer.get(), _inputBuffer.get() + sz);
    }
    return traits_type::to_int_type(*gptr());
}

IFileStreamBuf::int_type IFileStreamBuf::uflow() {
	if (gptr() == egptr()) {
		// read to buff and reset it
		int sz = 0;
		if ((sz = file.read(_inputBuffer.get(), _inputBufferSz)) <= 0) {
			return traits_type::eof();
		}
		
		setg(_inputBuffer.get(), _inputBuffer.get(), _inputBuffer.get() + sz);
    }
	
	int_type ret = traits_type::to_int_type(*gptr());
	gbump(1);
    return ret;
}

IFileStreamBuf::int_type IFileStreamBuf::pbackfail(IFileStreamBuf::int_type ch) {
	if (gptr() == eback() || (ch != traits_type::eof() && ch != (*(gptr()-1)))) {
        return traits_type::eof();
    }
	gbump(-1);
    return traits_type::to_int_type(*gptr()); 
}

std::streamsize IFileStreamBuf::showmanyc() {
	return egptr() - gptr();
}

OFileStreamBuf::OFileStreamBuf(File& file): file(file) {}

OFileStreamBuf::~OFileStreamBuf() {
}

std::streamsize OFileStreamBuf::xsputn(const char_type* s, std::streamsize n) {
	return file.write(s, n);
}

OFileStreamBuf::int_type OFileStreamBuf::overflow(OFileStreamBuf::int_type ch) {
	if (ch == traits_type::eof())
		return 0;

	return file.write((const char*)&ch, sizeof(const char));
}

}
