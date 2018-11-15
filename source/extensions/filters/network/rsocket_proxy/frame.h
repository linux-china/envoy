#pragma once

#include "envoy/buffer/buffer.h"
#include "extensions/filters/network/rsocket_proxy/payload.h"

#include <iostream>
#include "common/common/logger.h"

typedef unsigned char byte;

namespace Envoy {
    namespace Extensions {
        namespace NetworkFilters {
            namespace RSocket {
                enum class FrameType : byte {
                    RESERVED = 0x00,
                    SETUP = 0x01,
                    LEASE = 0x02,
                    KEEPALIVE = 0x03,
                    REQUEST_RESPONSE = 0x04,
                    REQUEST_FNF = 0x05,
                    REQUEST_STREAM = 0x06,
                    REQUEST_CHANNEL = 0x07,
                    REQUEST_N = 0x08,
                    CANCEL = 0x09,
                    PAYLOAD = 0x0A,
                    ERROR = 0x0B,
                    METADATA_PUSH = 0x0C,
                    RESUME = 0x0D,
                    RESUME_OK = 0x0E,
                    EXT = 0x3F
                };

                class Frame : public Payload {
                public:
                    Frame(Buffer::Instance &data, const std::string metadata_type_);

                    bool isMetadataPresent() { return this->metadata_present_; }

                    int getFrameLength() { return this->frame_len_; }

                    int getMetadataLength() { return this->metadata_len_; }

                    int getDataLength() { return this->data_len_; }

                    int getStreamId() { return this->stream_id_; }

                    bool hasMetadata() { return this->metadata_present_; }

                    void copyMetadataOut(void *data);

                    void copyDataOut(void *data);

                    std::string getMetadataUtf8();

                    std::string getDataUtf8();

                    FrameType getFrameType() { return this->frame_type_; }

                    std::string toString() {
                        return fmt::format(
                                "frame_len:{},stream_id:{},frame_type:{},metadata_present:{},metadata_offset:{},metadata_len:{},data_offset:{},data_len:{}",
                                this->frame_len_,
                                this->stream_id_,
                                static_cast<int>(this->frame_type_),
                                this->metadata_present_,
                                this->metadata_offset_,
                                this->metadata_len_,
                                this->data_offset_,
                                this->data_len_
                        );

                    }

                private:
                    Buffer::Instance &buffer_data_;
                    std::string metadata_type_;
                    int frame_len_;
                    int stream_id_;
                    FrameType frame_type_;
                    bool metadata_present_;
                    int flags_;
                    int metadata_offset_{-1};
                    int metadata_len_{0};
                    int data_offset_{-1};
                    int data_len_{0};
                };
            }
        }
    }
}