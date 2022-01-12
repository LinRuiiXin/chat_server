#ifndef CHAT_SERVER_CHAR_BUFFER_H
#define CHAT_SERVER_CHAR_BUFFER_H

#include <deque>
#include <utility>
#include <memory>

using std::deque;
using std::move;
using std::unique_ptr;

typedef unsigned int uint_32;
typedef unsigned long long uint_64;
typedef deque<char>::iterator buf_it;

class char_buffer final {

public:
    explicit char_buffer(): buffer(){}
    char_buffer(char_buffer &&_r_buffer) noexcept: buffer(move(_r_buffer.buffer)){}
    void write(const char*, uint_32); // 写入缓冲区
    unique_ptr<char[]> read(uint_64); // 读取若干个字符, 返回字符数组指针

    buf_it begin() { return buffer.begin(); }
    buf_it end() { return buffer.end(); }
    uint_32 size() { return buffer.size(); }

    char_buffer(const char_buffer &) = delete;
    char_buffer& operator=(const char_buffer &) = delete;

private:
    deque<char> buffer;

};


#endif //CHAT_SERVER_CHAR_BUFFER_H
