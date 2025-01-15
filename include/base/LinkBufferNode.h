#ifndef ROMCIM_CLIENT_LINKBUFFERNOFRNODE_H
#define ROMCIM_CLIENT_LINKBUFFERNOFRNODE_H

#include <string_view>

#include <boost/core/noncopyable.hpp>

namespace roc {
namespace base {

class LinkBufferNode : boost::noncopyable {

public:
    int len();
    bool isEmpty();
    void reset();
    std::string_view next(int n);
    std::string_view peek(int n);
    LinkBufferNode *release();

private:
    char *buf;
    int readIndex;
    int writeIndex;
    LinkBufferNode *nextNode;
    LinkBufferNode *originNode;
};
}// namespace bae
}// namespace roc

#endif