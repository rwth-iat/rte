This library supports functionalities for the openAAS project. It also has an interface for opcua, so that you can browse and read the objects of this library as an external Nodestore.

Objects
-AAS: This is an example how AAS could be structured and implemented. It has Folder for Property Value Statement Lists and a Lifecycle Archive for Lifecycleentries. Additionally an AAS has an Postoffice for receiving and sending messages from/to other AAS encoded with JSON.

-modelmanager: This is a manager object for creating and deleting AAS and there components. Also it supports methods for opcua to interact with an AAS object.

-LCEGenerator: With this Object you can generate Lifecycle-Entries in an AAS of your choice.

-Service: With this abstract class you can implement your own services, which can be located everywhere. You just have to define your Inputs and Outputs with the Flags "i" or "o" and reimplement the "CallMethod"-function. ExampleService-Object is an example how to implement a new service (all supported types are shown).

