#include "pb/im.pb.h"


void test() {
    roc::im::sdk::Message message;
    message.set_convertsion_id("100");
    message.mutable_ext()->insert({"key","value"});
}