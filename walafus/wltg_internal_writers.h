#pragma once


#include <fstream>
#include <memory>
#include <vector>
#include <zstd.h>
#include "util/types.h"
#include "util/chacha20.h"


class WltgBaseWritingStream
{
public:
    /**
     * @brief Write data to stream
     *
     * @param data pointer to data to read from
     * @param size amount of bytes to read from pointer and write to stream
     */
    virtual void write(const void* data, size_t size) = 0;

    /**
     * @brief Flush internal stream caches if there are any
     */
    virtual void flush() {}

    /**
     * @brief Write data to stream from std::ifstream
     *
     * @param src_fp std::ifstream to read from
     */
    virtual void write(std::ifstream& src_fp) {
        ubyte buf[1 << 18];
        do {
            src_fp.read((char*) buf, sizeof(buf));
            write(buf, src_fp.gcount());
        } while (src_fp.gcount() == sizeof(buf));
    }

    virtual ~WltgBaseWritingStream() {};
};


class WltgRealFileSystemWritingStream : public WltgBaseWritingStream
{
public:
    std::ofstream ofstream;

    WltgRealFileSystemWritingStream(std::ofstream&& dst) : ofstream(std::move(dst)) {}

    WltgRealFileSystemWritingStream(const char* filename) : ofstream(filename, std::ios::binary) {}

    void write(const void* data, size_t size) override;

    void flush() override;
};


class WltgZstdCompressionWritingStream : public WltgBaseWritingStream
{
public:
    uint compress_bound;
    std::unique_ptr<ubyte[]> compr_buf;
    WltgBaseWritingStream* dst_stream;
    uint max_size;
    int compression_level;
    std::vector<size_t> write_counts; // written compressed chunk sizes

    WltgZstdCompressionWritingStream(WltgBaseWritingStream* dst_stream_, uint max_size_, int compression_level_)
            : compress_bound(ZSTD_compressBound(max_size_)), compr_buf(std::make_unique<ubyte[]>(compress_bound)),
              max_size(max_size_), compression_level(compression_level_), dst_stream(dst_stream_) {}

    void write(const void* data, size_t size) override;
};


class WltgBufferedWritingStream : public WltgBaseWritingStream
{
public:
    std::unique_ptr<ubyte[]> dst_buf;
    uint block_size;
    uint fill_level{};
    WltgBaseWritingStream* dst_stream;

    WltgBufferedWritingStream(WltgBaseWritingStream* dst_stream_, uint block_size_)
            : dst_buf(std::make_unique<ubyte[]>(block_size_)), block_size(block_size_), dst_stream(dst_stream_) {}

    void write(const void* data_, size_t size) override;

    void flush() override;
};


class WltgChaCha20WritingStream : public WltgBaseWritingStream
{
public:
    ChaCha20State chacha20_state;
    ubyte cached_bytes[64];
    uint cache_size{};
    WltgBaseWritingStream* dst_stream;

    WltgChaCha20WritingStream(WltgBaseWritingStream* dst_stream_, const ubyte key[32], const ubyte nonce[12]);

    void write(const void* data, size_t size) override;

    void flush() override;
};
