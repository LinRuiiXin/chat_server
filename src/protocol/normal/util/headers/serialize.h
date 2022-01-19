#ifndef CHAT_SERVER_SERIALIZE_H
#define CHAT_SERVER_SERIALIZE_H

#include <string>
#include "../../../../generic/headers/size_types.h"
#include "../../../../util/headers/variant.h"

using std::string;
using std::move;
using std::unique_ptr;

typedef struct {
    unique_ptr<uint_8[]> buffer;
    uint_64 len;
} serialize_buffer;

template <typename T>
T parse_from_bytes(const void *, uint_32);

template <typename T>
serialize_buffer serialize_as_buffer(T &t);

template<typename T>
T parse_from_bytes(const void *buffer, uint_32 len) {
    const uint_8 *bytes = (uint_8*) buffer;
    T t;
    uint_32 head_f_len = variant_len(len);
    t.ParseFromArray(bytes + head_f_len, len - head_f_len);
    return move(t);
}

template <typename T>
serialize_buffer serialize_as_buffer(T &t) {
    uint_64 msg_len = t.ByteSizeLong();
    uint_64 head_f_len = variant_len(msg_len + variant_len(msg_len));
    uint_64 full_len = msg_len + head_f_len;
    unique_ptr<uint_8[]> byte_buffer(new uint_8[full_len]);
    auto *buffer_inner_ptr = byte_buffer.get();
    serialize_variant(full_len, buffer_inner_ptr);
    t.SerializeToArray(buffer_inner_ptr + head_f_len, msg_len);
    return serialize_buffer{ move(byte_buffer), full_len };
}

#endif //CHAT_SERVER_SERIALIZE_H
