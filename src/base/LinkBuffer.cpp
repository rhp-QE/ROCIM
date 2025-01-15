#include <base/LinkBuffer.h>

using namespace::roc::base;

//---------------------- public method begin ------------------

int LinkBuffer::length() {
    return len;
}

bool LinkBuffer::isEmpty() {
    return len == 0;
}

std::string_view LinkBuffer::next(int n) {
    
}

//---------------------- public method end --------------------

//---------------------- private method begin ------------------



//---------------------- private method end --------------------

