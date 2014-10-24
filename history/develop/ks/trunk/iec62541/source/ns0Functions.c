/*
 * ns0Functions.c
 *
 *  Created on: 24.10.2014
 *      Author: lars
 */
#include "libov/ov_macros.h"
#include "ksbase.h"
#include "iec62541.h"
#include "iec62541_helpers.h"
#include "NoneTicketAuthenticator.h"
#include "libov/ov_path.h"
#include "libov/ov_memstack.h"

OV_DLLFNCEXPORT	OV_INSTPTR_iec62541_uaBaseNodeType iec62541_uaNamespace0_getNodePtr(
		OV_UINT nodeId
) {
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT UA_Int32 iec62541_uaNamespace0_addNodes(
		UA_AddNodesItem *nodesToAdd,
		UA_UInt32 *indices,
		UA_UInt32 indicesSize,
		UA_AddNodesResult* addNodesResults,
		UA_DiagnosticInfo *diagnosticInfos
) {
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

OV_DLLFNCEXPORT UA_Int32 iec62541_uaNamespace0_addReferences(
		UA_AddReferencesItem* referencesToAdd,
		UA_UInt32 *indices,
		UA_UInt32 indicesSize,
		UA_StatusCode *addReferencesResults,
		UA_DiagnosticInfo diagnosticInfos
) {
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

OV_DLLFNCEXPORT UA_Int32 iec62541_uaNamespace0_deleteNodes(
		UA_DeleteNodesItem *nodesToDelete,
		UA_UInt32 *indices,
		UA_UInt32 indicesSize,
		UA_StatusCode *deleteNodesResults,
		UA_DiagnosticInfo *diagnosticInfos
) {
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

OV_DLLFNCEXPORT UA_Int32 iec62541_uaNamespace0_deleteReferences(
		UA_DeleteReferencesItem *referenceToDelete,
		UA_UInt32 *indices,
		UA_UInt32 indicesSize,
		UA_StatusCode deleteReferencesresults,
		UA_DiagnosticInfo diagnosticInfos
) {
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

OV_DLLFNCEXPORT UA_Int32 iec62541_uaNamespace0_readNodes(
		UA_ReadValueId *readValueIds,
		UA_UInt32 *indices,
		UA_UInt32 indicesSize,
		UA_DataValue *readNodesResults,
		UA_Boolean timeStampToReturn,
		UA_DiagnosticInfo *diagnosticInfos
) {
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

OV_DLLFNCEXPORT UA_Int32 iec62541_uaNamespace0_writeNodes(
		UA_WriteValue *writeValues,
		UA_UInt32 *indices,
		UA_UInt32 indicesSize,
		UA_StatusCode *writeNodesResults,
		UA_DiagnosticInfo *diagnosticInfo
) {
	return UA_STATUSCODE_BADNOTIMPLEMENTED;

}

OV_DLLFNCEXPORT UA_Int32 iec62541_uaNamespace0_browseNodes(
		UA_UInt32 requestedMaxReferencesPerNode,
		UA_BrowseDescription *browseDescriptions,
		UA_Int32 *indices,
		UA_UInt32 indicesSize,
		UA_BrowseResult *browseResults,
		UA_DiagnosticInfo *diagnosticInfos
) {
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

