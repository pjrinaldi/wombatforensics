#include <stack>
#include "../include/walafus/wltg_reader.h"
#include "../include/walafus/errors.h"


#define CHECK_READ_ERROR(condition, description, ...) \
do if (condition) {                               \
    if (walafus_on_error(WalafusFilesystemErrorCode::FILE_READ_ERROR, description, this)) \
        throw std::runtime_error(description);                                        \
    return __VA_ARGS__;                        \
} while(0)


void WltgReader::read_fs(const char* real_path, const ubyte encryption_key[32]) {
    read_stream = std::move(std::make_unique<WltgRealReadingStream>(real_path, 0));
    auto real_fp_stream = static_cast<WltgRealReadingStream*>(read_stream.get());
    auto& src_fp = real_fp_stream->ifstream;
    CHECK_READ_ERROR(!src_fp.good(), "[WLTG.reader]: failed to open file");

    // reading header
    FileHeader header;
    src_fp.read((char*) &header, sizeof(header));
    CHECK_READ_ERROR(src_fp.gcount() != sizeof(header),
                     "[WLTG.reader]: failed to read header");

    CHECK_READ_ERROR(header.magic != 0x77889945,
                     "[WLTG.reader]: failed to read; invalid magic");
    CHECK_READ_ERROR(header.inode_count > WLTG_MAX_INODE_CNT,
                     "[WLTG.reader]: failed to read; maximum number if inodes reached");
    CHECK_READ_ERROR(header.inode_count == 0,
                     "[WLTG.reader]: failed to read; no inodes present in the file");
    CHECK_READ_ERROR(header.string_pool_size > WLTG_MAX_STRING_POOL_SIZE,
                     "[WLTG.reader]: failed to read; maximum size of string pool reached");

    // reading inodes
    auto inodes = std::make_unique<Inode[]>(header.inode_count);
    nodes.resize(header.inode_count);
    src_fp.read((char*) inodes.get(), header.inode_count * sizeof(Inode));
    CHECK_READ_ERROR(src_fp.gcount() != header.inode_count * sizeof(Inode),
                     "[WLTG.reader]: failed to read inodes list");

    // reading strings
    string_pool = std::make_unique<char[]>(header.string_pool_size + 1);
    string_pool[header.string_pool_size] = '\0';
    string_pool_size = header.string_pool_size;
    src_fp.read(string_pool.get(), header.string_pool_size);
    CHECK_READ_ERROR(src_fp.gcount() != header.string_pool_size,
                     "[WLTG.reader]: failed to read string pool");

    ulong file_data_offset = (ulong) src_fp.tellg() + header.encryption_header_size + header.compression_header_size;
    real_fp_stream->pos_offset = file_data_offset;

    // reading encryption info
    CHECK_READ_ERROR(header.encryption_header_size &&
                     header.encryption_header_size != sizeof(EncryptionInfo) + sizeof(ChaCha20EncryptionInfo),
                     "[WLTG.reader]: failed to read; invalid encryption header size");
    CHECK_READ_ERROR(header.encryption_header_size && encryption_key == nullptr,
                     "[WLTG.reader]: failed to read; encryption key required, but not provided");
    if (header.encryption_header_size) {
        // reading base encryption header
        EncryptionInfo encryption_info;
        src_fp.read((char*) &encryption_info, sizeof(encryption_info));
        CHECK_READ_ERROR(src_fp.gcount() != sizeof(encryption_info),
                         "[WLTG.reader]: failed to read encryption header");

        // checking it is ChaCha20
        const char chacha20_string[] = {'C', 'h', 'a', 'C', 'h', 'a', '2', '0'};
        CHECK_READ_ERROR(encryption_info.type != *(ulong*) chacha20_string,
                         "[WLTG.reader]: failed to read; invalid encryption method");

        // reading ChaCha20 encryption header
        ChaCha20EncryptionInfo chacha20_encryption_info;
        src_fp.read((char*) &chacha20_encryption_info, sizeof(chacha20_encryption_info));
        CHECK_READ_ERROR(src_fp.gcount() != sizeof(chacha20_encryption_info),
                         "[WLTG.reader]: failed to read ChaCha20 encryption header");

        // checking encryption key validness
        ubyte check_hash[32];
        ChaCha20State::encrypt_and_hash_sha256(encryption_key, chacha20_encryption_info.nonce,
                                               chacha20_encryption_info.encrypt_nonce,
                                               sizeof(chacha20_encryption_info.encrypt_nonce),
                                               check_hash);
        CHECK_READ_ERROR(!!memcmp(check_hash, chacha20_encryption_info.encrypted_validation_hash, sizeof(check_hash)),
                         "[WLTG.reader]: failed to read; invalid encryption key");

        // adding decryption stream
        read_stream = std::move(
                std::make_unique<WltgChaCha20DecryptionReadingStream>(std::move(read_stream),
                                                                      encryption_key,
                                                                      chacha20_encryption_info.nonce));
    }

    // reading compression info
    CHECK_READ_ERROR(header.compression_header_size > WLTG_MAX_COMPRESSION_HEADER_SIZE,
                     "[WLTG.reader]: failed to read; maximum size of compression header reached");
    CHECK_READ_ERROR(header.compression_header_size && header.compression_header_size < sizeof(CompressionInfo),
                     "[WLTG.reader]: failed to read; compression header is smalled than required");
    if (header.compression_header_size) {
        std::vector<char> compression_info_storage(header.compression_header_size);
        auto compression_info = (CompressionInfo*) compression_info_storage.data();
        src_fp.read((char*) compression_info, header.compression_header_size);

        const char zstd_name[] = {'Z', 'S', 'T', 'D'};
        CHECK_READ_ERROR(src_fp.gcount() != header.compression_header_size,
                         "[WLTG.reader]: failed to read compression header");
        CHECK_READ_ERROR(compression_info->type != *(uint*) zstd_name,
                         "[WLTG.reader]: failed to read; unsupported compression type");
        CHECK_READ_ERROR(compression_info->block_size > WLTG_MAX_BLOCK_SIZE,
                         "[WLTG.reader]: failed to read; maximum size of uncompressed block size reached");
        CHECK_READ_ERROR(compression_info->block_size < WLTG_MIN_BLOCK_SIZE,
                         "[WLTG.reader]: failed to read; compression block size less than required");

        // writing compression block offsets
        auto block_count = (header.data_size + compression_info->block_size - 1) / compression_info->block_size + 1;
        CHECK_READ_ERROR(block_count * sizeof(CompressedBlock) > header.compression_header_size - sizeof(CompressionInfo),
                         "[WLTG.reader]: failed to read; not enough size for compression blocks");
        std::vector<ulong> block_offsets(block_count);
        for (int i = 0; i < block_count; ++i)
            block_offsets[i] = compression_info->blocks[i].start;

        // adding decompression stream
        read_stream = std::move(
                std::make_unique<WltgZstdDecompressionReadingStream>(std::move(read_stream),
                                                                     compression_info->block_size,
                                                                     compression_info->compress_bound,
                                                                     std::move(block_offsets)));
    }

    // parsing inode list
    root = &nodes[0];
    recursive_parse_inode(root, inodes.get(), 0);
    root->children.erase("/");
}

WltgReaderFileSystemNode* WltgReader::lookup_path(std::string_view path) const {
    std::stack<WltgReaderFileSystemNode*> nodes_stack;
    nodes_stack.push(root);

    while (!path.empty()) {
        auto slash_index = path.find('/');
        auto substring_last = slash_index == std::string_view::npos ? path.size() : slash_index;
        auto substring = path.substr(0, substring_last);

        if (substring.empty() || substring == ".") {
            path.remove_prefix(substring_last + 1);
            continue;
        }
        if (substring == "..") {
            if (nodes_stack.size() > 1)
                nodes_stack.pop();
            path.remove_prefix(substring_last + 1);
            continue;
        }

        auto iter = nodes_stack.top()->children.find(substring);
        if (iter == nodes_stack.top()->children.end()) {
            return nullptr;
        }
        nodes_stack.push(iter->second);

        path.remove_prefix(std::min(substring_last + 1, path.size()));
    }

    return nodes_stack.top();
}

void WltgReader::recursive_parse_inode(WltgReaderFileSystemNode* parent, Inode* inodes, uint inode_index) {
    while (inode_index != -1) {
        auto curr_node = &nodes[inode_index];
        auto inode = &inodes[inode_index];

        CHECK_READ_ERROR(inode->string_name >= string_pool_size,
                         "[WLTG.reader]: failed to read; referenced string outside of string pool");
        CHECK_READ_ERROR(inode->child_node_id != -1 && inode->child_node_id >= nodes.size(),
                         "[WLTG.reader]: failed to read; referenced child inode outside of inode array");
        CHECK_READ_ERROR(inode->next_node_id != -1 && inode->next_node_id >= nodes.size(),
                         "[WLTG.reader]: failed to read; referenced next inode outside of inode array");

        curr_node->name = std::string_view(&string_pool[inode->string_name]);
        curr_node->data_size = inode->data_size;
        curr_node->data_offset = inode->file_data;
        parent->children[curr_node->name] = curr_node;

        if (inode->child_node_id != -1) {
            recursive_parse_inode(curr_node, inodes, inode->child_node_id);
        }

        inode_index = inode->next_node_id;
    }
}

std::unique_ptr<BaseFileStream> WltgReader::open_file_read(std::string_view filename) {
    auto node = lookup_path(filename);
    if (!node)
        return nullptr;
    if (node->data_offset == -1)
        return nullptr;

    auto stream = std::make_unique<WltgFileStream>(this, node, nullptr, handles_list);
    if (stream->next)
        stream->next->prev = stream.get();
    handles_list = stream.get();
    return stream;
}

WltgReader::~WltgReader() {
    if (handles_list) {
        walafus_on_error(WalafusFilesystemErrorCode::FILE_WARNING,
                         "[WLTG.reader]: closing reader while some handles are alive", this);
    }
    for (auto handle = handles_list; handle; handle = handle->next) {
        handle->reader = nullptr;
        handle->node = nullptr;
    }
}

bool WltgReader::is_path_exists(std::string_view path) {
    return lookup_path(path) != nullptr;
}


WltgFileStream::WltgFileStream(WltgReader* reader_, WltgReaderFileSystemNode* node_,
                               WltgFileStream* prev_, WltgFileStream* next_)
: reader(reader_), node(node_), prev(prev_), next(next_) { }

void WltgFileStream::seek(ulong offset) {
    read_pos = offset;
}

ulong WltgFileStream::tell() const {
    return read_pos;
}

ulong WltgFileStream::read_into(void* dst, ulong size) {
    if (!node) return 0;
    return reader->read_stream->read(node->data_offset + read_pos, dst, size);
}

ulong WltgFileStream::size() const {
    if (!node) return 0;
    return node->data_size;
}

bool WltgFileStream::is_closed() const {
    return !node;
}

WltgFileStream::~WltgFileStream() {
    if (reader && reader->handles_list == this)
        reader->handles_list = next;
    if (prev)
        prev->next = next;
    if (next)
        next->prev = prev;
}
