#include <iostream>
#include "wltg_internal_readers.h"
#include "zstd.h"


ulong WltgRealReadingStream::read(ulong offset, void* dst, ulong size) {
    ifstream.seekg(offset + pos_offset, std::ios::beg);
    ifstream.read((char*) dst, size);
    return ifstream.gcount();
}

ulong WltgZstdDecompressionReadingStream::read(ulong offset, void* dst_, ulong size) {
    auto initial_size = size;
    auto dst = (ubyte*) dst_;

    while (size) {
        auto block_num = offset / block_size;
        if (block_num >= block_offsets.size()) // yes, >=, there's one dummy block at the end
            return initial_size - size;

        auto compr_block_offset = block_offsets[block_num];
        auto compr_block_size = block_offsets[block_num + 1] - compr_block_offset;
        if (compr_block_size > compress_bound) {
            return initial_size - size;
        }

        src_stream->read(compr_block_offset, temp_compressed_buffer.get(), compr_block_size);
        auto decompressed_size = ZSTD_decompress(temp_decompressed_buffer.get(), block_size,
                                                 temp_compressed_buffer.get(), compr_block_size);
        if (ZSTD_isError(decompressed_size)) {
            std::cout << "decompression error" << std::endl;
            return initial_size - size;
        }

        auto actual_size = std::min(size, decompressed_size - (offset % block_size)); // min(size, available in block)
        if (!actual_size) {
            return initial_size - size;
        }
        memcpy(dst, (ubyte*) temp_decompressed_buffer.get() + (offset % block_size), actual_size);

        dst += actual_size;
        size -= actual_size;
        offset += actual_size;
    }

    return initial_size;
}

ulong WltgChaCha20DecryptionReadingStream::read(ulong offset, void* dst_, ulong size) {
    auto dst = (ubyte*) dst_;
    auto initial_size = size;

    while (size) {
        ubyte source_bytes[64];
        auto aligned_offset = offset / 64 * 64;
        auto read_cnt = src_stream->read(aligned_offset, source_bytes, 64);

        if (read_cnt != 64) {
            return initial_size - size;
        }

        chacha20_state.set_counter(offset / 64);
        chacha20_state.encrypt_block((uint *) source_bytes);

        auto read_pos = offset - aligned_offset;
        auto read_size = std::min(size, 64 - read_pos);
        memcpy(dst, &source_bytes[read_pos], read_size);

        offset += read_size;
        dst += read_size;
        size -= read_size;
    }

    return initial_size;
}
