
/******************************************************************************
*
*   FILE
*   ----
*   nodeStoreFunctions.c
*
*   History
*   -------
*   2014-10-21   File created
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
#include "libov/ov_macros.h"


OV_DLLFNCEXPORT UA_Int32 iec62541_nodeStoreFunctions_addNodes(
		UA_AddNodesItem *nodesToAdd,
		UA_UInt32 *indices,
		UA_UInt32 indicesSize,
		UA_AddNodesResult* addNodesResults,
		UA_DiagnosticInfo *diagnosticInfos
) {
    return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

OV_DLLFNCEXPORT UA_Int32 iec62541_nodeStoreFunctions_addReferences(
		UA_AddReferencesItem* referencesToAdd,
		UA_UInt32 *indices,
		UA_UInt32 indicesSize,
		UA_StatusCode *addReferencesResults,
		UA_DiagnosticInfo diagnosticInfos
) {
    return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

OV_DLLFNCEXPORT UA_Int32 iec62541_nodeStoreFunctions_deleteNodes(
		UA_DeleteNodesItem *nodesToDelete,
		UA_UInt32 *indices,
		UA_UInt32 indicesSize,
		UA_StatusCode *deleteNodesResults,
		UA_DiagnosticInfo *diagnosticInfos
) {
    return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

OV_DLLFNCEXPORT UA_Int32 iec62541_nodeStoreFunctions_deleteReferences(
		UA_DeleteReferencesItem *referenceToDelete,
		UA_UInt32 *indices,
		UA_UInt32 indicesSize,
		UA_StatusCode deleteReferencesresults,
		UA_DiagnosticInfo diagnosticInfos
) {
    return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

OV_DLLFNCEXPORT UA_Int32 iec62541_nodeStoreFunctions_readNodes(
		UA_ReadValueId *readValueIds,
		UA_UInt32 *indices,
		UA_UInt32 indicesSize,
		UA_DataValue *readNodesResults,
		UA_Boolean timeStampToReturn,
		UA_DiagnosticInfo *diagnosticInfos
) {
    return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

OV_DLLFNCEXPORT UA_Int32 iec62541_nodeStoreFunctions_writeNodes(
		UA_WriteValue *writeValues,
		UA_UInt32 *indices,
		UA_UInt32 indicesSize,
		UA_StatusCode *writeNodesResults,
		UA_DiagnosticInfo *diagnosticInfo
) {
    return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

OV_DLLFNCEXPORT UA_Int32 iec62541_nodeStoreFunctions_browseNodes(
		UA_UInt32 requestedMaxReferencesPerNode,
		UA_BrowseDescription *browseDescriptions,
		UA_Int32 *indices,
		UA_UInt32 indicesSize,
		UA_BrowseResult *browseResults,
		UA_DiagnosticInfo *diagnosticInfos
) {
    return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

