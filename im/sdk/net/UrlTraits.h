//
// UrlTraits.h
//
// author: Ruan Huipeng
// date : 2025-03-23
// 

#ifndef ROC_IM_SDK_NET_URLTRAITS_H
#define ROC_IM_SDK_NET_URLTRAITS_H


// ------------- pre declare (begin) -------------------

namespace roc::im::sdk::net {
    class FetchMixedLinkMessagesReq;
    class FetchMixedLinkMessagesResp;

    class FetchSingleLinkMessagesReq;
    class FetchSingleLinkMessagesResp;

    class CmdMessage;
    class ContentMessage;
    class Message;

    class PushMessages;

    class RequestBody;
    class ResponseBody;

    class SendBatchMessageReq;
    class SendBatchMessageResp;
}  // namespace roc::im::sdk::net

// ------------- pre declare (end) -------------------


namespace roc::im::sdk::net {

extern const char send_message_url[];
extern const char fetch_mix_link_messages_url[];
extern const char fetch_single_link_messages_url[];
extern const char push_message_url[];

template <const char* url> 
struct UrlTraits {
    using request_type  = void;
    using response_type = void;
};

template <>
struct UrlTraits<send_message_url> {
    using request_type  = SendBatchMessageReq;
    using response_type = SendBatchMessageResp;
};

template <>
struct UrlTraits<push_message_url> {
    using request_type  = void;
    using response_type = PushMessages;
};

template <>
struct UrlTraits<fetch_mix_link_messages_url> {
    using request_type  = FetchMixedLinkMessagesReq;
    using response_type = FetchMixedLinkMessagesResp;
};

template <>
struct UrlTraits<fetch_single_link_messages_url> {
    using request_type  = FetchSingleLinkMessagesReq;
    using response_type = FetchSingleLinkMessagesResp;
};



} // namespace roc::im::sdk::net

#endif // ROC_IM_SDK_NET_URLTRAITS_H
