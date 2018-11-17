#include <iostream>

#include "envoy/buffer/buffer.h"
#include "envoy/network/connection.h"
#include "common/common/assert.h"

#include "extensions/filters/network/rsocket_proxy/rsocket.h"
#include "extensions/filters/network/rsocket_proxy/frame.h"


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
                    if (isRSocketData(data)) {
                        Frame frame{data, this->metadata_type_};
                        std::cout << "==================request start===============" << std::endl;
                        std::string metric_name = fmt::format("rsocket_type_{}_counter",
                                                              static_cast<int>(frame.getFrameType()));
                        this->scope_.counter(metric_name).inc();
                        std::cout << frame.toString() << std::endl;
                        this->stats_.request_counter_.inc();
                        std::cout << "==================request end===============" << std::endl;
                    }
                    // inject cluster name for next filter
                    return Network::FilterStatus::Continue;
                }

                // on data write back
                Network::FilterStatus RSocketFilter::onWrite(Buffer::Instance &data, bool) {
                    if (isRSocketData(data)) {
                        std::cout << "==================response start===============" << std::endl;
                        Frame frame{data, this->metadata_type_};
                        std::string metric_name = fmt::format("rsocket_type_{}_counter",
                                                              static_cast<int>(frame.getFrameType()));
                        this->scope_.counter(metric_name).inc();
                        std::cout << frame.toString() << std::endl;
                        std::cout << "==================response end===============" << std::endl;
                    }
                    return Network::FilterStatus::Continue;
                }

                bool RSocketFilter::isRSocketData(Buffer::Instance &data) {
                    int data_len = data.length();
                    //  3 bits frame length + 4 bits stream id + 2 bits(frame type & flags)
                    if (data_len >= 9) {
                        //Rsocket basic validation: frame length + 3 = data length
                        byte frame_len_array[3];
                        data.copyOut(0, 3, frame_len_array);
                        int frame_len = (static_cast<int>( frame_len_array[0]) << 16)
                                        | (static_cast<int>( frame_len_array[1]) << 8)
                                        | static_cast<int>( frame_len_array[2]);
                        // legal rsocket data
                        return frame_len + 3 == data_len;
                    }
                    return false;
                }
            } // namespace RSocket
        } // namespace NetworkFilters
    } // namespace Extensions
} // namespace Envoy