/*
 * @Author: Ruan Huipeng 
 * @Date: 2025-01-05 23:13:05 
 * @Last Modified by: Ruan Huipeng
 * @Last Modified time: 2025-01-12 16:08:05
 */

#ifndef ROCIM_CLIENT_LINKBUFFER
#define ROCIM_CLIENT_LINKBUFFER

#include <string_view>

#include <boost/core/noncopyable.hpp>

namespace roc {
namespace base {

class LinkBufferNode;

class LinkBuffer : boost::noncopyable {

    int length();

    bool isEmpty();

    std::string_view next(int n);

    std::string_view peek(int n);

    void skip(int n);

    void release();

    std::string readString(int n);

    LinkBuffer *slice(int n);

private:

    int len;
    LinkBufferNode *headNode;
    LinkBufferNode *readNode;
    LinkBufferNode *flushNode;
    LinkBufferNode *writeNode;

    bool isSingleNode(int n);

};

}//namespace base
}//namespace roc

#endif