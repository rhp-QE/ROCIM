#ifndef NET_BUFFER_HPP
#define NET_BUFFER_HPP

#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <cstddef>
#include <list>
#include <memory>
#include <mutex>
#include <cstring>

namespace roc::base {

class NetBuffer : public std::enable_shared_from_this<NetBuffer> {
public:
    // 内存块结构
    struct Block {
        size_t capacity;          // 块总容量
        size_t write_pos = 0;     // 写入位置
        size_t read_pos = 0;      // 消费位置
        std::unique_ptr<char[]> data; // 数据存储

        explicit Block(size_t size);
        void reset() noexcept;    // 重置块状态
        size_t writable() const noexcept; // 剩余可写空间
        size_t readable() const noexcept; // 剩余可读数据
    };

    // 读取游标（记录数据区间）
    struct ReadCursor {
        std::list<std::unique_ptr<Block>>::const_iterator start_block; // 起始块
        size_t start_offset;       // 起始块内的偏移
        std::list<std::unique_ptr<Block>>::const_iterator end_block;   // 结束块
        size_t end_offset;         // 结束块内的偏移
        size_t total_size;         // 区间总大小
    };

    // 读取结果（缓冲区 + 游标）
    struct ReadResult {
        public:
            std::vector<boost::asio::const_buffer> buffers; // 数据缓冲区
            ReadCursor cursor;                               // 对应的数据区间
            std::shared_ptr<NetBuffer> buffer_;

            void release() const;
            std::string readString() const;

            size_t readBytes(char *buf);
    };

    explicit NetBuffer(size_t init_block_size = 4096);
    
    // 核心接口
    void write(const void* data, size_t len);           // 用户主动写入数据（追加到末尾）
    std::vector<boost::asio::mutable_buffer> prepare_buffers(size_t hint = 0); // 获取可写缓冲区
    void commit(size_t written);                        // 提交写入数据
    ReadResult get_read_buffers(size_t size = 0);           // 返回数据及区间游标

    size_t length() const noexcept;                        // 当前可读数据总长度

private:
    mutable std::mutex mutex_;
    std::list<std::unique_ptr<Block>> blocks_;
    size_t default_block_size_;
    std::list<std::unique_ptr<Block>>::const_iterator write_block_cursor_; // 空闲块遍历游标
    std::list<std::unique_ptr<Block>>::const_iterator read_block_cursor_; // 空闲块遍历游标

    void append_new_block(size_t hint);                 // 创建新块
};

} /// namespace roc::base

#endif // NET_BUFFER_HPP