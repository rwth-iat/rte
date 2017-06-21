This library supports functionalities for the openAAS project. It also has an interface for opcua, so that you can browse and read the objects of this library as an external Nodestore.

Objects
-AAS: This is an example how AAS could be structured and implemented. It has Folder for Property Value Statement Lists and a Lifecycle Archive for Lifecycleentries. Additionally an AAS has an Postoffice for receiving and sending messages from/to other AAS encoded with JSON.

-modelmanager: This is a manager object for creating and deleting AAS and there components. Also it supports the message sending in a network with an ComCo and an External Postoffice. Also it supports methods for opcua to interact with an AAS object.

-nodesStoreFunctions: This object is for the interoperability with opcua. It adds an Nodestore to the opcua server and with an interface it supports the browsing and reading of AAS objects. Moreover it enable the methods of the modelmanager object.

-aasDI: This object is for transferring Lifecycle-Entries from an asset to an AAS. The Lifecycle-Entries have to be in a Lifecycle-Archive of an AAS on the asset which also have to be an rte-Server with an modelmanager and an object from nodeStoreFunctionType.

-LCEGenerator: With this Object you can generate Lifecycle-Entries in an AAS of your choice.

-Service:To develop a new service, create a new object of CLASS openaas/Service in XXX.ovm and define your own inputs and outpus (Inputs have to have the FLAGS="i" and outputs the FLAGS="o"). At least you have to overwrite the CallMethod with your own code. To execute the service just set the execute-input to TRUE.
