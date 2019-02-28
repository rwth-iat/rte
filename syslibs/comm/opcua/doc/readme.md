# OPCUA Library
This library provides the basic functionalities for opcua. It based on the [open62541](http://www.open62541.org/) stack, which is an open source C implementation of OPC UA. It supports a standard opcua server with an interface to browse rte from an opcua client.

For using the server functionality you just have to create an object of the type "uaServer".


## Changes and settings open62541

### Code changes

#### Export getConfig
UA_ServerConfig* UA_Server_getConfig(UA_Server *server);  
-->  
UA_ServerConfig* UA_EXPORT UA_Server_getConfig(UA_Server *server);  

#### Remove DataTypes const
typedef struct UA_DataTypeArray {  
    const struct UA_DataTypeArray *next;  
    const size_t typesSize;  
    const UA_DataType *types;  
} UA_DataTypeArray;  
-->  
typedef struct UA_DataTypeArray {  
    struct UA_DataTypeArray *next;  
    const size_t typesSize;  
    const UA_DataType *types;  
} UA_DataTypeArray;  

### ccmake settings
 BUILD_SHARED_LIBS                OFF  
 CMAKE_BUILD_TYPE                 Debug  
 CMAKE_INSTALL_PREFIX             /usr/local  
 LIB_INSTALL_DIR                  lib  
 UA_ARCHITECTURE                  posix  
 UA_BUILD_EXAMPLES                OFF  
 UA_BUILD_TOOLS                   OFF  
 UA_BUILD_UNIT_TESTS              OFF  
 UA_ENABLE_AMALGAMATION           ON  
 UA_ENABLE_COVERAGE               OFF  
 UA_ENABLE_DISCOVERY              ON  
 UA_ENABLE_DISCOVERY_MULTICAST    OFF  
 UA_ENABLE_ENCRYPTION             OFF  
 UA_ENABLE_EXPERIMENTAL_HISTORI   OFF  
 UA_ENABLE_HISTORIZING            ON  
 UA_ENABLE_METHODCALLS            ON  
 UA_ENABLE_NODEMANAGEMENT         ON  
 UA_ENABLE_QUERY                  OFF  
 UA_ENABLE_SUBSCRIPTIONS          ON  
 UA_ENABLE_SUBSCRIPTIONS_EVENTS   OFF  
 UA_LOGLEVEL                      300  
 UA_NAMESPACE_ZERO                REDUCED  