#ifndef CHAT_SERVER_BYTE_BUFFER_H
#define CHAT_SERVER_BYTE_BUFFER_H

#include <deque>
#include <string>
#include <utility>
#include <memory>
#include "../../generic/headers/size_types.h"

using std::string;
using std::move;
using std::unique_ptr;

class byte_buffer final {

public:
    explicit byte_buffer(): buffer(){}
    byte_buffer(byte_buffer &&_r_buffer) noexcept: buffer(move(_r_buffer.buffer)){}
    void write(const void *, uint_32);   // 写入缓冲区
    void remove_front(uint_32);          // 从头部移除

    const uint_8* inner_ptr(){ return (uint_8*) buffer.c_str(); }
    uint_32 size() { return buffer.size(); }

    byte_buffer(const byte_buffer &) = delete;
    byte_buffer& operator=(const byte_buffer &) = delete;

private:
    string buffer;

};


#endif //CHAT_SERVER_BYTE_BUFFER_H
