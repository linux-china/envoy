#include "extensions/filters/network/rsocket_proxy/frame.h"


namespace Envoy {
    namespace Extensions {
        namespace NetworkFilters {
            namespace RSocket {
                Frame::Frame(Buffer::Instance &data, const std::string metadata_type) :
                        buffer_data_(data), metadata_type_(metadata_type) {
                    int raw_data_len = data.length();
                    // frame length, first 3 bytes
                    byte frame_len_array[3];
                    data.copyOut(0, 3, frame_len_array);
                    this->frame_len_ = (static_cast<int>( frame_len_array[0]) << 16)
                                       | (static_cast<int>( frame_len_array[1]) << 8)
                                       | static_cast<int>( frame_len_array[2]);
                    // stream ID: 4 bytes
                    byte stream_id_array[4];
                    data.copyOut(3, 4, stream_id_array);
                    this->stream_id_ = (static_cast<int>( stream_id_array[0]) << 24)
                                       | (static_cast<int>( stream_id_array[1]) << 16)
                                       | (static_cast<int>( stream_id_array[2]) << 8)
                                       | (static_cast<int>( stream_id_array[3]));
                    //frame type(6 bits) && flags(10 bits)
                    byte frame_extra_array[2];
                    data.copyOut(7, 2, frame_extra_array);
                    this->frame_type_ = static_cast<FrameType>(frame_extra_array[0] >> 2);
                    this->flags_ = (static_cast<int>(frame_extra_array[0] & static_cast<byte>(3)) << 8)
                                   | (static_cast<int>( frame_extra_array[2]));
                    this->metadata_present_ = (frame_extra_array[0] & static_cast<byte>(1)) == 1;
                    //if metadata present, from 9
                    if (this->metadata_present_ && raw_data_len >= 12) {
                        ////REQUEST_RESPONSE, REQUEST_FNF (Fire-n-Forget), REQUEST_STREAM
                        if (this->getFrameType() == FrameType::REQUEST_RESPONSE
                            || this->getFrameType() == FrameType::REQUEST_FNF
                            || this->getFrameType() == FrameType::REQUEST_STREAM
                            || this->getFrameType() == FrameType::PAYLOAD
                            || this->getFrameType() == FrameType::ERROR
                            || this->getFrameType() == FrameType::METADATA_PUSH) {
                            //metadata
                            byte metadata_len_array[3];
                            data.copyOut(9, 3, metadata_len_array);
                            this->metadata_offset_ = 12;
                            this->metadata_len_ = (static_cast<int>( metadata_len_array[0]) << 16)
                                                  | (static_cast<int>( metadata_len_array[1]) << 8)
                                                  | (static_cast<int>( metadata_len_array[2]));
                            if (this->metadata_len_ > 0) {
                                //todo read first by of metadata and check metdata content type
                                byte metadata_first_byte[1];
                                this->buffer_data_.copyOut(this->metadata_offset_, 1, metadata_first_byte);
                                //todo check first byte of metadata
                            }
                        }
                    }
                    // frame data
                    if (this->metadata_offset_ > 0) {
                        this->data_offset_ = this->metadata_offset_ + this->metadata_len_;
                        this->data_len_ = raw_data_len - this->data_offset_;
                    } else {
                        if (raw_data_len > 9) {
                            this->data_offset_ = 9;
                            this->data_len_ = raw_data_len - this->data_offset_;
                        }
                    }
                }

                void Frame::copyMetadataOut(void *data) {
                    this->buffer_data_.copyOut(this->metadata_offset_, this->metadata_len_, data);
                }

                std::string Frame::getMetadataUtf8() {
                    if (this->metadata_len_ == 0) {
                        return "";
                    }
                    auto metadata = std::make_unique<char[]>(this->metadata_len_);
                    copyMetadataOut(metadata.get());
                    return std::string(metadata.get());
                }

                std::string Frame::getDataUtf8() {
                    if (this->data_len_ == 0) {
                        return "";
                    }
                    auto data = std::make_unique<char[]>(this->data_len_);
                    copyDataOut(data.get());
                    return std::string(data.get());
                }

                void Frame::copyDataOut(void *data) {
                    this->buffer_data_.copyOut(this->data_offset_, this->data_len_, data);
                }
            }
        }
    }
}