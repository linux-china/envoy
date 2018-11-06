#include "extensions/filters/network/rsocket_proxy/frame.h"


namespace Envoy {
    namespace Extensions {
        namespace NetworkFilters {
            namespace RSocket {
                Frame::Frame(Buffer::Instance &data) {
                    // frame length, first 3 bytes
                    byte frameLenArray[3];
                    data.copyOut(0, 3, frameLenArray);
                    this->frame_len = (static_cast<int>( frameLenArray[0]) << 16)
                                      | (static_cast<int>( frameLenArray[1]) << 8)
                                      | static_cast<int>( frameLenArray[2]);
                    // stream ID: 4 bytes
                    byte streamIdArray[4];
                    data.copyOut(3, 4, streamIdArray);
                    this->stream_id = (static_cast<int>( streamIdArray[0]) << 16)
                                      | (static_cast<int>( streamIdArray[1]) << 8)
                                      | (static_cast<int>( streamIdArray[2]));
                    //frame type(6 bits) && flags(10 bits)
                    byte frameExtraArray[2];
                    data.copyOut(7, 2, frameExtraArray);
                    this->frame_type = frameExtraArray[0] >> 2;
                    this->flags = (static_cast<int>(streamIdArray[0] & static_cast<byte>(3)) << 8)
                                  | (static_cast<int>( streamIdArray[2]));
                    this->metadata_present = (frameExtraArray[0] & static_cast<byte>(1)) == 1;
                    //if metadata present
                    if (this->metadata_present) {
                        switch (this->frame_type) {
                            case static_cast<byte>(0x04) : //REQUEST_RESPONSE
                                std::cout << "REQUEST_RESPONSE";
                                break;
                            case static_cast<byte>(0x06) : //REQUEST_STREAM
                                std::cout << "REQUEST_STREAM";
                                break;
                        }

                    }
                }
            }
        }
    }
}