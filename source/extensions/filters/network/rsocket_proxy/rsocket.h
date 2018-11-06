#pragma once

#include "envoy/network/filter.h"

#include "common/common/logger.h"

#include "envoy/stats/scope.h"

namespace Envoy {
    namespace Extensions {
        namespace NetworkFilters {
            namespace RSocket {

                /**
                 * All rsocket stats. @see stats_macros.h
                 */
                // clang-format off
#define ALL_RSOCKET_STATS(COUNTER, GAUGE, HISTOGRAM)  \
                  COUNTER(msg_request_counter) \
                  GAUGE(rsocket_upstream_connection_count) \
                  HISTOGRAM  (frame_0x04_request_qps) \
                  HISTOGRAM  (frame_0x04_response_qps) \
                  HISTOGRAM  (frame_0x06_request_qps) \
                  HISTOGRAM  (frame_0x06_response_qps)
                // clang-format on

                /**
                 * Struct definition for all rsocket stats. @see stats_macros.h
                 */
                struct RSocketStats {
                    ALL_RSOCKET_STATS(GENERATE_COUNTER_STRUCT, GENERATE_GAUGE_STRUCT, GENERATE_HISTOGRAM_STRUCT)
                };

                /**
                 * Implementation of a rsocket filter
                 */
                class RSocketFilter : public Network::Filter, Logger::Loggable<Logger::Id::filter> {
                public:
                    RSocketFilter(const std::string &stat_prefix, const std::string metadata_type, Stats::Scope &scope)
                            : stats_(generateStats(stat_prefix, scope)), metadata_type(metadata_type) {}

                    // Network::ReadFilter
                    Network::FilterStatus onData(Buffer::Instance &data, bool end_stream) override;

                    Network::FilterStatus onWrite(Buffer::Instance &data, bool end_stream) override;

                    Network::FilterStatus onNewConnection() override { return Network::FilterStatus::Continue; }

                    void initializeReadFilterCallbacks(Network::ReadFilterCallbacks &callbacks) override {
                        read_callbacks_ = &callbacks;
                        ENVOY_LOG(info, "read filter callback");
                    }

                private:
                    Network::ReadFilterCallbacks *read_callbacks_{};
                    RSocketStats stats_;
                    std::string metadata_type;

                    RSocketStats generateStats(const std::string &prefix, Stats::Scope &scope) {
                        return RSocketStats{ALL_RSOCKET_STATS(POOL_COUNTER_PREFIX(scope, prefix),
                                                              POOL_GAUGE_PREFIX(scope, prefix),
                                                              POOL_HISTOGRAM_PREFIX(scope, prefix)
                                            )};
                    }
                };

            } // namespace Tap
        } // namespace NetworkFilters
    } // namespace Extensions
} // namespace Envoy