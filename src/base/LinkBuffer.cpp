#include "base/LinkBuffer.h"
#include <algorithm>
#include <boost/asio/buffer.hpp>
#include <cstddef>
#include <iostream>
#include <list>
#include <memory>
#include <optional>
#include <system_error>

using namespace::roc::base;

int id = 0;

NetBuffer::Block::Block(size_t size) 
    : capacity(size), data(new char[size]) {}

void NetBuffer::Block::reset() noexcept {
    write_pos = 0;
    read_pos = 0;
}

size_t NetBuffer::Block::writable() const noexcept {
    return capacity - write_pos;
}

size_t NetBuffer::Block::readable() const noexcept {
    return write_pos - read_pos;
}

void NetBuffer::ReadResult::release() const {
 //   auto& it = cursor.start_block;


   // buffer_->read_block_cursor_ = buffer_->blocks_.begin();
}

std::string NetBuffer::ReadResult::readString() const {
    std::string result;

    // 计算所有缓冲区的总字节数
    size_t total_size = 0;

    for (const auto& buffer : buffers) {
        total_size += boost::asio::buffer_size(buffer);
    }

    // 预分配内存
    result.reserve(total_size);

    // 拼接数据
    for (const auto& buffer : buffers) {
        const char* data = boost::asio::buffer_cast<const char*>(buffer);
        size_t length = boost::asio::buffer_size(buffer);
        result.append(data, length);
    }
    
    return result;
}

size_t NetBuffer::ReadResult::readBytes(char* buf) {
    // 计算所有缓冲区的总字节数
    size_t total_size = 0;

    for (const auto& buffer : buffers) {
        total_size += boost::asio::buffer_size(buffer);
    }

    size_t pos = 0;
    for (const auto& buffer : buffers) {
        const char* data = boost::asio::buffer_cast<const char*>(buffer);
        size_t length = boost::asio::buffer_size(buffer);

        std::copy(data, data + length, buf + pos);
        pos += length;
    }
    return pos;
}

NetBuffer::NetBuffer(size_t init_block_size)
    : default_block_size_(init_block_size)
{
    blocks_.emplace_back(std::make_unique<Block>(init_block_size));
    write_block_cursor_ = blocks_.begin();
    read_block_cursor_ = blocks_.begin();
}

// 用户主动写入数据（通过 prepare_buffers 和 commit 实现追加）
void NetBuffer::write(const void* data, size_t len) {
    std::cout<<"[call write]"<<std::endl;
    const char* src = static_cast<const char*>(data);
    auto bufs = prepare_buffers(len);

    size_t written = 0;
    for (const auto& buf : bufs) {
        if (len == written) {
            break;
        }
        std::cout<<"[buf size]"<<buf.size()<<std::endl;
        const size_t copy_size = std::min(buf.size(), len - written);
        std::memcpy(buf.data(), src + written, copy_size);
        written += copy_size;
    }

    commit(written);
}

std::vector<boost::asio::mutable_buffer> NetBuffer::prepare_buffers(size_t hint) {
    /// 保证有足够空间
    if (writeable() < hint || writeable() == 0) {
        append_new_block(std::max(default_block_size_, hint));
    }

    hint = (hint == 0) ? writeable() : hint;
    size_t oo = hint;
    std::cout<<"[prepare]"<<hint<<std::endl;
    std::vector<boost::asio::mutable_buffer> bufs;
    std::list<std::unique_ptr<Block>>::const_iterator it = write_block_cursor_;

    size_t tt = 0;

    for(; it != blocks_.end(); ++it) {
        auto& block = *it;

        size_t chunk = std::min(hint, block->writable());

        bufs.emplace_back(block->data.get() + block->write_pos, chunk);
        tt += chunk;
        hint -= chunk;
        if (hint == 0) {
            break;
        }
    }

    ++id;
    std::cout<<"[call prepare buffer]"<<this<<" "<<tt<<" "<<id<<std::endl;

    std::cout<<"[block count]"<<this<<" "<<blocks_.size()<<std::endl;

    return bufs;
}

void NetBuffer::commit(size_t written) {
    if (written > writeable()) {
        std::cout<<"error"<<std::endl;
        return;
    }

    std::cout<<"[call commit] :"<<this<<" "<<written<<" "<<id<<std::endl;
    std::list<std::unique_ptr<Block>>::const_iterator it = write_block_cursor_;

    for(; it != blocks_.end(); ++it) {
        auto& block = *it;
        size_t chunk = std::min(block->writable(), written);
        block->write_pos += chunk;
        written -= chunk;

        std::cout<<"[block commit count]"<<chunk<<std::endl;

        if (written == 0) {
            break;
        }
    }

    write_block_cursor_ = it;
}

std::optional<NetBuffer::ReadResult> NetBuffer::get_read_buffers(size_t size) {

    if (size > readable() || readable() == 0) {
        std::cerr << "[base::buffer::get_read_buffer] readable count is not enough" << std::endl;
        return std::nullopt;
    }

    NetBuffer::ReadResult result;
    size = (size == 0) ? readable() : size;

    // 从全局游标位置开始读取
    auto it = read_block_cursor_;

    // 记录本次读取的起始位置
    std::vector<boost::asio::const_buffer> buffers;

    for (; it != blocks_.end(); ++it) {
        auto& block = *it;
        const size_t chunk = std::min(block->readable(), size);
        buffers.emplace_back(block->data.get() + block->read_pos, chunk);
        block->read_pos += chunk;
        size -= chunk;

        std::cout<<"[read from block count]"<<chunk<<std::endl;

        if (size == 0) {
            break;
        }
    }

    result.cursor.start_block = read_block_cursor_;
    result.buffers = buffers;
    result.buffer_ = shared_from_this();

    read_block_cursor_ = it;
    return result;
}

size_t NetBuffer::readable() const noexcept {
    std::list<std::unique_ptr<Block>>::const_iterator it = read_block_cursor_;
    size_t total = 0;

    for (; it != blocks_.end(); ++it) {
        auto& block = *it;
        total += block->readable();
    }

    return total;
}

size_t NetBuffer::writeable() const noexcept {
    std::list<std::unique_ptr<Block>>::const_iterator it = read_block_cursor_;
    size_t total = 0;

    for (; it != blocks_.end(); ++it) {
        auto& block = *it;
        total += block->writable();
    }

    return total;
}

void NetBuffer::append_new_block(size_t hint) {
    bool end = write_block_cursor_ == blocks_.end();
    const size_t new_size = std::max(default_block_size_, hint);
    blocks_.emplace_back(std::make_unique<Block>(new_size));
    if (end) {
        write_block_cursor_ = --blocks_.end();
    }
}