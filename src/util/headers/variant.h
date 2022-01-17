#ifndef CHAT_SERVER_VARIANT_H
#define CHAT_SERVER_VARIANT_H

#include <cstdint>

typedef uint8_t uint_8;
typedef uint32_t uint_32;
typedef uint64_t uint_64;

constexpr uint_8 HIGHEST = 1 << 7;
constexpr uint_8 HIGHEST_REDUCE_1 = HIGHEST - 1;

// 判断一个字节的最高位是不是 1
inline bool has_more(uint_8 a_byte) { return a_byte & HIGHEST; }

// 将 @param2 低 7 位的内容追加到 @param1, @param3 用于标示当前操作的字节为 variant 编码的第 n 位, 返回 bool 表示是否还需要继续追加
inline bool append(uint_64 *source, uint_8 a_byte, uint_32 count) {
    uint_64 temp = (a_byte & HIGHEST_REDUCE_1) << (count * 7);
    *source = *source | temp;
    return has_more(a_byte);
}

// 解码变长整型, 参考 variant 实现
uint_64 parse_variant(const void *);

// 编码整型, 写入 void* 所指向的空间, 返回长度
uint_32 serialize_variant(uint_64, void *);

// 返回整型需要占用的字节数
uint_32 variant_len(uint_64 source);

#endif //CHAT_SERVER_VARIANT_H
