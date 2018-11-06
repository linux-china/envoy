#pragma once

#include "envoy/buffer/buffer.h"
#include <iostream>

typedef unsigned char byte;

namespace Envoy {
    namespace Extensions {
        namespace NetworkFilters {
            namespace RSocket {
                class Frame {
                public:
                    Frame(Buffer::Instance &data);

                    bool isMetadataPresent() { return this->metadata_present; }

                    int getFrameLength() { return this->frame_len; }

                    int getMetadataLength() { return this->metadata_len; }

                    int getDataLength() { return this->data_len; }

                    int getStreamId() { return this->stream_id; }

                    void copyMetadataOut(void *data);

                    void copyDataOut(void *data);

                    byte getFrameType() { return this->frame_type; }

                private:
                    Buffer::Instance &buffer_data;
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