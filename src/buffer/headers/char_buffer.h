#ifndef CHAT_SERVER_CHAR_BUFFER_H
#define CHAT_SERVER_CHAR_BUFFER_H

#include <deque>
#include <string>
#include <utility>
#include <memory>

using std::string;
using std::move;
using std::unique_ptr;

typedef unsigned char       uint_8;
typedef unsigned int        uint_32;
typedef unsigned long long  uint_64;

class char_buffer final {

public:
    explicit char_buffer(): buffer(){}
    char_buffer(char_buffer &&_r_buffer) noexcept: buffer(move(_r_buffer.buffer)){}
    void write(const void *, uint_32);   // 写入缓冲区
    unique_ptr<uint_8[]> read(uint_32);  // 读取 n 字节
    void remove_front(uint_32);          // 从头部移除

    const uint_8* inner_ptr(){ return (uint_8*) buffer.c_str(); }
    uint_32 size() { return buffer.size(); }

    char_buffer(const char_buffer &) = delete;
    char_buffer& operator=(const char_buffer &) = delete;

private:
    string buffer;

};


#endif //CHAT_SERVER_CHAR_BUFFER_H
