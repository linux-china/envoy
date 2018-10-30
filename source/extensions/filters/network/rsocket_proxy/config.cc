#include <string>

#include "extensions/filters/network/rsocket_proxy/rsocket.h"
#include "extensions/filters/network/well_known_names.h"


#include "envoy/registry/registry.h"
#include "envoy/server/filter_config.h"

namespace Envoy {
    namespace Extensions {
        namespace NetworkFilters {
            namespace RSocket {
                /**
                 * Config registration for the tap filter. @see NamedNetworkFilterConfigFactory
                 */
                class RSocketConfigFactory : public Server::Configuration::NamedNetworkFilterConfigFactory {
                public:
                    // NamedNetworkFilterConfigFactory
                    Network::FilterFactoryCb createFilterFactory(const Json::Object &,
                                                                 Server::Configuration::FactoryContext &) override {
                        return [](Network::FilterManager &filter_manager) -> void {
                            auto filter = RSocketFilterSharedPtr();
                            filter_manager.addReadFilter(filter);
                            filter_manager.addWriteFilter(filter);
                        };
                    }

                    Network::FilterFactoryCb
                    createFilterFactoryFromProto(const Protobuf::Message &,
                                                 Server::Configuration::FactoryContext &) override {
                        return [](Network::FilterManager &filter_manager) -> void {
                            auto filter = RSocketFilterSharedPtr();
                            filter_manager.addReadFilter(filter);
                            filter_manager.addWriteFilter(filter);
                        };
                    }

                    ProtobufTypes::MessagePtr createEmptyConfigProto() override {
                        return std::make_unique<ProtobufWkt::Empty>();
                    }

                    std::string name() override { return NetworkFilterNames::get().RSocket; }

                };

                /**
                 * Static registration for the rsocket filter. @see RegisterFactory
                 */
                static Registry::RegisterFactory<RSocketConfigFactory, Server::Configuration::NamedNetworkFilterConfigFactory> registered_;

            } // namespace RSocket
        } // namespace NetworkFilters
    } // namespace Extensions
} // namespace Envoy