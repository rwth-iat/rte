# OPC UA Server library

This library provides the basic functionalities for opcua. It based on the [open62541](http://www.open62541.org/) stack, which is an open source C implementation of OPC UA. It supports a standard opcua server with an interface to browse rte from an opcua client.

For using the server functionality you just have to create an object of the type "uaServer".

## Open ToDos

* Interfaces
  * Delete `store`, `datatype` and `depencies` variables 
  * Remove `nodeset`, `unload` function
  * Make `load` function abstract
  * Adapt opcua_ovInterface.c
  * Allow to switch off ovInterface -> Documentation: Overwrite CheckNode function in CustomInterface with 'true'
* ServerToInterface
  * Allow only one custom interface: OneToOne
  * Update `link` and `unlink` to forbid linking while server is running and don't load or unload dependent interfaces 
* Server
  * Move serverConfig variables to uaServer
* open62541 Update
  * Update `createConfig` to new open62541 config
  * Implement Nodestore_Switch in OV
* OPC UA Simple library, that starts a server when the library is loaded
