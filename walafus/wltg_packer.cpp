#include <queue>
#include <stack>
#include <random>
#include <algorithm>
#include "wltg_packer.h"
#include "wltg_structure.h"
#include "wltg_internal_writers.h"

WltgPackerNode* WltgPackerNode::operator[](std::string& child_name)
{
    auto next_node = children.try_emplace(child_name);
    if (next_node.second)
        next_node.first->second = std::make_unique<WltgPackerNode>(child_name);
    return &*next_node.first->second;
}

WltgPacker::WltgPacker() : root(std::make_unique<WltgPackerNode>("/")) { }

WltgPackerNode* WltgPacker::lookup_path(const std::filesystem::path& path) {
    std::stack<WltgPackerNode*> nodes_stack;
    nodes_stack.push(&*root);

    for (const auto& token : path) {
        if (token.empty() || token == "/" || token == ".")
            continue;
        if (token == "..") {
            if (nodes_stack.size() > 1)
                nodes_stack.pop();
            continue;
        }
        auto token_name = token.string();
        nodes_stack.push((*nodes_stack.top())[token_name]);
    }

    return nodes_stack.top();
}

void WltgPacker::index_real_dir(const std::filesystem::path& virtual_path, const std::filesystem::path& real_path) {
    auto curr_node = lookup_path(virtual_path);

    index_real_dir(curr_node, real_path);
}

void WltgPacker::index_real_dir(WltgPackerNode* virtual_location, const std::filesystem::path& real_path) {
    for (const auto& entry : std::filesystem::directory_iterator(real_path)) {
        auto filename = entry.path().filename().string();
        auto node = (*virtual_location)[filename];
        node->real_path = entry.path().string();

        if (entry.is_directory()) {
            index_real_dir(node, entry.path());
        }

        else if (entry.is_regular_file()) {
            node->data_size = entry.file_size();
        }
    }
}

void WltgPacker::write_fs_blob(const char* real_path, int compression_level, ubyte encryption_key[32], bool prefer_min_size) {
    std::ofstream dst_fp(real_path, std::ios::binary);

    auto nodes = get_nodes_list();

    if (prefer_min_size) {
        std::sort(nodes.begin(), nodes.end(), [](const WltgPackerNode* a, const WltgPackerNode* b) {
            auto a_sep_index = a->name.find('.');
            auto b_sep_index = b->name.find('.');
            a_sep_index = a_sep_index == std::string::npos ? 0 : a_sep_index;
            b_sep_index = b_sep_index == std::string::npos ? 0 : b_sep_index;
            return a->name.substr(a_sep_index) < b->name.substr(b_sep_index);
        });
    }

    // strings
    std::unordered_map<std::string_view, uint> string_offsets;
    std::vector<std::string_view> strings;
    uint curr_string_offset = 0;

    for (auto node : nodes) {
        auto added_info = string_offsets.try_emplace(node->name, curr_string_offset);
        if (!added_info.second)
            continue;
        curr_string_offset += node->name.size() + 1;
        strings.emplace_back(node->name);
    }

    // inodes
    std::vector<Inode> inodes(nodes.size());
    ulong total_written = 0;

    for (int i = 0; i < nodes.size(); ++i) {
        inodes[i].string_name = string_offsets[nodes[i]->name];
        inodes[i].child_node_id = nodes[i]->tmp_inode_info.child ? nodes[i]->tmp_inode_info.child->tmp_inode_info.self_index : -1;
        inodes[i].next_node_id = nodes[i]->tmp_inode_info.next ? nodes[i]->tmp_inode_info.next->tmp_inode_info.self_index : -1;
        inodes[i].data_size = nodes[i]->data_size;
        if (nodes[i]->data_size != -1) {
            inodes[i].file_data = total_written;
            total_written += nodes[i]->data_size;
        }
        else
            inodes[i].file_data = -1;
    }

    // writing: file system header
    FileHeader header;
    header.magic = 0x77889945;
    header.inode_count = inodes.size();
    header.string_pool_size = curr_string_offset;
    header.data_size = total_written;
    header.compression_header_size = 0;
    header.encryption_header_size = 0;
    dst_fp.write((const char*) &header, sizeof(header));

    // writing: inodes
    dst_fp.write((const char*) inodes.data(), inodes.size() * sizeof(Inode));

    // writing: string pool
    for (int i = 0; i < strings.size(); ++i) {
        dst_fp.write(strings[i].data(), strings[i].size() + 1);
    }

    // creating real writing stream
    auto real_stream = new WltgRealFileSystemWritingStream(std::move(dst_fp));
    WltgBaseWritingStream* stream = real_stream;
    auto& new_dst_fp = real_stream->ofstream;

    // writing: encryption header
    WltgChaCha20WritingStream* encryption_stream = nullptr;
    if (encryption_key) {
        // filling EncryptionInfo
        EncryptionInfo encryption_header;
        const char chacha20_string[] = {'C', 'h', 'a', 'C', 'h', 'a', '2', '0'};
        encryption_header.type = *(ulong*) chacha20_string;

        std::random_device rd;

        // filling ChaCha20EncryptionInfo
        ChaCha20EncryptionInfo chacha20_encryption_header;
        for (int i = 0; i < sizeof(chacha20_encryption_header.encrypt_nonce);
             i += sizeof(std::random_device::result_type)) {
            *(std::random_device::result_type*) &chacha20_encryption_header.encrypt_nonce[i] = rd();
        }
        for (int i = 0; i < sizeof(chacha20_encryption_header.encrypted_validation_hash);
             i += sizeof(std::random_device::result_type)) {
            *(std::random_device::result_type*) &chacha20_encryption_header.encrypted_validation_hash[i] = rd();
        }
        ChaCha20State::encrypt_and_hash_sha256(encryption_key, chacha20_encryption_header.nonce,
                                               chacha20_encryption_header.encrypt_nonce,
                                               sizeof(chacha20_encryption_header.encrypt_nonce),
                                               chacha20_encryption_header.encrypted_validation_hash);

        // writing 2 encryption headers
        new_dst_fp.write((const char*) &encryption_header, sizeof(encryption_header));
        new_dst_fp.write((const char*) &chacha20_encryption_header, sizeof(chacha20_encryption_header));

        // re-writing base file header
        header.encryption_header_size = sizeof(encryption_header) + sizeof(chacha20_encryption_header);
        auto old_pos = new_dst_fp.tellp();
        new_dst_fp.seekp(0, std::ios::beg);
        new_dst_fp.write((const char*) &header, sizeof(header));
        new_dst_fp.seekp(old_pos, std::ios::beg);

        // creating writing stream
        stream = encryption_stream = new WltgChaCha20WritingStream(stream, encryption_key, chacha20_encryption_header.nonce);
    }

    auto block_size = 8 << 20;

    // writing: compression header
    CompressionInfo compression_header;
    if (compression_level) {
        const char zstd_name[] = {'Z', 'S', 'T', 'D'};
        compression_header.type = *(uint*) zstd_name;
        compression_header.block_size = block_size;
        compression_header.compress_bound = ZSTD_compressBound(block_size);
        new_dst_fp.write((char*) &compression_header, sizeof(CompressionInfo));
    }

    // writing: skipping compression blocks
    ulong compression_header_start = new_dst_fp.tellp();
    auto block_count = ((total_written + block_size - 1) / block_size) + 1;
    std::vector<CompressedBlock> comp_blocks(block_count);

    // writing: rewriting file system header because of modified compression header size
    if (compression_level) {
        header.compression_header_size = sizeof(CompressionInfo) + block_count * sizeof(CompressedBlock);
        new_dst_fp.seekp(0, std::ios::beg);
        new_dst_fp.write((char*) &header, sizeof(header));

        new_dst_fp.seekp(compression_header_start + block_count * sizeof(CompressedBlock), std::ios::beg);
    }

    // setting up a writing streams
    WltgZstdCompressionWritingStream* compression_stream = nullptr;
    if (compression_level)
        stream = (compression_stream = new WltgZstdCompressionWritingStream(stream, block_size, compression_level));
    auto buffered_stream = new WltgBufferedWritingStream(stream, block_size);
    stream = buffered_stream;

    // writing: file contents
    for (auto node : nodes) {
        if (node->data_size == -1)
            continue;
        std::ifstream src_fp(node->real_path, std::ios::binary);
        stream->write(src_fp);
    }
    stream->flush();

    // writing: compression offsets
    if (compression_level) {
        if (compression_stream->write_counts.size() != block_count - 1) {
            std::cout << "error writing file: amount of written blocks does not match amount of blocks predicted" << std::endl;
            return;
        }

        comp_blocks[0].start = 0;
        for (int i = 1; i < compression_stream->write_counts.size() + 1; ++i) {
            comp_blocks[i].start = comp_blocks[i - 1].start + compression_stream->write_counts[i - 1];
        }

        new_dst_fp.seekp(compression_header_start, std::ios::beg);
        new_dst_fp.write((char*) comp_blocks.data(), comp_blocks.size() * sizeof(comp_blocks[0]));
    }

    // cleanup
    delete buffered_stream;
    delete compression_stream;
    delete encryption_stream;
    delete real_stream;
}

std::vector<WltgPackerNode*> WltgPacker::get_nodes_list() {
    std::vector<WltgPackerNode*> nodes_list;
    std::stack<WltgPackerNode*> nodes_stack;
    nodes_stack.push(&*root);

    while (!nodes_stack.empty()) {
        auto curr_node = &*nodes_stack.top();
        nodes_stack.pop();
        curr_node->tmp_inode_info.self_index = nodes_list.size();
        curr_node->tmp_inode_info.child = nullptr;
        nodes_list.push_back(curr_node);

        for (auto& [_, child] : curr_node->children) {
            nodes_stack.push(&*child);
            child->tmp_inode_info.next = curr_node->tmp_inode_info.child;
            curr_node->tmp_inode_info.child = &*child;
        }
    }

    return nodes_list;

    /*std::queue<WltgPackerNode*> nodes_queue;
    nodes_queue.push(&*root);

    while (!nodes_queue.empty()) {
        auto curr_node = &*nodes_queue.front(); // &* here is to silence warning of maybe-invalidated reference (which is not stored at all)
        nodes_queue.pop();
        curr_node->tmp_inode_info.self_index = nodes_list.size();
        curr_node->tmp_inode_info.child = nullptr;
        nodes_list.push_back(curr_node);

        for (auto& [_, child] : curr_node->children) {
            nodes_queue.push(&*child);
            child->tmp_inode_info.next = curr_node->tmp_inode_info.child;
            curr_node->tmp_inode_info.child = &*child;
        }
    }

    return nodes_list;*/
}

void WltgPacker::write_fs_blob(const char* real_path, int compression_level, const char* password, bool prefer_min_size) {
    if (password) {
        ubyte encryption_key[32];
        hash_password(password, encryption_key);
        write_fs_blob(real_path, compression_level, encryption_key, prefer_min_size);
        std::cout << "dec key: " << (int) encryption_key[0] << " " << (int) encryption_key[1] << std::endl;
        secure_zero_mem(encryption_key, sizeof(encryption_key));
    }
    else {
        write_fs_blob(real_path, compression_level, (ubyte*) nullptr, prefer_min_size);
    }
}
