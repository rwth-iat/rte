This library provides the functionalities for the openAAS project. There is a Mapping for OPC UA which allows you to browse and read the AAS-related objects within an address space of an OPC UA server.

Objects
-AAS: This is an example how AAS could be structured and implemented. It has Folder for Property Value Statement Lists and a Lifecycle Archive for Lifecycleentries. Additionally, the AAS is able to receive messages encoded with JSON.

-modelmanager: This object can create and delete AAS and their components. Furthermore, it provides basic methods to interact with the AAS via OPC UA.

-nodesStoreFunctions: This object is for the interoperability with OPC UA. It adds an Nodestore to the opcua server and with an interface it supports the browsing and reading of AAS objects. Moreover, it enable the methods of the modelmanager object.

-aasDI: This object is for transferring Lifecycle-Entries from an asset to an AAS. The Lifecycle-Entries have to be in a Lifecycle-Archive of an AAS on the asset which also have to be an rte-Server with an modelmanager and an object from nodeStoreFunctionType.

-LCEGenerator: With this Object you can generate Lifecycle-Entries in an AAS of your choice.



