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

                    int getStreamId() { return this->stream_id; }

                    byte getFrameType() { return this->frame_type; }

                private:
                    int frame_len;
                    int stream_id;
                    byte frame_type;
                    bool metadata_present;
                    int flags;
                };
            }
        }
    }
}