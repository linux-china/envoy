#pragma once

#include "envoy/buffer/buffer.h"
#include "extensions/filters/network/rsocket_proxy/payload.h"

#include <iostream>

typedef unsigned char byte;

namespace Envoy {
    namespace Extensions {
        namespace NetworkFilters {
            namespace RSocket {
                class Frame : public Payload {
                public:
                    Frame(Buffer::Instance &data, const std::string metadata_type);

                    bool isMetadataPresent() { return this->metadata_present; }

                    int getFrameLength() { return this->frame_len; }

                    int getMetadataLength() { return this->metadata_len; }

                    int getDataLength() { return this->data_len; }

                    int getStreamId() { return this->stream_id; }

                    bool hasMetadata() { return this->metadata_present; }

                    void copyMetadataOut(void *data);

                    void copyDataOut(void *data);

                    std::string getMetadataUtf8();

                    std::string getDataUtf8();

                    byte getFrameType() { return this->frame_type; }

                private:
                    Buffer::Instance &buffer_data;
                    std::string metadata_type;
                    int frame_len;
                    int stream_id;
                    byte frame_type;
                    bool metadata_present;
                    int flags;
                    int metadata_offset{-1};
                    int metadata_len{0};
                    int data_offset{-1};
                    int data_len{0};
                };
            }
        }
    }
}