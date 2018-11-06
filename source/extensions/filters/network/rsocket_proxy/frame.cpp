#include "extensions/filters/network/rsocket_proxy/frame.h"


namespace Envoy {
    namespace Extensions {
        namespace NetworkFilters {
            namespace RSocket {
                Frame::Frame(Buffer::Instance &data): buffer_data(data) {
                    int raw_data_len = data.length();
                    // frame length, first 3 bytes
                    byte frame_len_array[3];
                    data.copyOut(0, 3, frame_len_array);
                    this->frame_len = (static_cast<int>( frame_len_array[0]) << 16)
                                      | (static_cast<int>( frame_len_array[1]) << 8)
                                      | static_cast<int>( frame_len_array[2]);
                    // stream ID: 4 bytes
                    byte stream_id_array[4];
                    data.copyOut(3, 4, stream_id_array);
                    this->stream_id = (static_cast<int>( stream_id_array[0]) << 24)
                                      | (static_cast<int>( stream_id_array[1]) << 16)
                                      | (static_cast<int>( stream_id_array[2]) << 8)
                                      | (static_cast<int>( stream_id_array[3]));
                    //frame type(6 bits) && flags(10 bits)
                    byte frame_extra_array[2];
                    data.copyOut(7, 2, frame_extra_array);
                    this->frame_type = frame_extra_array[0] >> 2;
                    this->flags = (static_cast<int>(frame_extra_array[0] & static_cast<byte>(3)) << 8)
                                  | (static_cast<int>( frame_extra_array[2]));
                    this->metadata_present = (frame_extra_array[0] & static_cast<byte>(1)) == 1;
                    //if metadata present
                    if (this->metadata_present && raw_data_len >= 12) {
                        ////REQUEST_RESPONSE, REQUEST_FNF (Fire-n-Forget), REQUEST_STREAM
                        if (this->getFrameType() == static_cast<byte>(0x04)
                            || this->getFrameType() == static_cast<byte>(0x05)
                            || this->getFrameType() == static_cast<byte>(0x06)) {
                            //metadata
                            byte metadata_len_array[3];
                            data.copyOut(9, 3, metadata_len_array);
                            this->metadata_offset = 12;
                            this->metadata_len = (static_cast<int>( metadata_len_array[0]) << 16)
                                                 | (static_cast<int>( metadata_len_array[1]) << 8)
                                                 | (static_cast<int>( metadata_len_array[2]));
                            std::cout << "Metadata Length: " << metadata_len << std::endl;
                            this->data_offset = 15;
                            this->data_len = raw_data_len - this->metadata_len;
                        }

                    }
                }

                void Frame::copyMetadataOut(void *data) {
                    this->buffer_data.copyOut(this->metadata_offset, metadata_len, data);
                }

                void Frame::copyDataOut(void *data) {
                    this->buffer_data.copyOut(this->data_offset, data_len, data);
                }
            }
        }
    }
}