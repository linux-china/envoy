RSocket proxy filter
=======================
RSocket is an application protocol providing Reactive Streams semantics. 

It enables the following symmetric interaction models via async message passing over a single connection:

* request/response (stream of 1)
* request/stream (finite stream of many)
* fire-and-forget (no response)
* channel (bi-directional streams)

# Code structure

* config.h: configuration for RSocket filter
* rsocket.h: RSocket filter
* frame.h: RSocket Frame
* payload.h: Rsocket Payload
* Envoy configuration for RSocket Filter:  /envoy/api/envoy/config/filter/network/rsocket_proxy/v2alpha1/rsocket_proxy.proto 

# Todo

* payload metadata decoding: protobuf & json
* upstream connection pool management
* metrics
* access log: debug level like rsocket-cli

# References

* RSocket Home: http://rsocket.io/
* RSocket Protocol: http://rsocket.io/docs/Protocol


