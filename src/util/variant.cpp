#include "headers/variant.h"

//constexpr uint_8 HIGHEST = 1 << 7;
//constexpr uint_8 HIGHEST_REDUCE_1 = HIGHEST - 1;

uint_64 parse_variant(const void *bytes) {
    auto u_bytes = (uint_8 *) bytes;
    uint_64 res = 0; int count = 0;
    while(true){
        uint_64 temp = (*u_bytes & HIGHEST_REDUCE_1) << (count * 7);
        res = res | temp;
        if(!(*u_bytes & HIGHEST)) break;
        ++u_bytes; ++count;
    }
    return res;
}

uint_32 serialize_variant(uint_64 source, void *to) {
    auto u_bytes = (uint_8 *) to;
    uint_32 count = 0;
    while (true) {
        *u_bytes = *u_bytes | source;
        if(!(source >>= 7)) { *u_bytes &= HIGHEST_REDUCE_1; break; }
        *u_bytes |= HIGHEST; ++u_bytes; ++count;
    }
    return count;
}

uint_32 calculate_variant(uint_64 source) {
    uint_32 count = 1;
    while ((source >>= 7)) ++count;
    return count;
}
