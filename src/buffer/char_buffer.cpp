#include "headers/char_buffer.h"

#include <cstdlib>

void log_err_and_exit(const char *);

void char_buffer::write(const char *source, uint_32 len) {
    for(const char *s_ptr = source; s_ptr < source + len; ++s_ptr)
        buffer.push_back(*s_ptr);
}

// 从字符队列中读取 nums 数量的字符到动态数组中，并返回
unique_ptr<char[]> char_buffer::read(uint_32 nums) {
    if(nums > buffer.size()) log_err_and_exit("[char_buffer] read size out of bounds\n");
    unique_ptr<char[]> out_buffer(new char[nums + 1]);
    for (int i = 0; i < nums; ++i) {
        out_buffer[i] = buffer.front();
        buffer.pop_front();
    }
    out_buffer[nums] = '\0';
    return out_buffer;
}

// 输出错误信息并退出程序
void log_err_and_exit(const char *msg){
    perror(msg);
    exit(-1);
}