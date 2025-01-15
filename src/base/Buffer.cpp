#include <algorithm>
#include <base/Buffer.h>
#include <boost/asio/buffers_iterator.hpp>
#include <boost/asio/detail/buffer_resize_guard.hpp>
#include <optional>

using namespace roc;
using namespace base;

//------------------------- public method begin -------------------

int Buffer::readableBytes() const {
    return writeIndex_ - readIndex_;
}

int Buffer::writeableBytes() const {
    return buffer_.size() - writeIndex_;
}

std::optional<std::string> Buffer::readString(int n) {
    if (readableBytes() < n) {
        return std::nullopt;
    }

    int index = writeIndex_;
    writeIndex_ += n;
    return std::string(index, n);
}

void Buffer::appendString(const std::string &str) {
    int len = str.length();
    if (writeableBytes() >= len) {
        std::copy(str.begin(), str.end(), buffer_.begin() + writeIndex_);
        return;
    }

    if (prependableBytes() + writeableBytes() >= len + leastPrependBytes_) {
        std::copy(buffer_.begin() + readIndex_, 
                   buffer_.begin() + writeIndex_, 
                 buffer_.begin() + leastPrependBytes_);
    } else {
        int newSize = buffer_.size() + len;
        buffer_.resize(newSize);
    }

    std::copy(str.begin(), str.end(), buffer_.begin() + writeIndex_);
    
    return;
}

//------------------------- public method end ---------------------

//------------------------- public method begin ---------------------

int Buffer::prependableBytes() const {
    return readIndex_;
}

//------------------------- public method end -----------------------
