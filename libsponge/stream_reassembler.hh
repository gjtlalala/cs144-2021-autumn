#ifndef SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH
#define SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH

#include "byte_stream.hh"

#include <cstdint>
#include <string>
#include <map>
using namespace std;
/*class UnorderData{
    public:
        size_t index;
        string data;
        UnorderData(size_t _index, string _data):index(_index), data(_data){}
        bool operator <(UnorderData t) const {return index < t.index;} 
};*/

//! \brief A class that assembles a series of excerpts from a byte stream (possibly out of order,
//! possibly overlapping) into an in-order byte stream.
class StreamReassembler {
  private:
    // Your code here -- add private members as necessary.

    ByteStream _output;  //!< The reassembled in-order byte stream
    size_t _capacity;    //!< The maximum number of bytes
    size_t first_unread;
    size_t first_unassembled;
    size_t first_unacceptable;
    //unordered_map<size_t,string> unorder_data;
    //list<size_t> unorder_index;
    //set<UnorderData> outoforderdata;
    //vector<char> data;
    bool _eof;
   
     size_t unassembled_bytes_cnt;
    std::map<size_t,string> unorder_data = {};




  public:
    //! \brief Construct a `StreamReassembler` that will store up to `capacity` bytes.
    //! \note This capacity limits both the bytes that have been reassembled,
    //! and those that have not yet been reassembled.

    StreamReassembler(const size_t capacity);

    //! \brief Receive a substring and write any newly contiguous bytes into the stream.
    //!
    //! The StreamReassembler will stay within the memory limits of the `capacity`.
    //! Bytes that would exceed the capacity are silently discarded.
    //!
    //! \param data the substring
    //! \param index indicates the index (place in sequence) of the first byte in `data`
    //! \param eof the last byte of `data` will be the last byte in the entire stream
    void push_substring(const std::string &data, const uint64_t index, const bool eof);

    //! \name Access the reassembled byte stream
    //!@{
    const ByteStream &stream_out() const { return _output; }
    ByteStream &stream_out() { return _output; }
    //!@}

    //! The number of bytes in the substrings stored but not yet reassembled
    //!
    //! \note If the byte at a particular index has been pushed more than once, it
    //! should only be counted once for the purpose of this function.
    size_t unassembled_bytes() const;

    //! \brief Is the internal state empty (other than the output stream)?
    //! \returns `true` if no substrings are waiting to be assembled
    bool empty() const;
     void check_if_need_merge(size_t index,const string& data);
     bool check_if_datasize_too_big(size_t index,string& data);
     bool write_output(size_t index, string& data);
     void insert_data(size_t index, const string& data);
    
};

#endif  // SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH
