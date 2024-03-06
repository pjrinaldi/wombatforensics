#include <iostream>
#include "wltg_internal_writers.h"
#include "util/chacha20.h"

void WltgRealFileSystemWritingStream::write(const void* data, size_t size) {
    ofstream.write((const char*) data, size);
}

void WltgRealFileSystemWritingStream::flush() {
    ofstream.flush();
}

void WltgZstdCompressionWritingStream::write(const void* data, size_t size) {
    auto compressed_size = ZSTD_compress(compr_buf.get(), compress_bound, data, size, compression_level);
    std::cout << "writing " << write_counts.size() << "th block; " << size << " source bytes as " <<
              compressed_size << " compressed" << std::endl;
    write_counts.push_back(compressed_size);
    dst_stream->write(compr_buf.get(), compressed_size);
}

void WltgBufferedWritingStream::write(const void* data_, size_t size) {
    auto data = (ubyte*) data_;
    while (size) {
        auto chunk_size = std::min(size, (size_t) block_size - fill_level);
        memcpy((ubyte*) dst_buf.get() + fill_level, data, chunk_size);

        size -= chunk_size;
        fill_level += chunk_size;
        data += chunk_size;

        if (fill_level == block_size) {
            dst_stream->write(dst_buf.get(), fill_level);
            fill_level = 0;
        }
    }
}

void WltgBufferedWritingStream::flush() {
    if (fill_level)
        dst_stream->write(dst_buf.get(), fill_level);
}

void WltgChaCha20WritingStream::write(const void* data_, size_t size) {
    auto data = (ubyte*) data_;

    while (size) {
        auto write_to_cache_size = std::min((size_t) 64 - cache_size, size);
        memcpy(&cached_bytes[cache_size], data, write_to_cache_size);

        data += write_to_cache_size;
        size -= write_to_cache_size;
        cache_size += write_to_cache_size;

        if (cache_size == 64)
            WltgChaCha20WritingStream::flush();
    }
}

void WltgChaCha20WritingStream::flush() {
    if (!cache_size)
        return;

    chacha20_state.encrypt_block((uint *) cached_bytes);
    chacha20_state.increment_counter();
    dst_stream->write(cached_bytes, 64);
    cache_size = 0;
}

WltgChaCha20WritingStream::WltgChaCha20WritingStream(WltgBaseWritingStream* dst_stream_,
                                                     const ubyte key[32], const ubyte nonce[12])
: dst_stream(dst_stream_), chacha20_state(key, nonce) { }
