#pragma once

#include "types.h"
#include "sha256.h"


inline void secure_zero_mem(volatile ubyte* ptr, size_t size) {
    // yes, this can be optimized to SecureZeroMemory, manual AVX/SSE instructions,
    // but this function is called so rare, that it will not impact performance at all
    for (size_t i = 0; i < size; ++i)
        ptr[i] = 0;
}

#define PBKDF2_MAX(x, y) ((x) > (y) ? (x) : (y))
#define PBKDF2_MIN(x, y) ((x) < (y) ? (x) : (y))

inline void pbkdf2(void (*pkf)(const void* src, size_t size, void* hash_dst), size_t hash_size,
                   const char* password,
                   const void* salt, size_t salt_size,
                   int iter_count,
                   void* out_key, size_t key_size)
{
    int password_size = strlen(password) + 1;
    int block_count = (key_size + hash_size - 1) / hash_size;

    int hash_source_size = password_size + PBKDF2_MAX(salt_size + sizeof(uint32_t), hash_size);
    uint8_t* block = (uint8_t*) alloca(hash_size);
    uint8_t* hash_source = (uint8_t*) alloca(hash_source_size);
    uint8_t* hash_out = (uint8_t*) alloca(hash_size);

    for (uint32_t block_i = 0; block_i < block_count; ++block_i) {
        memset(block, 0, hash_size);

        memset(hash_source, 0, hash_source_size);
        memcpy(hash_source, password, password_size);
        memcpy(&hash_source[password_size], salt, salt_size);
        memcpy(&hash_source[password_size + salt_size], &block_i, sizeof(uint32_t));

        for (int iter_i = 0; iter_i < iter_count; ++iter_i) {
            pkf(hash_source, hash_source_size, hash_out);

            for (int i = 0; i < hash_size; ++i)
                block[i] ^= hash_out[i];
            memcpy(&hash_source[password_size], block, salt_size);
            // not zeroing remaining memory (if there are any) after copying salt
        }

        //memcpy(&((uint8_t(*)[hash_size])&out_key)[block_i], block,
        //       PBKDF2_MIN(hash_size, key_size - (block_i * hash_size)));

        memcpy(((uint8_t*)out_key) + hash_size * block_i, block,
               PBKDF2_MIN(hash_size, key_size - (block_i * hash_size)));
    }

    secure_zero_mem(hash_out, hash_size);
    secure_zero_mem(hash_source, hash_source_size);
    secure_zero_mem(block, hash_size);
}

#undef PBKDF2_MIN
#undef PBKDF2_MAX


inline void hash_password(const char* password, ubyte out_key[32]) {
    const ubyte salt[] = {0x82, 0xFF, 0x78, 0x11, 0x91, 0x58, 0x37, 0x55};
    pbkdf2(
            [](const void* src, size_t size, void* dst) {
                sha256_state sha256_state;
                sha256_init(&sha256_state);
                sha256_append(&sha256_state, src, size);
                sha256_finalize_bytes(&sha256_state, dst);
            }, 32,
            password,
            salt, sizeof(salt),
            64,
            out_key, 32);
}
