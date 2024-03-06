#pragma once


#include <vector>
#include <string_view>
#include <memory>
#include "util/types.h"


class BaseFileStream
{
public:
    /**
     * @brief Read next data into specified location
     *
     * @param dst destination pointer to read into
     * @param size maximum size to read
     *
     * @return amount of bytes read
     */
    virtual ulong read_into(void* dst, ulong size) = 0;

    /**
     * @brief Read all remaining data from stream
     *
     * @param max_size maximum size to read from stream. -1 if all available data
     *
     * @return std::vector with stream data. Not larger than max_size
     */
    virtual std::vector<ubyte> read(slong max_size = -1) {
        auto read_size = max_size == -1 ? size() : std::min((ulong) max_size, size());

        std::vector<ubyte> ret(read_size);
        read_into(ret.data(), ret.size());
        return ret;
    }

    /**
     * @brief Get total file stream size
     *
     * @return amount of bytes in the stream
     */
    [[nodiscard]] virtual ulong size() const = 0;

    /**
     * @brief Set internal stream position to read from
     */
    virtual void seek(ulong offset) = 0;

    /**
     * @brief Get internal stream position to read from. Also may be used to get amount of bytes read from stream
     *
     * @return amount of bytes read from stream
     */
    [[nodiscard]] virtual ulong tell() const = 0;

    /**
     * @brief Check whether this handle was forcibly closed by filesystem or not.
     *  You can not read from closed handles
     *
     * @return true if closed, false otherwise
     */
    [[nodiscard]] virtual bool is_closed() const = 0;

    virtual ~BaseFileStream() = default;
};


class BaseFilesystemSource
{
public:
    /**
     * @brief Open file for reading
     *
     * @param filename name of the file to open
     *
     * @return std::unique_ptr to stream handle or nullptr if file does not exist
     */
    virtual std::unique_ptr<BaseFileStream> open_file_read(std::string_view filename) = 0;

    virtual bool is_path_exists(std::string_view path) = 0;

    virtual ~BaseFilesystemSource() = default;
};


class Filesystem
{
public:
    std::vector<BaseFilesystemSource*> sources;

    Filesystem();

    void add_source(BaseFilesystemSource* source);

    /**
     * @brief Open file for reading
     *
     * @param filename name of the file to open
     *
     * @return std::unique_ptr to stream handle or nullptr if file does not exist
     */
    std::unique_ptr<BaseFileStream> open_file_read(std::string_view filename);

    /**
     * @brief Read all file contents
     *
     * @param filename name of the file to read
     *
     * @return std::vector with file contents or empty if file does not exist
     */
    std::vector<ubyte> read_file(std::string_view filename);

    /**
     * @brief Read file contents into user-specified buffer
     *
     * @param filename name of the file to read
     * @param dst destination pointer to read into
     * @param available_size maximum amount of bytes to read
     *
     * @return amount of bytes read. Not larger than available_size
     */
    ulong read_file(std::string_view filename, void* dst, ulong available_size);

    void* open_file_write(std::string_view filename);
};
