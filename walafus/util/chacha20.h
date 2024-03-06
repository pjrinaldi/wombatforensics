#pragma once

#include <cstdint>
#include <cstring>
#include "types.h"
#include "crypto.h"
#include "sha256.h"


inline uint32_t chacha20_rotl32(uint32_t x, int n) {
    return (x << n) | (x >> (32 - n));
}


inline void chacha20_process_block(uint32_t block[16]) {
#define CHACHA20_QUARTERROUND(x, a, b, c, d) \
    x[a] += x[b]; x[d] = chacha20_rotl32(x[d] ^ x[a], 16); \
    x[c] += x[d]; x[b] = chacha20_rotl32(x[b] ^ x[c], 12); \
    x[a] += x[b]; x[d] = chacha20_rotl32(x[d] ^ x[a], 8); \
    x[c] += x[d]; x[b] = chacha20_rotl32(x[b] ^ x[c], 7);

    for (int i = 0; i < 10; i++)
    {
        CHACHA20_QUARTERROUND(block, 0, 4, 8, 12)
        CHACHA20_QUARTERROUND(block, 1, 5, 9, 13)
        CHACHA20_QUARTERROUND(block, 2, 6, 10, 14)
        CHACHA20_QUARTERROUND(block, 3, 7, 11, 15)
        CHACHA20_QUARTERROUND(block, 0, 5, 10, 15)
        CHACHA20_QUARTERROUND(block, 1, 6, 11, 12)
        CHACHA20_QUARTERROUND(block, 2, 7, 8, 13)
        CHACHA20_QUARTERROUND(block, 3, 4, 9, 14)
    }
#undef CHACHA20_QUARTERROUND
}


inline void chacha20_xor_block(uint32_t* __restrict dst_block, uint32_t* __restrict src_block) {
    for (int i = 0; i < 16; i++)
        dst_block[i] ^= src_block[i];
}


class ChaCha20State
{
public:
    uint state[16];

    ChaCha20State(const ubyte key[32], const ubyte nonce[12]) {
        state[0] = 0x61707865;
        state[1] = 0x3320646e;
        state[2] = 0x79622d32;
        state[3] = 0x6b206574;
        memcpy(&state[4], key, sizeof(ubyte[32]));
        state[12] = 0;
        memcpy(&state[13], nonce, sizeof(ubyte[12]));
    }

    void set_counter(uint value) {
        state[12] = value;
    }

    uint get_counter() {
        return state[12];
    }

    void increment_counter() {
        state[12] += 1;
    }

    void encrypt_block(uint block[16]) {
        uint local_chacha_block[16];
        memcpy(local_chacha_block, state, sizeof(state));
        chacha20_process_block(local_chacha_block);
        chacha20_xor_block(block, local_chacha_block);
    }

    ~ChaCha20State() {
        secure_zero_mem((volatile ubyte*) state, sizeof(state));
    }

    static void encrypt_and_hash_sha256(const ubyte key[32], const ubyte nonce[12],
                                        const ubyte* encrypt_data, size_t size, ubyte hash_out[256]) {
        uint local_block[16];
        ChaCha20State chacha20_state(key, nonce);
        sha256_state sha256_state;
        sha256_init(&sha256_state);

        while (size) {
            memset(local_block, 0, 64);
            memcpy(local_block, encrypt_data, std::min(size, (size_t) 64));
            chacha20_state.encrypt_block(local_block);
            chacha20_state.increment_counter();
            sha256_append(&sha256_state, local_block, 64);

            size -= std::min(size, (size_t) 64);
        }

        sha256_finalize_bytes(&sha256_state, hash_out);
    }
};
