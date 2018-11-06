#include "extensions/filters/network/rsocket_proxy/rsocket.h"
#include "extensions/filters/network/rsocket_proxy/frame.h"

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
                    if (isRSocketData(data)) {
                        Frame frame{data, this->metadata_type};
                        std::cout << "StreamId:" << frame.getStreamId() << std::endl;
                        std::cout << "FrameLength:" << frame.getFrameLength() << std::endl;
                        std::cout << "MetadataPresent:" << frame.isMetadataPresent() << std::endl;
                        if (frame.getFrameType() == static_cast<byte>(0x04)) {
                            this->stats_.frame_0x04_request_count_.inc();
                        }
                        if (frame.getMetadataLength() > 0) {
                            std::cout << "Metadata Length: " << frame.getMetadataLength() << std::endl;
                            std::cout << "Metadata: " << frame.getMetadataUtf8() << std::endl;
                        }
                        if (frame.getDataLength() > 0) {
                            std::cout << "Data Length: " << frame.getDataLength() << std::endl;
                            std::cout << "Data: " << frame.getDataUtf8() << std::endl;
                        }
                        this->stats_.request_counter_.inc();
                    }
                    // inject cluster name for next filter
                    return Network::FilterStatus::Continue;
                }

                // on data write back
                Network::FilterStatus RSocketFilter::onWrite(Buffer::Instance &data, bool) {
                    if (isRSocketData(data)) {
                        std::cout << "RSocket write back: " << data.length() << std::endl;
                        Frame frame{data, this->metadata_type};
                        std::cout << "Response type: " << static_cast<int>(frame.getFrameType()) << std::endl;
                        if (frame.getFrameType() == static_cast<byte>(0x0A)) {
                            std::cout << "Metrics: 0x0A" << std::endl;
                            this->stats_.frame_0x0A_response_count_.inc();
                        }
                        if (frame.getDataLength() > 0) {
                            std::cout << "Response Data Length: " << frame.getDataLength() << std::endl;
                            std::cout << "Response Data: " << frame.getDataUtf8() << std::endl;
                        }
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