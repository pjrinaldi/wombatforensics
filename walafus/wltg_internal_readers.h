#pragma once


#include <fstream>
#include <memory>
#include <vector>
#include "util/types.h"
#include "util/chacha20.h"


class WltgBaseReadingStream
{
public:
    /**
     * @brief Read data from stream
     *
     * @param offset starting point in the stream to read from
     * @param dst destination pointer to read into
     * @param size maximum amount of bytes to read
     *
     * @return amount of bytes read
    */
    virtual ulong read(ulong offset, void* dst, ulong size) = 0;

    virtual ~WltgBaseReadingStream() = default;
};


class WltgRealReadingStream : public WltgBaseReadingStream
{
public:
    std::ifstream ifstream;
    ulong pos_offset;

    explicit WltgRealReadingStream(const char* filename, ulong pos_offset_)
            : ifstream(filename, std::ios::binary), pos_offset(pos_offset_) {}

    explicit WltgRealReadingStream(std::ifstream&& ifstream_, ulong pos_offset_)
            : ifstream(std::move(ifstream_)), pos_offset(pos_offset_) {}

    ulong read(ulong offset, void* dst, ulong size) override;
};


class WltgZstdDecompressionReadingStream : public WltgBaseReadingStream
{
public:
    std::unique_ptr<WltgBaseReadingStream> src_stream;
    uint block_size; // reference size of decompressed block
    uint compress_bound; // maximum size of compressed block, = ZSTD_compressBound(block_size);
    std::vector<ulong> block_offsets; // last block is dummy and only used to calculate size of penult block

    std::unique_ptr<ubyte[]> temp_compressed_buffer; // size: compress_bound
    std::unique_ptr<ubyte[]> temp_decompressed_buffer; // size: block_size

    WltgZstdDecompressionReadingStream(std::unique_ptr<WltgBaseReadingStream>&& src, uint block_size_,
                                       uint compress_bound_, std::vector<ulong>&& block_offsets_)
            : src_stream(std::move(src)), block_size(block_size_), compress_bound(compress_bound_),
              block_offsets(std::move(block_offsets_)),
              temp_compressed_buffer(std::make_unique<ubyte[]>(compress_bound)),
              temp_decompressed_buffer(std::make_unique<ubyte[]>(block_size)) {}

    ulong read(ulong offset, void* dst_, ulong size) override;
};


class WltgChaCha20DecryptionReadingStream : public WltgBaseReadingStream
{
public:
    std::unique_ptr<WltgBaseReadingStream> src_stream;
    ChaCha20State chacha20_state;

    WltgChaCha20DecryptionReadingStream(std::unique_ptr<WltgBaseReadingStream>&& src,
                                        const ubyte key[32], const ubyte nonce[12])
            : src_stream(std::move(src)), chacha20_state(key, nonce) {}

    ulong read(ulong offset, void* dst, ulong size) override;
};
