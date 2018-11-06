#pragma once

#include <string>

#include "extensions/filters/network/rsocket_proxy/rsocket.h"
#include "extensions/filters/network/well_known_names.h"

#include "envoy/config/filter/network/rsocket_proxy/v2alpha1/rsocket_proxy.pb.h"
#include "envoy/config/filter/network/rsocket_proxy/v2alpha1/rsocket_proxy.pb.validate.h"

#include "envoy/registry/registry.h"
#include "envoy/server/filter_config.h"
#include "extensions/filters/network/common/factory_base.h"

namespace Envoy {
    namespace Extensions {
        namespace NetworkFilters {
            namespace RSocket {

                /**
                 * Config registration for the redis proxy filter. @see NamedNetworkFilterConfigFactory.
                 */
                class RSocketConfigFactory
                        : public Common::FactoryBase<envoy::extensions::filters::network::rsocket_proxy::v2alpha1::RSocketProxy> {
                public:
                    RSocketConfigFactory() : FactoryBase(NetworkFilterNames::get().RSocket) {}

                private:
                    Network::FilterFactoryCb createFilterFactoryFromProtoTyped(
                            const envoy::extensions::filters::network::rsocket_proxy::v2alpha1::RSocketProxy &proto_config,
                            Server::Configuration::FactoryContext &context) override;
                };

            } // namespace RSocket
        } // namespace NetworkFilters
    } // namespace Extensions
} // namespace Envoy
