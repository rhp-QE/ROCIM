#include "base/LinkBuffer.h"
#include <algorithm>
#include <boost/asio/buffer.hpp>
#include <cstddef>
#include <iostream>
#include <list>
#include <memory>
#include <system_error>

using namespace::roc::base;

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
    hint = hint == 0 ? 1 : hint;
    std::cout<<"[prepare]"<<hint<<std::endl;
    std::vector<boost::asio::mutable_buffer> bufs;
    std::list<std::unique_ptr<Block>>::const_iterator it = write_block_cursor_;

    for(; it != blocks_.end(); ++it) {
        auto& block = *it;

        bufs.emplace_back(block->data.get() + block->write_pos, block->writable());

        if (block->writable() >= hint) {
            hint = 0;
            break;
        } else {
            hint -= block->writable();
        }
    }

    if (hint > 0) {
        const size_t new_size = std::max(default_block_size_, hint);
        append_new_block(new_size);
        auto& new_block = blocks_.back();
        bufs.emplace_back(new_block->data.get(), new_block->writable());
    }

    return bufs;
}

void NetBuffer::commit(size_t written) {
    std::cout<<"[call commit] :"<<written<<std::endl;
    std::list<std::unique_ptr<Block>>::const_iterator it = write_block_cursor_;

    for(; it != blocks_.end(); ++it) {
        auto& block = *it;
        size_t blockCommit = std::min(block->writable(), written);
        block->write_pos += blockCommit;
        written -= blockCommit;

        std::cout<<"[block commit count]"<<blockCommit<<std::endl;

        if (written == 0) {
            break;
        }
    }

    if (it != blocks_.end() && (*it)->writable() == 0) {
        ++it;
    }

    write_block_cursor_ = it;

    if (written > 0) {
        std::cerr<<"[base::buffer::commit error] commit count over writeable"<<std::endl;
    }
}

NetBuffer::ReadResult NetBuffer::get_read_buffers(size_t size) {

    NetBuffer::ReadResult result;
    size_t len = length();

    size = (size == 0) ? len : size;

    // 从全局游标位置开始读取
    auto it = read_block_cursor_;

    // 记录本次读取的起始位置
    std::vector<boost::asio::const_buffer> buffers;

    for (; it != blocks_.end(); ++it) {
        auto& block = *it;
        const size_t readable = block->readable();
        const size_t chunk = std::min(readable, size);
        buffers.emplace_back(block->data.get() + block->read_pos, chunk);
        block->read_pos += chunk;
        size -= chunk;

        std::cout<<"[read from block count]"<<chunk<<std::endl;

        if (size == 0) {
            break;
        }
    }

    if (size != 0) {
        std::cerr << "[base::buffer::get_read_buffer error] readable count is not enough" << std::endl;
    }

    result.cursor.start_block = read_block_cursor_;
    result.buffers = buffers;
    result.buffer_ = shared_from_this();

    read_block_cursor_ = it;
    return result;
}

size_t NetBuffer::length() const noexcept {
    std::list<std::unique_ptr<Block>>::const_iterator it = read_block_cursor_;
    size_t total = 0;

    for (; it != blocks_.end(); ++it) {
        auto& block = *it;
        total += block->readable();
    }

    return total;
}

void NetBuffer::append_new_block(size_t hint) {
    const size_t new_size = std::max(default_block_size_, hint);
    blocks_.emplace_back(std::make_unique<Block>(new_size));
}