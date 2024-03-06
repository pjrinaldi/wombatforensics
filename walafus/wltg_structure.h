#pragma once
#include "util/types.h"

typedef ulong fileptr;

#pragma pack(push, 1)


struct DataBlockInfo
{
    fileptr start;
    uint block_size;
};


enum struct InodeType : uint
{
    UNKNOWN = 0,
    DIRECTORY,
    FILE
};


struct Inode
{
    uint string_name; // offset in string pool to null-terminated string
    uint next_node_id; // next node index or -1 (0xFFFFFFFF)
    uint child_node_id; // node id or -1 (0xFFFFFFFF)
    ulong data_size;
    fileptr file_data; // file ptr if data exists or -1
};


struct CompressedBlock
{
    fileptr start;
};


struct CompressionInfo
{
    uint type;
    uint block_size;
    uint compress_bound;
    CompressedBlock blocks[0];
};


struct ChaCha20EncryptionInfo
{
    ubyte nonce[12]; // ChaCha20 nonce
    ubyte encrypt_nonce[20]; // random bytes to encrypt, hash, and check validity
    ubyte encrypted_validation_hash[32]; // SHA-256
};


struct EncryptionInfo
{
    ulong type;
};


struct FileHeader
{
    uint magic;

    uint inode_count;
    uint string_pool_size;
    ulong data_size;

    ulong encryption_header_size;
    ulong compression_header_size;

    union { // union here is to silence compilation error on MSVC. in fact, this data go sequentially
        Inode inodes[0];
        ubyte string_pool[0]; // any collection of null-terminated strings. `string_pool_size` bytes
        DataBlockInfo blocks[0];
        ubyte data[0];
    };
};

#pragma pack(pop)
