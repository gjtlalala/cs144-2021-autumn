#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity)
    : _capacity(capacity), _size(capacity), _error(false), _nread(0), _nwrite(0), _stream(), _end(false) {}

size_t ByteStream::write(const string &data) {
    size_t len = min(data.size(), _size);
    for (size_t i = 0; i < len; i++) {
        _stream.push_back(data[i]);
    }
    _size -= len;
    _nwrite += len;
    /*if(len == data.size()){
        end_input();
    }*/
    return len;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    size_t availlen = min(len, buffer_size());
    string ret;
    for (size_t i = 0; i < availlen; i++) {
        ret += _stream.at(i);
    }
    return ret;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    size_t availlen = min(len, buffer_size());
    for (size_t i = 0; i < availlen; i++) {
        _stream.pop_front();
    }
    _size += availlen;
    _nread += availlen;
    // return ret;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    string ret;
    ret = peek_output(len);
    pop_output(len);
    return ret;
}

void ByteStream::end_input() { _end = true; }

bool ByteStream::input_ended() const { return _end; }

size_t ByteStream::buffer_size() const { return _stream.size(); }

bool ByteStream::buffer_empty() const { return _stream.empty(); }

bool ByteStream::eof() const { return input_ended() && buffer_empty(); }

size_t ByteStream::bytes_written() const { return _nwrite; }

size_t ByteStream::bytes_read() const { return _nread; }

size_t ByteStream::remaining_capacity() const { return _size; }
