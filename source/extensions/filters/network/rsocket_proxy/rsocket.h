#pragma once

#include "envoy/network/filter.h"

#include "common/common/logger.h"

namespace Envoy {
    namespace Extensions {
        namespace NetworkFilters {
            namespace RSocket {

                /**
                 * Implementation of a rsocket filter
                 */
                class RSocketFilter : public Network::Filter, Logger::Loggable<Logger::Id::filter> {
                public:
                    // Network::ReadFilter
                    Network::FilterStatus onData(Buffer::Instance &data, bool end_stream) override;

                    Network::FilterStatus onWrite(Buffer::Instance &data, bool end_stream) override;

                    Network::FilterStatus onNewConnection() override { return Network::FilterStatus::Continue; }

                    void initializeReadFilterCallbacks(Network::ReadFilterCallbacks &callbacks) override {
                        read_callbacks_ = &callbacks;
                    }

                private:
                    Network::ReadFilterCallbacks *read_callbacks_{};
                };
                typedef std::shared_ptr<RSocketFilter> RSocketFilterSharedPtr;

            } // namespace Tap
        } // namespace NetworkFilters
    } // namespace Extensions
} // namespace Envoy