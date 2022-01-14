#include "headers/char_buffer.h"

#include <cstdlib>

void log_err_and_exit(const char *);

void char_buffer::write(const void *source, uint_32 len) {
    buffer.append((char *)source, len);
}

unique_ptr<uint_8[]> char_buffer::read(uint_32 len) {
    if(len > buffer.size()) log_err_and_exit("remove length out of size");
    auto out_buffer = unique_ptr<uint_8[]>(new uint_8[len]);
    buffer.copy((char *) out_buffer.get(), len);
    remove_front(len);
    return out_buffer;
}

void char_buffer::remove_front(uint_32 len) {
    if(len > buffer.size()) log_err_and_exit("remove length out of size");
    buffer = buffer.substr(len, buffer.size());
}

// 输出错误信息并退出程序
void log_err_and_exit(const char *msg){
    perror(msg);
    exit(-1);
}