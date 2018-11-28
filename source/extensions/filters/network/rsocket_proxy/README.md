RSocket Proxy for Envoy
=================

### Vocabulary

* Frame: A single message containing a request, response, or protocol processing. in Envoy it's "Buffer::Instance& data" parameter of onData method
* Payload: metadata & data for application

### Ports

* Ingress port: 42253, route to 127.0.0.1
* Egress port: 43128, route to upstream RSocket service. The application uses this port on 127.0.0.1 to send RSocket call.

### Dynamic clusters

* Collection cluster metadata information and post to envoy admin API, then update xDS to fetch cluster.


### Todo 

* upstream manager like tcp proxy

### References

* RSocket Protocol: http://rsocket.io/docs/Protocol
