/*
 *Copyright [2017] [Chair of Process Control Engineering (ACPLT) RWTH Aachen University]
 *
 *Licensed under the Apache License, Version 2.0 (the "License");
 *you may not use this file except in compliance with the License.
 *You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *Unless required by applicable law or agreed to in writing, software
 *distributed under the License is distributed on an "AS IS" BASIS,
 *WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *See the License for the specific language governing permissions and
 *limitations under the License.
 */

#include "opcua_interface.h"
#include "ua_openaas_generated.h"
#include "ua_openaas_generated_handling.h"
#include "ua_openaas_generated_encoding_binary.h"
static UA_StatusCode parseToVariant(char* value, int valueType,
        UA_Variant *variant) {
    char* tail = NULL;

    UA_Variant_init(variant);
    switch (valueType) {
    case 0: {
        UA_Int32 *intVal = UA_Int32_new();
        *intVal = strtol(value, &tail, 10);
        UA_Variant_setScalar(variant, intVal, &UA_TYPES[UA_TYPES_INT32]);

        break;
    }
    case 1: {
        UA_Double *doubleVal = UA_Double_new();
        *doubleVal = strtold(value, &tail);
        UA_Variant_setScalar(variant, doubleVal, &UA_TYPES[UA_TYPES_DOUBLE]);
        break;
    }
    case 2: {
        UA_String *strVal = UA_String_new();
        UA_String strValtmp = UA_String_fromChars(value);
        UA_String_copy(&strValtmp, strVal);
        UA_Variant_setScalar(variant, strVal, &UA_TYPES[UA_TYPES_STRING]);
    }
        break;
    case 3: {
        //TODO datetime parsing
        UA_String *strVal = UA_String_new();
        UA_String strValtmp = UA_String_fromChars(value);
        UA_String_copy(&strValtmp, strVal);
        UA_Variant_setScalar(variant, strVal, &UA_TYPES[UA_TYPES_DATETIME]);
        break;
    }
    default:
        return UA_STATUSCODE_BADDATAENCODINGUNSUPPORTED;

    }
    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode parseFromVariant(char** value, int* valueType,
        UA_Variant variant) {

    if (variant.type == &UA_TYPES[UA_TYPES_INT32]) {
        sprintf(*value, "%i", *(UA_Int32*) variant.data);
        *valueType = 0;
    } else if (variant.type == &UA_TYPES[UA_TYPES_DOUBLE]) {
        sprintf(*value, "%f", *(UA_Double*) variant.data);
        *valueType = 1;
    } else if (variant.type == &UA_TYPES[UA_TYPES_STRING]) {
        sprintf(*value, "%s", ((UA_String*) variant.data)->data);
        *valueType = 2;
    } else if (variant.type == &UA_TYPES[UA_TYPES_DATETIME]) {
        //TODO datetime parsing
        sprintf(*value, "%s", ((UA_String*) variant.data)->data);
        *valueType = 3;
    } else {
        return UA_STATUSCODE_BADDATAENCODINGUNSUPPORTED;
    }
    // TODO
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode copyOPCUAStringToChar(UA_String src, char **dst) {
    if (src.data == NULL)
        return UA_STATUSCODE_GOOD;
    *dst = UA_malloc(sizeof(char) * (src.length + 1));
    if (*dst == NULL)
        return UA_STATUSCODE_BADOUTOFMEMORY;
    memcpy(*dst, src.data, src.length);
    *(*dst + src.length) = '\0';
    return UA_STATUSCODE_GOOD;
}

/* Asset administration shell */
UA_StatusCode call_CreateAAS(char* ipAddress, char* AASIdSpec, int AASIdType,
        char* name, char* AssetIdSpec, int AssetIdType) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    size_t argInSize = 3;
    size_t argOutSize;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }
    /* convert input to UA types */
    UA_Identification AASId;
    AASId.idType = AASIdType;
    AASId.idSpec = UA_String_fromChars(AASIdSpec);
    UA_Identification assetId;
    assetId.idType = AssetIdType;
    assetId.idSpec = UA_String_fromChars(AssetIdSpec);
    UA_String AASName = UA_String_fromChars(name);
    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[1], &AASName,
            &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&inputArgs[2], &assetId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);

    UA_Variant *output;
    UA_NodeId methNodeId = UA_NODEID_STRING(4,
            "/TechUnits/openAAS/ModelmanagerOpenAAS||createAAS");
    UA_NodeId objectId = UA_NODEID_STRING(4,
            "/TechUnits/openAAS/ModelmanagerOpenAAS");
    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);
        UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
    } else {
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
    }
    /*TODO evaluate Output (Status) */
    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    UA_Client_delete(client);
    return retval;
}

UA_StatusCode call_DeleteAAS(char* ipAddress, char* AASIdSpec, int AASIdType) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    size_t argInSize = 1;
    size_t argOutSize;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }
    /* convert input to UA types */
    UA_Identification AASId;
    AASId.idType = AASIdType;
    AASId.idSpec = UA_String_fromChars(AASIdSpec);

    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);

    UA_Variant *output;
    UA_NodeId methNodeId = UA_NODEID_STRING(4,
            "/TechUnits/openAAS/ModelmanagerOpenAAS||deleteAAS");
    UA_NodeId objectId = UA_NODEID_STRING(4,
            "/TechUnits/openAAS/ModelmanagerOpenAAS");
    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);
        UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
    } else {
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
    }
    /*TODO evaluate Output (Status) */
    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    UA_Client_delete(client);
    return retval;
}

/* Property value statement list */
UA_StatusCode call_CreatePVSL(char* ipAddress, char* AASIdSpec, int AASIdType,
        char* name, char* CarrierIdSpec, int CarrierIdType, char* CreatingInstanceIdSpec, int CreatingInstanceIdType) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    size_t argInSize = 4;
    size_t argOutSize;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }
    /* convert input to UA types */
    UA_Identification AASId;
    AASId.idType = AASIdType;
    AASId.idSpec = UA_String_fromChars(AASIdSpec);

    UA_String listName = UA_String_fromChars(name);

    UA_Identification carrierId;
    carrierId.idType = CarrierIdType;
    carrierId.idSpec = UA_String_fromChars(CarrierIdSpec);

    UA_Identification creatingInstance;
    creatingInstance.idType  = CreatingInstanceIdType;
    creatingInstance.idSpec = UA_String_fromChars(CreatingInstanceIdSpec);

    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[1], &listName,
            &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&inputArgs[2], &carrierId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[3], &creatingInstance,
                &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);


    UA_NodeId methNodeId = UA_NODEID_STRING(4,
            "/TechUnits/openAAS/ModelmanagerOpenAAS||createPVSL");
    UA_NodeId objectId = UA_NODEID_STRING(4,
            "/TechUnits/openAAS/ModelmanagerOpenAAS");
    UA_Variant *output;
    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);
        UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
    } else {
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
    }
    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    UA_Client_delete(client);
    return retval;
}

UA_StatusCode call_DeletePVSL(char* ipAddress, char* AASIdSpec, int AASIdType,
        char* name) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    size_t argInSize = 2;
    size_t argOutSize;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }
    /* convert input to UA types */
    UA_Identification AASId;
    AASId.idType = AASIdType;
    AASId.idSpec = UA_String_fromChars(AASIdSpec);

    UA_String listName = UA_String_fromChars(name);

    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[1], &listName,
            &UA_TYPES[UA_TYPES_STRING]);

    UA_NodeId methNodeId = UA_NODEID_STRING(4,
            "/TechUnits/openAAS/ModelmanagerOpenAAS||deletePVSL");
    UA_NodeId objectId = UA_NODEID_STRING(4,
            "/TechUnits/openAAS/ModelmanagerOpenAAS");
    UA_Variant *output;
    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);
        UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
    } else {
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
    }
    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    UA_Client_delete(client);
    return retval;
}

/* Property value statement */
UA_StatusCode call_CreatePVS(char* ipAddress, char* AASIdSpec, int AASIdType,
        char* PVSLName, char* Name, int ExpressionLogic,
        int ExpressionSemantic, char* Value, int ValueType, char* Unit,
        char* propertyReferenceIdSpec, int propertyReferenceIdType, int view, int visibility) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    size_t argInSize = 10;
    size_t argOutSize;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }

    /* convert input to UA types */
    UA_Identification AASIdK;
    AASIdK.idType = AASIdType;
    AASIdK.idSpec = UA_String_fromChars(AASIdSpec);

    UA_String listName = UA_String_fromChars(PVSLName);
    UA_String propertyname = UA_String_fromChars(Name);

    UA_DataValue dataValue;
    UA_DataValue_init(&dataValue);
    parseToVariant(Value, ValueType, &dataValue.value);
    dataValue.hasSourceTimestamp = true;
    dataValue.sourceTimestamp = UA_DateTime_now();
    dataValue.hasValue = true;

    UA_String unitStr = UA_String_fromChars(Unit);

    UA_Identification ID;
    ID.idSpec = UA_String_fromChars(propertyReferenceIdSpec);
    ID.idType = propertyReferenceIdType;

    UA_Variant_setScalarCopy(&inputArgs[0], &AASIdK,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[1], &listName,
            &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&inputArgs[2], &propertyname,
            &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&inputArgs[3], &ExpressionLogic,
            &UA_OPENAAS[UA_OPENAAS_EXPRESSIONSEMANTICENUM]);
    UA_Variant_setScalarCopy(&inputArgs[4], &ExpressionSemantic,
            &UA_OPENAAS[UA_OPENAAS_EXPRESSIONSEMANTICENUM]);
    UA_Variant_setScalarCopy(&inputArgs[5], &dataValue,
            &UA_TYPES[UA_TYPES_DATAVALUE]);
    UA_Variant_setScalarCopy(&inputArgs[6], &unitStr,
            &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&inputArgs[7], &ID,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[8], &view,
            &UA_OPENAAS[UA_OPENAAS_VIEWENUM]);

    UA_Variant_setScalarCopy(&inputArgs[9], &visibility,
            &UA_TYPES[UA_TYPES_INT32]);
    UA_NodeId methNodeId = UA_NODEID_STRING(4,
            "/TechUnits/openAAS/ModelmanagerOpenAAS||createPVS");
    UA_NodeId objectId = UA_NODEID_STRING(4,
            "/TechUnits/openAAS/ModelmanagerOpenAAS");
    UA_Variant *output;

    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);
        UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
    } else {
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
    }
    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    UA_Client_delete(client);
    return retval;
}

UA_StatusCode call_DeletePVS(char* ipAddress, char* AASIdSpec, int AASIdType,
        char* PVSLName, char* Name) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    size_t argInSize = 3;
    size_t argOutSize;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }

    /* convert input to UA types */
    UA_Identification AASIdK;
    AASIdK.idType = AASIdType;
    AASIdK.idSpec = UA_String_fromChars(AASIdSpec);

    UA_String listName = UA_String_fromChars(PVSLName);
    UA_String propertyname = UA_String_fromChars(Name);

    UA_Variant_setScalarCopy(&inputArgs[0], &AASIdK,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[1], &listName,
            &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&inputArgs[2], &propertyname,
            &UA_TYPES[UA_TYPES_STRING]);

    UA_NodeId methNodeId = UA_NODEID_STRING(4,
            "/TechUnits/openAAS/ModelmanagerOpenAAS||deletePVS");
    UA_NodeId objectId = UA_NODEID_STRING(4,
            "/TechUnits/openAAS/ModelmanagerOpenAAS");
    UA_Variant *output;

    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);
        UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
    } else {
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
    }
    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    UA_Client_delete(client);
    return retval;
}

UA_StatusCode call_SetPVS(char* ipAddress, char* AASIdSpec, int AASIdType,
        char* PVSLName, char* Name, int ExpressionLogic,
        int ExpressionSemantic, char* Value, int ValueType, char* Unit,
        char* propertyReferenceIdSpec, int propertyReferenceIdType, int view,int visibility) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    size_t argInSize = 9;
    size_t argOutSize;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }

    /* convert input to UA types */
    UA_Identification AASIdK;
    AASIdK.idType = AASIdType;
    AASIdK.idSpec = UA_String_fromChars(AASIdSpec);

    UA_String listName = UA_String_fromChars(PVSLName);
    UA_String propertyname = UA_String_fromChars(Name);

    UA_DataValue dataValue;
    UA_DataValue_init(&dataValue);
    parseToVariant(Value, ValueType, &dataValue.value);
    dataValue.hasSourceTimestamp = true;
    dataValue.sourceTimestamp = UA_DateTime_now();
    dataValue.hasValue = true;

    UA_String unitStr = UA_String_fromChars(Unit);

    UA_Identification propertyReference;
    propertyReference.idSpec = UA_String_fromChars(propertyReferenceIdSpec);
    propertyReference.idType = propertyReferenceIdType;

    UA_Variant_setScalarCopy(&inputArgs[0], &AASIdK,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[1], &listName,
            &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&inputArgs[2], &propertyname,
            &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&inputArgs[3], &ExpressionLogic,
            &UA_OPENAAS[UA_OPENAAS_EXPRESSIONLOGICENUM]);
    UA_Variant_setScalarCopy(&inputArgs[4], &ExpressionSemantic,
            &UA_OPENAAS[UA_OPENAAS_EXPRESSIONSEMANTICENUM]);
    UA_Variant_setScalarCopy(&inputArgs[5], &dataValue,
            &UA_TYPES[UA_TYPES_DATAVALUE]);
    UA_Variant_setScalarCopy(&inputArgs[6], &unitStr,
            &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&inputArgs[7], &propertyReference,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[8], &view,
            &UA_OPENAAS[UA_OPENAAS_VIEWENUM]);

    UA_Variant_setScalarCopy(&inputArgs[9], &visibility,
            &UA_TYPES[UA_TYPES_BOOLEAN]);
    UA_NodeId methNodeId = UA_NODEID_STRING(4,
            "/TechUnits/openAAS/ModelmanagerOpenAAS||setPVS");
    UA_NodeId objectId = UA_NODEID_STRING(4,
            "/TechUnits/openAAS/ModelmanagerOpenAAS");
    UA_Variant *output;

    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);
        UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
    } else {
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
    }
    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    UA_Client_delete(client);
    return retval;
}
// get the node id of a specific child node by its browse name
static void getNodeIdOfChildByBrowseName(UA_Client *client, UA_NodeId startId,
        UA_String browseNameToFind,UA_NodeId *foundId) {

    UA_BrowseRequest bReq;
    UA_BrowseRequest_init(&bReq);
    bReq.requestedMaxReferencesPerNode = 0;
    bReq.nodesToBrowse = UA_BrowseDescription_new();
    bReq.nodesToBrowseSize = 1;
    UA_NodeId_copy(&startId,&bReq.nodesToBrowse[0].nodeId);

    bReq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL;

    UA_BrowseResponse bResp = UA_Client_Service_browse(client, bReq);
    for (size_t i = 0; i < bResp.resultsSize; i++) {
        for (size_t j = 0; j < bResp.results[i].referencesSize; j++) {
            if (UA_String_equal(&browseNameToFind,
                    &bResp.results[i].references[j].displayName.text)) {
                UA_NodeId_copy(&bResp.results[i].references[j].nodeId.nodeId,foundId);
                goto clean;
            }
        }
    }

    UA_NodeId tmpNodeId = UA_NODEID_NUMERIC(0,0);
    UA_NodeId_copy(&tmpNodeId, foundId);
    clean:
      UA_BrowseRequest_deleteMembers(&bReq);
      UA_BrowseResponse_deleteMembers(&bResp);
    return;
}


static void FindChildrenWithSpecialType(UA_Client *client, UA_NodeId startId,
        UA_String TypedefinitionBrowseName, UA_ReferenceDescription **ids, size_t *idsSize) {
    UA_BrowseRequest bReq;
    UA_BrowseRequest_init(&bReq);
    bReq.requestedMaxReferencesPerNode = 0;
    bReq.nodesToBrowse = UA_BrowseDescription_new();
    bReq.nodesToBrowseSize = 1;

    UA_NodeId_copy(&startId, &bReq.nodesToBrowse[0].nodeId);
    bReq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL;
    size_t n = 0;
    UA_BrowseResponse bResp = UA_Client_Service_browse(client, bReq);
    if(bResp.resultsSize==0){
        *ids = NULL;
        *idsSize = 0;
        goto clean;
    }
    for (size_t i = 0; i < bResp.results[0].referencesSize; i++) {
        UA_BrowseRequest bReq1;
        UA_BrowseRequest_init(&bReq1);
        bReq1.requestedMaxReferencesPerNode = 0;
        bReq1.nodesToBrowse = UA_BrowseDescription_new();
        bReq1.nodesToBrowseSize = 1;
        UA_NodeId_copy(&bResp.results[0].references[i].nodeId.nodeId,&bReq1.nodesToBrowse[0].nodeId ); //could be a string node id

        bReq1.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL;
        bReq1.nodesToBrowse[0].referenceTypeId = UA_NODEID_NUMERIC(0,UA_NS0ID_HASTYPEDEFINITION);
        UA_BrowseResponse bResp1 = UA_Client_Service_browse(client, bReq1);
        for (size_t j = 0; j < bResp1.results[0].referencesSize; j++) {
            if (UA_String_equal(
                    &bResp1.results[0].references[j].displayName.text,
                    &TypedefinitionBrowseName)) {
                n++;
            }
        }
        UA_BrowseRequest_deleteMembers(&bReq1);
        UA_BrowseResponse_deleteMembers(&bResp1);
    }
    *idsSize = n;
    if(!n)
        goto clean;
    size_t m = 0;
    *ids = UA_Array_new(n, &UA_TYPES[UA_TYPES_REFERENCEDESCRIPTION]);
    for(size_t i=0;i<n;i++){
        UA_ReferenceDescription_init(&(*ids)[i]);
    }
    if(!*ids)
        goto clean;

    for (size_t i = 0; i < bResp.results[0].referencesSize; i++) {
        UA_BrowseRequest bReq1;
        UA_BrowseRequest_init(&bReq1);
        bReq1.requestedMaxReferencesPerNode = 0;
        bReq1.nodesToBrowse = UA_BrowseDescription_new();
        bReq1.nodesToBrowseSize = 1;
        UA_NodeId_copy(&bResp.results[0].references[i].nodeId.nodeId,&bReq1.nodesToBrowse[0].nodeId ); //could be a string node id

        bReq1.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL;
        bReq1.nodesToBrowse[0].referenceTypeId = UA_NODEID_NUMERIC(0,UA_NS0ID_HASTYPEDEFINITION);
        UA_BrowseResponse bResp1 = UA_Client_Service_browse(client, bReq1);
        for (size_t j = 0; j < bResp1.results[0].referencesSize; j++) {
            if (UA_String_equal(
                    &bResp1.results[0].references[j].displayName.text,
                    &TypedefinitionBrowseName)) {
                UA_ReferenceDescription_copy(&bResp.results[0].references[i], &((*ids)[m]));
                m++;
            }
        }
        UA_BrowseRequest_deleteMembers(&bReq1);
        UA_BrowseResponse_deleteMembers(&bResp1);
        if(m>=n){
            goto clean;
        }
    }
    clean:
    UA_BrowseRequest_deleteMembers(&bReq);
    UA_BrowseResponse_deleteMembers(&bResp);
    return;
}
static void getAllPVSLInFolder(UA_Client *client,UA_NodeId entryPoint, UA_ReferenceDescription** foundListIds, size_t *foundListIdsSize){
    UA_String tmpStr = UA_String_fromChars("PropertyValueStatementListType");
    FindChildrenWithSpecialType(client, entryPoint, tmpStr, foundListIds, foundListIdsSize);
    UA_String_deleteMembers(&tmpStr);
}


static int getAllPVSFromList(UA_Client *client,UA_NodeId entryPoint, UA_ReferenceDescription** foundListIds, size_t *foundListIdsSize){
    /*all property value statements are attached via HasProperty Reference */
    UA_String tmpStr = UA_String_fromChars("PropertyType");
    UA_ReferenceDescription *subNodes = NULL;

    size_t subNodesSize = 0;
    FindChildrenWithSpecialType(client, entryPoint,tmpStr, &subNodes, &subNodesSize);

    //filter out only nodes that have the datatype "PropertyValueStatement"
    size_t n=0;
    for(size_t i=0;i<subNodesSize;i++){
        UA_NodeId dataTypeNodeId;
        if(UA_STATUSCODE_GOOD==UA_Client_readDataTypeAttribute(client,subNodes[i].nodeId.nodeId,&dataTypeNodeId)){
            if(UA_OPENAAS[UA_OPENAAS_PROPERTYVALUESTATEMENT].typeId.identifier.numeric == dataTypeNodeId.identifier.numeric){ //ignore namespace index
                n++;
            }
        }
    }

    *foundListIds = UA_Array_new(n,&UA_TYPES[UA_TYPES_REFERENCEDESCRIPTION]);
    if(!foundListIds){
        *foundListIdsSize = 0;
        return -1;
    }

    *foundListIdsSize = n;
    n = 0;
    for(size_t i=0;i<subNodesSize;i++){
        UA_NodeId dataTypeNodeId;
        if(UA_STATUSCODE_GOOD==UA_Client_readDataTypeAttribute(client,subNodes[i].nodeId.nodeId,&dataTypeNodeId)){
            if(UA_OPENAAS[UA_OPENAAS_PROPERTYVALUESTATEMENT].typeId.identifier.numeric == dataTypeNodeId.identifier.numeric){ //ignore namespace index
                if(n<*foundListIdsSize){
                  //UA_ReferenceDescription *pref;
                  //UA_ReferenceDescription_new(pref);

                  UA_ReferenceDescription_copy(&subNodes[i], &(*foundListIds)[n]);
                  n++;
                }else
                    return subNodesSize;
            }
        }
    }
    UA_Array_delete(subNodes,subNodesSize,&UA_TYPES[UA_TYPES_REFERENCEDESCRIPTION]);
    UA_String_deleteMembers(&tmpStr);
    return 0;
}

UA_Boolean findAASNodeId(UA_Client *client,UA_Identification AASId, UA_NodeId* nodeId){
//dummy
    UA_Boolean found = false;


    size_t argInSize = 1;
    size_t argOutSize;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }

    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);

    UA_NodeId methNodeId = UA_NODEID_STRING(4,
            "/TechUnits/openAAS/ModelmanagerOpenAAS||getAASNodeId");
    UA_NodeId objectId = UA_NODEID_STRING(4,
            "/TechUnits/openAAS/ModelmanagerOpenAAS");
    UA_Variant *output;

    UA_StatusCode retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);
    } else {
        found = false;
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
        UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
        goto clean;
    }
    if(!UA_Variant_hasScalarType(output,&UA_TYPES[UA_TYPES_NODEID])){
        found = false;
        goto clean;
    }
    UA_NodeId_copy((UA_NodeId *)output->data,nodeId);

    found = true;
    clean:
      UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);

      return found;
}

int UA_PropertyValueStatement_toPvsType(UA_QualifiedName propertyName, UA_PropertyValueStatement *propertyValueStatement, pvsType *pvs){

    pvs->expressionSemantic = propertyValueStatement->expressionSemantic;

    if(propertyName.name.length>=MAX_STRING_SIZE-1)
        return -1;
    memcpy(pvs->name,propertyName.name.data,propertyName.name.length);

    if(propertyValueStatement->iD.idSpec.length>=MAX_STRING_SIZE-1)
        return -1;
    memcpy(pvs->IDIdSpec,propertyValueStatement->iD.idSpec.data,propertyValueStatement->iD.idSpec.length);
    pvs->IDIdSpec[propertyValueStatement->iD.idSpec.length] = 0;
    pvs->IDIdType = propertyValueStatement->iD.idType;

    pvs->expressionLogic = propertyValueStatement->expressionLogic;
    if(propertyValueStatement->unit.length>=MAX_STRING_SIZE-1)
        return -1;
    memcpy(pvs->unit,propertyValueStatement->unit.data,propertyValueStatement->unit.length);
    pvs->unit[propertyValueStatement->unit.length] = 0;
    pvs->visibility = propertyValueStatement->visibility;
    if(UA_Variant_hasScalarType(&propertyValueStatement->value, &UA_TYPES[UA_TYPES_INT16])) {
        sprintf(pvs->value,"%d",*(UA_Int16*)propertyValueStatement->value.data);
        pvs->valueType = 0;
    }else if(UA_Variant_hasScalarType(&propertyValueStatement->value,&UA_TYPES[UA_TYPES_INT32])){
        sprintf(pvs->value,"%d",*(UA_Int32*)propertyValueStatement->value.data);
        pvs->valueType = 0;
    }else if(UA_Variant_hasScalarType(&propertyValueStatement->value,&UA_TYPES[UA_TYPES_INT64])){
        sprintf(pvs->value,"%" PRId64 "",*(UA_Int64*)propertyValueStatement->value.data);
        pvs->valueType = 0;
    }else if(UA_Variant_hasScalarType(&propertyValueStatement->value,&UA_TYPES[UA_TYPES_FLOAT])){
        sprintf(pvs->value,"%f",*(UA_Float*)propertyValueStatement->value.data);
        pvs->valueType = 1;
    }else if(UA_Variant_hasScalarType(&propertyValueStatement->value,&UA_TYPES[UA_TYPES_DOUBLE])){
        sprintf(pvs->value,"%f",*(UA_Double*)propertyValueStatement->value.data);
        pvs->valueType = 1;
    }else if(UA_Variant_hasScalarType(&propertyValueStatement->value,&UA_TYPES[UA_TYPES_STRING])){
        UA_String *tmpString = (UA_String*)propertyValueStatement->value.data;
        if(tmpString->length>=MAX_STRING_SIZE)
            return -1;
        memcpy(tmpString->data,pvs->value,tmpString->length);
        pvs->valueType = 2;
    }
    pvs->view = propertyValueStatement->view;
    return 0;

}


int getPVSFromListByName(char* ipAddress,char*AASIdSpec,int AASIdType,char* listname, pvsType **pvs_c) {

    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }


    UA_Identification AASIdentification;

    AASIdentification.idSpec = UA_String_fromChars(AASIdSpec);
    AASIdentification.idType = AASIdType;

    UA_NodeId AASNodeId;
    findAASNodeId(client,AASIdentification,&AASNodeId);
    UA_Identification_deleteMembers(&AASIdentification);

    //find body folder of AAS
    UA_NodeId BodyFolderId;
    UA_String bodyStr = UA_String_fromChars("Body");
    getNodeIdOfChildByBrowseName(client,
            AASNodeId,
            bodyStr,&BodyFolderId);

    UA_String_deleteMembers(&bodyStr);
    //print all found PVSL of found AAS
    UA_ReferenceDescription *idsPVSL = NULL;
    size_t idsPVSLSize;

    getAllPVSLInFolder(client, BodyFolderId, &idsPVSL, &idsPVSLSize);
    UA_NodeId_deleteMembers(&BodyFolderId);
    //print found PVSL node Ids
    for (size_t i = 0; i < idsPVSLSize; i++) {
        switch (idsPVSL[i].nodeId.nodeId.identifierType) {
        case UA_NODEIDTYPE_NUMERIC:
            printf("Found NodeId Id: %i \n",
                    idsPVSL[i].nodeId.nodeId.identifier.numeric);
            break;
        case UA_NODEIDTYPE_STRING:
            printf("Found NodeId Id: %.*s \n",idsPVSL[i].nodeId.nodeId.identifier.string.length,
                    idsPVSL[i].nodeId.nodeId.identifier.string.data);
            break;
        default:
        	break;
        }
    }
    if(!idsPVSLSize){
        return -1;
    }
    UA_ReferenceDescription *foundStatementIds = NULL;
    size_t foundStatementIdsSize = 0;
    //print first list

    getAllPVSFromList(client, idsPVSL[0].nodeId.nodeId ,&foundStatementIds, &foundStatementIdsSize);

    for (size_t i = 0; i < foundStatementIdsSize; i++) {
        switch (foundStatementIds[i].nodeId.nodeId.identifierType) {
        case UA_NODEIDTYPE_NUMERIC:
            printf("Found statement with Id: %i \n",
                    foundStatementIds[i].nodeId.nodeId.identifier.numeric);
            break;
        case UA_NODEIDTYPE_STRING:
            printf("Found statement with Id: %.*s \n",foundStatementIds[i].nodeId.nodeId.identifier.string.length,
                    foundStatementIds[i].nodeId.nodeId.identifier.string.data);
            break;
        default:
			break;
        }
    }
    UA_Array_delete(idsPVSL,idsPVSLSize,&UA_TYPES[UA_TYPES_REFERENCEDESCRIPTION]);
    *pvs_c = calloc(foundStatementIdsSize,sizeof(pvsType));

    UA_PropertyValueStatement *propertyValueStatements = UA_Array_new(foundStatementIdsSize,&UA_OPENAAS[UA_OPENAAS_PROPERTYVALUESTATEMENT]);
    for(size_t i=0;i<foundStatementIdsSize;i++)
    {
        UA_PropertyValueStatement_init(&propertyValueStatements[i]);
    }
    for(size_t i=0;i<foundStatementIdsSize;i++){
        UA_Variant variant;
        UA_Client_readValueAttribute(client,foundStatementIds[i].nodeId.nodeId,&variant);

        if(UA_Variant_hasScalarType(&variant,&UA_TYPES[UA_TYPES_EXTENSIONOBJECT])){
            UA_ExtensionObject* ext = variant.data;
            size_t offset = 0;
            UA_PropertyValueStatement_decodeBinary(&ext->content.encoded.body,&offset,&propertyValueStatements[i]);
            printf("property reference id: %.*s\n",propertyValueStatements[i].iD.idSpec.length,propertyValueStatements[i].iD.idSpec.data);
            if(UA_Variant_hasArrayType(&propertyValueStatements->value,&UA_TYPES[UA_TYPES_DOUBLE]))
                printf("value: %lf \n",*(UA_Double*)propertyValueStatements[i].value.data);
            if(UA_Variant_hasArrayType(&propertyValueStatements->value,&UA_TYPES[UA_TYPES_INT32]))
                printf("value: %i \n",*(UA_Int32*)propertyValueStatements[i].value.data);
            UA_PropertyValueStatement_toPvsType(foundStatementIds[i].browseName,&propertyValueStatements[i],&((*pvs_c)[i]));
       }
       UA_Variant_deleteMembers(&variant);

    }
    UA_Array_delete(propertyValueStatements,foundStatementIdsSize,&UA_OPENAAS[UA_OPENAAS_PROPERTYVALUESTATEMENT]);
    UA_Array_delete(foundStatementIds, foundStatementIdsSize, &UA_TYPES[UA_TYPES_REFERENCEDESCRIPTION]);
    UA_NodeId_deleteMembers(&AASNodeId);
    UA_Client_delete(client);
    return foundStatementIdsSize;
//UA_StatusCode call_GetAllPVS(char* ipAddress, char* AASIdSpec, int AASIdType,char* PVSLName){

}
UA_StatusCode call_GetPVS(char* ipAddress, char* AASIdSpec, int AASIdType,
        char* PVSLName, char* Name, int* ExpressionLogic,
        int* ExpressionSemantic, char** Value, int* ValueType, char** Unit,
        char** IDIdSpec, int* IDIdType, int* view,int *visibility) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    size_t argInSize = 3;
    size_t argOutSize;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }

    /* convert input to UA types */
    UA_Identification AASIdK;
    AASIdK.idType = AASIdType;
    AASIdK.idSpec = UA_String_fromChars(AASIdSpec);

    UA_String listName = UA_String_fromChars(PVSLName);

    UA_String propertyname = UA_String_fromChars(Name);

    UA_Variant_setScalarCopy(&inputArgs[0], &AASIdK,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[1], &listName,
            &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&inputArgs[2], &propertyname,
            &UA_TYPES[UA_TYPES_STRING]);

    UA_NodeId methNodeId = UA_NODEID_STRING(4,
            "/TechUnits/openAAS/ModelmanagerOpenAAS||getPVS");
    UA_NodeId objectId = UA_NODEID_STRING(4,
            "/TechUnits/openAAS/ModelmanagerOpenAAS");
    UA_Variant *output;

    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);

        UA_ExpressionLogicEnum* UAExpressionLogic =
                (UA_ExpressionLogicEnum*) output[0].data;
        *ExpressionLogic = *UAExpressionLogic;

        UA_ExpressionSemanticEnum* UAExpressionSemantic =
                (UA_ExpressionSemanticEnum*) output[1].data;
        *ExpressionSemantic = *UAExpressionSemantic;

        UA_DataValue *dataValue = (UA_DataValue*) output[2].data;
        parseFromVariant(Value, ValueType, dataValue->value);

        UA_String *unitstr = (UA_String*) output[3].data;
        copyOPCUAStringToChar(*unitstr, Unit);

        UA_Identification *UAId =
                (UA_Identification*) output[4].data;
        copyOPCUAStringToChar(UAId->idSpec,
                IDIdSpec);
        *IDIdType = UAId->idType;

        UA_ViewEnum* UAView = (UA_ViewEnum*) output[5].data;
        *view = *UAView;


        *visibility = *(int*)output[6].data;
        
        UA_StatusCode *status = (UA_StatusCode*) output[7].data;
        retval = *status;

        UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
    } else {
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
    }
    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    UA_Client_delete(client);
    return retval;
}

/* LifeCycle Entry */
UA_StatusCode call_CreateLCE(char* ipAddress, char* AASIdSpec, int AASIdType,
        char* creatingInstanceIdSpec, int creatingInstanceIdType,
        char* writingInstanceIdSpec, int writingInstanceIdType,
        char* EventClass, char* Subject, UA_DateTime timeStamp, char* value,
        int valueType) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    size_t argInSize = 6;
    size_t argOutSize;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }
    /* convert input to UA types */
    UA_Identification AASId;
    AASId.idType = AASIdType;
    AASId.idSpec = UA_String_fromChars(AASIdSpec);

    UA_Identification creatingInstance;
    creatingInstance.idSpec = UA_String_fromChars(creatingInstanceIdSpec);
    creatingInstance.idType = creatingInstanceIdType;

    UA_Identification writingInstance;
    writingInstance.idSpec = UA_String_fromChars(writingInstanceIdSpec);
    writingInstance.idType = writingInstanceIdType;

    UA_String eventClassStr = UA_String_fromChars(EventClass);

    UA_String subjectStr = UA_String_fromChars(Subject);

    UA_DataValue dataValue;
    UA_DataValue_init(&dataValue);
    parseToVariant(value, valueType, &dataValue.value);
    dataValue.hasSourceTimestamp = true;
    dataValue.sourceTimestamp = timeStamp;
    dataValue.hasValue = true;

    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[1], &creatingInstance,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[2], &writingInstance,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[3], &eventClassStr,
            &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&inputArgs[4], &subjectStr,
            &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&inputArgs[5], &dataValue,
            &UA_TYPES[UA_TYPES_DATAVALUE]);

    UA_NodeId methNodeId = UA_NODEID_STRING(4,
            "/TechUnits/openAAS/ModelmanagerOpenAAS||createLCE");
    UA_NodeId objectId = UA_NODEID_STRING(4,
            "/TechUnits/openAAS/ModelmanagerOpenAAS");

    UA_Variant *output;
    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);
        UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
    } else {
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
    }

    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    return retval;
}

UA_StatusCode call_DeleteLCE(char* ipAddress, char* AASIdSpec, int AASIdType,
        long long LCEId) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    size_t argInSize = 2;
    size_t argOutSize;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }
    /* convert input to UA types */
    UA_Identification AASId;
    AASId.idType = AASIdType;
    AASId.idSpec = UA_String_fromChars(AASIdSpec);

    UA_UInt64 lceId = LCEId;

    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[1], &lceId, &UA_TYPES[UA_TYPES_UINT64]);

    UA_NodeId methNodeId = UA_NODEID_STRING(4,
            "/TechUnits/openAAS/ModelmanagerOpenAAS||deleteLCE");
    UA_NodeId objectId = UA_NODEID_STRING(4,
            "/TechUnits/openAAS/ModelmanagerOpenAAS");

    UA_Variant *output;
    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);
        UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
    } else {
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
    }

    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    return retval;
}

UA_StatusCode call_SetLCE(char* ipAddress, char* AASIdSpec, int AASIdType,
        long long lceId, char* creatingInstanceIdSpec,
        int creatingInstanceIdType, char* writingInstanceIdSpec,
        int writingInstanceIdType, char* EventClass, char* Subject,
        UA_DateTime timeStamp, char* value, int valueType) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    size_t argInSize = 7;
    size_t argOutSize;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }
    /* convert input to UA types */
    UA_Identification AASId;
    AASId.idType = AASIdType;
    AASId.idSpec = UA_String_fromChars(AASIdSpec);

    UA_Identification creatingInstance;
    creatingInstance.idSpec = UA_String_fromChars(creatingInstanceIdSpec);
    creatingInstance.idType = creatingInstanceIdType;

    UA_Identification writingInstance;
    writingInstance.idSpec = UA_String_fromChars(writingInstanceIdSpec);
    writingInstance.idType = writingInstanceIdType;

    UA_String eventClassStr = UA_String_fromChars(EventClass);

    UA_String subjectStr = UA_String_fromChars(Subject);

    UA_DataValue dataValue;
    UA_DataValue_init(&dataValue);
    parseToVariant(value, valueType, &dataValue.value);
    dataValue.hasSourceTimestamp = true;
    dataValue.sourceTimestamp = timeStamp;
    dataValue.hasValue = true;

    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[1], &lceId, &UA_TYPES[UA_TYPES_UINT64]);
    UA_Variant_setScalarCopy(&inputArgs[2], &creatingInstance,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[3], &writingInstance,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[4], &eventClassStr,
            &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&inputArgs[5], &subjectStr,
            &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&inputArgs[6], &dataValue,
            &UA_TYPES[UA_TYPES_DATAVALUE]);

    UA_NodeId methNodeId = UA_NODEID_STRING(4,
            "/TechUnits/openAAS/ModelmanagerOpenAAS||setLCE");
    UA_NodeId objectId = UA_NODEID_STRING(4,
            "/TechUnits/openAAS/ModelmanagerOpenAAS");

    UA_Variant *output;
    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);
        UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
    } else {
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
    }

    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    return retval;
}

UA_StatusCode call_GetLCE(char* ipAddress, char* AASIdSpec, int AASIdType,
        long long lceId, char** creatingInstanceIdSpec,
        int* creatingInstanceIdType, char** writingInstanceIdSpec,
        int* writingInstanceIdType, char** EventClass, char** Subject,
        UA_DateTime* timeStamp, char** value, int* valueType) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    size_t argInSize = 2;
    size_t argOutSize;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }
    /* convert input to UA types */
    UA_Identification AASId;
    AASId.idType = AASIdType;
    AASId.idSpec = UA_String_fromChars(AASIdSpec);

    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[1], &lceId, &UA_TYPES[UA_TYPES_UINT64]);

    UA_NodeId methNodeId = UA_NODEID_STRING(4,
            "/TechUnits/openAAS/ModelmanagerOpenAAS||getLCE");
    UA_NodeId objectId = UA_NODEID_STRING(4,
            "/TechUnits/openAAS/ModelmanagerOpenAAS");

    UA_Variant *output;
    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);

        UA_Identification *creatingInstance =
                (UA_Identification*) output[0].data;
        copyOPCUAStringToChar(creatingInstance->idSpec, creatingInstanceIdSpec);
        *creatingInstanceIdType = creatingInstance->idType;

        UA_Identification *writingInstance = (UA_Identification*) output[1].data;
        copyOPCUAStringToChar(writingInstance->idSpec, writingInstanceIdSpec);
        *writingInstanceIdType = writingInstance->idType;

        UA_String *eventClassStr = (UA_String*) output[2].data;
        copyOPCUAStringToChar(*eventClassStr, EventClass);

        UA_String *subjectStr = (UA_String*) output[3].data;
        copyOPCUAStringToChar(*subjectStr, Subject);

        UA_DataValue *dataValue = (UA_DataValue*) output[4].data;
        *timeStamp = dataValue->sourceTimestamp;
        parseFromVariant(value, valueType, dataValue->value);

        UA_StatusCode *status = (UA_StatusCode*) output[5].data;
        retval = *status;
        UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
    } else {
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
    }
    UA_Client_delete(client);
    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    return retval;
}


UA_StatusCode call_GetLastLCEs(char* ipAddress, char* AASIdSpec, int AASIdType,unsigned int count, lifeCycleEntryType **lifeCycleEntries, unsigned int lifeCycleEntriesCount) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_StatusCode retval = UA_Client_connect(client, ipAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int) retval;
    }
    size_t argInSize = 2;
    size_t argOutSize;
    UA_Variant *inputArgs = UA_Array_new(argInSize,
            &UA_TYPES[UA_TYPES_VARIANT]);
    for (size_t i = 0; i < argInSize; i++) {
        UA_Variant_init(&inputArgs[i]);
    }
    /* convert input to UA types */
    UA_Identification AASId;
    AASId.idType = AASIdType;
    AASId.idSpec = UA_String_fromChars(AASIdSpec);

    UA_Variant_setScalarCopy(&inputArgs[0], &AASId,
            &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
    UA_Variant_setScalarCopy(&inputArgs[1], &count, &UA_TYPES[UA_TYPES_UINT32]);

    UA_NodeId methNodeId = UA_NODEID_STRING(4,
            "/TechUnits/openAAS/ModelmanagerOpenAAS||getLastLCEs");
    UA_NodeId objectId = UA_NODEID_STRING(4,
            "/TechUnits/openAAS/ModelmanagerOpenAAS");

    UA_Variant *output;
    retval = UA_Client_call(client, objectId, methNodeId, argInSize, inputArgs,
            &argOutSize, &output);

    if (retval == UA_STATUSCODE_GOOD) {
        printf(
                "Method call was successful, and %lu returned values available.\n",
                (unsigned long) argOutSize);

        UA_Array_delete(output, argOutSize, &UA_TYPES[UA_TYPES_VARIANT]);
    } else {
        printf(
                "Method call was unsuccessful, and %x returned values available.\n",
                retval);
    }
    UA_Client_delete(client);
    UA_Array_delete(inputArgs, argInSize, &UA_TYPES[UA_TYPES_VARIANT]);
    return retval;
}


