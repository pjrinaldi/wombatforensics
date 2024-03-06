#pragma once


#include <unordered_map>
#include <iostream>
#include <memory>
#include "filesystem.h"
#include "wltg_internal_readers.h"
#include "wltg_structure.h"


#define WLTG_MAX_INODE_CNT 10'000'000
#define WLTG_MAX_STRING_POOL_SIZE 100'000'000
#define WLTG_MAX_COMPRESSION_HEADER_SIZE 8'000'000
#define WLTG_MIN_BLOCK_SIZE (1 << 10)
#define WLTG_MAX_BLOCK_SIZE (1 << 25)


class WltgReaderFileSystemNode
{
public:
    std::string_view name;
    std::unordered_map<std::string_view, WltgReaderFileSystemNode*> children;
    ulong data_offset;
    ulong data_size;

    void print_contents(int indent = 0) const {
        for (int i = 0; i < indent; ++i)
            std::cout << "|- ";
        std::cout << name << std::endl;

        for (const auto& [node_name, node] : children) {
            node->print_contents(indent + 1);
        }
    }
};


class WltgReader;
class WltgFileStream : public BaseFileStream
{
public:
    WltgReader* reader;
    WltgReaderFileSystemNode* node;
    ulong read_pos = 0;

    WltgFileStream* prev;
    WltgFileStream* next;

    WltgFileStream(WltgReader* reader_, WltgReaderFileSystemNode* node_, WltgFileStream* prev_, WltgFileStream* next_);

    void seek(ulong offset) override;

    [[nodiscard]] ulong tell() const override;

    [[nodiscard]] ulong size() const override;

    ulong read_into(void* dst, ulong size) override;

    [[nodiscard]] bool is_closed() const override;

    ~WltgFileStream() override;
};


class WltgReader : public BaseFilesystemSource
{
public:
    WltgReaderFileSystemNode* root = nullptr;
    std::vector<WltgReaderFileSystemNode> nodes;
    std::unique_ptr<char[]> string_pool{};
    uint string_pool_size{};

    std::unique_ptr<WltgBaseReadingStream> read_stream{};
    WltgFileStream* handles_list = nullptr;

    explicit WltgReader(const char* real_path, const ubyte encryption_key[32] = nullptr) {
        read_fs(real_path, encryption_key);
    }

    WltgReader(const char* real_path, const char* password) {
        if (password) {
            ubyte encryption_key[32];
            hash_password(password, encryption_key);
            std::cout << "dec key: " << (int) encryption_key[0] << " " << (int) encryption_key[1] << std::endl;
            read_fs(real_path, encryption_key);
            secure_zero_mem(encryption_key, sizeof(encryption_key));
        }
        else {
            read_fs(real_path, nullptr);
        }
    }

    [[nodiscard]] WltgReaderFileSystemNode* lookup_path(std::string_view path) const;

    std::unique_ptr<BaseFileStream> open_file_read(std::string_view filename) override;

    bool is_path_exists(std::string_view path) override;

    ~WltgReader() override;

protected:
    void read_fs(const char* real_path, const ubyte encryption_key[32]);

    void recursive_parse_inode(WltgReaderFileSystemNode* parent, Inode* inodes, uint inode_index);
};
