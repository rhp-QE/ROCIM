#include <base/LinkBufferNode.h>
#include <cstring>
#include <string_view>

using namespace::roc::base;

int LinkBufferNode::len() {
    return strlen(buf) -readIndex;
}

bool LinkBufferNode::isEmpty() {
    return strlen(buf) == readIndex;
}

std::string_view LinkBufferNode::next(int n) {
    int index = readIndex;
    readIndex += n;
    return std::string_view(buf + index, n);
}

std::string_view LinkBufferNode::peek(int n) {
    return std::string_view(buf, n);
}

LinkBufferNode *LinkBufferNode::release() {
    readIndex = 0;
    writeIndex = 0;
    originNode = this;
    nextNode = this;
    return this;
}


