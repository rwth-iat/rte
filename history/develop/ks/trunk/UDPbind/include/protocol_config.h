/*
 * This file defines which protocol (TCP/UDP) to use
 *
 */


//		#define IPbind_useTCP	1	/*	uncomment this line for use of TCP (library TCPbind)	*/
		#define IPbind_useUDP	1	/*	uncomment this line for use of UDP (library UDPbind)	*/



#if IPbind_useTCP && IPbind_useUDP
#error TCP and UDP cannot be used in the same library
#endif

#if IPbind_useTCP
#define	LIB_IPPROTO								"TCP"
#define IPbind_IPChannel 						TCPbind_TCPChannel
#define pclass_IPbind_IPChannel 				pclass_TCPbind_TCPChannel
#define is_of_class_IPbind_IPChannel 			is_of_class_TCPbind_TCPChannel
#define IPbind_IPChannel_socket_get 			TCPbind_TCPChannel_socket_get
#define IPbind_IPChannel_socket_set 			TCPbind_TCPChannel_socket_set
#define IPbind_IPChannel_SendData				TCPbind_TCPChannel_SendData
#define	OV_INSTPTR_IPbind_IPChannel				OV_INSTPTR_TCPbind_TCPChannel
#define	IPbind_IPChannel_AssociateClientHandler	TCPbind_TCPChannel_AssociateClientHandler
#define	IPbind_IPChannel_OpenLocalConn			TCPbind_TCPChannel_OpenLocalConn
#define	IPbind_IPChannel_OpenConnection			TCPbind_TCPChannel_OpenConnection
#define	IPbind_IPChannel_CloseConnection		TCPbind_TCPChannel_CloseConnection
#define	IPbind_IPChannel_startup				TCPbind_TCPChannel_startup
#define	IPbind_IPChannel_shutdown				TCPbind_TCPChannel_shutdown
#define	IPbind_IPChannel_typemethod				TCPbind_TCPChannel_typemethod

#define IPbind_IPListener						TCPbind_TCPListener
#define pclass_IPbind_IPListener				pclass_TCPbind_TCPListener
#define is_of_class_IPbind_IPListener 			is_of_class_TCPbind_TCPListener
#define	OV_INSTPTR_IPbind_IPListener			OV_INSTPTR_TCPbind_TCPListener
#define	IPbind_IPListener_port_get				TCPbind_TCPListener_port_get
#define	IPbind_IPListener_port_set				TCPbind_TCPListener_port_set
#define	IPbind_IPListener_constructor			TCPbind_TCPListener_constructor
#define	IPbind_IPListener_shutdown				TCPbind_TCPListener_shutdown
#define	IPbind_IPListener_typemethod			TCPbind_TCPListener_typemethod

#define	IPPbind_AssocSpecificClientHandler		TCPbind_AssocSpecificClientHandler
#define	passoc_IPPbind_AssocSpecificClientHandler		passoc_TCPbind_AssocSpecificClientHandler
#define	OV_CPT_IPPbind_AssocSpecificClientHandler		OV_CPT_TCPbind_AssocSpecificClientHandler
#define	OV_PCI_IPPbind_AssocSpecificClientHandler		OV_PCI_TCPbind_AssocSpecificClientHandler

#define	ov_library_setglobalvars_IPbind_new				ov_library_setglobalvars_TCPbind_new
#define	ov_library_setglobalvars_IPbind					ov_library_setglobalvars_TCPbind
#define	ov_library_open_IPbind_old						ov_library_open_TCPbind_old
#endif

#if IPbind_useUDP
#define	LIB_IPPROTO								"UDP"
#define IPbind_IPChannel 						UDPbind_UDPChannel
#define pclass_IPbind_IPChannel 				pclass_UDPbind_UDPChannel
#define is_of_class_IPbind_IPChannel 			is_of_class_UDPbind_UDPChannel
#define IPbind_IPChannel_socket_get 			UDPbind_UDPChannel_socket_get
#define IPbind_IPChannel_socket_set 			UDPbind_UDPChannel_socket_set
#define IPbind_IPChannel_SendData				UDPbind_UDPChannel_SendData
#define	OV_INSTPTR_IPbind_IPChannel				OV_INSTPTR_UDPbind_UDPChannel
#define	IPbind_IPChannel_AssociateClientHandler	UDPbind_UDPChannel_AssociateClientHandler
#define	IPbind_IPChannel_OpenLocalConn			UDPbind_UDPChannel_OpenLocalConn
#define	IPbind_IPChannel_OpenConnection			UDPbind_UDPChannel_OpenConnection
#define	IPbind_IPChannel_CloseConnection		UDPbind_UDPChannel_CloseConnection
#define	IPbind_IPChannel_startup				UDPbind_UDPChannel_startup
#define	IPbind_IPChannel_shutdown				UDPbind_UDPChannel_shutdown
#define	IPbind_IPChannel_typemethod				UDPbind_UDPChannel_typemethod

#define IPbind_IPListener						UDPbind_UDPListener
#define pclass_IPbind_IPListener				pclass_UDPbind_UDPListener
#define is_of_class_IPbind_IPListener 			is_of_class_UDPbind_UDPListener
#define	OV_INSTPTR_IPbind_IPListener			OV_INSTPTR_UDPbind_UDPListener
#define	IPbind_IPListener_port_get				UDPbind_UDPListener_port_get
#define	IPbind_IPListener_port_set				UDPbind_UDPListener_port_set
#define	IPbind_IPListener_constructor			UDPbind_UDPListener_constructor
#define	IPbind_IPListener_shutdown				UDPbind_UDPListener_shutdown
#define	IPbind_IPListener_typemethod			UDPbind_UDPListener_typemethod

#define	IPPbind_AssocSpecificClientHandler		UDPbind_AssocSpecificClientHandler
#define	passoc_IPPbind_AssocSpecificClientHandler		passoc_UDPbind_AssocSpecificClientHandler
#define	OV_CPT_IPPbind_AssocSpecificClientHandler		OV_CPT_UDPbind_AssocSpecificClientHandler
#define	OV_PCI_IPPbind_AssocSpecificClientHandler		OV_PCI_UDPbind_AssocSpecificClientHandler
#endif

#if !IPbind_useTCP && !IPbind_useUDP
#error You must use exactly one protocol (TCP/UDP)
#endif
