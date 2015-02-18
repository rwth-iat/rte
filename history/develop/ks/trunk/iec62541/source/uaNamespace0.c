
/******************************************************************************
*
*   FILE
*   ----
*   uaNamespace0.c
*
*   History
*   -------
*   2014-10-24   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_iec62541
#define OV_COMPILE_LIBRARY_iec62541
#endif


#include "iec62541.h"
#include "iec62541_helpers.h"
#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include "ks_logfile.h"

#define CREATE_NS0_OBJECT(PPARENT,POBJECT,IDENTIFIER,NS0ID) \
		Ov_CreateObject(iec62541_uaObjectNode, POBJECT, PPARENT, IDENTIFIER); \
	if(POBJECT){ \
		((OV_INSTPTR_iec62541_uaObjectNode)POBJECT)->v_NodeId = NS0ID; \
	}
#define CREATE_OBJECT_TYPE(PPARENT,POBJECT,IDENTIFIER,NS0ID) \
		Ov_CreateObject(iec62541_uaObjectTypeNode, POBJECT, PPARENT, IDENTIFIER); \
	if(POBJECT){ \
		((OV_INSTPTR_iec62541_uaObjectTypeNode)POBJECT)->v_NodeId = NS0ID; \
	}
#define CREATE_REFERENCE_TYPE(PPARENT,POBJECT,IDENTIFIER,NS0ID,ISABSTRACT) \
	Ov_CreateObject(iec62541_uaReferenceTypeNode, POBJECT, PPARENT, IDENTIFIER); \
	if(POBJECT){ \
		((OV_INSTPTR_iec62541_uaReferenceTypeNode)POBJECT)->v_NodeId = NS0ID; \
		((OV_INSTPTR_iec62541_uaReferenceTypeNode)POBJECT)->v_IsAbstract = ISABSTRACT; \
	}
#define CREATE_NS0_VARIABLE_OBJECT(PPARENT,POBJECT,IDENTIFIER,NS0ID) \
	Ov_CreateObject(iec62541_uaVariableNode, POBJECT, PPARENT, IDENTIFIER); \
	if(POBJECT){ \
		((OV_INSTPTR_iec62541_uaVariableNode)POBJECT)->v_NodeId = NS0ID; \
	}
#define CREATE_NS0_VARIABLE_TYPE(PPARENT,POBJECT,IDENTIFIER,NS0ID) \
	Ov_CreateObject(iec62541_uaVariableTypeNode, POBJECT, PPARENT, IDENTIFIER); \
	if(POBJECT){ \
		((OV_INSTPTR_iec62541_uaVariableTypeNode)POBJECT)->v_NodeId = NS0ID; \
	}

#define CREATE_DATA_TYPE(PPARENT,POBJECT,IDENTIFIER,NS0ID,ISABSTRACT) \
	Ov_CreateObject(iec62541_uaDataTypeNode, POBJECT, PPARENT, IDENTIFIER); \
	if(POBJECT){ \
		((OV_INSTPTR_iec62541_uaReferenceTypeNode)POBJECT)->v_NodeId = NS0ID; \
		((OV_INSTPTR_iec62541_uaReferenceTypeNode)POBJECT)->v_IsAbstract = ISABSTRACT; \
	}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaNamespace0_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_iec62541_uaNamespace0 pinst = Ov_StaticPtrCast(iec62541_uaNamespace0, pobj);
    OV_RESULT    result;
    OV_INSTPTR_ov_object pOtherObject = NULL;
    OV_ANY		tempAny = {.value.vartype = OV_VT_VOID, .value.valueunion.val_string = NULL};
    OV_STRING 	namespaces [] = { "http://opcfoundation.org/UA/", "http://acplt.org/OV" };

    OV_INSTPTR_iec62541_uaReferenceTypeNode tmpRef;
    OV_INSTPTR_iec62541_uaReferenceTypeNode branch0;
//    OV_INSTPTR_iec62541_uaReferenceTypeNode branch1;
    OV_INSTPTR_iec62541_uaReferenceTypeNode branch2;
    OV_INSTPTR_iec62541_uaReferenceTypeNode branch3;
    OV_INSTPTR_iec62541_uaReferenceTypeNode branch4;
    OV_INSTPTR_iec62541_uaReferenceTypeNode branch5;
    OV_INSTPTR_iec62541_uaReferenceTypeNode branch6;
    OV_INSTPTR_iec62541_uaReferenceTypeNode branch7;
    OV_INSTPTR_iec62541_uaObjectNode	root;
    OV_INSTPTR_iec62541_uaObjectNode	obj;
    OV_INSTPTR_iec62541_uaObjectNode	ov;
    OV_INSTPTR_iec62541_uaObjectNode	types;
    OV_INSTPTR_iec62541_uaObjectNode	dataTypes;
    OV_INSTPTR_iec62541_uaObjectNode	objects;
    OV_INSTPTR_iec62541_uaObjectNode	server;
    OV_INSTPTR_iec62541_uaObjectNode	referenceTypes;
    OV_INSTPTR_iec62541_uaObjectNode	ov_namespace;
    OV_INSTPTR_iec62541_uaVariableNode	serverStatus;
    OV_INSTPTR_iec62541_uaVariableNode	namespaceArray;
    OV_INSTPTR_iec62541_uaVariableNode	State;
    OV_INSTPTR_iec62541_uaDataTypeNode	pDataType;
    OV_INSTPTR_iec62541_uaVariableTypeNode	pBaseDataVariableType;
    OV_INSTPTR_iec62541_uaObjectTypeNode	pBaseObjectType;

    /* do what the base class does first */
    result = ov_object_constructor(pobj);
    if(Ov_Fail(result))
         return result;


   /* do what */
    Ov_ForEachChild(ov_instantiation, pclass_iec62541_uaNamespace0, pOtherObject){
    	if(pOtherObject != pobj){
    		KS_logfile_error(("%s: cannot instantiate - uaNamespace0 instance already exists", pinst->v_identifier));
    		return OV_ERR_ALREADYEXISTS;
    	}
    }

    //get the container
    CREATE_NS0_OBJECT(pinst,obj,"Root",84);
    root = obj;
    	CREATE_NS0_OBJECT(root,objects,"Objects",85);
    	CREATE_NS0_OBJECT(root,ov,"ov",UA_NODEID_OVROOT);
    	CREATE_NS0_OBJECT(root,obj,"Types",86);
    	types = obj;
    		CREATE_NS0_OBJECT(types,obj,"DataTypes",90);
    		dataTypes = obj;
				CREATE_DATA_TYPE(dataTypes,pDataType,"Boolean",1,FALSE);
				CREATE_DATA_TYPE(dataTypes,pDataType,"Int32",6,FALSE);
				CREATE_DATA_TYPE(dataTypes,pDataType,"UInt32",7,FALSE);
				CREATE_DATA_TYPE(dataTypes,pDataType,"Float",10,FALSE);
				CREATE_DATA_TYPE(dataTypes,pDataType,"Double",11,FALSE);
				CREATE_DATA_TYPE(dataTypes,pDataType,"String",12,FALSE);
				CREATE_DATA_TYPE(dataTypes,pDataType,"DateTime",13,FALSE);
				CREATE_DATA_TYPE(dataTypes,pDataType,"Enumeration",29,FALSE);
				CREATE_DATA_TYPE(dataTypes,pDataType,"ServerStatusDataType",862,FALSE);

    		CREATE_NS0_OBJECT(types,obj,"ReferenceTypes",91);
    		referenceTypes = obj;

    	CREATE_NS0_OBJECT(objects,server,"Server",2253)
    	CREATE_NS0_VARIABLE_OBJECT(server,namespaceArray,"NamespaceArray",2255);
    	namespaceArray->v_ArrayDimensions = 1;
    	namespaceArray->v_Value.value.valueunion.val_string_vec.veclen = 0;
    	namespaceArray->v_Value.value.vartype = OV_VT_VOID;
    	tempAny.value.vartype = OV_VT_STRING_VEC;
    	tempAny.value.valueunion.val_string_vec.veclen = 2;
    	tempAny.value.valueunion.val_string_vec.value = namespaces;
    	Ov_SetAnyValue(&(namespaceArray->v_Value), &tempAny);
    	namespaceArray->v_DataType = 12;

    	CREATE_NS0_VARIABLE_OBJECT(server,serverStatus,"ServerStatus",2256);
    	serverStatus->v_DataType	=	862;	//ServerStatusDataType
    	CREATE_NS0_VARIABLE_OBJECT(serverStatus,State,"State",2259);
    	State->v_Value.value.vartype = OV_VT_INT;
    	State->v_Value.value.valueunion.val_int = 0;
    	State->v_DataType	=	29;	//Enumeration

        CREATE_NS0_OBJECT(pinst,ov_namespace,"OV",9999);


    CREATE_REFERENCE_TYPE(referenceTypes,tmpRef,"References",31,FALSE);
    	branch0 = tmpRef;
    	CREATE_REFERENCE_TYPE(branch0,tmpRef,"HierarchicalReferences",33,FALSE);
    	branch2 = tmpRef;
			CREATE_REFERENCE_TYPE(branch2,tmpRef,"HasEventSource",36,FALSE);
				branch3 = tmpRef;
				CREATE_REFERENCE_TYPE(branch3,tmpRef,"HasNotifier",48,FALSE);
			CREATE_REFERENCE_TYPE(branch2,tmpRef,"HasChild",34,FALSE);
				branch4 = tmpRef;
					CREATE_REFERENCE_TYPE(branch4,tmpRef,"Aggregates",44,FALSE);
					branch5 = tmpRef;
						CREATE_REFERENCE_TYPE(branch5,tmpRef,"HasProperty",46,FALSE);
						CREATE_REFERENCE_TYPE(branch5,tmpRef,"HasCompoent",47,FALSE);
						branch6 = tmpRef;
							CREATE_REFERENCE_TYPE(branch6,tmpRef,"HasOrderedCompoent",49,FALSE);
					CREATE_REFERENCE_TYPE(branch4,tmpRef,"HasSubtype",45,FALSE);

			CREATE_REFERENCE_TYPE(branch2,tmpRef,"Organizes",35,FALSE);
		CREATE_REFERENCE_TYPE(branch0,tmpRef,"NonHierarchicalReferences",32,FALSE);
		branch7 = tmpRef;
			CREATE_REFERENCE_TYPE(branch7,tmpRef,"HasModelParent",50,FALSE);
			CREATE_REFERENCE_TYPE(branch7,tmpRef,"GeneratesEvent",41,FALSE);
			CREATE_REFERENCE_TYPE(branch7,tmpRef,"HasEncoding",38,FALSE);
			CREATE_REFERENCE_TYPE(branch7,tmpRef,"HasModellingRule",37,FALSE);
			CREATE_REFERENCE_TYPE(branch7,tmpRef,"HasDescription",39,FALSE);
			CREATE_REFERENCE_TYPE(branch7,tmpRef,"HasTypeDefinition",40,FALSE);

		CREATE_NS0_OBJECT(types,obj,"VariableTypes",89);
			CREATE_NS0_VARIABLE_TYPE(obj, pBaseDataVariableType, "BaseDataVariableType", 63);

		CREATE_NS0_OBJECT(types,obj,"ObjectTypes",88);
		CREATE_OBJECT_TYPE(obj, pBaseObjectType, "BaseObjectType", 58);



	   //CREATE_NS0_OBJECT(pinst,rootObj,"Views",84);


    return OV_ERR_OK;
}

OV_DLLFNCEXPORT void iec62541_uaNamespace0_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
//    OV_INSTPTR_iec62541_uaNamespace0 pinst = Ov_StaticPtrCast(iec62541_uaNamespace0, pobj);

    /* do what the base class does first */
    ov_object_startup(pobj);

    /* do what */


    return;
}


