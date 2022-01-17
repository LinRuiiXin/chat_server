#include "headers/byte_buffer.h"

#include <cstdlib>

void log_err_and_exit(const char *);

void byte_buffer::write(const void *source, uint_32 len) {
    buffer.append((char *)source, len);
}

void byte_buffer::remove_front(uint_32 len) {
    if(len > buffer.size()) log_err_and_exit("remove length out of size");
    buffer = buffer.substr(len, buffer.size());
}

// 输出错误信息并退出程序
void log_err_and_exit(const char *msg){
    perror(msg);
    exit(-1);
}