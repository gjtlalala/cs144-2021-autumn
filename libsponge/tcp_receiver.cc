#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;
#include <iostream>
void TCPReceiver::segment_received(const TCPSegment &seg) {
    TCPHeader head = seg.header();
    if (head.syn) {
        _isn = head.seqno.raw_value();  // omit syn
                                        //_isn.emplace(head.seqno);
        // return;//SYN包可能带内容，所以不能直接return
        _syn = true;
    }
    if (!_syn)
        return;

    if (head.fin) {
        _fin = true;
    }
    WrappingInt32 psn = head.seqno;
    // uint64_t index = unwrap(psn, _isn.value(), _checkpoint);
    uint64_t absolute_seqno = unwrap(psn, WrappingInt32(_isn), _checkpoint);
    _checkpoint = absolute_seqno;
    if (!absolute_seqno)
        cout << "absolute_seqno = 0" << endl;
    _reassembler.push_substring(seg.payload().copy(), head.syn ? 0 : absolute_seqno - 1, _fin);
    // 有种错误是，syn 下一个包seqno也为syn，这时因为index大于所以不会写入,
    //所以要用head.syn做判断条件，不是absolutw_seqno == 0,否则错误seqno包的内容会写入
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    if (!_syn)
        return {};
    size_t shift = 1;
    if (_fin && _reassembler.stream_out().input_ended())  //可能乱序到达，确保end才加2，因为fin包没有内容，所以要+2
                                                          // if(_fin)
        shift = 2;
    return wrap(_reassembler.get_first_unassembled() + shift, WrappingInt32(_isn));
}

size_t TCPReceiver::window_size() const { return _reassembler.stream_out().remaining_capacity(); }
