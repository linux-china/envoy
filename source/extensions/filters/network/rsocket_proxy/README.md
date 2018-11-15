RSocket Proxy for Envoy
=================

# Vocabulary

* Frame: A single message containing a request, response, or protocol processing. in Envoy it's "Buffer::Instance& data" parameter of onData method
* Payload: metadata & data for application

# References

* RSocket Protocol: http://rsocket.io/docs/Protocol
