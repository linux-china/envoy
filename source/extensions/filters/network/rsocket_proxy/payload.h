#pragma once

#include "envoy/buffer/buffer.h"
#include <string>

namespace Envoy {
    namespace Extensions {
        namespace NetworkFilters {
            namespace RSocket {
                class Payload {
                public:
                    virtual ~Payload() {}

                    virtual bool hasMetadata() PURE;

                    virtual void copyMetadataOut(void *data) PURE;

                    virtual void copyDataOut(void *data) PURE;

                    virtual std::string getMetadataUtf8() PURE;

                    virtual std::string getDataUtf8() PURE;

                };
            }
        }
    }
}