#include <string>

#include "extensions/filters/network/rsocket_proxy/rsocket.h"
#include "extensions/filters/network/rsocket_proxy/config.h"
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

                Network::FilterFactoryCb RSocketConfigFactory::createFilterFactoryFromProtoTyped(
                        const envoy::extensions::filters::network::rsocket_proxy::v2alpha1::RSocketProxy &proto_config,
                        Server::Configuration::FactoryContext &context) {

                    const std::string stat_prefix = fmt::format("rsocket.{}.", proto_config.stat_prefix());
                    //todo factory context to add config for filter
                    Stats::Scope &scope_ = context.scope();
                    const std::string &prefix = proto_config.stat_prefix();
                    return [&](Network::FilterManager &filter_manager) -> void {
                        filter_manager.addFilter(std::make_shared<RSocketFilter>(prefix, scope_));
                    };
                }

                /**
                 * Static registration for the rsocket filter. @see RegisterFactory
                 */
                static Registry::RegisterFactory<RSocketConfigFactory, Server::Configuration::NamedNetworkFilterConfigFactory> registered_;

            } // namespace RSocket
        } // namespace NetworkFilters
    } // namespace Extensions
} // namespace Envoy