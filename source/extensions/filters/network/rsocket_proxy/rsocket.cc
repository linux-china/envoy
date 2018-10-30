#include "extensions/filters/network/rsocket_proxy/rsocket.h"

#include "envoy/buffer/buffer.h"
#include "envoy/network/connection.h"

#include "common/common/assert.h"

#include <iostream>

using namespace std;
typedef unsigned char byte;
typedef long long MsgID;

namespace Envoy {
    namespace Extensions {
        namespace NetworkFilters {
            namespace RSocket {
                //on data received
                Network::FilterStatus RSocketFilter::onData(Buffer::Instance &data, bool) {
                    ENVOY_CONN_LOG(trace, "tap: got {} bytes", read_callbacks_->connection(), data.length());
                    //data: header + body
                    std::cout << "Received bytes from requester: " << data.length() << std::endl;
                    // inject cluster name for next filter
                    return Network::FilterStatus::Continue;
                }
                // on data write back
                Network::FilterStatus RSocketFilter::onWrite(Buffer::Instance &data, bool) {
                    std::cout << "Write bytes to requester: " << data.length() << std::endl;
                    return Network::FilterStatus::Continue;
                }
            } // namespace RSocket
        } // namespace NetworkFilters
    } // namespace Extensions
} // namespace Envoy