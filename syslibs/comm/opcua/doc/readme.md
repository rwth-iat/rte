# OPCUA Library
This library provides the basic functionalities for opcua. It based on the [open62541](http://www.open62541.org/) stack, which is an open source C implementation of OPC UA. It supports a standard opcua server with an interface to browse rte from an opcua client.

For using the server functionality you just have to create an object of the type "uaServer".

## Changes and settings open62541

### Code changes

#### Export getConfig
```cpp
UA_ServerConfig* UA_Server_getConfig(UA_Server *server);  
```
-->  

```cpp
UA_ServerConfig* UA_EXPORT UA_Server_getConfig(UA_Server *server);  
```
#### Remove DataTypes const
```cpp
typedef struct UA_DataTypeArray {  
    const struct UA_DataTypeArray *next;  
    const size_t typesSize;  
    const UA_DataType *types;  
} UA_DataTypeArray;  
```
-->  

```cpp
typedef struct UA_DataTypeArray {  
    struct UA_DataTypeArray *next;  
    const size_t typesSize;  
    const UA_DataType *types;  
} UA_DataTypeArray;  
```
#### Add writeability for namespace array (ua_server_ns0.c)
```cpp
//________________________ACPLT DELETE START______________________
//
//    /* Test if we append to the existing namespaces */
//    if(newNamespacesSize <= server->namespacesSize)
//        return UA_STATUSCODE_BADTYPEMISMATCH;
//
//    /* Test if the existing namespaces are unchanged */
//    for(size_t i = 0; i < server->namespacesSize; ++i) {
//        if(!UA_String_equal(&server->namespaces[i], &newNamespaces[i]))
//            return UA_STATUSCODE_BADINTERNALERROR;
//    }
//
//    /* Add namespaces */
//    for(size_t i = server->namespacesSize; i < newNamespacesSize; ++i)
//        addNamespace(server, newNamespaces[i]);
//________________________ACPLT DELETE END________________________
```
-->  

```cpp
//________________________ACPLT INSERT START______________________
    UA_String * newNamespacesCopy = NULL;
    if(UA_Array_copy(newNamespaces, newNamespacesSize,
    		&newNamespacesCopy, &UA_TYPES[UA_TYPES_STRING]) != UA_STATUSCODE_GOOD)
    	return UA_STATUSCODE_BADOUTOFMEMORY;
    //This code block must use a lock
    UA_String *oldNamespaces = server->namespaces;
    size_t oldNamespacesSize = newNamespacesSize;
    server->namespaces = newNamespacesCopy;
    server->namespacesSize = newNamespacesSize;
    //End lock zone
    UA_Array_delete(oldNamespaces, oldNamespacesSize, &UA_TYPES[UA_TYPES_STRING]);
//________________________ACPLT INSERT END________________________
```

### ccmake settings
```
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
 ```