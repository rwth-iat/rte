/* THIS IS A SINGLE-FILE DISTRIBUTION CONCATENATED FROM THE OPEN62541 SOURCES 
 * visit http://open62541.org/ for information about this software
 * Git-Revision: v0.1.0-RC4-539-g141947e
 */
 
 /*
 * Copyright (C) 2015 the contributors as stated in the AUTHORS file
 *
 * This file is part of open62541. open62541 is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser General
 * Public License, version 3 (as published by the Free Software Foundation) with
 * a static linking exception as stated in the LICENSE file provided with
 * open62541.
 *
 * open62541 is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 */

#ifndef OPEN62541_H_
#define OPEN62541_H_

#ifdef __cplusplus
extern "C" {
#endif


/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src_generated/ua_config.h" ***********************************/


#ifndef _XOPEN_SOURCE
# define _XOPEN_SOURCE 500
#endif

#define UA_LOGLEVEL 300
/* #undef UA_MULTITHREADING */
/* #undef ENABLE_METHODCALLS */
#define ENABLE_SUBSCRIPTIONS

/* Function Export */
#ifdef _WIN32
# ifdef UA_DYNAMIC_LINKING
#  ifdef __GNUC__
#   define UA_EXPORT __attribute__ ((dllexport))
#  else
#   define UA_EXPORT __declspec(dllexport)
#  endif
# else
#  ifdef __GNUC__
#   define UA_EXPORT __attribute__ ((dllexport))
#  else
#   define UA_EXPORT __declspec(dllimport)
#  endif
# endif
#else
# if __GNUC__ || __clang__
#  define UA_EXPORT __attribute__ ((visibility ("default")))
# else
#  define UA_EXPORT
# endif
#endif

/* Endianness */
#if defined(__linux__) || defined(__APPLE__)
#  ifndef __QNX__
#    if defined(__APPLE__) || defined(__MACH__)
#     include <machine/endian.h>
#    else
#     include <endian.h>
#    endif
#  endif
# if ( __BYTE_ORDER != __LITTLE_ENDIAN )
#  define UA_NON_LITTLEENDIAN_ARCHITECTURE
# endif
#endif

/* Force non-little endian manually by setting the following. */
// #define UA_NON_LITTLEENDIAN_ARCHITECTURE
// #define htole16(x) {...}(x)
// #define htole32(x) {...}(x)
// #define htole64(x) {...}(x)
// #define le16toh(x) {...}(x)
// #define le32toh(x) {...}(x)
// #define le64toh(x) {...}(x)

/* Mixed Endian */
#ifdef __ARM_ARCH_4T__
# define UA_MIXED_ENDIAN
# define UA_NON_LITTLEENDIAN_ARCHITECTURE
#endif

/* Aligned Memory Access */
#if defined(__arm__) && !defined(__ARM_FEATURE_UNALIGNED)
# define UA_ALIGNED_MEMORY_ACCESS
#endif



/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/include/ua_statuscodes.h" ***********************************/


enum UA_StatusCode {
	UA_STATUSCODE_GOOD = 0x00,
	UA_STATUSCODE_BADUNEXPECTEDERROR = 0x80010000, // An unexpected error occurred.
	UA_STATUSCODE_BADINTERNALERROR = 0x80020000, // An internal error occurred as a result of a programming or configuration error.
	UA_STATUSCODE_BADOUTOFMEMORY = 0x80030000, // Not enough memory to complete the operation.
	UA_STATUSCODE_BADRESOURCEUNAVAILABLE = 0x80040000, // An operating system resource is not available.
	UA_STATUSCODE_BADCOMMUNICATIONERROR = 0x80050000, // A low level communication error occurred.
	UA_STATUSCODE_BADENCODINGERROR = 0x80060000, // Encoding halted because of invalid data in the objects being serialized.
	UA_STATUSCODE_BADDECODINGERROR = 0x80070000, // Decoding halted because of invalid data in the stream.
	UA_STATUSCODE_BADENCODINGLIMITSEXCEEDED = 0x80080000, // The message encoding/decoding limits imposed by the stack have been exceeded.
	UA_STATUSCODE_BADREQUESTTOOLARGE = 0x80b80000, // The request message size exceeds limits set by the server.
	UA_STATUSCODE_BADRESPONSETOOLARGE = 0x80b90000, // The response message size exceeds limits set by the client.
	UA_STATUSCODE_BADUNKNOWNRESPONSE = 0x80090000, // An unrecognized response was received from the server.
	UA_STATUSCODE_BADTIMEOUT = 0x800a0000, // The operation timed out.
	UA_STATUSCODE_BADSERVICEUNSUPPORTED = 0x800b0000, // The server does not support the requested service.
	UA_STATUSCODE_BADSHUTDOWN = 0x800c0000, // The operation was cancelled because the application is shutting down.
	UA_STATUSCODE_BADSERVERNOTCONNECTED = 0x800d0000, // The operation could not complete because the client is not connected to the server.
	UA_STATUSCODE_BADSERVERHALTED = 0x800e0000, // The server has stopped and cannot process any requests.
	UA_STATUSCODE_BADNOTHINGTODO = 0x800f0000, // There was nothing to do because the client passed a list of operations with no elements.
	UA_STATUSCODE_BADTOOMANYOPERATIONS = 0x80100000, // The request could not be processed because it specified too many operations.
	UA_STATUSCODE_BADTOOMANYMONITOREDITEMS = 0x80db0000, // The request could not be processed because there are too many monitored items in the subscription.
	UA_STATUSCODE_BADDATATYPEIDUNKNOWN = 0x80110000, // The extension object cannot be (de)serialized because the data type id is not recognized.
	UA_STATUSCODE_BADCERTIFICATEINVALID = 0x80120000, // The certificate provided as a parameter is not valid.
	UA_STATUSCODE_BADSECURITYCHECKSFAILED = 0x80130000, // An error occurred verifying security.
	UA_STATUSCODE_BADCERTIFICATETIMEINVALID = 0x80140000, // The Certificate has expired or is not yet valid.
	UA_STATUSCODE_BADCERTIFICATEISSUERTIMEINVALID = 0x80150000, // An Issuer Certificate has expired or is not yet valid.
	UA_STATUSCODE_BADCERTIFICATEHOSTNAMEINVALID = 0x80160000, // The HostName used to connect to a Server does not match a HostName in the Certificate.
	UA_STATUSCODE_BADCERTIFICATEURIINVALID = 0x80170000, // The URI specified in the ApplicationDescription does not match the URI in the Certificate.
	UA_STATUSCODE_BADCERTIFICATEUSENOTALLOWED = 0x80180000, // The Certificate may not be used for the requested operation.
	UA_STATUSCODE_BADCERTIFICATEISSUERUSENOTALLOWED = 0x80190000, // The Issuer Certificate may not be used for the requested operation.
	UA_STATUSCODE_BADCERTIFICATEUNTRUSTED = 0x801a0000, // The Certificate is not trusted.
	UA_STATUSCODE_BADCERTIFICATEREVOCATIONUNKNOWN = 0x801b0000, // It was not possible to determine if the Certificate has been revoked.
	UA_STATUSCODE_BADCERTIFICATEISSUERREVOCATIONUNKNOWN = 0x801c0000, // It was not possible to determine if the Issuer Certificate has been revoked.
	UA_STATUSCODE_BADCERTIFICATEREVOKED = 0x801d0000, // The Certificate has been revoked.
	UA_STATUSCODE_BADCERTIFICATEISSUERREVOKED = 0x801e0000, // The Issuer Certificate has been revoked.
	UA_STATUSCODE_BADUSERACCESSDENIED = 0x801f0000, // User does not have permission to perform the requested operation.
	UA_STATUSCODE_BADIDENTITYTOKENINVALID = 0x80200000, // The user identity token is not valid.
	UA_STATUSCODE_BADIDENTITYTOKENREJECTED = 0x80210000, // The user identity token is valid but the server has rejected it.
	UA_STATUSCODE_BADSECURECHANNELIDINVALID = 0x80220000, // The specified secure channel is no longer valid.
	UA_STATUSCODE_BADINVALIDTIMESTAMP = 0x80230000, // The timestamp is outside the range allowed by the server.
	UA_STATUSCODE_BADNONCEINVALID = 0x80240000, // The nonce does appear to be not a random value or it is not the correct length.
	UA_STATUSCODE_BADSESSIONIDINVALID = 0x80250000, // The session id is not valid.
	UA_STATUSCODE_BADSESSIONCLOSED = 0x80260000, // The session was closed by the client.
	UA_STATUSCODE_BADSESSIONNOTACTIVATED = 0x80270000, // The session cannot be used because ActivateSession has not been called.
	UA_STATUSCODE_BADSUBSCRIPTIONIDINVALID = 0x80280000, // The subscription id is not valid.
	UA_STATUSCODE_BADREQUESTHEADERINVALID = 0x802a0000, // The header for the request is missing or invalid.
	UA_STATUSCODE_BADTIMESTAMPSTORETURNINVALID = 0x802b0000, // The timestamps to return parameter is invalid.
	UA_STATUSCODE_BADREQUESTCANCELLEDBYCLIENT = 0x802c0000, // The request was cancelled by the client.
	UA_STATUSCODE_GOODSUBSCRIPTIONTRANSFERRED = 0x002d0000, // The subscription was transferred to another session.
	UA_STATUSCODE_GOODCOMPLETESASYNCHRONOUSLY = 0x002e0000, // The processing will complete asynchronously.
	UA_STATUSCODE_GOODOVERLOAD = 0x002f0000, // Sampling has slowed down due to resource limitations.
	UA_STATUSCODE_GOODCLAMPED = 0x00300000, // The value written was accepted but was clamped.
	UA_STATUSCODE_BADNOCOMMUNICATION = 0x80310000, // Communication with the data source is defined, but not established, and there is no last known value available.
	UA_STATUSCODE_BADWAITINGFORINITIALDATA = 0x80320000, // Waiting for the server to obtain values from the underlying data source.
	UA_STATUSCODE_BADNODEIDINVALID = 0x80330000, // The syntax of the node id is not valid.
	UA_STATUSCODE_BADNODEIDUNKNOWN = 0x80340000, // The node id refers to a node that does not exist in the server address space.
	UA_STATUSCODE_BADATTRIBUTEIDINVALID = 0x80350000, // The attribute is not supported for the specified Node.
	UA_STATUSCODE_BADINDEXRANGEINVALID = 0x80360000, // The syntax of the index range parameter is invalid.
	UA_STATUSCODE_BADINDEXRANGENODATA = 0x80370000, // No data exists within the range of indexes specified.
	UA_STATUSCODE_BADDATAENCODINGINVALID = 0x80380000, // The data encoding is invalid.
	UA_STATUSCODE_BADDATAENCODINGUNSUPPORTED = 0x80390000, // The server does not support the requested data encoding for the node.
	UA_STATUSCODE_BADNOTREADABLE = 0x803a0000, // The access level does not allow reading or subscribing to the Node.
	UA_STATUSCODE_BADNOTWRITABLE = 0x803b0000, // The access level does not allow writing to the Node.
	UA_STATUSCODE_BADOUTOFRANGE = 0x803c0000, // The value was out of range.
	UA_STATUSCODE_BADNOTSUPPORTED = 0x803d0000, // The requested operation is not supported.
	UA_STATUSCODE_BADNOTFOUND = 0x803e0000, // A requested item was not found or a search operation ended without success.
	UA_STATUSCODE_BADOBJECTDELETED = 0x803f0000, // The object cannot be used because it has been deleted.
	UA_STATUSCODE_BADNOTIMPLEMENTED = 0x80400000, // Requested operation is not implemented.
	UA_STATUSCODE_BADMONITORINGMODEINVALID = 0x80410000, // The monitoring mode is invalid.
	UA_STATUSCODE_BADMONITOREDITEMIDINVALID = 0x80420000, // The monitoring item id does not refer to a valid monitored item.
	UA_STATUSCODE_BADMONITOREDITEMFILTERINVALID = 0x80430000, // The monitored item filter parameter is not valid.
	UA_STATUSCODE_BADMONITOREDITEMFILTERUNSUPPORTED = 0x80440000, // The server does not support the requested monitored item filter.
	UA_STATUSCODE_BADFILTERNOTALLOWED = 0x80450000, // A monitoring filter cannot be used in combination with the attribute specified.
	UA_STATUSCODE_BADSTRUCTUREMISSING = 0x80460000, // A mandatory structured parameter was missing or null.
	UA_STATUSCODE_BADEVENTFILTERINVALID = 0x80470000, // The event filter is not valid.
	UA_STATUSCODE_BADCONTENTFILTERINVALID = 0x80480000, // The content filter is not valid.
	UA_STATUSCODE_BADFILTEROPERATORINVALID = 0x80c10000, // An unrecognized operator was provided in a filter.
	UA_STATUSCODE_BADFILTEROPERATORUNSUPPORTED = 0x80c20000, // A valid operator was provided, but the server does not provide support for this filter operator.
	UA_STATUSCODE_BADFILTEROPERANDCOUNTMISMATCH = 0x80c30000, // The number of operands provided for the filter operator was less then expected for the operand provided.
	UA_STATUSCODE_BADFILTEROPERANDINVALID = 0x80490000, // The operand used in a content filter is not valid.
	UA_STATUSCODE_BADFILTERELEMENTINVALID = 0x80c40000, // The referenced element is not a valid element in the content filter.
	UA_STATUSCODE_BADFILTERLITERALINVALID = 0x80c50000, // The referenced literal is not a valid value.
	UA_STATUSCODE_BADCONTINUATIONPOINTINVALID = 0x804a0000, // The continuation point provide is longer valid.
	UA_STATUSCODE_BADNOCONTINUATIONPOINTS = 0x804b0000, // The operation could not be processed because all continuation points have been allocated.
	UA_STATUSCODE_BADREFERENCETYPEIDINVALID = 0x804c0000, // The operation could not be processed because all continuation points have been allocated.
	UA_STATUSCODE_BADBROWSEDIRECTIONINVALID = 0x804d0000, // The browse direction is not valid.
	UA_STATUSCODE_BADNODENOTINVIEW = 0x804e0000, // The node is not part of the view.
	UA_STATUSCODE_BADSERVERURIINVALID = 0x804f0000, // The ServerUri is not a valid URI.
	UA_STATUSCODE_BADSERVERNAMEMISSING = 0x80500000, // No ServerName was specified.
	UA_STATUSCODE_BADDISCOVERYURLMISSING = 0x80510000, // No DiscoveryUrl was specified.
	UA_STATUSCODE_BADSEMPAHOREFILEMISSING = 0x80520000, // The semaphore file specified by the client is not valid.
	UA_STATUSCODE_BADREQUESTTYPEINVALID = 0x80530000, // The security token request type is not valid.
	UA_STATUSCODE_BADSECURITYMODEREJECTED = 0x80540000, // The security mode does not meet the requirements set by the Server.
	UA_STATUSCODE_BADSECURITYPOLICYREJECTED = 0x80550000, // The security policy does not meet the requirements set by the Server.
	UA_STATUSCODE_BADTOOMANYSESSIONS = 0x80560000, // The server has reached its maximum number of sessions.
	UA_STATUSCODE_BADUSERSIGNATUREINVALID = 0x80570000, // The user token signature is missing or invalid.
	UA_STATUSCODE_BADAPPLICATIONSIGNATUREINVALID = 0x80580000, // The signature generated with the client certificate is missing or invalid.
	UA_STATUSCODE_BADNOVALIDCERTIFICATES = 0x80590000, // The client did not provide at least one software certificate that is valid and meets the profile requirements for the server.
	UA_STATUSCODE_BADIDENTITYCHANGENOTSUPPORTED = 0x80c60000, // The Server does not support changing the user identity assigned to the session.
	UA_STATUSCODE_BADREQUESTCANCELLEDBYREQUEST = 0x805a0000, // The request was canceled by the client with the Cancel service.
	UA_STATUSCODE_BADPARENTNODEIDINVALID = 0x805b0000, // The parent node id does not to refer to a valid node.
	UA_STATUSCODE_BADREFERENCENOTALLOWED = 0x805c0000, // The reference could not be created because it violates constraints imposed by the data model.
	UA_STATUSCODE_BADNODEIDREJECTED = 0x805d0000, // The requested node id was reject because it was either invalid or server does not allow node ids to be specified by the client.
	UA_STATUSCODE_BADNODEIDEXISTS = 0x805e0000, // The requested node id is already used by another node.
	UA_STATUSCODE_BADNODECLASSINVALID = 0x805f0000, // The node class is not valid.
	UA_STATUSCODE_BADBROWSENAMEINVALID = 0x80600000, // The browse name is invalid.
	UA_STATUSCODE_BADBROWSENAMEDUPLICATED = 0x80610000, // The browse name is not unique among nodes that share the same relationship with the parent.
	UA_STATUSCODE_BADNODEATTRIBUTESINVALID = 0x80620000, // The node attributes are not valid for the node class.
	UA_STATUSCODE_BADTYPEDEFINITIONINVALID = 0x80630000, // The type definition node id does not reference an appropriate type node.
	UA_STATUSCODE_BADSOURCENODEIDINVALID = 0x80640000, // The source node id does not reference a valid node.
	UA_STATUSCODE_BADTARGETNODEIDINVALID = 0x80650000, // The target node id does not reference a valid node.
	UA_STATUSCODE_BADDUPLICATEREFERENCENOTALLOWED = 0x80660000, // The reference type between the nodes is already defined.
	UA_STATUSCODE_BADINVALIDSELFREFERENCE = 0x80670000, // The server does not allow this type of self reference on this node.
	UA_STATUSCODE_BADREFERENCELOCALONLY = 0x80680000, // The reference type is not valid for a reference to a remote server.
	UA_STATUSCODE_BADNODELETERIGHTS = 0x80690000, // The server will not allow the node to be deleted.
	UA_STATUSCODE_UNCERTAINREFERENCENOTDELETED = 0x40bc0000, // The server was not able to delete all target references.
	UA_STATUSCODE_BADSERVERINDEXINVALID = 0x806a0000, // The server index is not valid.
	UA_STATUSCODE_BADVIEWIDUNKNOWN = 0x806b0000, // The view id does not refer to a valid view node.
	UA_STATUSCODE_BADVIEWTIMESTAMPINVALID = 0x80c90000, // The view timestamp is not available or not supported.
	UA_STATUSCODE_BADVIEWPARAMETERMISMATCH = 0x80ca0000, // The view parameters are not consistent with each other.
	UA_STATUSCODE_BADVIEWVERSIONINVALID = 0x80cb0000, // The view version is not available or not supported.
	UA_STATUSCODE_UNCERTAINNOTALLNODESAVAILABLE = 0x40c00000, // The list of references may not be complete because the underlying system is not available.
	UA_STATUSCODE_GOODRESULTSMAYBEINCOMPLETE = 0x00ba0000, // The server should have followed a reference to a node in a remote server but did not. The result set may be incomplete.
	UA_STATUSCODE_BADNOTTYPEDEFINITION = 0x80c80000, // The provided Nodeid was not a type definition nodeid.
	UA_STATUSCODE_UNCERTAINREFERENCEOUTOFSERVER = 0x406c0000, // One of the references to follow in the relative path references to a node in the address space in another server.
	UA_STATUSCODE_BADTOOMANYMATCHES = 0x806d0000, // The requested operation has too many matches to return.
	UA_STATUSCODE_BADQUERYTOOCOMPLEX = 0x806e0000, // The requested operation requires too many resources in the server.
	UA_STATUSCODE_BADNOMATCH = 0x806f0000, // The requested operation has no match to return.
	UA_STATUSCODE_BADMAXAGEINVALID = 0x80700000, // The max age parameter is invalid.
	UA_STATUSCODE_BADHISTORYOPERATIONINVALID = 0x80710000, // The history details parameter is not valid.
	UA_STATUSCODE_BADHISTORYOPERATIONUNSUPPORTED = 0x80720000, // The server does not support the requested operation.
	UA_STATUSCODE_BADINVALIDTIMESTAMPARGUMENT = 0x80bd0000, // The defined timestamp to return was invalid.
	UA_STATUSCODE_BADWRITENOTSUPPORTED = 0x80730000, // The server not does support writing the combination of value, status and timestamps provided.
	UA_STATUSCODE_BADTYPEMISMATCH = 0x80740000, // The value supplied for the attribute is not of the same type as the attribute's value.
	UA_STATUSCODE_BADMETHODINVALID = 0x80750000, // The method id does not refer to a method for the specified object.
	UA_STATUSCODE_BADARGUMENTSMISSING = 0x80760000, // The client did not specify all of the input arguments for the method.
	UA_STATUSCODE_BADTOOMANYSUBSCRIPTIONS = 0x80770000, // The server has reached its  maximum number of subscriptions.
	UA_STATUSCODE_BADTOOMANYPUBLISHREQUESTS = 0x80780000, // The server has reached the maximum number of queued publish requests.
	UA_STATUSCODE_BADNOSUBSCRIPTION = 0x80790000, // There is no subscription available for this session.
	UA_STATUSCODE_BADSEQUENCENUMBERUNKNOWN = 0x807a0000, // The sequence number is unknown to the server.
	UA_STATUSCODE_BADMESSAGENOTAVAILABLE = 0x807b0000, // The requested notification message is no longer available.
	UA_STATUSCODE_BADINSUFFICIENTCLIENTPROFILE = 0x807c0000, // The Client of the current Session does not support one or more Profiles that are necessary for the Subscription.
	UA_STATUSCODE_BADSTATENOTACTIVE = 0x80bf0000, // The sub-state machine is not currently active.
	UA_STATUSCODE_BADTCPSERVERTOOBUSY = 0x807d0000, // The server cannot process the request because it is too busy.
	UA_STATUSCODE_BADTCPMESSAGETYPEINVALID = 0x807e0000, // The type of the message specified in the header invalid.
	UA_STATUSCODE_BADTCPSECURECHANNELUNKNOWN = 0x807f0000, // The SecureChannelId and/or TokenId are not currently in use.
	UA_STATUSCODE_BADTCPMESSAGETOOLARGE = 0x80800000, // The size of the message specified in the header is too large.
	UA_STATUSCODE_BADTCPNOTENOUGHRESOURCES = 0x80810000, // There are not enough resources to process the request.
	UA_STATUSCODE_BADTCPINTERNALERROR = 0x80820000, // An internal error occurred.
	UA_STATUSCODE_BADTCPENDPOINTURLINVALID = 0x80830000, // The Server does not recognize the QueryString specified.
	UA_STATUSCODE_BADREQUESTINTERRUPTED = 0x80840000, // The request could not be sent because of a network interruption.
	UA_STATUSCODE_BADREQUESTTIMEOUT = 0x80850000, // Timeout occurred while processing the request.
	UA_STATUSCODE_BADSECURECHANNELCLOSED = 0x80860000, // The secure channel has been closed.
	UA_STATUSCODE_BADSECURECHANNELTOKENUNKNOWN = 0x80870000, // The token has expired or is not recognized.
	UA_STATUSCODE_BADSEQUENCENUMBERINVALID = 0x80880000, // The sequence number is not valid.
	UA_STATUSCODE_BADPROTOCOLVERSIONUNSUPPORTED = 0x80be0000, // The applications do not have compatible protocol versions.
	UA_STATUSCODE_BADCONFIGURATIONERROR = 0x80890000, // There is a problem with the configuration that affects the usefulness of the value.
	UA_STATUSCODE_BADNOTCONNECTED = 0x808a0000, // The variable should receive its value from another variable, but has never been configured to do so.
	UA_STATUSCODE_BADDEVICEFAILURE = 0x808b0000, // There has been a failure in the device/data source that generates the value that has affected the value.
	UA_STATUSCODE_BADSENSORFAILURE = 0x808c0000, // There has been a failure in the sensor from which the value is derived by the device/data source.
	UA_STATUSCODE_BADOUTOFSERVICE = 0x808d0000, // The source of the data is not operational.
	UA_STATUSCODE_BADDEADBANDFILTERINVALID = 0x808e0000, // The deadband filter is not valid.
	UA_STATUSCODE_UNCERTAINNOCOMMUNICATIONLASTUSABLEVALUE = 0x408f0000, // Communication to the data source has failed. The variable value is the last value that had a good quality.
	UA_STATUSCODE_UNCERTAINLASTUSABLEVALUE = 0x40900000, // Whatever was updating this value has stopped doing so.
	UA_STATUSCODE_UNCERTAINSUBSTITUTEVALUE = 0x40910000, // The value is an operational value that was manually overwritten.
	UA_STATUSCODE_UNCERTAININITIALVALUE = 0x40920000, // The value is an initial value for a variable that normally receives its value from another variable.
	UA_STATUSCODE_UNCERTAINSENSORNOTACCURATE = 0x40930000, // The value is at one of the sensor limits.
	UA_STATUSCODE_UNCERTAINENGINEERINGUNITSEXCEEDED = 0x40940000, // The value is outside of the range of values defined for this parameter.
	UA_STATUSCODE_UNCERTAINSUBNORMAL = 0x40950000, // The value is derived from multiple sources and has less than the required number of Good sources.
	UA_STATUSCODE_GOODLOCALOVERRIDE = 0x00960000, // The value has been overridden.
	UA_STATUSCODE_BADREFRESHINPROGRESS = 0x80970000, // This Condition refresh failed, a Condition refresh operation is already in progress.
	UA_STATUSCODE_BADCONDITIONALREADYDISABLED = 0x80980000, // This condition has already been disabled.
	UA_STATUSCODE_BADCONDITIONALREADYENABLED = 0x80cc0000, // This condition has already been enabled.
	UA_STATUSCODE_BADCONDITIONDISABLED = 0x80990000, // Property not available, this condition is disabled.
	UA_STATUSCODE_BADEVENTIDUNKNOWN = 0x809a0000, // The specified event id is not recognized.
	UA_STATUSCODE_BADEVENTNOTACKNOWLEDGEABLE = 0x80bb0000, // The event cannot be acknowledged.
	UA_STATUSCODE_BADDIALOGNOTACTIVE = 0x80cd0000, // The dialog condition is not active.
	UA_STATUSCODE_BADDIALOGRESPONSEINVALID = 0x80ce0000, // The response is not valid for the dialog.
	UA_STATUSCODE_BADCONDITIONBRANCHALREADYACKED = 0x80cf0000, // The condition branch has already been acknowledged.
	UA_STATUSCODE_BADCONDITIONBRANCHALREADYCONFIRMED = 0x80d00000, // The condition branch has already been confirmed.
	UA_STATUSCODE_BADCONDITIONALREADYSHELVED = 0x80d10000, // The condition has already been shelved.
	UA_STATUSCODE_BADCONDITIONNOTSHELVED = 0x80d20000, // The condition is not currently shelved.
	UA_STATUSCODE_BADSHELVINGTIMEOUTOFRANGE = 0x80d30000, // The shelving time not within an acceptable range.
	UA_STATUSCODE_BADNODATA = 0x809b0000, // No data exists for the requested time range or event filter.
	UA_STATUSCODE_BADBOUNDNOTFOUND = 0x80d70000, // No data found to provide upper or lower bound value.
	UA_STATUSCODE_BADBOUNDNOTSUPPORTED = 0x80d80000, // The server cannot retrieve a bound for the variable.
	UA_STATUSCODE_BADDATALOST = 0x809d0000, // Data is missing due to collection started/stopped/lost.
	UA_STATUSCODE_BADDATAUNAVAILABLE = 0x809e0000, // Expected data is unavailable for the requested time range due to an un-mounted volume, an off-line archive or tape, or similar reason for temporary unavailability.
	UA_STATUSCODE_BADENTRYEXISTS = 0x809f0000, // The data or event was not successfully inserted because a matching entry exists.
	UA_STATUSCODE_BADNOENTRYEXISTS = 0x80a00000, // The data or event was not successfully updated because no matching entry exists.
	UA_STATUSCODE_BADTIMESTAMPNOTSUPPORTED = 0x80a10000, // The client requested history using a timestamp format the server does not support (i.e requested ServerTimestamp when server only supports SourceTimestamp).
	UA_STATUSCODE_GOODENTRYINSERTED = 0x00a20000, // The data or event was successfully inserted into the historical database.
	UA_STATUSCODE_GOODENTRYREPLACED = 0x00a30000, // The data or event field was successfully replaced in the historical database.
	UA_STATUSCODE_UNCERTAINDATASUBNORMAL = 0x40a40000, // The value is derived from multiple values and has less than the required number of Good values.
	UA_STATUSCODE_GOODNODATA = 0x00a50000, // No data exists for the requested time range or event filter.
	UA_STATUSCODE_GOODMOREDATA = 0x00a60000, // The data or event field was successfully replaced in the historical database.
	UA_STATUSCODE_BADAGGREGATELISTMISMATCH = 0x80d40000, // The requested number of Aggregates does not match the requested number of NodeIds.
	UA_STATUSCODE_BADAGGREGATENOTSUPPORTED = 0x80d50000, // The requested Aggregate is not support by the server.
	UA_STATUSCODE_BADAGGREGATEINVALIDINPUTS = 0x80d60000, // The aggregate value could not be derived due to invalid data inputs.
	UA_STATUSCODE_BADAGGREGATECONFIGURATIONREJECTED = 0x80da0000, // The aggregate configuration is not valid for specified node.
	UA_STATUSCODE_GOODDATAIGNORED = 0x00d90000, // The request specifies fields which are not valid for the EventType or cannot be saved by the historian.
	UA_STATUSCODE_GOODCOMMUNICATIONEVENT = 0x00a70000, // The communication layer has raised an event.
	UA_STATUSCODE_GOODSHUTDOWNEVENT = 0x00a80000, // The system is shutting down.
	UA_STATUSCODE_GOODCALLAGAIN = 0x00a90000, // The operation is not finished and needs to be called again.
	UA_STATUSCODE_GOODNONCRITICALTIMEOUT = 0x00aa0000, // A non-critical timeout occurred.
	UA_STATUSCODE_BADINVALIDARGUMENT = 0x80ab0000, // One or more arguments are invalid.
	UA_STATUSCODE_BADCONNECTIONREJECTED = 0x80ac0000, // Could not establish a network connection to remote server.
	UA_STATUSCODE_BADDISCONNECT = 0x80ad0000, // The server has disconnected from the client.
	UA_STATUSCODE_BADCONNECTIONCLOSED = 0x80ae0000, // The network connection has been closed.
	UA_STATUSCODE_BADINVALIDSTATE = 0x80af0000, // The operation cannot be completed because the object is closed, uninitialized or in some other invalid state.
	UA_STATUSCODE_BADENDOFSTREAM = 0x80b00000, // Cannot move beyond end of the stream.
	UA_STATUSCODE_BADNODATAAVAILABLE = 0x80b10000, // No data is currently available for reading from a non-blocking stream.
	UA_STATUSCODE_BADWAITINGFORRESPONSE = 0x80b20000, // The asynchronous operation is waiting for a response.
	UA_STATUSCODE_BADOPERATIONABANDONED = 0x80b30000, // The asynchronous operation was abandoned by the caller.
	UA_STATUSCODE_BADEXPECTEDSTREAMTOBLOCK = 0x80b40000, // The stream did not return all data requested (possibly because it is a non-blocking stream).
	UA_STATUSCODE_BADWOULDBLOCK = 0x80b50000, // Non blocking behaviour is required and the operation would block.
	UA_STATUSCODE_BADSYNTAXERROR = 0x80b60000, // A value had an invalid syntax.
	UA_STATUSCODE_BADMAXCONNECTIONSREACHED = 0x80b70000 // The operation could not be finished because all available connections are in use.
};


/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/include/ua_types.h" ***********************************/

/*
 * Copyright (C) 2014 the contributors as stated in the AUTHORS file
 *
 * This file is part of open62541. open62541 is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser General
 * Public License, version 3 (as published by the Free Software Foundation) with
 * a static linking exception as stated in the LICENSE file provided with
 * open62541.
 *
 * open62541 is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 */


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/** @brief A two-state logical value (true or false). */
typedef bool UA_Boolean;
#define UA_TRUE true
#define UA_FALSE false

/** @brief An integer value between -129 and 127. */
typedef int8_t UA_SByte;
#define UA_SBYTE_MAX 127
#define UA_SBYTE_MIN -128

/** @brief An integer value between 0 and 256. */
typedef uint8_t UA_Byte;
#define UA_BYTE_MAX 256
#define UA_BYTE_MIN 0

/** @brief An integer value between -32 768 and 32 767. */
typedef int16_t UA_Int16;
#define UA_INT16_MAX 32767
#define UA_INT16_MIN -32768

/** @brief An integer value between 0 and 65 535. */
typedef uint16_t UA_UInt16;
#define UA_UINT16_MAX 65535
#define UA_UINT16_MIN 0

/** @brief An integer value between -2 147 483 648 and 2 147 483 647. */
typedef int32_t UA_Int32;
#define UA_INT32_MAX 2147483647
#define UA_INT32_MIN -2147483648

/** @brief An integer value between 0 and 429 4967 295. */
typedef uint32_t UA_UInt32;
#define UA_UINT32_MAX 4294967295
#define UA_UINT32_MIN 0

/** @brief An integer value between -10 223 372 036 854 775 808 and 9 223 372 036 854 775 807 */
typedef int64_t UA_Int64;
#define UA_INT64_MAX (int64_t)9223372036854775807
#define UA_INT64_MIN (int64_t)-9223372036854775808

/** @brief An integer value between 0 and 18 446 744 073 709 551 615. */
typedef uint64_t UA_UInt64;
#define UA_UINT64_MAX (int64_t)18446744073709551615
#define UA_UINT64_MIN (int64_t)0

/** @brief An IEEE single precision (32 bit) floating point value. */
typedef float UA_Float;

/** @brief An IEEE double precision (64 bit) floating point value. */
typedef double UA_Double;

/** @brief A sequence of Unicode characters. */
typedef struct {
    UA_Int32 length; ///< The length of the string
    UA_Byte *data; ///< The string's content (not null-terminated)
} UA_String;

/** @brief An instance in time.
 * A DateTime value is encoded as a 64-bit signed integer which represents the
 * number of 100 nanosecond intervals since January 1, 1601 (UTC).
 */
typedef UA_Int64 UA_DateTime; // 100 nanosecond resolution

/** @brief A 16 byte value that can be used as a globally unique identifier. */
typedef struct {
    UA_UInt32 data1;
    UA_UInt16 data2;
    UA_UInt16 data3;
    UA_Byte   data4[8];
} UA_Guid;

/** @brief A sequence of octets. */
typedef UA_String UA_ByteString;

/** @brief An XML element. */
typedef UA_String UA_XmlElement;

/** @brief An identifier for a node in the address space of an OPC UA Server. */
/* The shortened numeric types are introduced during encoding. */
enum UA_NodeIdType {
    UA_NODEIDTYPE_NUMERIC    = 2,
    UA_NODEIDTYPE_STRING     = 3,
    UA_NODEIDTYPE_GUID       = 4,
    UA_NODEIDTYPE_BYTESTRING = 5
};

typedef struct {
    UA_UInt16 namespaceIndex; ///< The namespace index of the node
    enum UA_NodeIdType identifierType; ///< The type of the node identifier
    union {
        UA_UInt32     numeric;
        UA_String     string;
        UA_Guid       guid;
        UA_ByteString byteString;
    } identifier; ///< The node identifier
} UA_NodeId;

/** @brief A NodeId that allows the namespace URI to be specified instead of an index. */
typedef struct {
    UA_NodeId nodeId; ///< The nodeid without extensions
    UA_String namespaceUri; ///< The Uri of the namespace (tindex in the nodeId is ignored)
    UA_UInt32 serverIndex;  ///< The index of the server
} UA_ExpandedNodeId;

/** @brief A numeric identifier for a error or condition that is associated with a value or an operation. */
typedef enum UA_StatusCode UA_StatusCode; // StatusCodes aren't an enum(=int) since 32 unsigned bits are needed. See also ua_statuscodes.h */

/** @brief A name qualified by a namespace. */
typedef struct {
    UA_UInt16 namespaceIndex; ///< The namespace index
    UA_String name; ///< The actual name
} UA_QualifiedName;

/** @brief Human readable text with an optional locale identifier. */
typedef struct {
    UA_String locale; ///< The locale (e.g. "en-US")
    UA_String text; ///< The actual text
} UA_LocalizedText;

/** @brief A structure that contains an application specific data type that may
    not be recognized by the receiver. */
typedef struct {
    UA_NodeId typeId; ///< The nodeid of the datatype
    enum {
        UA_EXTENSIONOBJECT_ENCODINGMASK_NOBODYISENCODED  = 0,
        UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISBYTESTRING = 1,
        UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISXML        = 2
    } encoding; ///< The encoding of the contained data
    UA_ByteString body; ///< The bytestring of the encoded data
} UA_ExtensionObject;

struct UA_DataType;
typedef struct UA_DataType UA_DataType; 

/** @brief Variants store (arrays of) any data type. Either they provide a pointer to the data in
 *   memory, or functions from which the data can be accessed. Variants are replaced together with
 *   the data they store (exception: use a data source).
 *
 * Variant semantics:
 *  - arrayLength = -1 && data == NULL: empty variant
 *  - arrayLength = -1 && data == !NULL: variant holds a single element (a scalar)
 *  - arrayLength >= 0: variant holds an array of the appropriate length
 *                      data can be NULL if arrayLength == 0
 */

typedef struct {
    const UA_DataType *type; ///< The nodeid of the datatype
    enum {
        UA_VARIANT_DATA, ///< The data is "owned" by this variant (copied and deleted together)
        UA_VARIANT_DATA_NODELETE, /**< The data is "borrowed" by the variant and shall not be
                                       deleted at the end of this variant's lifecycle. It is not
                                       possible to overwrite borrowed data due to concurrent access.
                                       Use a custom datasource with a mutex. */
    } storageType; ///< Shall the data be deleted together with the variant
    UA_Int32  arrayLength;  ///< the number of elements in the data-pointer
    void     *data; ///< points to the scalar or array data
    UA_Int32  arrayDimensionsSize; ///< the number of dimensions the data-array has
    UA_Int32 *arrayDimensions; ///< the length of each dimension of the data-array
} UA_Variant;

/** @brief A data value with an associated status code and timestamps. */
typedef struct {
    UA_Boolean    hasValue : 1;
    UA_Boolean    hasStatus : 1;
    UA_Boolean    hasSourceTimestamp : 1;
    UA_Boolean    hasServerTimestamp : 1;
    UA_Boolean    hasSourcePicoseconds : 1;
    UA_Boolean    hasServerPicoseconds : 1;
    UA_Variant    value;
    UA_StatusCode status;
    UA_DateTime   sourceTimestamp;
    UA_Int16      sourcePicoseconds;
    UA_DateTime   serverTimestamp;
    UA_Int16      serverPicoseconds;
} UA_DataValue;

/** @brief A structure that contains detailed error and diagnostic information associated with a StatusCode. */
typedef struct UA_DiagnosticInfo {
    UA_Boolean    hasSymbolicId : 1;
    UA_Boolean    hasNamespaceUri : 1;
    UA_Boolean    hasLocalizedText : 1;
    UA_Boolean    hasLocale : 1;
    UA_Boolean    hasAdditionalInfo : 1;
    UA_Boolean    hasInnerStatusCode : 1;
    UA_Boolean    hasInnerDiagnosticInfo : 1;
    UA_Int32      symbolicId;
    UA_Int32      namespaceUri;
    UA_Int32      localizedText;
    UA_Int32      locale;
    UA_String     additionalInfo;
    UA_StatusCode innerStatusCode;
    struct UA_DiagnosticInfo *innerDiagnosticInfo;
} UA_DiagnosticInfo;

#define UA_TYPE_HANDLING_FUNCTIONS(TYPE)                             \
    TYPE UA_EXPORT * TYPE##_new(void);                               \
    void UA_EXPORT TYPE##_init(TYPE * p);                            \
    void UA_EXPORT TYPE##_delete(TYPE * p);                          \
    void UA_EXPORT TYPE##_deleteMembers(TYPE * p);                   \
    UA_StatusCode UA_EXPORT TYPE##_copy(const TYPE *src, TYPE *dst);

/* Functions for all types */
UA_TYPE_HANDLING_FUNCTIONS(UA_Boolean)
UA_TYPE_HANDLING_FUNCTIONS(UA_SByte)
UA_TYPE_HANDLING_FUNCTIONS(UA_Byte)
UA_TYPE_HANDLING_FUNCTIONS(UA_Int16)
UA_TYPE_HANDLING_FUNCTIONS(UA_UInt16)
UA_TYPE_HANDLING_FUNCTIONS(UA_Int32)
UA_TYPE_HANDLING_FUNCTIONS(UA_UInt32)
UA_TYPE_HANDLING_FUNCTIONS(UA_Int64)
UA_TYPE_HANDLING_FUNCTIONS(UA_UInt64)
UA_TYPE_HANDLING_FUNCTIONS(UA_Float)
UA_TYPE_HANDLING_FUNCTIONS(UA_Double)
UA_TYPE_HANDLING_FUNCTIONS(UA_String)
#define UA_DateTime_new UA_Int64_new
#define UA_DateTime_init UA_Int64_init
#define UA_DateTime_delete UA_Int64_delete
#define UA_DateTime_deleteMembers UA_Int64_deleteMembers
#define UA_DateTime_copy UA_Int64_copy
UA_TYPE_HANDLING_FUNCTIONS(UA_Guid)
#define UA_ByteString_new UA_String_new
#define UA_ByteString_init UA_String_init
#define UA_ByteString_delete UA_String_delete
#define UA_ByteString_deleteMembers UA_String_deleteMembers
#define UA_ByteString_copy UA_String_copy
#define UA_XmlElement_new UA_String_new
#define UA_XmlElement_init UA_String_init
#define UA_XmlElement_delete UA_String_delete
#define UA_XmlElement_deleteMembers UA_String_deleteMembers
#define UA_XmlElement_copy UA_String_copy
UA_TYPE_HANDLING_FUNCTIONS(UA_NodeId)
UA_TYPE_HANDLING_FUNCTIONS(UA_ExpandedNodeId)
#define UA_StatusCode_new() UA_Int32_new()
#define UA_StatusCode_init(p) UA_Int32_init((UA_Int32*)p)
#define UA_StatusCode_delete(p) UA_Int32_delete((UA_Int32*)p)
#define UA_StatusCode_deleteMembers(p) UA_Int32_deleteMembers((UA_Int32*)p)
#define UA_StatusCode_copy(p, q) UA_Int32_copy((UA_Int32*)p, (UA_Int32*)q)
UA_TYPE_HANDLING_FUNCTIONS(UA_QualifiedName)
UA_TYPE_HANDLING_FUNCTIONS(UA_LocalizedText)
UA_TYPE_HANDLING_FUNCTIONS(UA_ExtensionObject)
UA_TYPE_HANDLING_FUNCTIONS(UA_DataValue)
UA_TYPE_HANDLING_FUNCTIONS(UA_Variant)
UA_TYPE_HANDLING_FUNCTIONS(UA_DiagnosticInfo)

/**********************************************/
/* Custom functions for the builtin datatypes */
/**********************************************/

/* String */
/** Copy a (zero terminated) char-array into a UA_String. Memory for the string data is
    allocated. If the memory cannot be allocated, a null-string is returned. */
UA_String UA_EXPORT UA_String_fromChars(char const src[]);

#define UA_STRING_NULL (UA_String) {-1, (UA_Byte*)0 }
#define UA_STRING(CHARS) (UA_String) {strlen(CHARS), (UA_Byte*)CHARS }
#define UA_STRING_ALLOC(CHARS) UA_String_fromChars(CHARS)

/** Compares two strings */
UA_Boolean UA_EXPORT UA_String_equal(const UA_String *string1, const UA_String *string2);

/** Printf a char-array into a UA_String. Memory for the string data is allocated. */
UA_StatusCode UA_EXPORT UA_String_copyprintf(char const fmt[], UA_String *dst, ...);

/* DateTime */
/** Returns the current time */
UA_DateTime UA_EXPORT UA_DateTime_now(void);

typedef struct UA_DateTimeStruct {
    UA_Int16 nanoSec;
    UA_Int16 microSec;
    UA_Int16 milliSec;
    UA_Int16 sec;
    UA_Int16 min;
    UA_Int16 hour;
    UA_Int16 day;
    UA_Int16 month;
    UA_Int16 year;
} UA_DateTimeStruct;

UA_DateTimeStruct UA_EXPORT UA_DateTime_toStruct(UA_DateTime time);
UA_StatusCode UA_EXPORT UA_DateTime_toString(UA_DateTime time, UA_String *timeString);

/* Guid */
/** Compares two guids */
UA_Boolean UA_EXPORT UA_Guid_equal(const UA_Guid *g1, const UA_Guid *g2);
    
/** Returns a randomly generated guid. Do not use for security-critical entropy! */
UA_Guid UA_EXPORT UA_Guid_random(UA_UInt32 *seed);

/* ByteString */
#define UA_BYTESTRING_NULL (UA_ByteString) {-1, (UA_Byte*)0 }
#define UA_ByteString_equal(string1, string2) \
    UA_String_equal((const UA_String*) string1, (const UA_String*)string2)

/** Allocates memory of size length for the bytestring. The content is not set to zero. */
UA_StatusCode UA_EXPORT UA_ByteString_newMembers(UA_ByteString *p, UA_Int32 length);

/* NodeId */
/** Compares two nodeids */
UA_Boolean UA_EXPORT UA_NodeId_equal(const UA_NodeId *n1, const UA_NodeId *n2);

/** Is the nodeid a null-nodeid? */
UA_Boolean UA_EXPORT UA_NodeId_isNull(const UA_NodeId *p);

UA_NodeId UA_EXPORT UA_NodeId_fromInteger(UA_UInt16 nsIndex, UA_Int32 identifier);
UA_NodeId UA_EXPORT UA_NodeId_fromCharString(UA_UInt16 nsIndex, char identifier[]);
UA_NodeId UA_EXPORT UA_NodeId_fromCharStringCopy(UA_UInt16 nsIndex, char const identifier[]);
UA_NodeId UA_EXPORT UA_NodeId_fromString(UA_UInt16 nsIndex, UA_String identifier);
UA_NodeId UA_EXPORT UA_NodeId_fromStringCopy(UA_UInt16 nsIndex, UA_String identifier);
UA_NodeId UA_EXPORT UA_NodeId_fromGuid(UA_UInt16 nsIndex, UA_Guid identifier);
UA_NodeId UA_EXPORT UA_NodeId_fromCharByteString(UA_UInt16 nsIndex, char identifier[]);
UA_NodeId UA_EXPORT UA_NodeId_fromCharByteStringCopy(UA_UInt16 nsIndex, char const identifier[]);
UA_NodeId UA_EXPORT UA_NodeId_fromByteString(UA_UInt16 nsIndex, UA_ByteString identifier);
UA_NodeId UA_EXPORT UA_NodeId_fromByteStringCopy(UA_UInt16 nsIndex, UA_ByteString identifier);

#define UA_NODEID_NUMERIC(NSID, NUMERICID) UA_NodeId_fromInteger(NSID, NUMERICID)
#define UA_NODEID_STRING(NSID, CHARS) UA_NodeId_fromCharString(NSID, CHARS)
#define UA_NODEID_STRING_ALLOC(NSID, CHARS) UA_NodeId_fromCharStringCopy(NSID, CHARS)
#define UA_NODEID_GUID(NSID, GUID) UA_NodeId_fromGuid(NSID, GUID)
#define UA_NODEID_BYTESTRING(NSID, CHARS) UA_NodeId_fromCharByteString(NSID, CHARS)
#define UA_NODEID_BYTESTRING_ALLOC(NSID, CHARS) UA_NodeId_fromCharByteStringCopy(NSID, CHARS)
#define UA_NODEID_NULL UA_NODEID_NUMERIC(0,0)

/* ExpandedNodeId */
UA_Boolean UA_EXPORT UA_ExpandedNodeId_isNull(const UA_ExpandedNodeId *p);

#define UA_EXPANDEDNODEID_NUMERIC(NSID, NUMERICID) (UA_ExpandedNodeId) {            \
        .nodeId = {.namespaceIndex = NSID, .identifierType = UA_NODEIDTYPE_NUMERIC, \
                   .identifier.numeric = NUMERICID },                                   \
        .serverIndex = 0, .namespaceUri = {.data = (UA_Byte*)0, .length = -1} }
    
/* QualifiedName */
#define UA_QUALIFIEDNAME(NSID, CHARS) (const UA_QualifiedName) {    \
        .namespaceIndex = NSID, .name = UA_STRING(CHARS) }
#define UA_QUALIFIEDNAME_ALLOC(NSID, CHARS) (UA_QualifiedName) {    \
        .namespaceIndex = NSID, .name = UA_STRING_ALLOC(CHARS) }

/* LocalizedText */
#define UA_LOCALIZEDTEXT(LOCALE, TEXT) (const UA_LocalizedText) {       \
        .locale = UA_STRING(LOCALE), .text = UA_STRING(TEXT) }
#define UA_LOCALIZEDTEXT_ALLOC(LOCALE, TEXT) (UA_LocalizedText) {       \
        .locale = UA_STRING_ALLOC(LOCALE), .text = UA_STRING_ALLOC(TEXT) }

/* Variant */

/**
 * Returns true if the variant contains a scalar value. Note that empty variants contain an array of
 * length -1 (undefined).
 *
 * @param v The variant
 * @return Does the variant contain a scalar value.
 */
UA_Boolean UA_EXPORT UA_Variant_isScalar(const UA_Variant *v);
    
/**
 * Set the variant to a scalar value that already resides in memory. The value takes on the
 * lifecycle of the variant and is deleted with it.
 *
 * @param v The variant
 * @param p A pointer to the value data
 * @param type The datatype of the value in question
 * @return Indicates whether the operation succeeded or returns an error code
 */
UA_StatusCode UA_EXPORT UA_Variant_setScalar(UA_Variant *v, void *p, const UA_DataType *type);

/**
 * Set the variant to a scalar value that is copied from an existing variable.
 *
 * @param v The variant
 * @param p A pointer to the value data
 * @param type The datatype of the value
 * @return Indicates whether the operation succeeded or returns an error code
 */
UA_StatusCode UA_EXPORT UA_Variant_setScalarCopy(UA_Variant *v, const void *p, const UA_DataType *type);

/**
 * Set the variant to an array that already resides in memory. The array takes on the lifecycle of
 * the variant and is deleted with it.
 *
 * @param v The variant
 * @param array A pointer to the array data
 * @param elements The size of the array
 * @param type The datatype of the array
 * @return Indicates whether the operation succeeded or returns an error code
 */
UA_StatusCode UA_EXPORT UA_Variant_setArray(UA_Variant *v, void *array, UA_Int32 elements,
                                            const UA_DataType *type);

/**
 * Set the variant to an array that is copied from an existing array.
 *
 * @param v The variant
 * @param array A pointer to the array data
 * @param elements The size of the array
 * @param type The datatype of the array
 * @return Indicates whether the operation succeeded or returns an error code
 */
UA_StatusCode UA_EXPORT UA_Variant_setArrayCopy(UA_Variant *v, const void *array, UA_Int32 elements,
                                                const UA_DataType *type);

/** @brief NumericRanges are used select a subset in a (multidimensional)
    variant array. NumericRange has no official type structure in the standard.
    On the wire, it only exists as an encoded string, such as "1:2,0:3,5". The
    colon separates min/max index and the comma separates dimensions. A single
    value indicates a range with a single element (min==max). */
typedef struct {
    UA_Int32 dimensionsSize;
    struct UA_NumericRangeDimension {
        UA_UInt32 min;
        UA_UInt32 max;
    } *dimensions;
} UA_NumericRange;

/**
 * Copy the variant, but use only a subset of the (multidimensional) array into a variant. Returns
 * an error code if the variant is not an array or if the indicated range does not fit.
 *
 * @param src The source variant
 * @param dst The target variant
 * @param range The range of the copied data
 * @return Returns UA_STATUSCODE_GOOD or an error code
 */
UA_StatusCode UA_EXPORT UA_Variant_copyRange(const UA_Variant *src, UA_Variant *dst, UA_NumericRange range);

/**
 * Insert a range of data into an existing variant. The data array can't be reused afterwards if it
 * contains types without a fixed size (e.g. strings) since they take on the lifetime of the
 * variant.
 *
 * @param v The variant
 * @param dataArray The data array. The type must match the variant
 * @param dataArraySize The length of the data array. This is checked to match the range size.
 * @param range The range of where the new data is inserted
 * @return Returns UA_STATUSCODE_GOOD or an error code
 */
UA_StatusCode UA_EXPORT UA_Variant_setRange(UA_Variant *v, void *dataArray, UA_Int32 dataArraySize,
                                            const UA_NumericRange range);

/**
 * Deep-copy a range of data into an existing variant.
 *
 * @param v The variant
 * @param dataArray The data array. The type must match the variant
 * @param dataArraySize The length of the data array. This is checked to match the range size.
 * @param range The range of where the new data is inserted
 * @return Returns UA_STATUSCODE_GOOD or an error code
 */
UA_StatusCode UA_EXPORT UA_Variant_setRangeCopy(UA_Variant *v, const void *dataArray, UA_Int32 dataArraySize,
                                                const UA_NumericRange range);

/****************************/
/* Structured Type Handling */
/****************************/

#define UA_MAX_TYPE_MEMBERS 13 // Maximum number of members per complex type

#ifndef _WIN32
# define UA_BITFIELD(SIZE) : SIZE
#else
# define UA_BITFIELD(SIZE)
#endif

#define UA_IS_BUILTIN(ID) (ID <= UA_TYPES_DIAGNOSTICINFO)

typedef struct {
    UA_UInt16 memberTypeIndex UA_BITFIELD(9); ///< Index of the member in the datatypetable
    UA_Boolean namespaceZero UA_BITFIELD(1); /**< The type of the member is defined in namespace
                                                  zero. In this implementation, types from custom
                                                  namespace may contain members from the same
                                                  namespace or ns0 only.*/
    UA_Byte padding UA_BITFIELD(5); /**< How much padding is there before this member element? For
                                         arrays this is split into 2 bytes padding before the
                                         length index (max 4 bytes) and 3 bytes padding before the
                                         pointer (max 8 bytes) */
    UA_Boolean isArray UA_BITFIELD(1); ///< The member is an array of the given type
} UA_DataTypeMember;
    
struct UA_DataType {
    UA_NodeId typeId; ///< The nodeid of the type
    ptrdiff_t memSize UA_BITFIELD(16); ///< Size of the struct in memory
    UA_UInt16 typeIndex UA_BITFIELD(13); ///< Index of the type in the datatypetable
    UA_Boolean namespaceZero UA_BITFIELD(1); ///< The type is defined in namespace zero
    UA_Boolean fixedSize UA_BITFIELD(1); ///< The type (and its members) contains no pointers
    UA_Boolean zeroCopyable UA_BITFIELD(1); ///< Can the type be copied directly off the stream?
    UA_Byte membersSize; ///< How many members does the type have?
    UA_DataTypeMember members[UA_MAX_TYPE_MEMBERS];
};

/**
 * Allocates and initializes a variable of type dataType
 *
 * @param dataType The datatype description
 * @return Returns the memory location of the variable or (void*)0 if no memory is available
 */
void UA_EXPORT * UA_new(const UA_DataType *dataType);

/**
 * Initializes a variable to default values
 *
 * @param p The memory location of the variable
 * @param dataType The datatype description
 */
void UA_EXPORT UA_init(void *p, const UA_DataType *dataType);

/**
 * Copies the content of two variables. If copying fails (e.g. because no memory was available for
 * an array), then dst is emptied and initialized to prevent memory leaks.
 *
 * @param src The memory location of the source variable
 * @param dst The memory location of the destination variable
 * @param dataType The datatype description
 * @return Indicates whether the operation succeeded or returns an error code
 */
UA_StatusCode UA_EXPORT UA_copy(const void *src, void *dst, const UA_DataType *dataType);

/**
 * Deletes the dynamically assigned content of a variable (e.g. a member-array). Afterwards, the
 * variable can be safely deleted without causing memory leaks. But the variable is not initialized
 * and may contain old data that is not memory-relevant.
 *
 * @param p The memory location of the variable
 * @param dataType The datatype description of the variable
 */
void UA_EXPORT UA_deleteMembers(void *p, const UA_DataType *dataType);

void UA_EXPORT UA_deleteMembersUntil(void *p, const UA_DataType *dataType, UA_Int32 lastMember);

/**
 * Deletes (frees) a variable and all of its content.
 *
 * @param p The memory location of the variable
 * @param dataType The datatype description of the variable
 */
void UA_EXPORT UA_delete(void *p, const UA_DataType *dataType);

/********************/
/* Array operations */
/********************/

#define MAX_ARRAY_SIZE 104857600 // arrays must be smaller than 100MB

/**
 * Allocates and initializes an array of variables of a specific type
 *
 * @param dataType The datatype description
 * @param elements The number of elements in the array
 * @return Returns the memory location of the variable or (void*)0 if no memory could be allocated
 */
void UA_EXPORT * UA_Array_new(const UA_DataType *dataType, UA_Int32 elements);

/**
 * Allocates and copies an array. dst is set to (void*)0 if not enough memory is available.
 *
 * @param src The memory location of the source array
 * @param dst The memory location where the pointer to the destination array is written
 * @param dataType The datatype of the array members
 * @param elements The size of the array
 * @return Indicates whether the operation succeeded or returns an error code
 */
UA_StatusCode UA_EXPORT UA_Array_copy(const void *src, void **dst, const UA_DataType *dataType, UA_Int32 elements);

/**
 * Deletes an array.
 *
 * @param p The memory location of the array
 * @param dataType The datatype of the array members
 * @param elements The size of the array
 */
void UA_EXPORT UA_Array_delete(void *p, const UA_DataType *dataType, UA_Int32 elements);

/* These are not generated from XML. Server *and* client need them. */
typedef enum {
    UA_ATTRIBUTEID_NODEID                  = 1,
    UA_ATTRIBUTEID_NODECLASS               = 2,
    UA_ATTRIBUTEID_BROWSENAME              = 3,
    UA_ATTRIBUTEID_DISPLAYNAME             = 4,
    UA_ATTRIBUTEID_DESCRIPTION             = 5,
    UA_ATTRIBUTEID_WRITEMASK               = 6,
    UA_ATTRIBUTEID_USERWRITEMASK           = 7,
    UA_ATTRIBUTEID_ISABSTRACT              = 8,
    UA_ATTRIBUTEID_SYMMETRIC               = 9,
    UA_ATTRIBUTEID_INVERSENAME             = 10,
    UA_ATTRIBUTEID_CONTAINSNOLOOPS         = 11,
    UA_ATTRIBUTEID_EVENTNOTIFIER           = 12,
    UA_ATTRIBUTEID_VALUE                   = 13,
    UA_ATTRIBUTEID_DATATYPE                = 14,
    UA_ATTRIBUTEID_VALUERANK               = 15,
    UA_ATTRIBUTEID_ARRAYDIMENSIONS         = 16,
    UA_ATTRIBUTEID_ACCESSLEVEL             = 17,
    UA_ATTRIBUTEID_USERACCESSLEVEL         = 18,
    UA_ATTRIBUTEID_MINIMUMSAMPLINGINTERVAL = 19,
    UA_ATTRIBUTEID_HISTORIZING             = 20,
    UA_ATTRIBUTEID_EXECUTABLE              = 21,
    UA_ATTRIBUTEID_USEREXECUTABLE          = 22
} UA_AttributeId;

#ifdef __cplusplus
} // extern "C"
#endif


/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src_generated/ua_nodeids.h" ***********************************/

/**********************************************************
 * C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src_generated/ua_nodeids.hgen -- do not modify
 **********************************************************
 * Generated from C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/tools/schema/NodeIds.csv with script C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/tools/generate_nodeids.py
 * on host SPECTRE by user lars at 2015-09-08 01:27:13
 **********************************************************/
 

#define UA_NS0ID_BOOLEAN 1 // DataType
#define UA_NS0ID_SBYTE 2 // DataType
#define UA_NS0ID_BYTE 3 // DataType
#define UA_NS0ID_INT16 4 // DataType
#define UA_NS0ID_UINT16 5 // DataType
#define UA_NS0ID_INT32 6 // DataType
#define UA_NS0ID_UINT32 7 // DataType
#define UA_NS0ID_INT64 8 // DataType
#define UA_NS0ID_UINT64 9 // DataType
#define UA_NS0ID_FLOAT 10 // DataType
#define UA_NS0ID_DOUBLE 11 // DataType
#define UA_NS0ID_STRING 12 // DataType
#define UA_NS0ID_DATETIME 13 // DataType
#define UA_NS0ID_GUID 14 // DataType
#define UA_NS0ID_BYTESTRING 15 // DataType
#define UA_NS0ID_XMLELEMENT 16 // DataType
#define UA_NS0ID_NODEID 17 // DataType
#define UA_NS0ID_EXPANDEDNODEID 18 // DataType
#define UA_NS0ID_STATUSCODE 19 // DataType
#define UA_NS0ID_QUALIFIEDNAME 20 // DataType
#define UA_NS0ID_LOCALIZEDTEXT 21 // DataType
#define UA_NS0ID_STRUCTURE 22 // DataType
#define UA_NS0ID_DATAVALUE 23 // DataType
#define UA_NS0ID_BASEDATATYPE 24 // DataType
#define UA_NS0ID_DIAGNOSTICINFO 25 // DataType
#define UA_NS0ID_NUMBER 26 // DataType
#define UA_NS0ID_INTEGER 27 // DataType
#define UA_NS0ID_UINTEGER 28 // DataType
#define UA_NS0ID_ENUMERATION 29 // DataType
#define UA_NS0ID_IMAGE 30 // DataType
#define UA_NS0ID_REFERENCES 31 // ReferenceType
#define UA_NS0ID_NONHIERARCHICALREFERENCES 32 // ReferenceType
#define UA_NS0ID_HIERARCHICALREFERENCES 33 // ReferenceType
#define UA_NS0ID_HASCHILD 34 // ReferenceType
#define UA_NS0ID_ORGANIZES 35 // ReferenceType
#define UA_NS0ID_HASEVENTSOURCE 36 // ReferenceType
#define UA_NS0ID_HASMODELLINGRULE 37 // ReferenceType
#define UA_NS0ID_HASENCODING 38 // ReferenceType
#define UA_NS0ID_HASDESCRIPTION 39 // ReferenceType
#define UA_NS0ID_HASTYPEDEFINITION 40 // ReferenceType
#define UA_NS0ID_GENERATESEVENT 41 // ReferenceType
#define UA_NS0ID_AGGREGATES 44 // ReferenceType
#define UA_NS0ID_HASSUBTYPE 45 // ReferenceType
#define UA_NS0ID_HASPROPERTY 46 // ReferenceType
#define UA_NS0ID_HASCOMPONENT 47 // ReferenceType
#define UA_NS0ID_HASNOTIFIER 48 // ReferenceType
#define UA_NS0ID_HASORDEREDCOMPONENT 49 // ReferenceType
#define UA_NS0ID_FROMSTATE 51 // ReferenceType
#define UA_NS0ID_TOSTATE 52 // ReferenceType
#define UA_NS0ID_HASCAUSE 53 // ReferenceType
#define UA_NS0ID_HASEFFECT 54 // ReferenceType
#define UA_NS0ID_HASHISTORICALCONFIGURATION 56 // ReferenceType
#define UA_NS0ID_BASEOBJECTTYPE 58 // ObjectType
#define UA_NS0ID_FOLDERTYPE 61 // ObjectType
#define UA_NS0ID_BASEVARIABLETYPE 62 // VariableType
#define UA_NS0ID_BASEDATAVARIABLETYPE 63 // VariableType
#define UA_NS0ID_PROPERTYTYPE 68 // VariableType
#define UA_NS0ID_DATATYPEDESCRIPTIONTYPE 69 // VariableType
#define UA_NS0ID_DATATYPEDICTIONARYTYPE 72 // VariableType
#define UA_NS0ID_DATATYPESYSTEMTYPE 75 // ObjectType
#define UA_NS0ID_DATATYPEENCODINGTYPE 76 // ObjectType
#define UA_NS0ID_MODELLINGRULETYPE 77 // ObjectType
#define UA_NS0ID_MODELLINGRULE_MANDATORY 78 // Object
#define UA_NS0ID_MODELLINGRULE_MANDATORYSHARED 79 // Object
#define UA_NS0ID_MODELLINGRULE_OPTIONAL 80 // Object
#define UA_NS0ID_MODELLINGRULE_EXPOSESITSARRAY 83 // Object
#define UA_NS0ID_ROOTFOLDER 84 // Object
#define UA_NS0ID_OBJECTSFOLDER 85 // Object
#define UA_NS0ID_TYPESFOLDER 86 // Object
#define UA_NS0ID_VIEWSFOLDER 87 // Object
#define UA_NS0ID_OBJECTTYPESFOLDER 88 // Object
#define UA_NS0ID_VARIABLETYPESFOLDER 89 // Object
#define UA_NS0ID_DATATYPESFOLDER 90 // Object
#define UA_NS0ID_REFERENCETYPESFOLDER 91 // Object
#define UA_NS0ID_XMLSCHEMA_TYPESYSTEM 92 // Object
#define UA_NS0ID_OPCBINARYSCHEMA_TYPESYSTEM 93 // Object
#define UA_NS0ID_MODELLINGRULE_MANDATORY_NAMINGRULE 112 // Variable
#define UA_NS0ID_MODELLINGRULE_OPTIONAL_NAMINGRULE 113 // Variable
#define UA_NS0ID_MODELLINGRULE_EXPOSESITSARRAY_NAMINGRULE 114 // Variable
#define UA_NS0ID_MODELLINGRULE_MANDATORYSHARED_NAMINGRULE 116 // Variable
#define UA_NS0ID_HASSUBSTATEMACHINE 117 // ReferenceType
#define UA_NS0ID_NAMINGRULETYPE 120 // DataType
#define UA_NS0ID_IDTYPE 256 // DataType
#define UA_NS0ID_NODECLASS 257 // DataType
#define UA_NS0ID_NODE 258 // DataType
#define UA_NS0ID_OBJECTNODE 261 // DataType
#define UA_NS0ID_OBJECTTYPENODE 264 // DataType
#define UA_NS0ID_VARIABLENODE 267 // DataType
#define UA_NS0ID_VARIABLETYPENODE 270 // DataType
#define UA_NS0ID_REFERENCETYPENODE 273 // DataType
#define UA_NS0ID_METHODNODE 276 // DataType
#define UA_NS0ID_VIEWNODE 279 // DataType
#define UA_NS0ID_DATATYPENODE 282 // DataType
#define UA_NS0ID_REFERENCENODE 285 // DataType
#define UA_NS0ID_INTEGERID 288 // DataType
#define UA_NS0ID_COUNTER 289 // DataType
#define UA_NS0ID_DURATION 290 // DataType
#define UA_NS0ID_NUMERICRANGE 291 // DataType
#define UA_NS0ID_TIME 292 // DataType
#define UA_NS0ID_DATE 293 // DataType
#define UA_NS0ID_UTCTIME 294 // DataType
#define UA_NS0ID_LOCALEID 295 // DataType
#define UA_NS0ID_ARGUMENT 296 // DataType
#define UA_NS0ID_STATUSRESULT 299 // DataType
#define UA_NS0ID_MESSAGESECURITYMODE 302 // DataType
#define UA_NS0ID_USERTOKENTYPE 303 // DataType
#define UA_NS0ID_USERTOKENPOLICY 304 // DataType
#define UA_NS0ID_APPLICATIONTYPE 307 // DataType
#define UA_NS0ID_APPLICATIONDESCRIPTION 308 // DataType
#define UA_NS0ID_APPLICATIONINSTANCECERTIFICATE 311 // DataType
#define UA_NS0ID_ENDPOINTDESCRIPTION 312 // DataType
#define UA_NS0ID_SECURITYTOKENREQUESTTYPE 315 // DataType
#define UA_NS0ID_USERIDENTITYTOKEN 316 // DataType
#define UA_NS0ID_ANONYMOUSIDENTITYTOKEN 319 // DataType
#define UA_NS0ID_USERNAMEIDENTITYTOKEN 322 // DataType
#define UA_NS0ID_X509IDENTITYTOKEN 325 // DataType
#define UA_NS0ID_ENDPOINTCONFIGURATION 331 // DataType
#define UA_NS0ID_COMPLIANCELEVEL 334 // DataType
#define UA_NS0ID_SUPPORTEDPROFILE 335 // DataType
#define UA_NS0ID_BUILDINFO 338 // DataType
#define UA_NS0ID_SOFTWARECERTIFICATE 341 // DataType
#define UA_NS0ID_SIGNEDSOFTWARECERTIFICATE 344 // DataType
#define UA_NS0ID_ATTRIBUTEWRITEMASK 347 // DataType
#define UA_NS0ID_NODEATTRIBUTESMASK 348 // DataType
#define UA_NS0ID_NODEATTRIBUTES 349 // DataType
#define UA_NS0ID_OBJECTATTRIBUTES 352 // DataType
#define UA_NS0ID_VARIABLEATTRIBUTES 355 // DataType
#define UA_NS0ID_METHODATTRIBUTES 358 // DataType
#define UA_NS0ID_OBJECTTYPEATTRIBUTES 361 // DataType
#define UA_NS0ID_VARIABLETYPEATTRIBUTES 364 // DataType
#define UA_NS0ID_REFERENCETYPEATTRIBUTES 367 // DataType
#define UA_NS0ID_DATATYPEATTRIBUTES 370 // DataType
#define UA_NS0ID_VIEWATTRIBUTES 373 // DataType
#define UA_NS0ID_ADDNODESITEM 376 // DataType
#define UA_NS0ID_ADDREFERENCESITEM 379 // DataType
#define UA_NS0ID_DELETENODESITEM 382 // DataType
#define UA_NS0ID_DELETEREFERENCESITEM 385 // DataType
#define UA_NS0ID_SESSIONAUTHENTICATIONTOKEN 388 // DataType
#define UA_NS0ID_REQUESTHEADER 389 // DataType
#define UA_NS0ID_RESPONSEHEADER 392 // DataType
#define UA_NS0ID_SERVICEFAULT 395 // DataType
#define UA_NS0ID_FINDSERVERSREQUEST 420 // DataType
#define UA_NS0ID_FINDSERVERSRESPONSE 423 // DataType
#define UA_NS0ID_GETENDPOINTSREQUEST 426 // DataType
#define UA_NS0ID_GETENDPOINTSRESPONSE 429 // DataType
#define UA_NS0ID_REGISTEREDSERVER 432 // DataType
#define UA_NS0ID_REGISTERSERVERREQUEST 435 // DataType
#define UA_NS0ID_REGISTERSERVERRESPONSE 438 // DataType
#define UA_NS0ID_CHANNELSECURITYTOKEN 441 // DataType
#define UA_NS0ID_OPENSECURECHANNELREQUEST 444 // DataType
#define UA_NS0ID_OPENSECURECHANNELRESPONSE 447 // DataType
#define UA_NS0ID_CLOSESECURECHANNELREQUEST 450 // DataType
#define UA_NS0ID_CLOSESECURECHANNELRESPONSE 453 // DataType
#define UA_NS0ID_SIGNATUREDATA 456 // DataType
#define UA_NS0ID_CREATESESSIONREQUEST 459 // DataType
#define UA_NS0ID_CREATESESSIONRESPONSE 462 // DataType
#define UA_NS0ID_ACTIVATESESSIONREQUEST 465 // DataType
#define UA_NS0ID_ACTIVATESESSIONRESPONSE 468 // DataType
#define UA_NS0ID_CLOSESESSIONREQUEST 471 // DataType
#define UA_NS0ID_CLOSESESSIONRESPONSE 474 // DataType
#define UA_NS0ID_CANCELREQUEST 477 // DataType
#define UA_NS0ID_CANCELRESPONSE 480 // DataType
#define UA_NS0ID_ADDNODESRESULT 483 // DataType
#define UA_NS0ID_ADDNODESREQUEST 486 // DataType
#define UA_NS0ID_ADDNODESRESPONSE 489 // DataType
#define UA_NS0ID_ADDREFERENCESREQUEST 492 // DataType
#define UA_NS0ID_ADDREFERENCESRESPONSE 495 // DataType
#define UA_NS0ID_DELETENODESREQUEST 498 // DataType
#define UA_NS0ID_DELETENODESRESPONSE 501 // DataType
#define UA_NS0ID_DELETEREFERENCESREQUEST 504 // DataType
#define UA_NS0ID_DELETEREFERENCESRESPONSE 507 // DataType
#define UA_NS0ID_BROWSEDIRECTION 510 // DataType
#define UA_NS0ID_VIEWDESCRIPTION 511 // DataType
#define UA_NS0ID_BROWSEDESCRIPTION 514 // DataType
#define UA_NS0ID_BROWSERESULTMASK 517 // DataType
#define UA_NS0ID_REFERENCEDESCRIPTION 518 // DataType
#define UA_NS0ID_CONTINUATIONPOINT 521 // DataType
#define UA_NS0ID_BROWSERESULT 522 // DataType
#define UA_NS0ID_BROWSEREQUEST 525 // DataType
#define UA_NS0ID_BROWSERESPONSE 528 // DataType
#define UA_NS0ID_BROWSENEXTREQUEST 531 // DataType
#define UA_NS0ID_BROWSENEXTRESPONSE 534 // DataType
#define UA_NS0ID_RELATIVEPATHELEMENT 537 // DataType
#define UA_NS0ID_RELATIVEPATH 540 // DataType
#define UA_NS0ID_BROWSEPATH 543 // DataType
#define UA_NS0ID_BROWSEPATHTARGET 546 // DataType
#define UA_NS0ID_BROWSEPATHRESULT 549 // DataType
#define UA_NS0ID_TRANSLATEBROWSEPATHSTONODEIDSREQUEST 552 // DataType
#define UA_NS0ID_TRANSLATEBROWSEPATHSTONODEIDSRESPONSE 555 // DataType
#define UA_NS0ID_REGISTERNODESREQUEST 558 // DataType
#define UA_NS0ID_REGISTERNODESRESPONSE 561 // DataType
#define UA_NS0ID_UNREGISTERNODESREQUEST 564 // DataType
#define UA_NS0ID_UNREGISTERNODESRESPONSE 567 // DataType
#define UA_NS0ID_QUERYDATADESCRIPTION 570 // DataType
#define UA_NS0ID_NODETYPEDESCRIPTION 573 // DataType
#define UA_NS0ID_FILTEROPERATOR 576 // DataType
#define UA_NS0ID_QUERYDATASET 577 // DataType
#define UA_NS0ID_NODEREFERENCE 580 // DataType
#define UA_NS0ID_CONTENTFILTERELEMENT 583 // DataType
#define UA_NS0ID_CONTENTFILTER 586 // DataType
#define UA_NS0ID_FILTEROPERAND 589 // DataType
#define UA_NS0ID_ELEMENTOPERAND 592 // DataType
#define UA_NS0ID_LITERALOPERAND 595 // DataType
#define UA_NS0ID_ATTRIBUTEOPERAND 598 // DataType
#define UA_NS0ID_SIMPLEATTRIBUTEOPERAND 601 // DataType
#define UA_NS0ID_CONTENTFILTERELEMENTRESULT 604 // DataType
#define UA_NS0ID_CONTENTFILTERRESULT 607 // DataType
#define UA_NS0ID_PARSINGRESULT 610 // DataType
#define UA_NS0ID_QUERYFIRSTREQUEST 613 // DataType
#define UA_NS0ID_QUERYFIRSTRESPONSE 616 // DataType
#define UA_NS0ID_QUERYNEXTREQUEST 619 // DataType
#define UA_NS0ID_QUERYNEXTRESPONSE 622 // DataType
#define UA_NS0ID_TIMESTAMPSTORETURN 625 // DataType
#define UA_NS0ID_READVALUEID 626 // DataType
#define UA_NS0ID_READREQUEST 629 // DataType
#define UA_NS0ID_READRESPONSE 632 // DataType
#define UA_NS0ID_HISTORYREADVALUEID 635 // DataType
#define UA_NS0ID_HISTORYREADRESULT 638 // DataType
#define UA_NS0ID_HISTORYREADDETAILS 641 // DataType
#define UA_NS0ID_READEVENTDETAILS 644 // DataType
#define UA_NS0ID_READRAWMODIFIEDDETAILS 647 // DataType
#define UA_NS0ID_READPROCESSEDDETAILS 650 // DataType
#define UA_NS0ID_READATTIMEDETAILS 653 // DataType
#define UA_NS0ID_HISTORYDATA 656 // DataType
#define UA_NS0ID_HISTORYEVENT 659 // DataType
#define UA_NS0ID_HISTORYREADREQUEST 662 // DataType
#define UA_NS0ID_HISTORYREADRESPONSE 665 // DataType
#define UA_NS0ID_WRITEVALUE 668 // DataType
#define UA_NS0ID_WRITEREQUEST 671 // DataType
#define UA_NS0ID_WRITERESPONSE 674 // DataType
#define UA_NS0ID_HISTORYUPDATEDETAILS 677 // DataType
#define UA_NS0ID_UPDATEDATADETAILS 680 // DataType
#define UA_NS0ID_UPDATEEVENTDETAILS 683 // DataType
#define UA_NS0ID_DELETERAWMODIFIEDDETAILS 686 // DataType
#define UA_NS0ID_DELETEATTIMEDETAILS 689 // DataType
#define UA_NS0ID_DELETEEVENTDETAILS 692 // DataType
#define UA_NS0ID_HISTORYUPDATERESULT 695 // DataType
#define UA_NS0ID_HISTORYUPDATEREQUEST 698 // DataType
#define UA_NS0ID_HISTORYUPDATERESPONSE 701 // DataType
#define UA_NS0ID_CALLMETHODREQUEST 704 // DataType
#define UA_NS0ID_CALLMETHODRESULT 707 // DataType
#define UA_NS0ID_CALLREQUEST 710 // DataType
#define UA_NS0ID_CALLRESPONSE 713 // DataType
#define UA_NS0ID_MONITORINGMODE 716 // DataType
#define UA_NS0ID_DATACHANGETRIGGER 717 // DataType
#define UA_NS0ID_DEADBANDTYPE 718 // DataType
#define UA_NS0ID_MONITORINGFILTER 719 // DataType
#define UA_NS0ID_DATACHANGEFILTER 722 // DataType
#define UA_NS0ID_EVENTFILTER 725 // DataType
#define UA_NS0ID_AGGREGATEFILTER 728 // DataType
#define UA_NS0ID_MONITORINGFILTERRESULT 731 // DataType
#define UA_NS0ID_EVENTFILTERRESULT 734 // DataType
#define UA_NS0ID_AGGREGATEFILTERRESULT 737 // DataType
#define UA_NS0ID_MONITORINGPARAMETERS 740 // DataType
#define UA_NS0ID_MONITOREDITEMCREATEREQUEST 743 // DataType
#define UA_NS0ID_MONITOREDITEMCREATERESULT 746 // DataType
#define UA_NS0ID_CREATEMONITOREDITEMSREQUEST 749 // DataType
#define UA_NS0ID_CREATEMONITOREDITEMSRESPONSE 752 // DataType
#define UA_NS0ID_MONITOREDITEMMODIFYREQUEST 755 // DataType
#define UA_NS0ID_MONITOREDITEMMODIFYRESULT 758 // DataType
#define UA_NS0ID_MODIFYMONITOREDITEMSREQUEST 761 // DataType
#define UA_NS0ID_MODIFYMONITOREDITEMSRESPONSE 764 // DataType
#define UA_NS0ID_SETMONITORINGMODEREQUEST 767 // DataType
#define UA_NS0ID_SETMONITORINGMODERESPONSE 770 // DataType
#define UA_NS0ID_SETTRIGGERINGREQUEST 773 // DataType
#define UA_NS0ID_SETTRIGGERINGRESPONSE 776 // DataType
#define UA_NS0ID_DELETEMONITOREDITEMSREQUEST 779 // DataType
#define UA_NS0ID_DELETEMONITOREDITEMSRESPONSE 782 // DataType
#define UA_NS0ID_CREATESUBSCRIPTIONREQUEST 785 // DataType
#define UA_NS0ID_CREATESUBSCRIPTIONRESPONSE 788 // DataType
#define UA_NS0ID_MODIFYSUBSCRIPTIONREQUEST 791 // DataType
#define UA_NS0ID_MODIFYSUBSCRIPTIONRESPONSE 794 // DataType
#define UA_NS0ID_SETPUBLISHINGMODEREQUEST 797 // DataType
#define UA_NS0ID_SETPUBLISHINGMODERESPONSE 800 // DataType
#define UA_NS0ID_NOTIFICATIONMESSAGE 803 // DataType
#define UA_NS0ID_MONITOREDITEMNOTIFICATION 806 // DataType
#define UA_NS0ID_DATACHANGENOTIFICATION 809 // DataType
#define UA_NS0ID_STATUSCHANGENOTIFICATION 818 // DataType
#define UA_NS0ID_SUBSCRIPTIONACKNOWLEDGEMENT 821 // DataType
#define UA_NS0ID_PUBLISHREQUEST 824 // DataType
#define UA_NS0ID_PUBLISHRESPONSE 827 // DataType
#define UA_NS0ID_REPUBLISHREQUEST 830 // DataType
#define UA_NS0ID_REPUBLISHRESPONSE 833 // DataType
#define UA_NS0ID_TRANSFERRESULT 836 // DataType
#define UA_NS0ID_TRANSFERSUBSCRIPTIONSREQUEST 839 // DataType
#define UA_NS0ID_TRANSFERSUBSCRIPTIONSRESPONSE 842 // DataType
#define UA_NS0ID_DELETESUBSCRIPTIONSREQUEST 845 // DataType
#define UA_NS0ID_DELETESUBSCRIPTIONSRESPONSE 848 // DataType
#define UA_NS0ID_REDUNDANCYSUPPORT 851 // DataType
#define UA_NS0ID_SERVERSTATE 852 // DataType
#define UA_NS0ID_REDUNDANTSERVERDATATYPE 853 // DataType
#define UA_NS0ID_SAMPLINGINTERVALDIAGNOSTICSDATATYPE 856 // DataType
#define UA_NS0ID_SERVERDIAGNOSTICSSUMMARYDATATYPE 859 // DataType
#define UA_NS0ID_SERVERSTATUSDATATYPE 862 // DataType
#define UA_NS0ID_SESSIONDIAGNOSTICSDATATYPE 865 // DataType
#define UA_NS0ID_SESSIONSECURITYDIAGNOSTICSDATATYPE 868 // DataType
#define UA_NS0ID_SERVICECOUNTERDATATYPE 871 // DataType
#define UA_NS0ID_SUBSCRIPTIONDIAGNOSTICSDATATYPE 874 // DataType
#define UA_NS0ID_MODELCHANGESTRUCTUREDATATYPE 877 // DataType
#define UA_NS0ID_RANGE 884 // DataType
#define UA_NS0ID_EUINFORMATION 887 // DataType
#define UA_NS0ID_EXCEPTIONDEVIATIONFORMAT 890 // DataType
#define UA_NS0ID_ANNOTATION 891 // DataType
#define UA_NS0ID_PROGRAMDIAGNOSTICDATATYPE 894 // DataType
#define UA_NS0ID_SEMANTICCHANGESTRUCTUREDATATYPE 897 // DataType
#define UA_NS0ID_EVENTNOTIFICATIONLIST 914 // DataType
#define UA_NS0ID_EVENTFIELDLIST 917 // DataType
#define UA_NS0ID_HISTORYEVENTFIELDLIST 920 // DataType
#define UA_NS0ID_HISTORYUPDATEEVENTRESULT 929 // DataType
#define UA_NS0ID_ISSUEDIDENTITYTOKEN 938 // DataType
#define UA_NS0ID_NOTIFICATIONDATA 945 // DataType
#define UA_NS0ID_AGGREGATECONFIGURATION 948 // DataType
#define UA_NS0ID_IMAGEBMP 2000 // DataType
#define UA_NS0ID_IMAGEGIF 2001 // DataType
#define UA_NS0ID_IMAGEJPG 2002 // DataType
#define UA_NS0ID_IMAGEPNG 2003 // DataType
#define UA_NS0ID_SERVERTYPE 2004 // ObjectType
#define UA_NS0ID_SERVERCAPABILITIESTYPE 2013 // ObjectType
#define UA_NS0ID_SERVERDIAGNOSTICSTYPE 2020 // ObjectType
#define UA_NS0ID_SESSIONSDIAGNOSTICSSUMMARYTYPE 2026 // ObjectType
#define UA_NS0ID_SESSIONDIAGNOSTICSOBJECTTYPE 2029 // ObjectType
#define UA_NS0ID_VENDORSERVERINFOTYPE 2033 // ObjectType
#define UA_NS0ID_SERVERREDUNDANCYTYPE 2034 // ObjectType
#define UA_NS0ID_TRANSPARENTREDUNDANCYTYPE 2036 // ObjectType
#define UA_NS0ID_NONTRANSPARENTREDUNDANCYTYPE 2039 // ObjectType
#define UA_NS0ID_BASEEVENTTYPE 2041 // ObjectType
#define UA_NS0ID_AUDITEVENTTYPE 2052 // ObjectType
#define UA_NS0ID_AUDITSECURITYEVENTTYPE 2058 // ObjectType
#define UA_NS0ID_AUDITCHANNELEVENTTYPE 2059 // ObjectType
#define UA_NS0ID_AUDITOPENSECURECHANNELEVENTTYPE 2060 // ObjectType
#define UA_NS0ID_AUDITSESSIONEVENTTYPE 2069 // ObjectType
#define UA_NS0ID_AUDITCREATESESSIONEVENTTYPE 2071 // ObjectType
#define UA_NS0ID_AUDITACTIVATESESSIONEVENTTYPE 2075 // ObjectType
#define UA_NS0ID_AUDITCANCELEVENTTYPE 2078 // ObjectType
#define UA_NS0ID_AUDITCERTIFICATEEVENTTYPE 2080 // ObjectType
#define UA_NS0ID_AUDITCERTIFICATEDATAMISMATCHEVENTTYPE 2082 // ObjectType
#define UA_NS0ID_AUDITCERTIFICATEEXPIREDEVENTTYPE 2085 // ObjectType
#define UA_NS0ID_AUDITCERTIFICATEINVALIDEVENTTYPE 2086 // ObjectType
#define UA_NS0ID_AUDITCERTIFICATEUNTRUSTEDEVENTTYPE 2087 // ObjectType
#define UA_NS0ID_AUDITCERTIFICATEREVOKEDEVENTTYPE 2088 // ObjectType
#define UA_NS0ID_AUDITCERTIFICATEMISMATCHEVENTTYPE 2089 // ObjectType
#define UA_NS0ID_AUDITNODEMANAGEMENTEVENTTYPE 2090 // ObjectType
#define UA_NS0ID_AUDITADDNODESEVENTTYPE 2091 // ObjectType
#define UA_NS0ID_AUDITDELETENODESEVENTTYPE 2093 // ObjectType
#define UA_NS0ID_AUDITADDREFERENCESEVENTTYPE 2095 // ObjectType
#define UA_NS0ID_AUDITDELETEREFERENCESEVENTTYPE 2097 // ObjectType
#define UA_NS0ID_AUDITUPDATEEVENTTYPE 2099 // ObjectType
#define UA_NS0ID_AUDITWRITEUPDATEEVENTTYPE 2100 // ObjectType
#define UA_NS0ID_AUDITHISTORYUPDATEEVENTTYPE 2104 // ObjectType
#define UA_NS0ID_AUDITUPDATEMETHODEVENTTYPE 2127 // ObjectType
#define UA_NS0ID_SYSTEMEVENTTYPE 2130 // ObjectType
#define UA_NS0ID_DEVICEFAILUREEVENTTYPE 2131 // ObjectType
#define UA_NS0ID_BASEMODELCHANGEEVENTTYPE 2132 // ObjectType
#define UA_NS0ID_GENERALMODELCHANGEEVENTTYPE 2133 // ObjectType
#define UA_NS0ID_SERVERVENDORCAPABILITYTYPE 2137 // VariableType
#define UA_NS0ID_SERVERSTATUSTYPE 2138 // VariableType
#define UA_NS0ID_SERVERDIAGNOSTICSSUMMARYTYPE 2150 // VariableType
#define UA_NS0ID_SAMPLINGINTERVALDIAGNOSTICSARRAYTYPE 2164 // VariableType
#define UA_NS0ID_SAMPLINGINTERVALDIAGNOSTICSTYPE 2165 // VariableType
#define UA_NS0ID_SUBSCRIPTIONDIAGNOSTICSARRAYTYPE 2171 // VariableType
#define UA_NS0ID_SUBSCRIPTIONDIAGNOSTICSTYPE 2172 // VariableType
#define UA_NS0ID_SESSIONDIAGNOSTICSARRAYTYPE 2196 // VariableType
#define UA_NS0ID_SESSIONDIAGNOSTICSVARIABLETYPE 2197 // VariableType
#define UA_NS0ID_SESSIONSECURITYDIAGNOSTICSARRAYTYPE 2243 // VariableType
#define UA_NS0ID_SESSIONSECURITYDIAGNOSTICSTYPE 2244 // VariableType
#define UA_NS0ID_SERVER 2253 // Object
#define UA_NS0ID_SERVER_SERVERARRAY 2254 // Variable
#define UA_NS0ID_SERVER_NAMESPACEARRAY 2255 // Variable
#define UA_NS0ID_SERVER_SERVERSTATUS 2256 // Variable
#define UA_NS0ID_SERVER_SERVERSTATUS_STARTTIME 2257 // Variable
#define UA_NS0ID_SERVER_SERVERSTATUS_CURRENTTIME 2258 // Variable
#define UA_NS0ID_SERVER_SERVERSTATUS_STATE 2259 // Variable
#define UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO 2260 // Variable
#define UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO_PRODUCTNAME 2261 // Variable
#define UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO_PRODUCTURI 2262 // Variable
#define UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO_MANUFACTURERNAME 2263 // Variable
#define UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO_SOFTWAREVERSION 2264 // Variable
#define UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO_BUILDNUMBER 2265 // Variable
#define UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO_BUILDDATE 2266 // Variable
#define UA_NS0ID_SERVER_SERVICELEVEL 2267 // Variable
#define UA_NS0ID_SERVER_SERVERCAPABILITIES 2268 // Object
#define UA_NS0ID_SERVER_SERVERCAPABILITIES_SERVERPROFILEARRAY 2269 // Variable
#define UA_NS0ID_SERVER_SERVERCAPABILITIES_LOCALEIDARRAY 2271 // Variable
#define UA_NS0ID_SERVER_SERVERCAPABILITIES_MINSUPPORTEDSAMPLERATE 2272 // Variable
#define UA_NS0ID_SERVER_SERVERDIAGNOSTICS 2274 // Object
#define UA_NS0ID_SERVER_SERVERDIAGNOSTICS_SERVERDIAGNOSTICSSUMMARY 2275 // Variable
#define UA_NS0ID_SERVER_SERVERDIAGNOSTICS_SERVERDIAGNOSTICSSUMMARY_SERVERVIEWCOUNT 2276 // Variable
#define UA_NS0ID_SERVER_SERVERDIAGNOSTICS_SERVERDIAGNOSTICSSUMMARY_CURRENTSESSIONCOUNT 2277 // Variable
#define UA_NS0ID_SERVER_SERVERDIAGNOSTICS_SERVERDIAGNOSTICSSUMMARY_CUMULATEDSESSIONCOUNT 2278 // Variable
#define UA_NS0ID_SERVER_SERVERDIAGNOSTICS_SERVERDIAGNOSTICSSUMMARY_SECURITYREJECTEDSESSIONCOUNT 2279 // Variable
#define UA_NS0ID_SERVER_SERVERDIAGNOSTICS_SERVERDIAGNOSTICSSUMMARY_SESSIONTIMEOUTCOUNT 2281 // Variable
#define UA_NS0ID_SERVER_SERVERDIAGNOSTICS_SERVERDIAGNOSTICSSUMMARY_SESSIONABORTCOUNT 2282 // Variable
#define UA_NS0ID_SERVER_SERVERDIAGNOSTICS_SERVERDIAGNOSTICSSUMMARY_PUBLISHINGINTERVALCOUNT 2284 // Variable
#define UA_NS0ID_SERVER_SERVERDIAGNOSTICS_SERVERDIAGNOSTICSSUMMARY_CURRENTSUBSCRIPTIONCOUNT 2285 // Variable
#define UA_NS0ID_SERVER_SERVERDIAGNOSTICS_SERVERDIAGNOSTICSSUMMARY_CUMULATEDSUBSCRIPTIONCOUNT 2286 // Variable
#define UA_NS0ID_SERVER_SERVERDIAGNOSTICS_SERVERDIAGNOSTICSSUMMARY_SECURITYREJECTEDREQUESTSCOUNT 2287 // Variable
#define UA_NS0ID_SERVER_SERVERDIAGNOSTICS_SERVERDIAGNOSTICSSUMMARY_REJECTEDREQUESTSCOUNT 2288 // Variable
#define UA_NS0ID_SERVER_SERVERDIAGNOSTICS_SAMPLINGINTERVALDIAGNOSTICSARRAY 2289 // Variable
#define UA_NS0ID_SERVER_SERVERDIAGNOSTICS_SUBSCRIPTIONDIAGNOSTICSARRAY 2290 // Variable
#define UA_NS0ID_SERVER_SERVERDIAGNOSTICS_ENABLEDFLAG 2294 // Variable
#define UA_NS0ID_SERVER_VENDORSERVERINFO 2295 // Object
#define UA_NS0ID_SERVER_SERVERREDUNDANCY 2296 // Object
#define UA_NS0ID_STATEMACHINETYPE 2299 // ObjectType
#define UA_NS0ID_STATETYPE 2307 // ObjectType
#define UA_NS0ID_INITIALSTATETYPE 2309 // ObjectType
#define UA_NS0ID_TRANSITIONTYPE 2310 // ObjectType
#define UA_NS0ID_TRANSITIONEVENTTYPE 2311 // ObjectType
#define UA_NS0ID_AUDITUPDATESTATEEVENTTYPE 2315 // ObjectType
#define UA_NS0ID_HISTORICALDATACONFIGURATIONTYPE 2318 // ObjectType
#define UA_NS0ID_HISTORYSERVERCAPABILITIESTYPE 2330 // ObjectType
#define UA_NS0ID_AGGREGATEFUNCTIONTYPE 2340 // ObjectType
#define UA_NS0ID_AGGREGATEFUNCTION_INTERPOLATIVE 2341 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_AVERAGE 2342 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_TIMEAVERAGE 2343 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_TOTAL 2344 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_MINIMUM 2346 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_MAXIMUM 2347 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_MINIMUMACTUALTIME 2348 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_MAXIMUMACTUALTIME 2349 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_RANGE 2350 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_ANNOTATIONCOUNT 2351 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_COUNT 2352 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_NUMBEROFTRANSITIONS 2355 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_START 2357 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_END 2358 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_DELTA 2359 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_DURATIONGOOD 2360 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_DURATIONBAD 2361 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_PERCENTGOOD 2362 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_PERCENTBAD 2363 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_WORSTQUALITY 2364 // Object
#define UA_NS0ID_DATAITEMTYPE 2365 // VariableType
#define UA_NS0ID_ANALOGITEMTYPE 2368 // VariableType
#define UA_NS0ID_DISCRETEITEMTYPE 2372 // VariableType
#define UA_NS0ID_TWOSTATEDISCRETETYPE 2373 // VariableType
#define UA_NS0ID_MULTISTATEDISCRETETYPE 2376 // VariableType
#define UA_NS0ID_PROGRAMTRANSITIONEVENTTYPE 2378 // ObjectType
#define UA_NS0ID_PROGRAMDIAGNOSTICTYPE 2380 // VariableType
#define UA_NS0ID_PROGRAMSTATEMACHINETYPE 2391 // ObjectType
#define UA_NS0ID_SERVER_SERVERCAPABILITIES_MAXBROWSECONTINUATIONPOINTS 2735 // Variable
#define UA_NS0ID_SERVER_SERVERCAPABILITIES_MAXQUERYCONTINUATIONPOINTS 2736 // Variable
#define UA_NS0ID_SERVER_SERVERCAPABILITIES_MAXHISTORYCONTINUATIONPOINTS 2737 // Variable
#define UA_NS0ID_SEMANTICCHANGEEVENTTYPE 2738 // ObjectType
#define UA_NS0ID_AUDITURLMISMATCHEVENTTYPE 2748 // ObjectType
#define UA_NS0ID_STATEVARIABLETYPE 2755 // VariableType
#define UA_NS0ID_FINITESTATEVARIABLETYPE 2760 // VariableType
#define UA_NS0ID_TRANSITIONVARIABLETYPE 2762 // VariableType
#define UA_NS0ID_FINITETRANSITIONVARIABLETYPE 2767 // VariableType
#define UA_NS0ID_FINITESTATEMACHINETYPE 2771 // ObjectType
#define UA_NS0ID_CONDITIONTYPE 2782 // ObjectType
#define UA_NS0ID_REFRESHSTARTEVENTTYPE 2787 // ObjectType
#define UA_NS0ID_REFRESHENDEVENTTYPE 2788 // ObjectType
#define UA_NS0ID_REFRESHREQUIREDEVENTTYPE 2789 // ObjectType
#define UA_NS0ID_AUDITCONDITIONEVENTTYPE 2790 // ObjectType
#define UA_NS0ID_AUDITCONDITIONENABLEEVENTTYPE 2803 // ObjectType
#define UA_NS0ID_AUDITCONDITIONCOMMENTEVENTTYPE 2829 // ObjectType
#define UA_NS0ID_DIALOGCONDITIONTYPE 2830 // ObjectType
#define UA_NS0ID_ACKNOWLEDGEABLECONDITIONTYPE 2881 // ObjectType
#define UA_NS0ID_ALARMCONDITIONTYPE 2915 // ObjectType
#define UA_NS0ID_SHELVEDSTATEMACHINETYPE 2929 // ObjectType
#define UA_NS0ID_LIMITALARMTYPE 2955 // ObjectType
#define UA_NS0ID_SERVER_SERVERSTATUS_SECONDSTILLSHUTDOWN 2992 // Variable
#define UA_NS0ID_SERVER_SERVERSTATUS_SHUTDOWNREASON 2993 // Variable
#define UA_NS0ID_SERVER_AUDITING 2994 // Variable
#define UA_NS0ID_SERVER_SERVERCAPABILITIES_MODELLINGRULES 2996 // Object
#define UA_NS0ID_SERVER_SERVERCAPABILITIES_AGGREGATEFUNCTIONS 2997 // Object
#define UA_NS0ID_AUDITHISTORYEVENTUPDATEEVENTTYPE 2999 // ObjectType
#define UA_NS0ID_AUDITHISTORYVALUEUPDATEEVENTTYPE 3006 // ObjectType
#define UA_NS0ID_AUDITHISTORYDELETEEVENTTYPE 3012 // ObjectType
#define UA_NS0ID_AUDITHISTORYRAWMODIFYDELETEEVENTTYPE 3014 // ObjectType
#define UA_NS0ID_AUDITHISTORYATTIMEDELETEEVENTTYPE 3019 // ObjectType
#define UA_NS0ID_AUDITHISTORYEVENTDELETEEVENTTYPE 3022 // ObjectType
#define UA_NS0ID_EVENTQUEUEOVERFLOWEVENTTYPE 3035 // ObjectType
#define UA_NS0ID_EVENTTYPESFOLDER 3048 // Object
#define UA_NS0ID_BUILDINFOTYPE 3051 // VariableType
#define UA_NS0ID_DEFAULTBINARY 3062 // Object
#define UA_NS0ID_DEFAULTXML 3063 // Object
#define UA_NS0ID_ALWAYSGENERATESEVENT 3065 // ReferenceType
#define UA_NS0ID_ICON 3067 // Variable
#define UA_NS0ID_NODEVERSION 3068 // Variable
#define UA_NS0ID_LOCALTIME 3069 // Variable
#define UA_NS0ID_ALLOWNULLS 3070 // Variable
#define UA_NS0ID_ENUMVALUES 3071 // Variable
#define UA_NS0ID_INPUTARGUMENTS 3072 // Variable
#define UA_NS0ID_OUTPUTARGUMENTS 3073 // Variable
#define UA_NS0ID_SERVER_SERVERCAPABILITIES_SOFTWARECERTIFICATES 3704 // Variable
#define UA_NS0ID_SERVER_SERVERDIAGNOSTICS_SERVERDIAGNOSTICSSUMMARY_REJECTEDSESSIONCOUNT 3705 // Variable
#define UA_NS0ID_SERVER_SERVERDIAGNOSTICS_SESSIONSDIAGNOSTICSSUMMARY 3706 // Object
#define UA_NS0ID_SERVER_SERVERDIAGNOSTICS_SESSIONSDIAGNOSTICSSUMMARY_SESSIONDIAGNOSTICSARRAY 3707 // Variable
#define UA_NS0ID_SERVER_SERVERDIAGNOSTICS_SESSIONSDIAGNOSTICSSUMMARY_SESSIONSECURITYDIAGNOSTICSARRAY 3708 // Variable
#define UA_NS0ID_SERVER_SERVERREDUNDANCY_REDUNDANCYSUPPORT 3709 // Variable
#define UA_NS0ID_PROGRAMTRANSITIONAUDITEVENTTYPE 3806 // ObjectType
#define UA_NS0ID_ADDCOMMENTMETHODTYPE 3863 // Method
#define UA_NS0ID_TIMEDSHELVEMETHODTYPE 6102 // Method
#define UA_NS0ID_ENUMVALUETYPE 7594 // DataType
#define UA_NS0ID_MESSAGESECURITYMODE_ENUMSTRINGS 7595 // Variable
#define UA_NS0ID_COMPLIANCELEVEL_ENUMSTRINGS 7599 // Variable
#define UA_NS0ID_BROWSEDIRECTION_ENUMSTRINGS 7603 // Variable
#define UA_NS0ID_FILTEROPERATOR_ENUMSTRINGS 7605 // Variable
#define UA_NS0ID_TIMESTAMPSTORETURN_ENUMSTRINGS 7606 // Variable
#define UA_NS0ID_MONITORINGMODE_ENUMSTRINGS 7608 // Variable
#define UA_NS0ID_DATACHANGETRIGGER_ENUMSTRINGS 7609 // Variable
#define UA_NS0ID_REDUNDANCYSUPPORT_ENUMSTRINGS 7611 // Variable
#define UA_NS0ID_SERVERSTATE_ENUMSTRINGS 7612 // Variable
#define UA_NS0ID_EXCEPTIONDEVIATIONFORMAT_ENUMSTRINGS 7614 // Variable
#define UA_NS0ID_TIMEZONEDATATYPE 8912 // DataType
#define UA_NS0ID_LOCKTYPE 8921 // ObjectType
#define UA_NS0ID_SERVERLOCK 8924 // Object
#define UA_NS0ID_SERVERLOCK_LOCK 8925 // Method
#define UA_NS0ID_SERVERLOCK_UNLOCK 8926 // Method
#define UA_NS0ID_AUDITCONDITIONRESPONDEVENTTYPE 8927 // ObjectType
#define UA_NS0ID_AUDITCONDITIONACKNOWLEDGEEVENTTYPE 8944 // ObjectType
#define UA_NS0ID_AUDITCONDITIONCONFIRMEVENTTYPE 8961 // ObjectType
#define UA_NS0ID_TWOSTATEVARIABLETYPE 8995 // VariableType
#define UA_NS0ID_CONDITIONVARIABLETYPE 9002 // VariableType
#define UA_NS0ID_HASTRUESUBSTATE 9004 // ReferenceType
#define UA_NS0ID_HASFALSESUBSTATE 9005 // ReferenceType
#define UA_NS0ID_HASCONDITION 9006 // ReferenceType
#define UA_NS0ID_CONDITIONREFRESHMETHODTYPE 9007 // Method
#define UA_NS0ID_DIALOGRESPONSEMETHODTYPE 9031 // Method
#define UA_NS0ID_EXCLUSIVELIMITSTATEMACHINETYPE 9318 // ObjectType
#define UA_NS0ID_EXCLUSIVELIMITALARMTYPE 9341 // ObjectType
#define UA_NS0ID_EXCLUSIVELEVELALARMTYPE 9482 // ObjectType
#define UA_NS0ID_EXCLUSIVERATEOFCHANGEALARMTYPE 9623 // ObjectType
#define UA_NS0ID_EXCLUSIVEDEVIATIONALARMTYPE 9764 // ObjectType
#define UA_NS0ID_NONEXCLUSIVELIMITALARMTYPE 9906 // ObjectType
#define UA_NS0ID_NONEXCLUSIVELEVELALARMTYPE 10060 // ObjectType
#define UA_NS0ID_NONEXCLUSIVERATEOFCHANGEALARMTYPE 10214 // ObjectType
#define UA_NS0ID_NONEXCLUSIVEDEVIATIONALARMTYPE 10368 // ObjectType
#define UA_NS0ID_DISCRETEALARMTYPE 10523 // ObjectType
#define UA_NS0ID_OFFNORMALALARMTYPE 10637 // ObjectType
#define UA_NS0ID_TRIPALARMTYPE 10751 // ObjectType
#define UA_NS0ID_AUDITCONDITIONSHELVINGEVENTTYPE 11093 // ObjectType
#define UA_NS0ID_BASECONDITIONCLASSTYPE 11163 // ObjectType
#define UA_NS0ID_PROCESSCONDITIONCLASSTYPE 11164 // ObjectType
#define UA_NS0ID_MAINTENANCECONDITIONCLASSTYPE 11165 // ObjectType
#define UA_NS0ID_SYSTEMCONDITIONCLASSTYPE 11166 // ObjectType
#define UA_NS0ID_AGGREGATECONFIGURATIONTYPE 11187 // ObjectType
#define UA_NS0ID_HISTORYSERVERCAPABILITIES 11192 // Object
#define UA_NS0ID_HISTORYSERVERCAPABILITIES_ACCESSHISTORYDATACAPABILITY 11193 // Variable
#define UA_NS0ID_HISTORYSERVERCAPABILITIES_INSERTDATACAPABILITY 11196 // Variable
#define UA_NS0ID_HISTORYSERVERCAPABILITIES_REPLACEDATACAPABILITY 11197 // Variable
#define UA_NS0ID_HISTORYSERVERCAPABILITIES_UPDATEDATACAPABILITY 11198 // Variable
#define UA_NS0ID_HISTORYSERVERCAPABILITIES_DELETERAWCAPABILITY 11199 // Variable
#define UA_NS0ID_HISTORYSERVERCAPABILITIES_DELETEATTIMECAPABILITY 11200 // Variable
#define UA_NS0ID_HISTORYSERVERCAPABILITIES_AGGREGATEFUNCTIONS 11201 // Object
#define UA_NS0ID_HACONFIGURATION 11202 // Object
#define UA_NS0ID_HACONFIGURATION_AGGREGATECONFIGURATION 11203 // Object
#define UA_NS0ID_HACONFIGURATION_AGGREGATECONFIGURATION_TREATUNCERTAINASBAD 11204 // Variable
#define UA_NS0ID_HACONFIGURATION_AGGREGATECONFIGURATION_PERCENTDATABAD 11205 // Variable
#define UA_NS0ID_HACONFIGURATION_AGGREGATECONFIGURATION_PERCENTDATAGOOD 11206 // Variable
#define UA_NS0ID_HACONFIGURATION_AGGREGATECONFIGURATION_USESLOPEDEXTRAPOLATION 11207 // Variable
#define UA_NS0ID_HACONFIGURATION_STEPPED 11208 // Variable
#define UA_NS0ID_HACONFIGURATION_DEFINITION 11209 // Variable
#define UA_NS0ID_HACONFIGURATION_MAXTIMEINTERVAL 11210 // Variable
#define UA_NS0ID_HACONFIGURATION_MINTIMEINTERVAL 11211 // Variable
#define UA_NS0ID_HACONFIGURATION_EXCEPTIONDEVIATION 11212 // Variable
#define UA_NS0ID_HACONFIGURATION_EXCEPTIONDEVIATIONFORMAT 11213 // Variable
#define UA_NS0ID_ANNOTATIONS 11214 // Variable
#define UA_NS0ID_HISTORICALEVENTFILTER 11215 // Variable
#define UA_NS0ID_MODIFICATIONINFO 11216 // DataType
#define UA_NS0ID_HISTORYMODIFIEDDATA 11217 // DataType
#define UA_NS0ID_HISTORYUPDATETYPE 11234 // DataType
#define UA_NS0ID_MULTISTATEVALUEDISCRETETYPE 11238 // VariableType
#define UA_NS0ID_HISTORYSERVERCAPABILITIES_ACCESSHISTORYEVENTSCAPABILITY 11242 // Variable
#define UA_NS0ID_HISTORYSERVERCAPABILITIES_MAXRETURNDATAVALUES 11273 // Variable
#define UA_NS0ID_HISTORYSERVERCAPABILITIES_MAXRETURNEVENTVALUES 11274 // Variable
#define UA_NS0ID_HISTORYSERVERCAPABILITIES_INSERTANNOTATIONCAPABILITY 11275 // Variable
#define UA_NS0ID_HISTORYSERVERCAPABILITIES_INSERTEVENTCAPABILITY 11281 // Variable
#define UA_NS0ID_HISTORYSERVERCAPABILITIES_REPLACEEVENTCAPABILITY 11282 // Variable
#define UA_NS0ID_HISTORYSERVERCAPABILITIES_UPDATEEVENTCAPABILITY 11283 // Variable
#define UA_NS0ID_AGGREGATEFUNCTION_TIMEAVERAGE2 11285 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_MINIMUM2 11286 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_MAXIMUM2 11287 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_RANGE2 11288 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_WORSTQUALITY2 11292 // Object
#define UA_NS0ID_PERFORMUPDATETYPE 11293 // DataType
#define UA_NS0ID_UPDATESTRUCTUREDATADETAILS 11295 // DataType
#define UA_NS0ID_AGGREGATEFUNCTION_TOTAL2 11304 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_MINIMUMACTUALTIME2 11305 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_MAXIMUMACTUALTIME2 11306 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_DURATIONINSTATEZERO 11307 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_DURATIONINSTATENONZERO 11308 // Object
#define UA_NS0ID_SERVER_SERVERREDUNDANCY_CURRENTSERVERID 11312 // Variable
#define UA_NS0ID_SERVER_SERVERREDUNDANCY_REDUNDANTSERVERARRAY 11313 // Variable
#define UA_NS0ID_SERVER_SERVERREDUNDANCY_SERVERURIARRAY 11314 // Variable
#define UA_NS0ID_AGGREGATEFUNCTION_STANDARDDEVIATIONSAMPLE 11426 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_STANDARDDEVIATIONPOPULATION 11427 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_VARIANCESAMPLE 11428 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_VARIANCEPOPULATION 11429 // Object
#define UA_NS0ID_ENUMSTRINGS 11432 // Variable
#define UA_NS0ID_VALUEASTEXT 11433 // Variable
#define UA_NS0ID_PROGRESSEVENTTYPE 11436 // ObjectType
#define UA_NS0ID_SYSTEMSTATUSCHANGEEVENTTYPE 11446 // ObjectType
#define UA_NS0ID_OPTIONSETTYPE 11487 // VariableType
#define UA_NS0ID_SERVER_GETMONITOREDITEMS 11492 // Method
#define UA_NS0ID_SERVER_GETMONITOREDITEMS_INPUTARGUMENTS 11493 // Variable
#define UA_NS0ID_SERVER_GETMONITOREDITEMS_OUTPUTARGUMENTS 11494 // Variable
#define UA_NS0ID_GETMONITOREDITEMSMETHODTYPE 11495 // Method
#define UA_NS0ID_MAXSTRINGLENGTH 11498 // Variable
#define UA_NS0ID_HISTORYSERVERCAPABILITIES_DELETEEVENTCAPABILITY 11502 // Variable
#define UA_NS0ID_HACONFIGURATION_STARTOFARCHIVE 11503 // Variable
#define UA_NS0ID_HACONFIGURATION_STARTOFONLINEARCHIVE 11504 // Variable
#define UA_NS0ID_AGGREGATEFUNCTION_STARTBOUND 11505 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_ENDBOUND 11506 // Object
#define UA_NS0ID_AGGREGATEFUNCTION_DELTABOUNDS 11507 // Object
#define UA_NS0ID_MODELLINGRULE_OPTIONALPLACEHOLDER 11508 // Object
#define UA_NS0ID_MODELLINGRULE_OPTIONALPLACEHOLDER_NAMINGRULE 11509 // Variable
#define UA_NS0ID_MODELLINGRULE_MANDATORYPLACEHOLDER 11510 // Object
#define UA_NS0ID_MODELLINGRULE_MANDATORYPLACEHOLDER_NAMINGRULE 11511 // Variable
#define UA_NS0ID_MAXARRAYLENGTH 11512 // Variable
#define UA_NS0ID_ENGINEERINGUNITS 11513 // Variable
#define UA_NS0ID_OPERATIONLIMITSTYPE 11564 // ObjectType
#define UA_NS0ID_FILETYPE 11575 // ObjectType
#define UA_NS0ID_ADDRESSSPACEFILETYPE 11595 // ObjectType
#define UA_NS0ID_NAMESPACEMETADATATYPE 11616 // ObjectType
#define UA_NS0ID_NAMESPACESTYPE 11645 // ObjectType
#define UA_NS0ID_SERVER_SERVERCAPABILITIES_MAXARRAYLENGTH 11702 // Variable
#define UA_NS0ID_SERVER_SERVERCAPABILITIES_MAXSTRINGLENGTH 11703 // Variable
#define UA_NS0ID_SERVER_SERVERCAPABILITIES_OPERATIONLIMITS 11704 // Object
#define UA_NS0ID_SERVER_SERVERCAPABILITIES_OPERATIONLIMITS_MAXNODESPERREAD 11705 // Variable
#define UA_NS0ID_SERVER_SERVERCAPABILITIES_OPERATIONLIMITS_MAXNODESPERWRITE 11707 // Variable
#define UA_NS0ID_SERVER_SERVERCAPABILITIES_OPERATIONLIMITS_MAXNODESPERMETHODCALL 11709 // Variable
#define UA_NS0ID_SERVER_SERVERCAPABILITIES_OPERATIONLIMITS_MAXNODESPERBROWSE 11710 // Variable
#define UA_NS0ID_SERVER_SERVERCAPABILITIES_OPERATIONLIMITS_MAXNODESPERREGISTERNODES 11711 // Variable
#define UA_NS0ID_SERVER_SERVERCAPABILITIES_OPERATIONLIMITS_MAXNODESPERTRANSLATEBROWSEPATHSTONODEIDS 11712 // Variable
#define UA_NS0ID_SERVER_SERVERCAPABILITIES_OPERATIONLIMITS_MAXNODESPERNODEMANAGEMENT 11713 // Variable
#define UA_NS0ID_SERVER_SERVERCAPABILITIES_OPERATIONLIMITS_MAXMONITOREDITEMSPERCALL 11714 // Variable
#define UA_NS0ID_SERVER_NAMESPACES 11715 // Object
#define UA_NS0ID_SERVER_NAMESPACES_ADDRESSSPACEFILE 11716 // Object
#define UA_NS0ID_SERVER_NAMESPACES_ADDRESSSPACEFILE_SIZE 11717 // Variable
#define UA_NS0ID_SERVER_NAMESPACES_ADDRESSSPACEFILE_WRITEABLE 11718 // Variable
#define UA_NS0ID_SERVER_NAMESPACES_ADDRESSSPACEFILE_USERWRITEABLE 11719 // Variable
#define UA_NS0ID_SERVER_NAMESPACES_ADDRESSSPACEFILE_OPENCOUNT 11720 // Variable
#define UA_NS0ID_SERVER_NAMESPACES_ADDRESSSPACEFILE_OPEN 11721 // Method
#define UA_NS0ID_SERVER_NAMESPACES_ADDRESSSPACEFILE_OPEN_INPUTARGUMENTS 11722 // Variable
#define UA_NS0ID_SERVER_NAMESPACES_ADDRESSSPACEFILE_OPEN_OUTPUTARGUMENTS 11723 // Variable
#define UA_NS0ID_SERVER_NAMESPACES_ADDRESSSPACEFILE_CLOSE 11724 // Method
#define UA_NS0ID_SERVER_NAMESPACES_ADDRESSSPACEFILE_CLOSE_INPUTARGUMENTS 11725 // Variable
#define UA_NS0ID_SERVER_NAMESPACES_ADDRESSSPACEFILE_READ 11726 // Method
#define UA_NS0ID_SERVER_NAMESPACES_ADDRESSSPACEFILE_READ_INPUTARGUMENTS 11727 // Variable
#define UA_NS0ID_SERVER_NAMESPACES_ADDRESSSPACEFILE_READ_OUTPUTARGUMENTS 11728 // Variable
#define UA_NS0ID_SERVER_NAMESPACES_ADDRESSSPACEFILE_WRITE 11729 // Method
#define UA_NS0ID_SERVER_NAMESPACES_ADDRESSSPACEFILE_WRITE_INPUTARGUMENTS 11730 // Variable
#define UA_NS0ID_SERVER_NAMESPACES_ADDRESSSPACEFILE_GETPOSITION 11731 // Method
#define UA_NS0ID_SERVER_NAMESPACES_ADDRESSSPACEFILE_GETPOSITION_INPUTARGUMENTS 11732 // Variable
#define UA_NS0ID_SERVER_NAMESPACES_ADDRESSSPACEFILE_GETPOSITION_OUTPUTARGUMENTS 11733 // Variable
#define UA_NS0ID_SERVER_NAMESPACES_ADDRESSSPACEFILE_SETPOSITION 11734 // Method
#define UA_NS0ID_SERVER_NAMESPACES_ADDRESSSPACEFILE_SETPOSITION_INPUTARGUMENTS 11735 // Variable
#define UA_NS0ID_SERVER_NAMESPACES_ADDRESSSPACEFILE_EXPORTNAMESPACE 11736 // Method
#define UA_NS0ID_BITFIELDMASKDATATYPE 11737 // DataType
#define UA_NS0ID_OPENMETHODTYPE 11738 // Method
#define UA_NS0ID_CLOSEMETHODTYPE 11741 // Method
#define UA_NS0ID_READMETHODTYPE 11743 // Method
#define UA_NS0ID_WRITEMETHODTYPE 11746 // Method
#define UA_NS0ID_GETPOSITIONMETHODTYPE 11748 // Method
#define UA_NS0ID_SETPOSITIONMETHODTYPE 11751 // Method
#define UA_NS0ID_SYSTEMOFFNORMALALARMTYPE 11753 // ObjectType
#define UA_NS0ID_AUDITPROGRAMTRANSITIONEVENTTYPE 11856 // ObjectType
#define UA_NS0ID_HACONFIGURATION_AGGREGATEFUNCTIONS 11877 // Object
#define UA_NS0ID_NODECLASS_ENUMVALUES 11878 // Variable
#define UA_NS0ID_INSTANCENODE 11879 // DataType
#define UA_NS0ID_TYPENODE 11880 // DataType
#define UA_NS0ID_NODEATTRIBUTESMASK_ENUMVALUES 11881 // Variable
#define UA_NS0ID_ATTRIBUTEWRITEMASK_ENUMVALUES 11882 // Variable
#define UA_NS0ID_BROWSERESULTMASK_ENUMVALUES 11883 // Variable
#define UA_NS0ID_OPENFILEMODE 11939 // DataType
#define UA_NS0ID_OPENFILEMODE_ENUMVALUES 11940 // Variable
#define UA_NS0ID_MODELCHANGESTRUCTUREVERBMASK 11941 // DataType
#define UA_NS0ID_MODELCHANGESTRUCTUREVERBMASK_ENUMVALUES 11942 // Variable
#define UA_NS0ID_ENDPOINTURLLISTDATATYPE 11943 // DataType
#define UA_NS0ID_NETWORKGROUPDATATYPE 11944 // DataType
#define UA_NS0ID_NONTRANSPARENTNETWORKREDUNDANCYTYPE 11945 // ObjectType
#define UA_NS0ID_ARRAYITEMTYPE 12021 // VariableType
#define UA_NS0ID_YARRAYITEMTYPE 12029 // VariableType
#define UA_NS0ID_XYARRAYITEMTYPE 12038 // VariableType
#define UA_NS0ID_IMAGEITEMTYPE 12047 // VariableType
#define UA_NS0ID_CUBEITEMTYPE 12057 // VariableType
#define UA_NS0ID_NDIMENSIONARRAYITEMTYPE 12068 // VariableType
#define UA_NS0ID_AXISSCALEENUMERATION 12077 // DataType
#define UA_NS0ID_AXISSCALEENUMERATION_ENUMSTRINGS 12078 // Variable
#define UA_NS0ID_AXISINFORMATION 12079 // DataType
#define UA_NS0ID_XVTYPE 12080 // DataType
#define UA_NS0ID_SERVER_SERVERCAPABILITIES_OPERATIONLIMITS_MAXNODESPERHISTORYREADDATA 12165 // Variable
#define UA_NS0ID_SERVER_SERVERCAPABILITIES_OPERATIONLIMITS_MAXNODESPERHISTORYREADEVENTS 12166 // Variable
#define UA_NS0ID_SERVER_SERVERCAPABILITIES_OPERATIONLIMITS_MAXNODESPERHISTORYUPDATEDATA 12167 // Variable
#define UA_NS0ID_SERVER_SERVERCAPABILITIES_OPERATIONLIMITS_MAXNODESPERHISTORYUPDATEEVENTS 12168 // Variable
#define UA_NS0ID_VIEWVERSION 12170 // Variable
#define UA_NS0ID_COMPLEXNUMBERTYPE 12171 // DataType
#define UA_NS0ID_DOUBLECOMPLEXNUMBERTYPE 12172 // DataType
#define UA_NS0ID_HASMODELPARENT 50 // ReferenceType


/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src_generated/ua_types_generated.h" ***********************************/

/**
* @file ua_types_generated.h
*
* @brief Autogenerated data types
*
* Generated from Opc.Ua.Types.bsd with script C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/tools/generate_datatypes.py
* on host SPECTRE by user lars at 2015-09-08 01:27:13
*/


#ifdef __cplusplus
extern "C" {
#endif


/**
* @ingroup types
*
* @defgroup ua_types_generated Autogenerated ua_types Types
*
* @brief Data structures that are autogenerated from an XML-Schema.
*
* @{
*/

#define UA_TYPES_COUNT 136

extern UA_EXPORT const UA_DataType *UA_TYPES;

#define UA_TYPES_BOOLEAN 0
#define UA_TYPES_SBYTE 1
#define UA_TYPES_BYTE 2
#define UA_TYPES_INT16 3
#define UA_TYPES_UINT16 4
#define UA_TYPES_INT32 5
#define UA_TYPES_UINT32 6
#define UA_TYPES_INT64 7
#define UA_TYPES_UINT64 8
#define UA_TYPES_FLOAT 9
#define UA_TYPES_DOUBLE 10
#define UA_TYPES_STRING 11
#define UA_TYPES_DATETIME 12
#define UA_TYPES_GUID 13
#define UA_TYPES_BYTESTRING 14
#define UA_TYPES_XMLELEMENT 15
#define UA_TYPES_NODEID 16
#define UA_TYPES_EXPANDEDNODEID 17
#define UA_TYPES_STATUSCODE 18
#define UA_TYPES_QUALIFIEDNAME 19
#define UA_TYPES_LOCALIZEDTEXT 20
#define UA_TYPES_EXTENSIONOBJECT 21
#define UA_TYPES_DATAVALUE 22
#define UA_TYPES_VARIANT 23
#define UA_TYPES_DIAGNOSTICINFO 24

/** @brief The type of identifier used in a node id. */
typedef enum { 
    UA_IDTYPE_NUMERIC = 0,
    UA_IDTYPE_STRING = 1,
    UA_IDTYPE_GUID = 2,
    UA_IDTYPE_OPAQUE = 3
} UA_IdType;
#define UA_TYPES_IDTYPE 25
#define UA_IdType_new (UA_IdType*)UA_Int32_new
#define UA_IdType_init(p) UA_Int32_init((UA_Int32*)p)
#define UA_IdType_delete(p) UA_Int32_delete((UA_Int32*)p)
#define UA_IdType_deleteMembers(p) UA_Int32_deleteMembers((UA_Int32*)p)
#define UA_IdType_copy(src, dst) UA_Int32_copy((const UA_Int32*)src, (UA_Int32*)dst)
#define UA_IdType_encodeBinary(src, dst, offset) UA_Int32_encodeBinary((UA_Int32*)src, dst, offset)
#define UA_IdType_decodeBinary(src, offset, dst) UA_Int32_decodeBinary(src, offset, (UA_Int32*)dst)

/** @brief A mask specifying the class of the node. */
typedef enum { 
    UA_NODECLASS_UNSPECIFIED = 0,
    UA_NODECLASS_OBJECT = 1,
    UA_NODECLASS_VARIABLE = 2,
    UA_NODECLASS_METHOD = 4,
    UA_NODECLASS_OBJECTTYPE = 8,
    UA_NODECLASS_VARIABLETYPE = 16,
    UA_NODECLASS_REFERENCETYPE = 32,
    UA_NODECLASS_DATATYPE = 64,
    UA_NODECLASS_VIEW = 128
} UA_NodeClass;
#define UA_TYPES_NODECLASS 26
#define UA_NodeClass_new (UA_NodeClass*)UA_Int32_new
#define UA_NodeClass_init(p) UA_Int32_init((UA_Int32*)p)
#define UA_NodeClass_delete(p) UA_Int32_delete((UA_Int32*)p)
#define UA_NodeClass_deleteMembers(p) UA_Int32_deleteMembers((UA_Int32*)p)
#define UA_NodeClass_copy(src, dst) UA_Int32_copy((const UA_Int32*)src, (UA_Int32*)dst)
#define UA_NodeClass_encodeBinary(src, dst, offset) UA_Int32_encodeBinary((UA_Int32*)src, dst, offset)
#define UA_NodeClass_decodeBinary(src, offset, dst) UA_Int32_decodeBinary(src, offset, (UA_Int32*)dst)

/** @brief Specifies a reference which belongs to a node. */
typedef struct {
    UA_NodeId referenceTypeId;
    UA_Boolean isInverse;
    UA_ExpandedNodeId targetId;
} UA_ReferenceNode;
#define UA_TYPES_REFERENCENODE 27
#define UA_ReferenceNode_new() (UA_ReferenceNode*)UA_new(&UA_TYPES[UA_TYPES_REFERENCENODE])
#define UA_ReferenceNode_init(p) UA_init(p, &UA_TYPES[UA_TYPES_REFERENCENODE])
#define UA_ReferenceNode_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_REFERENCENODE])
#define UA_ReferenceNode_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_REFERENCENODE])
#define UA_ReferenceNode_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_REFERENCENODE])
#define UA_ReferenceNode_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_REFERENCENODE], dst, offset)
#define UA_ReferenceNode_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_REFERENCENODE])

/** @brief An argument for a method. */
typedef struct {
    UA_String name;
    UA_NodeId dataType;
    UA_Int32 valueRank;
    UA_Int32 arrayDimensionsSize;
    UA_UInt32 *arrayDimensions;
    UA_LocalizedText description;
} UA_Argument;
#define UA_TYPES_ARGUMENT 28
#define UA_Argument_new() (UA_Argument*)UA_new(&UA_TYPES[UA_TYPES_ARGUMENT])
#define UA_Argument_init(p) UA_init(p, &UA_TYPES[UA_TYPES_ARGUMENT])
#define UA_Argument_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_ARGUMENT])
#define UA_Argument_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_ARGUMENT])
#define UA_Argument_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_ARGUMENT])
#define UA_Argument_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_ARGUMENT], dst, offset)
#define UA_Argument_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_ARGUMENT])

/** @brief The types of applications. */
typedef enum { 
    UA_APPLICATIONTYPE_SERVER = 0,
    UA_APPLICATIONTYPE_CLIENT = 1,
    UA_APPLICATIONTYPE_CLIENTANDSERVER = 2,
    UA_APPLICATIONTYPE_DISCOVERYSERVER = 3
} UA_ApplicationType;
#define UA_TYPES_APPLICATIONTYPE 29
#define UA_ApplicationType_new (UA_ApplicationType*)UA_Int32_new
#define UA_ApplicationType_init(p) UA_Int32_init((UA_Int32*)p)
#define UA_ApplicationType_delete(p) UA_Int32_delete((UA_Int32*)p)
#define UA_ApplicationType_deleteMembers(p) UA_Int32_deleteMembers((UA_Int32*)p)
#define UA_ApplicationType_copy(src, dst) UA_Int32_copy((const UA_Int32*)src, (UA_Int32*)dst)
#define UA_ApplicationType_encodeBinary(src, dst, offset) UA_Int32_encodeBinary((UA_Int32*)src, dst, offset)
#define UA_ApplicationType_decodeBinary(src, offset, dst) UA_Int32_decodeBinary(src, offset, (UA_Int32*)dst)

/** @brief Describes an application and how to find it. */
typedef struct {
    UA_String applicationUri;
    UA_String productUri;
    UA_LocalizedText applicationName;
    UA_ApplicationType applicationType;
    UA_String gatewayServerUri;
    UA_String discoveryProfileUri;
    UA_Int32 discoveryUrlsSize;
    UA_String *discoveryUrls;
} UA_ApplicationDescription;
#define UA_TYPES_APPLICATIONDESCRIPTION 30
#define UA_ApplicationDescription_new() (UA_ApplicationDescription*)UA_new(&UA_TYPES[UA_TYPES_APPLICATIONDESCRIPTION])
#define UA_ApplicationDescription_init(p) UA_init(p, &UA_TYPES[UA_TYPES_APPLICATIONDESCRIPTION])
#define UA_ApplicationDescription_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_APPLICATIONDESCRIPTION])
#define UA_ApplicationDescription_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_APPLICATIONDESCRIPTION])
#define UA_ApplicationDescription_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_APPLICATIONDESCRIPTION])
#define UA_ApplicationDescription_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_APPLICATIONDESCRIPTION], dst, offset)
#define UA_ApplicationDescription_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_APPLICATIONDESCRIPTION])

/** @brief The header passed with every server request. */
typedef struct {
    UA_NodeId authenticationToken;
    UA_DateTime timestamp;
    UA_UInt32 requestHandle;
    UA_UInt32 returnDiagnostics;
    UA_String auditEntryId;
    UA_UInt32 timeoutHint;
    UA_ExtensionObject additionalHeader;
} UA_RequestHeader;
#define UA_TYPES_REQUESTHEADER 31
#define UA_RequestHeader_new() (UA_RequestHeader*)UA_new(&UA_TYPES[UA_TYPES_REQUESTHEADER])
#define UA_RequestHeader_init(p) UA_init(p, &UA_TYPES[UA_TYPES_REQUESTHEADER])
#define UA_RequestHeader_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_REQUESTHEADER])
#define UA_RequestHeader_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_REQUESTHEADER])
#define UA_RequestHeader_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_REQUESTHEADER])
#define UA_RequestHeader_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_REQUESTHEADER], dst, offset)
#define UA_RequestHeader_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_REQUESTHEADER])

/** @brief The header passed with every server response. */
typedef struct {
    UA_DateTime timestamp;
    UA_UInt32 requestHandle;
    UA_StatusCode serviceResult;
    UA_DiagnosticInfo serviceDiagnostics;
    UA_Int32 stringTableSize;
    UA_String *stringTable;
    UA_ExtensionObject additionalHeader;
} UA_ResponseHeader;
#define UA_TYPES_RESPONSEHEADER 32
#define UA_ResponseHeader_new() (UA_ResponseHeader*)UA_new(&UA_TYPES[UA_TYPES_RESPONSEHEADER])
#define UA_ResponseHeader_init(p) UA_init(p, &UA_TYPES[UA_TYPES_RESPONSEHEADER])
#define UA_ResponseHeader_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_RESPONSEHEADER])
#define UA_ResponseHeader_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_RESPONSEHEADER])
#define UA_ResponseHeader_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_RESPONSEHEADER])
#define UA_ResponseHeader_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_RESPONSEHEADER], dst, offset)
#define UA_ResponseHeader_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_RESPONSEHEADER])

/** @brief The response returned by all services when there is a service level error. */
typedef struct {
    UA_ResponseHeader responseHeader;
} UA_ServiceFault;
#define UA_TYPES_SERVICEFAULT 33
#define UA_ServiceFault_new() (UA_ServiceFault*)UA_new(&UA_TYPES[UA_TYPES_SERVICEFAULT])
#define UA_ServiceFault_init(p) UA_init(p, &UA_TYPES[UA_TYPES_SERVICEFAULT])
#define UA_ServiceFault_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_SERVICEFAULT])
#define UA_ServiceFault_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_SERVICEFAULT])
#define UA_ServiceFault_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_SERVICEFAULT])
#define UA_ServiceFault_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_SERVICEFAULT], dst, offset)
#define UA_ServiceFault_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_SERVICEFAULT])

/** @brief Finds the servers known to the discovery server. */
typedef struct {
    UA_RequestHeader requestHeader;
    UA_String endpointUrl;
    UA_Int32 localeIdsSize;
    UA_String *localeIds;
    UA_Int32 serverUrisSize;
    UA_String *serverUris;
} UA_FindServersRequest;
#define UA_TYPES_FINDSERVERSREQUEST 34
#define UA_FindServersRequest_new() (UA_FindServersRequest*)UA_new(&UA_TYPES[UA_TYPES_FINDSERVERSREQUEST])
#define UA_FindServersRequest_init(p) UA_init(p, &UA_TYPES[UA_TYPES_FINDSERVERSREQUEST])
#define UA_FindServersRequest_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_FINDSERVERSREQUEST])
#define UA_FindServersRequest_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_FINDSERVERSREQUEST])
#define UA_FindServersRequest_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_FINDSERVERSREQUEST])
#define UA_FindServersRequest_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_FINDSERVERSREQUEST], dst, offset)
#define UA_FindServersRequest_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_FINDSERVERSREQUEST])

/** @brief Finds the servers known to the discovery server. */
typedef struct {
    UA_ResponseHeader responseHeader;
    UA_Int32 serversSize;
    UA_ApplicationDescription *servers;
} UA_FindServersResponse;
#define UA_TYPES_FINDSERVERSRESPONSE 35
#define UA_FindServersResponse_new() (UA_FindServersResponse*)UA_new(&UA_TYPES[UA_TYPES_FINDSERVERSRESPONSE])
#define UA_FindServersResponse_init(p) UA_init(p, &UA_TYPES[UA_TYPES_FINDSERVERSRESPONSE])
#define UA_FindServersResponse_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_FINDSERVERSRESPONSE])
#define UA_FindServersResponse_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_FINDSERVERSRESPONSE])
#define UA_FindServersResponse_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_FINDSERVERSRESPONSE])
#define UA_FindServersResponse_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_FINDSERVERSRESPONSE], dst, offset)
#define UA_FindServersResponse_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_FINDSERVERSRESPONSE])

/** @brief The type of security to use on a message. */
typedef enum { 
    UA_MESSAGESECURITYMODE_INVALID = 0,
    UA_MESSAGESECURITYMODE_NONE = 1,
    UA_MESSAGESECURITYMODE_SIGN = 2,
    UA_MESSAGESECURITYMODE_SIGNANDENCRYPT = 3
} UA_MessageSecurityMode;
#define UA_TYPES_MESSAGESECURITYMODE 36
#define UA_MessageSecurityMode_new (UA_MessageSecurityMode*)UA_Int32_new
#define UA_MessageSecurityMode_init(p) UA_Int32_init((UA_Int32*)p)
#define UA_MessageSecurityMode_delete(p) UA_Int32_delete((UA_Int32*)p)
#define UA_MessageSecurityMode_deleteMembers(p) UA_Int32_deleteMembers((UA_Int32*)p)
#define UA_MessageSecurityMode_copy(src, dst) UA_Int32_copy((const UA_Int32*)src, (UA_Int32*)dst)
#define UA_MessageSecurityMode_encodeBinary(src, dst, offset) UA_Int32_encodeBinary((UA_Int32*)src, dst, offset)
#define UA_MessageSecurityMode_decodeBinary(src, offset, dst) UA_Int32_decodeBinary(src, offset, (UA_Int32*)dst)

/** @brief The possible user token types. */
typedef enum { 
    UA_USERTOKENTYPE_ANONYMOUS = 0,
    UA_USERTOKENTYPE_USERNAME = 1,
    UA_USERTOKENTYPE_CERTIFICATE = 2,
    UA_USERTOKENTYPE_ISSUEDTOKEN = 3
} UA_UserTokenType;
#define UA_TYPES_USERTOKENTYPE 37
#define UA_UserTokenType_new (UA_UserTokenType*)UA_Int32_new
#define UA_UserTokenType_init(p) UA_Int32_init((UA_Int32*)p)
#define UA_UserTokenType_delete(p) UA_Int32_delete((UA_Int32*)p)
#define UA_UserTokenType_deleteMembers(p) UA_Int32_deleteMembers((UA_Int32*)p)
#define UA_UserTokenType_copy(src, dst) UA_Int32_copy((const UA_Int32*)src, (UA_Int32*)dst)
#define UA_UserTokenType_encodeBinary(src, dst, offset) UA_Int32_encodeBinary((UA_Int32*)src, dst, offset)
#define UA_UserTokenType_decodeBinary(src, offset, dst) UA_Int32_decodeBinary(src, offset, (UA_Int32*)dst)

/** @brief Describes a user token that can be used with a server. */
typedef struct {
    UA_String policyId;
    UA_UserTokenType tokenType;
    UA_String issuedTokenType;
    UA_String issuerEndpointUrl;
    UA_String securityPolicyUri;
} UA_UserTokenPolicy;
#define UA_TYPES_USERTOKENPOLICY 38
#define UA_UserTokenPolicy_new() (UA_UserTokenPolicy*)UA_new(&UA_TYPES[UA_TYPES_USERTOKENPOLICY])
#define UA_UserTokenPolicy_init(p) UA_init(p, &UA_TYPES[UA_TYPES_USERTOKENPOLICY])
#define UA_UserTokenPolicy_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_USERTOKENPOLICY])
#define UA_UserTokenPolicy_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_USERTOKENPOLICY])
#define UA_UserTokenPolicy_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_USERTOKENPOLICY])
#define UA_UserTokenPolicy_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_USERTOKENPOLICY], dst, offset)
#define UA_UserTokenPolicy_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_USERTOKENPOLICY])

/** @brief The description of a endpoint that can be used to access a server. */
typedef struct {
    UA_String endpointUrl;
    UA_ApplicationDescription server;
    UA_ByteString serverCertificate;
    UA_MessageSecurityMode securityMode;
    UA_String securityPolicyUri;
    UA_Int32 userIdentityTokensSize;
    UA_UserTokenPolicy *userIdentityTokens;
    UA_String transportProfileUri;
    UA_Byte securityLevel;
} UA_EndpointDescription;
#define UA_TYPES_ENDPOINTDESCRIPTION 39
#define UA_EndpointDescription_new() (UA_EndpointDescription*)UA_new(&UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION])
#define UA_EndpointDescription_init(p) UA_init(p, &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION])
#define UA_EndpointDescription_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION])
#define UA_EndpointDescription_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION])
#define UA_EndpointDescription_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION])
#define UA_EndpointDescription_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION], dst, offset)
#define UA_EndpointDescription_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION])

/** @brief Gets the endpoints used by the server. */
typedef struct {
    UA_RequestHeader requestHeader;
    UA_String endpointUrl;
    UA_Int32 localeIdsSize;
    UA_String *localeIds;
    UA_Int32 profileUrisSize;
    UA_String *profileUris;
} UA_GetEndpointsRequest;
#define UA_TYPES_GETENDPOINTSREQUEST 40
#define UA_GetEndpointsRequest_new() (UA_GetEndpointsRequest*)UA_new(&UA_TYPES[UA_TYPES_GETENDPOINTSREQUEST])
#define UA_GetEndpointsRequest_init(p) UA_init(p, &UA_TYPES[UA_TYPES_GETENDPOINTSREQUEST])
#define UA_GetEndpointsRequest_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_GETENDPOINTSREQUEST])
#define UA_GetEndpointsRequest_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_GETENDPOINTSREQUEST])
#define UA_GetEndpointsRequest_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_GETENDPOINTSREQUEST])
#define UA_GetEndpointsRequest_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_GETENDPOINTSREQUEST], dst, offset)
#define UA_GetEndpointsRequest_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_GETENDPOINTSREQUEST])

/** @brief Gets the endpoints used by the server. */
typedef struct {
    UA_ResponseHeader responseHeader;
    UA_Int32 endpointsSize;
    UA_EndpointDescription *endpoints;
} UA_GetEndpointsResponse;
#define UA_TYPES_GETENDPOINTSRESPONSE 41
#define UA_GetEndpointsResponse_new() (UA_GetEndpointsResponse*)UA_new(&UA_TYPES[UA_TYPES_GETENDPOINTSRESPONSE])
#define UA_GetEndpointsResponse_init(p) UA_init(p, &UA_TYPES[UA_TYPES_GETENDPOINTSRESPONSE])
#define UA_GetEndpointsResponse_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_GETENDPOINTSRESPONSE])
#define UA_GetEndpointsResponse_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_GETENDPOINTSRESPONSE])
#define UA_GetEndpointsResponse_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_GETENDPOINTSRESPONSE])
#define UA_GetEndpointsResponse_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_GETENDPOINTSRESPONSE], dst, offset)
#define UA_GetEndpointsResponse_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_GETENDPOINTSRESPONSE])

/** @brief Indicates whether a token if being created or renewed. */
typedef enum { 
    UA_SECURITYTOKENREQUESTTYPE_ISSUE = 0,
    UA_SECURITYTOKENREQUESTTYPE_RENEW = 1
} UA_SecurityTokenRequestType;
#define UA_TYPES_SECURITYTOKENREQUESTTYPE 42
#define UA_SecurityTokenRequestType_new (UA_SecurityTokenRequestType*)UA_Int32_new
#define UA_SecurityTokenRequestType_init(p) UA_Int32_init((UA_Int32*)p)
#define UA_SecurityTokenRequestType_delete(p) UA_Int32_delete((UA_Int32*)p)
#define UA_SecurityTokenRequestType_deleteMembers(p) UA_Int32_deleteMembers((UA_Int32*)p)
#define UA_SecurityTokenRequestType_copy(src, dst) UA_Int32_copy((const UA_Int32*)src, (UA_Int32*)dst)
#define UA_SecurityTokenRequestType_encodeBinary(src, dst, offset) UA_Int32_encodeBinary((UA_Int32*)src, dst, offset)
#define UA_SecurityTokenRequestType_decodeBinary(src, offset, dst) UA_Int32_decodeBinary(src, offset, (UA_Int32*)dst)

/** @brief The token that identifies a set of keys for an active secure channel. */
typedef struct {
    UA_UInt32 channelId;
    UA_UInt32 tokenId;
    UA_DateTime createdAt;
    UA_UInt32 revisedLifetime;
} UA_ChannelSecurityToken;
#define UA_TYPES_CHANNELSECURITYTOKEN 43
#define UA_ChannelSecurityToken_new() (UA_ChannelSecurityToken*)UA_new(&UA_TYPES[UA_TYPES_CHANNELSECURITYTOKEN])
#define UA_ChannelSecurityToken_init(p) UA_init(p, &UA_TYPES[UA_TYPES_CHANNELSECURITYTOKEN])
#define UA_ChannelSecurityToken_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_CHANNELSECURITYTOKEN])
#define UA_ChannelSecurityToken_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_CHANNELSECURITYTOKEN])
#define UA_ChannelSecurityToken_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_CHANNELSECURITYTOKEN])
#define UA_ChannelSecurityToken_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CHANNELSECURITYTOKEN], dst, offset)
#define UA_ChannelSecurityToken_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CHANNELSECURITYTOKEN])

/** @brief Creates a secure channel with a server. */
typedef struct {
    UA_RequestHeader requestHeader;
    UA_UInt32 clientProtocolVersion;
    UA_SecurityTokenRequestType requestType;
    UA_MessageSecurityMode securityMode;
    UA_ByteString clientNonce;
    UA_UInt32 requestedLifetime;
} UA_OpenSecureChannelRequest;
#define UA_TYPES_OPENSECURECHANNELREQUEST 44
#define UA_OpenSecureChannelRequest_new() (UA_OpenSecureChannelRequest*)UA_new(&UA_TYPES[UA_TYPES_OPENSECURECHANNELREQUEST])
#define UA_OpenSecureChannelRequest_init(p) UA_init(p, &UA_TYPES[UA_TYPES_OPENSECURECHANNELREQUEST])
#define UA_OpenSecureChannelRequest_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_OPENSECURECHANNELREQUEST])
#define UA_OpenSecureChannelRequest_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_OPENSECURECHANNELREQUEST])
#define UA_OpenSecureChannelRequest_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_OPENSECURECHANNELREQUEST])
#define UA_OpenSecureChannelRequest_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_OPENSECURECHANNELREQUEST], dst, offset)
#define UA_OpenSecureChannelRequest_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_OPENSECURECHANNELREQUEST])

/** @brief Creates a secure channel with a server. */
typedef struct {
    UA_ResponseHeader responseHeader;
    UA_UInt32 serverProtocolVersion;
    UA_ChannelSecurityToken securityToken;
    UA_ByteString serverNonce;
} UA_OpenSecureChannelResponse;
#define UA_TYPES_OPENSECURECHANNELRESPONSE 45
#define UA_OpenSecureChannelResponse_new() (UA_OpenSecureChannelResponse*)UA_new(&UA_TYPES[UA_TYPES_OPENSECURECHANNELRESPONSE])
#define UA_OpenSecureChannelResponse_init(p) UA_init(p, &UA_TYPES[UA_TYPES_OPENSECURECHANNELRESPONSE])
#define UA_OpenSecureChannelResponse_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_OPENSECURECHANNELRESPONSE])
#define UA_OpenSecureChannelResponse_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_OPENSECURECHANNELRESPONSE])
#define UA_OpenSecureChannelResponse_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_OPENSECURECHANNELRESPONSE])
#define UA_OpenSecureChannelResponse_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_OPENSECURECHANNELRESPONSE], dst, offset)
#define UA_OpenSecureChannelResponse_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_OPENSECURECHANNELRESPONSE])

/** @brief Closes a secure channel. */
typedef struct {
    UA_RequestHeader requestHeader;
} UA_CloseSecureChannelRequest;
#define UA_TYPES_CLOSESECURECHANNELREQUEST 46
#define UA_CloseSecureChannelRequest_new() (UA_CloseSecureChannelRequest*)UA_new(&UA_TYPES[UA_TYPES_CLOSESECURECHANNELREQUEST])
#define UA_CloseSecureChannelRequest_init(p) UA_init(p, &UA_TYPES[UA_TYPES_CLOSESECURECHANNELREQUEST])
#define UA_CloseSecureChannelRequest_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_CLOSESECURECHANNELREQUEST])
#define UA_CloseSecureChannelRequest_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_CLOSESECURECHANNELREQUEST])
#define UA_CloseSecureChannelRequest_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_CLOSESECURECHANNELREQUEST])
#define UA_CloseSecureChannelRequest_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CLOSESECURECHANNELREQUEST], dst, offset)
#define UA_CloseSecureChannelRequest_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CLOSESECURECHANNELREQUEST])

/** @brief Closes a secure channel. */
typedef struct {
    UA_ResponseHeader responseHeader;
} UA_CloseSecureChannelResponse;
#define UA_TYPES_CLOSESECURECHANNELRESPONSE 47
#define UA_CloseSecureChannelResponse_new() (UA_CloseSecureChannelResponse*)UA_new(&UA_TYPES[UA_TYPES_CLOSESECURECHANNELRESPONSE])
#define UA_CloseSecureChannelResponse_init(p) UA_init(p, &UA_TYPES[UA_TYPES_CLOSESECURECHANNELRESPONSE])
#define UA_CloseSecureChannelResponse_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_CLOSESECURECHANNELRESPONSE])
#define UA_CloseSecureChannelResponse_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_CLOSESECURECHANNELRESPONSE])
#define UA_CloseSecureChannelResponse_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_CLOSESECURECHANNELRESPONSE])
#define UA_CloseSecureChannelResponse_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CLOSESECURECHANNELRESPONSE], dst, offset)
#define UA_CloseSecureChannelResponse_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CLOSESECURECHANNELRESPONSE])

/** @brief A software certificate with a digital signature. */
typedef struct {
    UA_ByteString certificateData;
    UA_ByteString signature;
} UA_SignedSoftwareCertificate;
#define UA_TYPES_SIGNEDSOFTWARECERTIFICATE 48
#define UA_SignedSoftwareCertificate_new() (UA_SignedSoftwareCertificate*)UA_new(&UA_TYPES[UA_TYPES_SIGNEDSOFTWARECERTIFICATE])
#define UA_SignedSoftwareCertificate_init(p) UA_init(p, &UA_TYPES[UA_TYPES_SIGNEDSOFTWARECERTIFICATE])
#define UA_SignedSoftwareCertificate_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_SIGNEDSOFTWARECERTIFICATE])
#define UA_SignedSoftwareCertificate_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_SIGNEDSOFTWARECERTIFICATE])
#define UA_SignedSoftwareCertificate_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_SIGNEDSOFTWARECERTIFICATE])
#define UA_SignedSoftwareCertificate_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_SIGNEDSOFTWARECERTIFICATE], dst, offset)
#define UA_SignedSoftwareCertificate_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_SIGNEDSOFTWARECERTIFICATE])

/** @brief A digital signature. */
typedef struct {
    UA_String algorithm;
    UA_ByteString signature;
} UA_SignatureData;
#define UA_TYPES_SIGNATUREDATA 49
#define UA_SignatureData_new() (UA_SignatureData*)UA_new(&UA_TYPES[UA_TYPES_SIGNATUREDATA])
#define UA_SignatureData_init(p) UA_init(p, &UA_TYPES[UA_TYPES_SIGNATUREDATA])
#define UA_SignatureData_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_SIGNATUREDATA])
#define UA_SignatureData_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_SIGNATUREDATA])
#define UA_SignatureData_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_SIGNATUREDATA])
#define UA_SignatureData_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_SIGNATUREDATA], dst, offset)
#define UA_SignatureData_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_SIGNATUREDATA])

/** @brief Creates a new session with the server. */
typedef struct {
    UA_RequestHeader requestHeader;
    UA_ApplicationDescription clientDescription;
    UA_String serverUri;
    UA_String endpointUrl;
    UA_String sessionName;
    UA_ByteString clientNonce;
    UA_ByteString clientCertificate;
    UA_Double requestedSessionTimeout;
    UA_UInt32 maxResponseMessageSize;
} UA_CreateSessionRequest;
#define UA_TYPES_CREATESESSIONREQUEST 50
#define UA_CreateSessionRequest_new() (UA_CreateSessionRequest*)UA_new(&UA_TYPES[UA_TYPES_CREATESESSIONREQUEST])
#define UA_CreateSessionRequest_init(p) UA_init(p, &UA_TYPES[UA_TYPES_CREATESESSIONREQUEST])
#define UA_CreateSessionRequest_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_CREATESESSIONREQUEST])
#define UA_CreateSessionRequest_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_CREATESESSIONREQUEST])
#define UA_CreateSessionRequest_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_CREATESESSIONREQUEST])
#define UA_CreateSessionRequest_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CREATESESSIONREQUEST], dst, offset)
#define UA_CreateSessionRequest_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CREATESESSIONREQUEST])

/** @brief Creates a new session with the server. */
typedef struct {
    UA_ResponseHeader responseHeader;
    UA_NodeId sessionId;
    UA_NodeId authenticationToken;
    UA_Double revisedSessionTimeout;
    UA_ByteString serverNonce;
    UA_ByteString serverCertificate;
    UA_Int32 serverEndpointsSize;
    UA_EndpointDescription *serverEndpoints;
    UA_Int32 serverSoftwareCertificatesSize;
    UA_SignedSoftwareCertificate *serverSoftwareCertificates;
    UA_SignatureData serverSignature;
    UA_UInt32 maxRequestMessageSize;
} UA_CreateSessionResponse;
#define UA_TYPES_CREATESESSIONRESPONSE 51
#define UA_CreateSessionResponse_new() (UA_CreateSessionResponse*)UA_new(&UA_TYPES[UA_TYPES_CREATESESSIONRESPONSE])
#define UA_CreateSessionResponse_init(p) UA_init(p, &UA_TYPES[UA_TYPES_CREATESESSIONRESPONSE])
#define UA_CreateSessionResponse_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_CREATESESSIONRESPONSE])
#define UA_CreateSessionResponse_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_CREATESESSIONRESPONSE])
#define UA_CreateSessionResponse_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_CREATESESSIONRESPONSE])
#define UA_CreateSessionResponse_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CREATESESSIONRESPONSE], dst, offset)
#define UA_CreateSessionResponse_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CREATESESSIONRESPONSE])

/** @brief A base type for a user identity token. */
typedef struct {
    UA_String policyId;
} UA_UserIdentityToken;
#define UA_TYPES_USERIDENTITYTOKEN 52
#define UA_UserIdentityToken_new() (UA_UserIdentityToken*)UA_new(&UA_TYPES[UA_TYPES_USERIDENTITYTOKEN])
#define UA_UserIdentityToken_init(p) UA_init(p, &UA_TYPES[UA_TYPES_USERIDENTITYTOKEN])
#define UA_UserIdentityToken_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_USERIDENTITYTOKEN])
#define UA_UserIdentityToken_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_USERIDENTITYTOKEN])
#define UA_UserIdentityToken_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_USERIDENTITYTOKEN])
#define UA_UserIdentityToken_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_USERIDENTITYTOKEN], dst, offset)
#define UA_UserIdentityToken_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_USERIDENTITYTOKEN])

/** @brief A token representing an anonymous user. */
typedef struct {
    UA_String policyId;
} UA_AnonymousIdentityToken;
#define UA_TYPES_ANONYMOUSIDENTITYTOKEN 53
#define UA_AnonymousIdentityToken_new() (UA_AnonymousIdentityToken*)UA_new(&UA_TYPES[UA_TYPES_ANONYMOUSIDENTITYTOKEN])
#define UA_AnonymousIdentityToken_init(p) UA_init(p, &UA_TYPES[UA_TYPES_ANONYMOUSIDENTITYTOKEN])
#define UA_AnonymousIdentityToken_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_ANONYMOUSIDENTITYTOKEN])
#define UA_AnonymousIdentityToken_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_ANONYMOUSIDENTITYTOKEN])
#define UA_AnonymousIdentityToken_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_ANONYMOUSIDENTITYTOKEN])
#define UA_AnonymousIdentityToken_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_ANONYMOUSIDENTITYTOKEN], dst, offset)
#define UA_AnonymousIdentityToken_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_ANONYMOUSIDENTITYTOKEN])

/** @brief A token representing a user identified by a user name and password. */
typedef struct {
    UA_String policyId;
    UA_String userName;
    UA_ByteString password;
    UA_String encryptionAlgorithm;
} UA_UserNameIdentityToken;
#define UA_TYPES_USERNAMEIDENTITYTOKEN 54
#define UA_UserNameIdentityToken_new() (UA_UserNameIdentityToken*)UA_new(&UA_TYPES[UA_TYPES_USERNAMEIDENTITYTOKEN])
#define UA_UserNameIdentityToken_init(p) UA_init(p, &UA_TYPES[UA_TYPES_USERNAMEIDENTITYTOKEN])
#define UA_UserNameIdentityToken_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_USERNAMEIDENTITYTOKEN])
#define UA_UserNameIdentityToken_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_USERNAMEIDENTITYTOKEN])
#define UA_UserNameIdentityToken_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_USERNAMEIDENTITYTOKEN])
#define UA_UserNameIdentityToken_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_USERNAMEIDENTITYTOKEN], dst, offset)
#define UA_UserNameIdentityToken_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_USERNAMEIDENTITYTOKEN])

/** @brief Activates a session with the server. */
typedef struct {
    UA_RequestHeader requestHeader;
    UA_SignatureData clientSignature;
    UA_Int32 clientSoftwareCertificatesSize;
    UA_SignedSoftwareCertificate *clientSoftwareCertificates;
    UA_Int32 localeIdsSize;
    UA_String *localeIds;
    UA_ExtensionObject userIdentityToken;
    UA_SignatureData userTokenSignature;
} UA_ActivateSessionRequest;
#define UA_TYPES_ACTIVATESESSIONREQUEST 55
#define UA_ActivateSessionRequest_new() (UA_ActivateSessionRequest*)UA_new(&UA_TYPES[UA_TYPES_ACTIVATESESSIONREQUEST])
#define UA_ActivateSessionRequest_init(p) UA_init(p, &UA_TYPES[UA_TYPES_ACTIVATESESSIONREQUEST])
#define UA_ActivateSessionRequest_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_ACTIVATESESSIONREQUEST])
#define UA_ActivateSessionRequest_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_ACTIVATESESSIONREQUEST])
#define UA_ActivateSessionRequest_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_ACTIVATESESSIONREQUEST])
#define UA_ActivateSessionRequest_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_ACTIVATESESSIONREQUEST], dst, offset)
#define UA_ActivateSessionRequest_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_ACTIVATESESSIONREQUEST])

/** @brief Activates a session with the server. */
typedef struct {
    UA_ResponseHeader responseHeader;
    UA_ByteString serverNonce;
    UA_Int32 resultsSize;
    UA_StatusCode *results;
    UA_Int32 diagnosticInfosSize;
    UA_DiagnosticInfo *diagnosticInfos;
} UA_ActivateSessionResponse;
#define UA_TYPES_ACTIVATESESSIONRESPONSE 56
#define UA_ActivateSessionResponse_new() (UA_ActivateSessionResponse*)UA_new(&UA_TYPES[UA_TYPES_ACTIVATESESSIONRESPONSE])
#define UA_ActivateSessionResponse_init(p) UA_init(p, &UA_TYPES[UA_TYPES_ACTIVATESESSIONRESPONSE])
#define UA_ActivateSessionResponse_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_ACTIVATESESSIONRESPONSE])
#define UA_ActivateSessionResponse_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_ACTIVATESESSIONRESPONSE])
#define UA_ActivateSessionResponse_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_ACTIVATESESSIONRESPONSE])
#define UA_ActivateSessionResponse_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_ACTIVATESESSIONRESPONSE], dst, offset)
#define UA_ActivateSessionResponse_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_ACTIVATESESSIONRESPONSE])

/** @brief Closes a session with the server. */
typedef struct {
    UA_RequestHeader requestHeader;
    UA_Boolean deleteSubscriptions;
} UA_CloseSessionRequest;
#define UA_TYPES_CLOSESESSIONREQUEST 57
#define UA_CloseSessionRequest_new() (UA_CloseSessionRequest*)UA_new(&UA_TYPES[UA_TYPES_CLOSESESSIONREQUEST])
#define UA_CloseSessionRequest_init(p) UA_init(p, &UA_TYPES[UA_TYPES_CLOSESESSIONREQUEST])
#define UA_CloseSessionRequest_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_CLOSESESSIONREQUEST])
#define UA_CloseSessionRequest_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_CLOSESESSIONREQUEST])
#define UA_CloseSessionRequest_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_CLOSESESSIONREQUEST])
#define UA_CloseSessionRequest_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CLOSESESSIONREQUEST], dst, offset)
#define UA_CloseSessionRequest_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CLOSESESSIONREQUEST])

/** @brief Closes a session with the server. */
typedef struct {
    UA_ResponseHeader responseHeader;
} UA_CloseSessionResponse;
#define UA_TYPES_CLOSESESSIONRESPONSE 58
#define UA_CloseSessionResponse_new() (UA_CloseSessionResponse*)UA_new(&UA_TYPES[UA_TYPES_CLOSESESSIONRESPONSE])
#define UA_CloseSessionResponse_init(p) UA_init(p, &UA_TYPES[UA_TYPES_CLOSESESSIONRESPONSE])
#define UA_CloseSessionResponse_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_CLOSESESSIONRESPONSE])
#define UA_CloseSessionResponse_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_CLOSESESSIONRESPONSE])
#define UA_CloseSessionResponse_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_CLOSESESSIONRESPONSE])
#define UA_CloseSessionResponse_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CLOSESESSIONRESPONSE], dst, offset)
#define UA_CloseSessionResponse_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CLOSESESSIONRESPONSE])

/** @brief The bits used to specify default attributes for a new node. */
typedef enum { 
    UA_NODEATTRIBUTESMASK_NONE = 0,
    UA_NODEATTRIBUTESMASK_ACCESSLEVEL = 1,
    UA_NODEATTRIBUTESMASK_ARRAYDIMENSIONS = 2,
    UA_NODEATTRIBUTESMASK_BROWSENAME = 4,
    UA_NODEATTRIBUTESMASK_CONTAINSNOLOOPS = 8,
    UA_NODEATTRIBUTESMASK_DATATYPE = 16,
    UA_NODEATTRIBUTESMASK_DESCRIPTION = 32,
    UA_NODEATTRIBUTESMASK_DISPLAYNAME = 64,
    UA_NODEATTRIBUTESMASK_EVENTNOTIFIER = 128,
    UA_NODEATTRIBUTESMASK_EXECUTABLE = 256,
    UA_NODEATTRIBUTESMASK_HISTORIZING = 512,
    UA_NODEATTRIBUTESMASK_INVERSENAME = 1024,
    UA_NODEATTRIBUTESMASK_ISABSTRACT = 2048,
    UA_NODEATTRIBUTESMASK_MINIMUMSAMPLINGINTERVAL = 4096,
    UA_NODEATTRIBUTESMASK_NODECLASS = 8192,
    UA_NODEATTRIBUTESMASK_NODEID = 16384,
    UA_NODEATTRIBUTESMASK_SYMMETRIC = 32768,
    UA_NODEATTRIBUTESMASK_USERACCESSLEVEL = 65536,
    UA_NODEATTRIBUTESMASK_USEREXECUTABLE = 131072,
    UA_NODEATTRIBUTESMASK_USERWRITEMASK = 262144,
    UA_NODEATTRIBUTESMASK_VALUERANK = 524288,
    UA_NODEATTRIBUTESMASK_WRITEMASK = 1048576,
    UA_NODEATTRIBUTESMASK_VALUE = 2097152,
    UA_NODEATTRIBUTESMASK_ALL = 4194303,
    UA_NODEATTRIBUTESMASK_BASENODE = 1335396,
    UA_NODEATTRIBUTESMASK_OBJECT = 1335524,
    UA_NODEATTRIBUTESMASK_OBJECTTYPEORDATATYPE = 1337444,
    UA_NODEATTRIBUTESMASK_VARIABLE = 4026999,
    UA_NODEATTRIBUTESMASK_VARIABLETYPE = 3958902,
    UA_NODEATTRIBUTESMASK_METHOD = 1466724,
    UA_NODEATTRIBUTESMASK_REFERENCETYPE = 1371236,
    UA_NODEATTRIBUTESMASK_VIEW = 1335532
} UA_NodeAttributesMask;
#define UA_TYPES_NODEATTRIBUTESMASK 59
#define UA_NodeAttributesMask_new (UA_NodeAttributesMask*)UA_Int32_new
#define UA_NodeAttributesMask_init(p) UA_Int32_init((UA_Int32*)p)
#define UA_NodeAttributesMask_delete(p) UA_Int32_delete((UA_Int32*)p)
#define UA_NodeAttributesMask_deleteMembers(p) UA_Int32_deleteMembers((UA_Int32*)p)
#define UA_NodeAttributesMask_copy(src, dst) UA_Int32_copy((const UA_Int32*)src, (UA_Int32*)dst)
#define UA_NodeAttributesMask_encodeBinary(src, dst, offset) UA_Int32_encodeBinary((UA_Int32*)src, dst, offset)
#define UA_NodeAttributesMask_decodeBinary(src, offset, dst) UA_Int32_decodeBinary(src, offset, (UA_Int32*)dst)

/** @brief The base attributes for all nodes. */
typedef struct {
    UA_UInt32 specifiedAttributes;
    UA_LocalizedText displayName;
    UA_LocalizedText description;
    UA_UInt32 writeMask;
    UA_UInt32 userWriteMask;
} UA_NodeAttributes;
#define UA_TYPES_NODEATTRIBUTES 60
#define UA_NodeAttributes_new() (UA_NodeAttributes*)UA_new(&UA_TYPES[UA_TYPES_NODEATTRIBUTES])
#define UA_NodeAttributes_init(p) UA_init(p, &UA_TYPES[UA_TYPES_NODEATTRIBUTES])
#define UA_NodeAttributes_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_NODEATTRIBUTES])
#define UA_NodeAttributes_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_NODEATTRIBUTES])
#define UA_NodeAttributes_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_NODEATTRIBUTES])
#define UA_NodeAttributes_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_NODEATTRIBUTES], dst, offset)
#define UA_NodeAttributes_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_NODEATTRIBUTES])

/** @brief The attributes for an object node. */
typedef struct {
    UA_UInt32 specifiedAttributes;
    UA_LocalizedText displayName;
    UA_LocalizedText description;
    UA_UInt32 writeMask;
    UA_UInt32 userWriteMask;
    UA_Byte eventNotifier;
} UA_ObjectAttributes;
#define UA_TYPES_OBJECTATTRIBUTES 61
#define UA_ObjectAttributes_new() (UA_ObjectAttributes*)UA_new(&UA_TYPES[UA_TYPES_OBJECTATTRIBUTES])
#define UA_ObjectAttributes_init(p) UA_init(p, &UA_TYPES[UA_TYPES_OBJECTATTRIBUTES])
#define UA_ObjectAttributes_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_OBJECTATTRIBUTES])
#define UA_ObjectAttributes_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_OBJECTATTRIBUTES])
#define UA_ObjectAttributes_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_OBJECTATTRIBUTES])
#define UA_ObjectAttributes_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_OBJECTATTRIBUTES], dst, offset)
#define UA_ObjectAttributes_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_OBJECTATTRIBUTES])

/** @brief The attributes for a variable node. */
typedef struct {
    UA_UInt32 specifiedAttributes;
    UA_LocalizedText displayName;
    UA_LocalizedText description;
    UA_UInt32 writeMask;
    UA_UInt32 userWriteMask;
    UA_Variant value;
    UA_NodeId dataType;
    UA_Int32 valueRank;
    UA_Int32 arrayDimensionsSize;
    UA_UInt32 *arrayDimensions;
    UA_Byte accessLevel;
    UA_Byte userAccessLevel;
    UA_Double minimumSamplingInterval;
    UA_Boolean historizing;
} UA_VariableAttributes;
#define UA_TYPES_VARIABLEATTRIBUTES 62
#define UA_VariableAttributes_new() (UA_VariableAttributes*)UA_new(&UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES])
#define UA_VariableAttributes_init(p) UA_init(p, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES])
#define UA_VariableAttributes_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES])
#define UA_VariableAttributes_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES])
#define UA_VariableAttributes_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES])
#define UA_VariableAttributes_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES], dst, offset)
#define UA_VariableAttributes_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES])

/** @brief The attributes for an object type node. */
typedef struct {
    UA_UInt32 specifiedAttributes;
    UA_LocalizedText displayName;
    UA_LocalizedText description;
    UA_UInt32 writeMask;
    UA_UInt32 userWriteMask;
    UA_Boolean isAbstract;
} UA_ObjectTypeAttributes;
#define UA_TYPES_OBJECTTYPEATTRIBUTES 63
#define UA_ObjectTypeAttributes_new() (UA_ObjectTypeAttributes*)UA_new(&UA_TYPES[UA_TYPES_OBJECTTYPEATTRIBUTES])
#define UA_ObjectTypeAttributes_init(p) UA_init(p, &UA_TYPES[UA_TYPES_OBJECTTYPEATTRIBUTES])
#define UA_ObjectTypeAttributes_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_OBJECTTYPEATTRIBUTES])
#define UA_ObjectTypeAttributes_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_OBJECTTYPEATTRIBUTES])
#define UA_ObjectTypeAttributes_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_OBJECTTYPEATTRIBUTES])
#define UA_ObjectTypeAttributes_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_OBJECTTYPEATTRIBUTES], dst, offset)
#define UA_ObjectTypeAttributes_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_OBJECTTYPEATTRIBUTES])

/** @brief The attributes for a reference type node. */
typedef struct {
    UA_UInt32 specifiedAttributes;
    UA_LocalizedText displayName;
    UA_LocalizedText description;
    UA_UInt32 writeMask;
    UA_UInt32 userWriteMask;
    UA_Boolean isAbstract;
    UA_Boolean symmetric;
    UA_LocalizedText inverseName;
} UA_ReferenceTypeAttributes;
#define UA_TYPES_REFERENCETYPEATTRIBUTES 64
#define UA_ReferenceTypeAttributes_new() (UA_ReferenceTypeAttributes*)UA_new(&UA_TYPES[UA_TYPES_REFERENCETYPEATTRIBUTES])
#define UA_ReferenceTypeAttributes_init(p) UA_init(p, &UA_TYPES[UA_TYPES_REFERENCETYPEATTRIBUTES])
#define UA_ReferenceTypeAttributes_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_REFERENCETYPEATTRIBUTES])
#define UA_ReferenceTypeAttributes_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_REFERENCETYPEATTRIBUTES])
#define UA_ReferenceTypeAttributes_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_REFERENCETYPEATTRIBUTES])
#define UA_ReferenceTypeAttributes_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_REFERENCETYPEATTRIBUTES], dst, offset)
#define UA_ReferenceTypeAttributes_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_REFERENCETYPEATTRIBUTES])

/** @brief The attributes for a view node. */
typedef struct {
    UA_UInt32 specifiedAttributes;
    UA_LocalizedText displayName;
    UA_LocalizedText description;
    UA_UInt32 writeMask;
    UA_UInt32 userWriteMask;
    UA_Boolean containsNoLoops;
    UA_Byte eventNotifier;
} UA_ViewAttributes;
#define UA_TYPES_VIEWATTRIBUTES 65
#define UA_ViewAttributes_new() (UA_ViewAttributes*)UA_new(&UA_TYPES[UA_TYPES_VIEWATTRIBUTES])
#define UA_ViewAttributes_init(p) UA_init(p, &UA_TYPES[UA_TYPES_VIEWATTRIBUTES])
#define UA_ViewAttributes_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_VIEWATTRIBUTES])
#define UA_ViewAttributes_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_VIEWATTRIBUTES])
#define UA_ViewAttributes_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_VIEWATTRIBUTES])
#define UA_ViewAttributes_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_VIEWATTRIBUTES], dst, offset)
#define UA_ViewAttributes_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_VIEWATTRIBUTES])

/** @brief A request to add a node to the server address space. */
typedef struct {
    UA_ExpandedNodeId parentNodeId;
    UA_NodeId referenceTypeId;
    UA_ExpandedNodeId requestedNewNodeId;
    UA_QualifiedName browseName;
    UA_NodeClass nodeClass;
    UA_ExtensionObject nodeAttributes;
    UA_ExpandedNodeId typeDefinition;
} UA_AddNodesItem;
#define UA_TYPES_ADDNODESITEM 66
#define UA_AddNodesItem_new() (UA_AddNodesItem*)UA_new(&UA_TYPES[UA_TYPES_ADDNODESITEM])
#define UA_AddNodesItem_init(p) UA_init(p, &UA_TYPES[UA_TYPES_ADDNODESITEM])
#define UA_AddNodesItem_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_ADDNODESITEM])
#define UA_AddNodesItem_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_ADDNODESITEM])
#define UA_AddNodesItem_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_ADDNODESITEM])
#define UA_AddNodesItem_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_ADDNODESITEM], dst, offset)
#define UA_AddNodesItem_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_ADDNODESITEM])

/** @brief A result of an add node operation. */
typedef struct {
    UA_StatusCode statusCode;
    UA_NodeId addedNodeId;
} UA_AddNodesResult;
#define UA_TYPES_ADDNODESRESULT 67
#define UA_AddNodesResult_new() (UA_AddNodesResult*)UA_new(&UA_TYPES[UA_TYPES_ADDNODESRESULT])
#define UA_AddNodesResult_init(p) UA_init(p, &UA_TYPES[UA_TYPES_ADDNODESRESULT])
#define UA_AddNodesResult_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_ADDNODESRESULT])
#define UA_AddNodesResult_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_ADDNODESRESULT])
#define UA_AddNodesResult_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_ADDNODESRESULT])
#define UA_AddNodesResult_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_ADDNODESRESULT], dst, offset)
#define UA_AddNodesResult_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_ADDNODESRESULT])

/** @brief Adds one or more nodes to the server address space. */
typedef struct {
    UA_RequestHeader requestHeader;
    UA_Int32 nodesToAddSize;
    UA_AddNodesItem *nodesToAdd;
} UA_AddNodesRequest;
#define UA_TYPES_ADDNODESREQUEST 68
#define UA_AddNodesRequest_new() (UA_AddNodesRequest*)UA_new(&UA_TYPES[UA_TYPES_ADDNODESREQUEST])
#define UA_AddNodesRequest_init(p) UA_init(p, &UA_TYPES[UA_TYPES_ADDNODESREQUEST])
#define UA_AddNodesRequest_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_ADDNODESREQUEST])
#define UA_AddNodesRequest_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_ADDNODESREQUEST])
#define UA_AddNodesRequest_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_ADDNODESREQUEST])
#define UA_AddNodesRequest_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_ADDNODESREQUEST], dst, offset)
#define UA_AddNodesRequest_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_ADDNODESREQUEST])

/** @brief Adds one or more nodes to the server address space. */
typedef struct {
    UA_ResponseHeader responseHeader;
    UA_Int32 resultsSize;
    UA_AddNodesResult *results;
    UA_Int32 diagnosticInfosSize;
    UA_DiagnosticInfo *diagnosticInfos;
} UA_AddNodesResponse;
#define UA_TYPES_ADDNODESRESPONSE 69
#define UA_AddNodesResponse_new() (UA_AddNodesResponse*)UA_new(&UA_TYPES[UA_TYPES_ADDNODESRESPONSE])
#define UA_AddNodesResponse_init(p) UA_init(p, &UA_TYPES[UA_TYPES_ADDNODESRESPONSE])
#define UA_AddNodesResponse_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_ADDNODESRESPONSE])
#define UA_AddNodesResponse_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_ADDNODESRESPONSE])
#define UA_AddNodesResponse_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_ADDNODESRESPONSE])
#define UA_AddNodesResponse_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_ADDNODESRESPONSE], dst, offset)
#define UA_AddNodesResponse_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_ADDNODESRESPONSE])

/** @brief A request to add a reference to the server address space. */
typedef struct {
    UA_NodeId sourceNodeId;
    UA_NodeId referenceTypeId;
    UA_Boolean isForward;
    UA_String targetServerUri;
    UA_ExpandedNodeId targetNodeId;
    UA_NodeClass targetNodeClass;
} UA_AddReferencesItem;
#define UA_TYPES_ADDREFERENCESITEM 70
#define UA_AddReferencesItem_new() (UA_AddReferencesItem*)UA_new(&UA_TYPES[UA_TYPES_ADDREFERENCESITEM])
#define UA_AddReferencesItem_init(p) UA_init(p, &UA_TYPES[UA_TYPES_ADDREFERENCESITEM])
#define UA_AddReferencesItem_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_ADDREFERENCESITEM])
#define UA_AddReferencesItem_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_ADDREFERENCESITEM])
#define UA_AddReferencesItem_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_ADDREFERENCESITEM])
#define UA_AddReferencesItem_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_ADDREFERENCESITEM], dst, offset)
#define UA_AddReferencesItem_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_ADDREFERENCESITEM])

/** @brief Adds one or more references to the server address space. */
typedef struct {
    UA_RequestHeader requestHeader;
    UA_Int32 referencesToAddSize;
    UA_AddReferencesItem *referencesToAdd;
} UA_AddReferencesRequest;
#define UA_TYPES_ADDREFERENCESREQUEST 71
#define UA_AddReferencesRequest_new() (UA_AddReferencesRequest*)UA_new(&UA_TYPES[UA_TYPES_ADDREFERENCESREQUEST])
#define UA_AddReferencesRequest_init(p) UA_init(p, &UA_TYPES[UA_TYPES_ADDREFERENCESREQUEST])
#define UA_AddReferencesRequest_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_ADDREFERENCESREQUEST])
#define UA_AddReferencesRequest_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_ADDREFERENCESREQUEST])
#define UA_AddReferencesRequest_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_ADDREFERENCESREQUEST])
#define UA_AddReferencesRequest_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_ADDREFERENCESREQUEST], dst, offset)
#define UA_AddReferencesRequest_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_ADDREFERENCESREQUEST])

/** @brief Adds one or more references to the server address space. */
typedef struct {
    UA_ResponseHeader responseHeader;
    UA_Int32 resultsSize;
    UA_StatusCode *results;
    UA_Int32 diagnosticInfosSize;
    UA_DiagnosticInfo *diagnosticInfos;
} UA_AddReferencesResponse;
#define UA_TYPES_ADDREFERENCESRESPONSE 72
#define UA_AddReferencesResponse_new() (UA_AddReferencesResponse*)UA_new(&UA_TYPES[UA_TYPES_ADDREFERENCESRESPONSE])
#define UA_AddReferencesResponse_init(p) UA_init(p, &UA_TYPES[UA_TYPES_ADDREFERENCESRESPONSE])
#define UA_AddReferencesResponse_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_ADDREFERENCESRESPONSE])
#define UA_AddReferencesResponse_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_ADDREFERENCESRESPONSE])
#define UA_AddReferencesResponse_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_ADDREFERENCESRESPONSE])
#define UA_AddReferencesResponse_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_ADDREFERENCESRESPONSE], dst, offset)
#define UA_AddReferencesResponse_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_ADDREFERENCESRESPONSE])

/** @brief A request to delete a node to the server address space. */
typedef struct {
    UA_NodeId nodeId;
    UA_Boolean deleteTargetReferences;
} UA_DeleteNodesItem;
#define UA_TYPES_DELETENODESITEM 73
#define UA_DeleteNodesItem_new() (UA_DeleteNodesItem*)UA_new(&UA_TYPES[UA_TYPES_DELETENODESITEM])
#define UA_DeleteNodesItem_init(p) UA_init(p, &UA_TYPES[UA_TYPES_DELETENODESITEM])
#define UA_DeleteNodesItem_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_DELETENODESITEM])
#define UA_DeleteNodesItem_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_DELETENODESITEM])
#define UA_DeleteNodesItem_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_DELETENODESITEM])
#define UA_DeleteNodesItem_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_DELETENODESITEM], dst, offset)
#define UA_DeleteNodesItem_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_DELETENODESITEM])

/** @brief Delete one or more nodes from the server address space. */
typedef struct {
    UA_RequestHeader requestHeader;
    UA_Int32 nodesToDeleteSize;
    UA_DeleteNodesItem *nodesToDelete;
} UA_DeleteNodesRequest;
#define UA_TYPES_DELETENODESREQUEST 74
#define UA_DeleteNodesRequest_new() (UA_DeleteNodesRequest*)UA_new(&UA_TYPES[UA_TYPES_DELETENODESREQUEST])
#define UA_DeleteNodesRequest_init(p) UA_init(p, &UA_TYPES[UA_TYPES_DELETENODESREQUEST])
#define UA_DeleteNodesRequest_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_DELETENODESREQUEST])
#define UA_DeleteNodesRequest_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_DELETENODESREQUEST])
#define UA_DeleteNodesRequest_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_DELETENODESREQUEST])
#define UA_DeleteNodesRequest_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_DELETENODESREQUEST], dst, offset)
#define UA_DeleteNodesRequest_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_DELETENODESREQUEST])

/** @brief Delete one or more nodes from the server address space. */
typedef struct {
    UA_ResponseHeader responseHeader;
    UA_Int32 resultsSize;
    UA_StatusCode *results;
    UA_Int32 diagnosticInfosSize;
    UA_DiagnosticInfo *diagnosticInfos;
} UA_DeleteNodesResponse;
#define UA_TYPES_DELETENODESRESPONSE 75
#define UA_DeleteNodesResponse_new() (UA_DeleteNodesResponse*)UA_new(&UA_TYPES[UA_TYPES_DELETENODESRESPONSE])
#define UA_DeleteNodesResponse_init(p) UA_init(p, &UA_TYPES[UA_TYPES_DELETENODESRESPONSE])
#define UA_DeleteNodesResponse_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_DELETENODESRESPONSE])
#define UA_DeleteNodesResponse_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_DELETENODESRESPONSE])
#define UA_DeleteNodesResponse_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_DELETENODESRESPONSE])
#define UA_DeleteNodesResponse_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_DELETENODESRESPONSE], dst, offset)
#define UA_DeleteNodesResponse_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_DELETENODESRESPONSE])

/** @brief A request to delete a node from the server address space. */
typedef struct {
    UA_NodeId sourceNodeId;
    UA_NodeId referenceTypeId;
    UA_Boolean isForward;
    UA_ExpandedNodeId targetNodeId;
    UA_Boolean deleteBidirectional;
} UA_DeleteReferencesItem;
#define UA_TYPES_DELETEREFERENCESITEM 76
#define UA_DeleteReferencesItem_new() (UA_DeleteReferencesItem*)UA_new(&UA_TYPES[UA_TYPES_DELETEREFERENCESITEM])
#define UA_DeleteReferencesItem_init(p) UA_init(p, &UA_TYPES[UA_TYPES_DELETEREFERENCESITEM])
#define UA_DeleteReferencesItem_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_DELETEREFERENCESITEM])
#define UA_DeleteReferencesItem_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_DELETEREFERENCESITEM])
#define UA_DeleteReferencesItem_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_DELETEREFERENCESITEM])
#define UA_DeleteReferencesItem_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_DELETEREFERENCESITEM], dst, offset)
#define UA_DeleteReferencesItem_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_DELETEREFERENCESITEM])

/** @brief Delete one or more references from the server address space. */
typedef struct {
    UA_RequestHeader requestHeader;
    UA_Int32 referencesToDeleteSize;
    UA_DeleteReferencesItem *referencesToDelete;
} UA_DeleteReferencesRequest;
#define UA_TYPES_DELETEREFERENCESREQUEST 77
#define UA_DeleteReferencesRequest_new() (UA_DeleteReferencesRequest*)UA_new(&UA_TYPES[UA_TYPES_DELETEREFERENCESREQUEST])
#define UA_DeleteReferencesRequest_init(p) UA_init(p, &UA_TYPES[UA_TYPES_DELETEREFERENCESREQUEST])
#define UA_DeleteReferencesRequest_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_DELETEREFERENCESREQUEST])
#define UA_DeleteReferencesRequest_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_DELETEREFERENCESREQUEST])
#define UA_DeleteReferencesRequest_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_DELETEREFERENCESREQUEST])
#define UA_DeleteReferencesRequest_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_DELETEREFERENCESREQUEST], dst, offset)
#define UA_DeleteReferencesRequest_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_DELETEREFERENCESREQUEST])

/** @brief Delete one or more references from the server address space. */
typedef struct {
    UA_ResponseHeader responseHeader;
    UA_Int32 resultsSize;
    UA_StatusCode *results;
    UA_Int32 diagnosticInfosSize;
    UA_DiagnosticInfo *diagnosticInfos;
} UA_DeleteReferencesResponse;
#define UA_TYPES_DELETEREFERENCESRESPONSE 78
#define UA_DeleteReferencesResponse_new() (UA_DeleteReferencesResponse*)UA_new(&UA_TYPES[UA_TYPES_DELETEREFERENCESRESPONSE])
#define UA_DeleteReferencesResponse_init(p) UA_init(p, &UA_TYPES[UA_TYPES_DELETEREFERENCESRESPONSE])
#define UA_DeleteReferencesResponse_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_DELETEREFERENCESRESPONSE])
#define UA_DeleteReferencesResponse_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_DELETEREFERENCESRESPONSE])
#define UA_DeleteReferencesResponse_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_DELETEREFERENCESRESPONSE])
#define UA_DeleteReferencesResponse_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_DELETEREFERENCESRESPONSE], dst, offset)
#define UA_DeleteReferencesResponse_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_DELETEREFERENCESRESPONSE])

/** @brief The directions of the references to return. */
typedef enum { 
    UA_BROWSEDIRECTION_FORWARD = 0,
    UA_BROWSEDIRECTION_INVERSE = 1,
    UA_BROWSEDIRECTION_BOTH = 2
} UA_BrowseDirection;
#define UA_TYPES_BROWSEDIRECTION 79
#define UA_BrowseDirection_new (UA_BrowseDirection*)UA_Int32_new
#define UA_BrowseDirection_init(p) UA_Int32_init((UA_Int32*)p)
#define UA_BrowseDirection_delete(p) UA_Int32_delete((UA_Int32*)p)
#define UA_BrowseDirection_deleteMembers(p) UA_Int32_deleteMembers((UA_Int32*)p)
#define UA_BrowseDirection_copy(src, dst) UA_Int32_copy((const UA_Int32*)src, (UA_Int32*)dst)
#define UA_BrowseDirection_encodeBinary(src, dst, offset) UA_Int32_encodeBinary((UA_Int32*)src, dst, offset)
#define UA_BrowseDirection_decodeBinary(src, offset, dst) UA_Int32_decodeBinary(src, offset, (UA_Int32*)dst)

/** @brief The view to browse. */
typedef struct {
    UA_NodeId viewId;
    UA_DateTime timestamp;
    UA_UInt32 viewVersion;
} UA_ViewDescription;
#define UA_TYPES_VIEWDESCRIPTION 80
#define UA_ViewDescription_new() (UA_ViewDescription*)UA_new(&UA_TYPES[UA_TYPES_VIEWDESCRIPTION])
#define UA_ViewDescription_init(p) UA_init(p, &UA_TYPES[UA_TYPES_VIEWDESCRIPTION])
#define UA_ViewDescription_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_VIEWDESCRIPTION])
#define UA_ViewDescription_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_VIEWDESCRIPTION])
#define UA_ViewDescription_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_VIEWDESCRIPTION])
#define UA_ViewDescription_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_VIEWDESCRIPTION], dst, offset)
#define UA_ViewDescription_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_VIEWDESCRIPTION])

/** @brief A request to browse the the references from a node. */
typedef struct {
    UA_NodeId nodeId;
    UA_BrowseDirection browseDirection;
    UA_NodeId referenceTypeId;
    UA_Boolean includeSubtypes;
    UA_UInt32 nodeClassMask;
    UA_UInt32 resultMask;
} UA_BrowseDescription;
#define UA_TYPES_BROWSEDESCRIPTION 81
#define UA_BrowseDescription_new() (UA_BrowseDescription*)UA_new(&UA_TYPES[UA_TYPES_BROWSEDESCRIPTION])
#define UA_BrowseDescription_init(p) UA_init(p, &UA_TYPES[UA_TYPES_BROWSEDESCRIPTION])
#define UA_BrowseDescription_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_BROWSEDESCRIPTION])
#define UA_BrowseDescription_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_BROWSEDESCRIPTION])
#define UA_BrowseDescription_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_BROWSEDESCRIPTION])
#define UA_BrowseDescription_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_BROWSEDESCRIPTION], dst, offset)
#define UA_BrowseDescription_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_BROWSEDESCRIPTION])

/** @brief A bit mask which specifies what should be returned in a browse response. */
typedef enum { 
    UA_BROWSERESULTMASK_NONE = 0,
    UA_BROWSERESULTMASK_REFERENCETYPEID = 1,
    UA_BROWSERESULTMASK_ISFORWARD = 2,
    UA_BROWSERESULTMASK_NODECLASS = 4,
    UA_BROWSERESULTMASK_BROWSENAME = 8,
    UA_BROWSERESULTMASK_DISPLAYNAME = 16,
    UA_BROWSERESULTMASK_TYPEDEFINITION = 32,
    UA_BROWSERESULTMASK_ALL = 63,
    UA_BROWSERESULTMASK_REFERENCETYPEINFO = 3,
    UA_BROWSERESULTMASK_TARGETINFO = 60
} UA_BrowseResultMask;
#define UA_TYPES_BROWSERESULTMASK 82
#define UA_BrowseResultMask_new (UA_BrowseResultMask*)UA_Int32_new
#define UA_BrowseResultMask_init(p) UA_Int32_init((UA_Int32*)p)
#define UA_BrowseResultMask_delete(p) UA_Int32_delete((UA_Int32*)p)
#define UA_BrowseResultMask_deleteMembers(p) UA_Int32_deleteMembers((UA_Int32*)p)
#define UA_BrowseResultMask_copy(src, dst) UA_Int32_copy((const UA_Int32*)src, (UA_Int32*)dst)
#define UA_BrowseResultMask_encodeBinary(src, dst, offset) UA_Int32_encodeBinary((UA_Int32*)src, dst, offset)
#define UA_BrowseResultMask_decodeBinary(src, offset, dst) UA_Int32_decodeBinary(src, offset, (UA_Int32*)dst)

/** @brief The description of a reference. */
typedef struct {
    UA_NodeId referenceTypeId;
    UA_Boolean isForward;
    UA_ExpandedNodeId nodeId;
    UA_QualifiedName browseName;
    UA_LocalizedText displayName;
    UA_NodeClass nodeClass;
    UA_ExpandedNodeId typeDefinition;
} UA_ReferenceDescription;
#define UA_TYPES_REFERENCEDESCRIPTION 83
#define UA_ReferenceDescription_new() (UA_ReferenceDescription*)UA_new(&UA_TYPES[UA_TYPES_REFERENCEDESCRIPTION])
#define UA_ReferenceDescription_init(p) UA_init(p, &UA_TYPES[UA_TYPES_REFERENCEDESCRIPTION])
#define UA_ReferenceDescription_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_REFERENCEDESCRIPTION])
#define UA_ReferenceDescription_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_REFERENCEDESCRIPTION])
#define UA_ReferenceDescription_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_REFERENCEDESCRIPTION])
#define UA_ReferenceDescription_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_REFERENCEDESCRIPTION], dst, offset)
#define UA_ReferenceDescription_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_REFERENCEDESCRIPTION])

/** @brief The result of a browse operation. */
typedef struct {
    UA_StatusCode statusCode;
    UA_ByteString continuationPoint;
    UA_Int32 referencesSize;
    UA_ReferenceDescription *references;
} UA_BrowseResult;
#define UA_TYPES_BROWSERESULT 84
#define UA_BrowseResult_new() (UA_BrowseResult*)UA_new(&UA_TYPES[UA_TYPES_BROWSERESULT])
#define UA_BrowseResult_init(p) UA_init(p, &UA_TYPES[UA_TYPES_BROWSERESULT])
#define UA_BrowseResult_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_BROWSERESULT])
#define UA_BrowseResult_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_BROWSERESULT])
#define UA_BrowseResult_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_BROWSERESULT])
#define UA_BrowseResult_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_BROWSERESULT], dst, offset)
#define UA_BrowseResult_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_BROWSERESULT])

/** @brief Browse the references for one or more nodes from the server address space. */
typedef struct {
    UA_RequestHeader requestHeader;
    UA_ViewDescription view;
    UA_UInt32 requestedMaxReferencesPerNode;
    UA_Int32 nodesToBrowseSize;
    UA_BrowseDescription *nodesToBrowse;
} UA_BrowseRequest;
#define UA_TYPES_BROWSEREQUEST 85
#define UA_BrowseRequest_new() (UA_BrowseRequest*)UA_new(&UA_TYPES[UA_TYPES_BROWSEREQUEST])
#define UA_BrowseRequest_init(p) UA_init(p, &UA_TYPES[UA_TYPES_BROWSEREQUEST])
#define UA_BrowseRequest_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_BROWSEREQUEST])
#define UA_BrowseRequest_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_BROWSEREQUEST])
#define UA_BrowseRequest_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_BROWSEREQUEST])
#define UA_BrowseRequest_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_BROWSEREQUEST], dst, offset)
#define UA_BrowseRequest_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_BROWSEREQUEST])

/** @brief Browse the references for one or more nodes from the server address space. */
typedef struct {
    UA_ResponseHeader responseHeader;
    UA_Int32 resultsSize;
    UA_BrowseResult *results;
    UA_Int32 diagnosticInfosSize;
    UA_DiagnosticInfo *diagnosticInfos;
} UA_BrowseResponse;
#define UA_TYPES_BROWSERESPONSE 86
#define UA_BrowseResponse_new() (UA_BrowseResponse*)UA_new(&UA_TYPES[UA_TYPES_BROWSERESPONSE])
#define UA_BrowseResponse_init(p) UA_init(p, &UA_TYPES[UA_TYPES_BROWSERESPONSE])
#define UA_BrowseResponse_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_BROWSERESPONSE])
#define UA_BrowseResponse_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_BROWSERESPONSE])
#define UA_BrowseResponse_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_BROWSERESPONSE])
#define UA_BrowseResponse_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_BROWSERESPONSE], dst, offset)
#define UA_BrowseResponse_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_BROWSERESPONSE])

/** @brief Continues one or more browse operations. */
typedef struct {
    UA_RequestHeader requestHeader;
    UA_Boolean releaseContinuationPoints;
    UA_Int32 continuationPointsSize;
    UA_ByteString *continuationPoints;
} UA_BrowseNextRequest;
#define UA_TYPES_BROWSENEXTREQUEST 87
#define UA_BrowseNextRequest_new() (UA_BrowseNextRequest*)UA_new(&UA_TYPES[UA_TYPES_BROWSENEXTREQUEST])
#define UA_BrowseNextRequest_init(p) UA_init(p, &UA_TYPES[UA_TYPES_BROWSENEXTREQUEST])
#define UA_BrowseNextRequest_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_BROWSENEXTREQUEST])
#define UA_BrowseNextRequest_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_BROWSENEXTREQUEST])
#define UA_BrowseNextRequest_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_BROWSENEXTREQUEST])
#define UA_BrowseNextRequest_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_BROWSENEXTREQUEST], dst, offset)
#define UA_BrowseNextRequest_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_BROWSENEXTREQUEST])

/** @brief Continues one or more browse operations. */
typedef struct {
    UA_ResponseHeader responseHeader;
    UA_Int32 resultsSize;
    UA_BrowseResult *results;
    UA_Int32 diagnosticInfosSize;
    UA_DiagnosticInfo *diagnosticInfos;
} UA_BrowseNextResponse;
#define UA_TYPES_BROWSENEXTRESPONSE 88
#define UA_BrowseNextResponse_new() (UA_BrowseNextResponse*)UA_new(&UA_TYPES[UA_TYPES_BROWSENEXTRESPONSE])
#define UA_BrowseNextResponse_init(p) UA_init(p, &UA_TYPES[UA_TYPES_BROWSENEXTRESPONSE])
#define UA_BrowseNextResponse_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_BROWSENEXTRESPONSE])
#define UA_BrowseNextResponse_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_BROWSENEXTRESPONSE])
#define UA_BrowseNextResponse_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_BROWSENEXTRESPONSE])
#define UA_BrowseNextResponse_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_BROWSENEXTRESPONSE], dst, offset)
#define UA_BrowseNextResponse_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_BROWSENEXTRESPONSE])

/** @brief An element in a relative path. */
typedef struct {
    UA_NodeId referenceTypeId;
    UA_Boolean isInverse;
    UA_Boolean includeSubtypes;
    UA_QualifiedName targetName;
} UA_RelativePathElement;
#define UA_TYPES_RELATIVEPATHELEMENT 89
#define UA_RelativePathElement_new() (UA_RelativePathElement*)UA_new(&UA_TYPES[UA_TYPES_RELATIVEPATHELEMENT])
#define UA_RelativePathElement_init(p) UA_init(p, &UA_TYPES[UA_TYPES_RELATIVEPATHELEMENT])
#define UA_RelativePathElement_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_RELATIVEPATHELEMENT])
#define UA_RelativePathElement_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_RELATIVEPATHELEMENT])
#define UA_RelativePathElement_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_RELATIVEPATHELEMENT])
#define UA_RelativePathElement_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_RELATIVEPATHELEMENT], dst, offset)
#define UA_RelativePathElement_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_RELATIVEPATHELEMENT])

/** @brief A relative path constructed from reference types and browse names. */
typedef struct {
    UA_Int32 elementsSize;
    UA_RelativePathElement *elements;
} UA_RelativePath;
#define UA_TYPES_RELATIVEPATH 90
#define UA_RelativePath_new() (UA_RelativePath*)UA_new(&UA_TYPES[UA_TYPES_RELATIVEPATH])
#define UA_RelativePath_init(p) UA_init(p, &UA_TYPES[UA_TYPES_RELATIVEPATH])
#define UA_RelativePath_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_RELATIVEPATH])
#define UA_RelativePath_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_RELATIVEPATH])
#define UA_RelativePath_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_RELATIVEPATH])
#define UA_RelativePath_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_RELATIVEPATH], dst, offset)
#define UA_RelativePath_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_RELATIVEPATH])

/** @brief A request to translate a path into a node id. */
typedef struct {
    UA_NodeId startingNode;
    UA_RelativePath relativePath;
} UA_BrowsePath;
#define UA_TYPES_BROWSEPATH 91
#define UA_BrowsePath_new() (UA_BrowsePath*)UA_new(&UA_TYPES[UA_TYPES_BROWSEPATH])
#define UA_BrowsePath_init(p) UA_init(p, &UA_TYPES[UA_TYPES_BROWSEPATH])
#define UA_BrowsePath_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_BROWSEPATH])
#define UA_BrowsePath_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_BROWSEPATH])
#define UA_BrowsePath_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_BROWSEPATH])
#define UA_BrowsePath_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_BROWSEPATH], dst, offset)
#define UA_BrowsePath_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_BROWSEPATH])

/** @brief The target of the translated path. */
typedef struct {
    UA_ExpandedNodeId targetId;
    UA_UInt32 remainingPathIndex;
} UA_BrowsePathTarget;
#define UA_TYPES_BROWSEPATHTARGET 92
#define UA_BrowsePathTarget_new() (UA_BrowsePathTarget*)UA_new(&UA_TYPES[UA_TYPES_BROWSEPATHTARGET])
#define UA_BrowsePathTarget_init(p) UA_init(p, &UA_TYPES[UA_TYPES_BROWSEPATHTARGET])
#define UA_BrowsePathTarget_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_BROWSEPATHTARGET])
#define UA_BrowsePathTarget_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_BROWSEPATHTARGET])
#define UA_BrowsePathTarget_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_BROWSEPATHTARGET])
#define UA_BrowsePathTarget_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_BROWSEPATHTARGET], dst, offset)
#define UA_BrowsePathTarget_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_BROWSEPATHTARGET])

/** @brief The result of a translate opearation. */
typedef struct {
    UA_StatusCode statusCode;
    UA_Int32 targetsSize;
    UA_BrowsePathTarget *targets;
} UA_BrowsePathResult;
#define UA_TYPES_BROWSEPATHRESULT 93
#define UA_BrowsePathResult_new() (UA_BrowsePathResult*)UA_new(&UA_TYPES[UA_TYPES_BROWSEPATHRESULT])
#define UA_BrowsePathResult_init(p) UA_init(p, &UA_TYPES[UA_TYPES_BROWSEPATHRESULT])
#define UA_BrowsePathResult_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_BROWSEPATHRESULT])
#define UA_BrowsePathResult_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_BROWSEPATHRESULT])
#define UA_BrowsePathResult_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_BROWSEPATHRESULT])
#define UA_BrowsePathResult_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_BROWSEPATHRESULT], dst, offset)
#define UA_BrowsePathResult_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_BROWSEPATHRESULT])

/** @brief Translates one or more paths in the server address space. */
typedef struct {
    UA_RequestHeader requestHeader;
    UA_Int32 browsePathsSize;
    UA_BrowsePath *browsePaths;
} UA_TranslateBrowsePathsToNodeIdsRequest;
#define UA_TYPES_TRANSLATEBROWSEPATHSTONODEIDSREQUEST 94
#define UA_TranslateBrowsePathsToNodeIdsRequest_new() (UA_TranslateBrowsePathsToNodeIdsRequest*)UA_new(&UA_TYPES[UA_TYPES_TRANSLATEBROWSEPATHSTONODEIDSREQUEST])
#define UA_TranslateBrowsePathsToNodeIdsRequest_init(p) UA_init(p, &UA_TYPES[UA_TYPES_TRANSLATEBROWSEPATHSTONODEIDSREQUEST])
#define UA_TranslateBrowsePathsToNodeIdsRequest_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_TRANSLATEBROWSEPATHSTONODEIDSREQUEST])
#define UA_TranslateBrowsePathsToNodeIdsRequest_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_TRANSLATEBROWSEPATHSTONODEIDSREQUEST])
#define UA_TranslateBrowsePathsToNodeIdsRequest_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_TRANSLATEBROWSEPATHSTONODEIDSREQUEST])
#define UA_TranslateBrowsePathsToNodeIdsRequest_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_TRANSLATEBROWSEPATHSTONODEIDSREQUEST], dst, offset)
#define UA_TranslateBrowsePathsToNodeIdsRequest_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_TRANSLATEBROWSEPATHSTONODEIDSREQUEST])

/** @brief Translates one or more paths in the server address space. */
typedef struct {
    UA_ResponseHeader responseHeader;
    UA_Int32 resultsSize;
    UA_BrowsePathResult *results;
    UA_Int32 diagnosticInfosSize;
    UA_DiagnosticInfo *diagnosticInfos;
} UA_TranslateBrowsePathsToNodeIdsResponse;
#define UA_TYPES_TRANSLATEBROWSEPATHSTONODEIDSRESPONSE 95
#define UA_TranslateBrowsePathsToNodeIdsResponse_new() (UA_TranslateBrowsePathsToNodeIdsResponse*)UA_new(&UA_TYPES[UA_TYPES_TRANSLATEBROWSEPATHSTONODEIDSRESPONSE])
#define UA_TranslateBrowsePathsToNodeIdsResponse_init(p) UA_init(p, &UA_TYPES[UA_TYPES_TRANSLATEBROWSEPATHSTONODEIDSRESPONSE])
#define UA_TranslateBrowsePathsToNodeIdsResponse_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_TRANSLATEBROWSEPATHSTONODEIDSRESPONSE])
#define UA_TranslateBrowsePathsToNodeIdsResponse_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_TRANSLATEBROWSEPATHSTONODEIDSRESPONSE])
#define UA_TranslateBrowsePathsToNodeIdsResponse_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_TRANSLATEBROWSEPATHSTONODEIDSRESPONSE])
#define UA_TranslateBrowsePathsToNodeIdsResponse_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_TRANSLATEBROWSEPATHSTONODEIDSRESPONSE], dst, offset)
#define UA_TranslateBrowsePathsToNodeIdsResponse_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_TRANSLATEBROWSEPATHSTONODEIDSRESPONSE])

/** @brief Registers one or more nodes for repeated use within a session. */
typedef struct {
    UA_RequestHeader requestHeader;
    UA_Int32 nodesToRegisterSize;
    UA_NodeId *nodesToRegister;
} UA_RegisterNodesRequest;
#define UA_TYPES_REGISTERNODESREQUEST 96
#define UA_RegisterNodesRequest_new() (UA_RegisterNodesRequest*)UA_new(&UA_TYPES[UA_TYPES_REGISTERNODESREQUEST])
#define UA_RegisterNodesRequest_init(p) UA_init(p, &UA_TYPES[UA_TYPES_REGISTERNODESREQUEST])
#define UA_RegisterNodesRequest_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_REGISTERNODESREQUEST])
#define UA_RegisterNodesRequest_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_REGISTERNODESREQUEST])
#define UA_RegisterNodesRequest_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_REGISTERNODESREQUEST])
#define UA_RegisterNodesRequest_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_REGISTERNODESREQUEST], dst, offset)
#define UA_RegisterNodesRequest_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_REGISTERNODESREQUEST])

/** @brief Registers one or more nodes for repeated use within a session. */
typedef struct {
    UA_ResponseHeader responseHeader;
    UA_Int32 registeredNodeIdsSize;
    UA_NodeId *registeredNodeIds;
} UA_RegisterNodesResponse;
#define UA_TYPES_REGISTERNODESRESPONSE 97
#define UA_RegisterNodesResponse_new() (UA_RegisterNodesResponse*)UA_new(&UA_TYPES[UA_TYPES_REGISTERNODESRESPONSE])
#define UA_RegisterNodesResponse_init(p) UA_init(p, &UA_TYPES[UA_TYPES_REGISTERNODESRESPONSE])
#define UA_RegisterNodesResponse_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_REGISTERNODESRESPONSE])
#define UA_RegisterNodesResponse_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_REGISTERNODESRESPONSE])
#define UA_RegisterNodesResponse_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_REGISTERNODESRESPONSE])
#define UA_RegisterNodesResponse_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_REGISTERNODESRESPONSE], dst, offset)
#define UA_RegisterNodesResponse_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_REGISTERNODESRESPONSE])

/** @brief Unregisters one or more previously registered nodes. */
typedef struct {
    UA_RequestHeader requestHeader;
    UA_Int32 nodesToUnregisterSize;
    UA_NodeId *nodesToUnregister;
} UA_UnregisterNodesRequest;
#define UA_TYPES_UNREGISTERNODESREQUEST 98
#define UA_UnregisterNodesRequest_new() (UA_UnregisterNodesRequest*)UA_new(&UA_TYPES[UA_TYPES_UNREGISTERNODESREQUEST])
#define UA_UnregisterNodesRequest_init(p) UA_init(p, &UA_TYPES[UA_TYPES_UNREGISTERNODESREQUEST])
#define UA_UnregisterNodesRequest_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_UNREGISTERNODESREQUEST])
#define UA_UnregisterNodesRequest_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_UNREGISTERNODESREQUEST])
#define UA_UnregisterNodesRequest_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_UNREGISTERNODESREQUEST])
#define UA_UnregisterNodesRequest_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_UNREGISTERNODESREQUEST], dst, offset)
#define UA_UnregisterNodesRequest_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_UNREGISTERNODESREQUEST])

/** @brief Unregisters one or more previously registered nodes. */
typedef struct {
    UA_ResponseHeader responseHeader;
} UA_UnregisterNodesResponse;
#define UA_TYPES_UNREGISTERNODESRESPONSE 99
#define UA_UnregisterNodesResponse_new() (UA_UnregisterNodesResponse*)UA_new(&UA_TYPES[UA_TYPES_UNREGISTERNODESRESPONSE])
#define UA_UnregisterNodesResponse_init(p) UA_init(p, &UA_TYPES[UA_TYPES_UNREGISTERNODESRESPONSE])
#define UA_UnregisterNodesResponse_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_UNREGISTERNODESRESPONSE])
#define UA_UnregisterNodesResponse_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_UNREGISTERNODESRESPONSE])
#define UA_UnregisterNodesResponse_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_UNREGISTERNODESRESPONSE])
#define UA_UnregisterNodesResponse_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_UNREGISTERNODESRESPONSE], dst, offset)
#define UA_UnregisterNodesResponse_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_UNREGISTERNODESRESPONSE])

typedef enum { 
    UA_TIMESTAMPSTORETURN_SOURCE = 0,
    UA_TIMESTAMPSTORETURN_SERVER = 1,
    UA_TIMESTAMPSTORETURN_BOTH = 2,
    UA_TIMESTAMPSTORETURN_NEITHER = 3
} UA_TimestampsToReturn;
#define UA_TYPES_TIMESTAMPSTORETURN 100
#define UA_TimestampsToReturn_new (UA_TimestampsToReturn*)UA_Int32_new
#define UA_TimestampsToReturn_init(p) UA_Int32_init((UA_Int32*)p)
#define UA_TimestampsToReturn_delete(p) UA_Int32_delete((UA_Int32*)p)
#define UA_TimestampsToReturn_deleteMembers(p) UA_Int32_deleteMembers((UA_Int32*)p)
#define UA_TimestampsToReturn_copy(src, dst) UA_Int32_copy((const UA_Int32*)src, (UA_Int32*)dst)
#define UA_TimestampsToReturn_encodeBinary(src, dst, offset) UA_Int32_encodeBinary((UA_Int32*)src, dst, offset)
#define UA_TimestampsToReturn_decodeBinary(src, offset, dst) UA_Int32_decodeBinary(src, offset, (UA_Int32*)dst)

typedef struct {
    UA_NodeId nodeId;
    UA_UInt32 attributeId;
    UA_String indexRange;
    UA_QualifiedName dataEncoding;
} UA_ReadValueId;
#define UA_TYPES_READVALUEID 101
#define UA_ReadValueId_new() (UA_ReadValueId*)UA_new(&UA_TYPES[UA_TYPES_READVALUEID])
#define UA_ReadValueId_init(p) UA_init(p, &UA_TYPES[UA_TYPES_READVALUEID])
#define UA_ReadValueId_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_READVALUEID])
#define UA_ReadValueId_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_READVALUEID])
#define UA_ReadValueId_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_READVALUEID])
#define UA_ReadValueId_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_READVALUEID], dst, offset)
#define UA_ReadValueId_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_READVALUEID])

typedef struct {
    UA_RequestHeader requestHeader;
    UA_Double maxAge;
    UA_TimestampsToReturn timestampsToReturn;
    UA_Int32 nodesToReadSize;
    UA_ReadValueId *nodesToRead;
} UA_ReadRequest;
#define UA_TYPES_READREQUEST 102
#define UA_ReadRequest_new() (UA_ReadRequest*)UA_new(&UA_TYPES[UA_TYPES_READREQUEST])
#define UA_ReadRequest_init(p) UA_init(p, &UA_TYPES[UA_TYPES_READREQUEST])
#define UA_ReadRequest_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_READREQUEST])
#define UA_ReadRequest_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_READREQUEST])
#define UA_ReadRequest_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_READREQUEST])
#define UA_ReadRequest_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_READREQUEST], dst, offset)
#define UA_ReadRequest_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_READREQUEST])

typedef struct {
    UA_ResponseHeader responseHeader;
    UA_Int32 resultsSize;
    UA_DataValue *results;
    UA_Int32 diagnosticInfosSize;
    UA_DiagnosticInfo *diagnosticInfos;
} UA_ReadResponse;
#define UA_TYPES_READRESPONSE 103
#define UA_ReadResponse_new() (UA_ReadResponse*)UA_new(&UA_TYPES[UA_TYPES_READRESPONSE])
#define UA_ReadResponse_init(p) UA_init(p, &UA_TYPES[UA_TYPES_READRESPONSE])
#define UA_ReadResponse_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_READRESPONSE])
#define UA_ReadResponse_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_READRESPONSE])
#define UA_ReadResponse_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_READRESPONSE])
#define UA_ReadResponse_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_READRESPONSE], dst, offset)
#define UA_ReadResponse_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_READRESPONSE])

typedef struct {
    UA_NodeId nodeId;
    UA_UInt32 attributeId;
    UA_String indexRange;
    UA_DataValue value;
} UA_WriteValue;
#define UA_TYPES_WRITEVALUE 104
#define UA_WriteValue_new() (UA_WriteValue*)UA_new(&UA_TYPES[UA_TYPES_WRITEVALUE])
#define UA_WriteValue_init(p) UA_init(p, &UA_TYPES[UA_TYPES_WRITEVALUE])
#define UA_WriteValue_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_WRITEVALUE])
#define UA_WriteValue_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_WRITEVALUE])
#define UA_WriteValue_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_WRITEVALUE])
#define UA_WriteValue_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_WRITEVALUE], dst, offset)
#define UA_WriteValue_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_WRITEVALUE])

typedef struct {
    UA_RequestHeader requestHeader;
    UA_Int32 nodesToWriteSize;
    UA_WriteValue *nodesToWrite;
} UA_WriteRequest;
#define UA_TYPES_WRITEREQUEST 105
#define UA_WriteRequest_new() (UA_WriteRequest*)UA_new(&UA_TYPES[UA_TYPES_WRITEREQUEST])
#define UA_WriteRequest_init(p) UA_init(p, &UA_TYPES[UA_TYPES_WRITEREQUEST])
#define UA_WriteRequest_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_WRITEREQUEST])
#define UA_WriteRequest_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_WRITEREQUEST])
#define UA_WriteRequest_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_WRITEREQUEST])
#define UA_WriteRequest_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_WRITEREQUEST], dst, offset)
#define UA_WriteRequest_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_WRITEREQUEST])

typedef struct {
    UA_ResponseHeader responseHeader;
    UA_Int32 resultsSize;
    UA_StatusCode *results;
    UA_Int32 diagnosticInfosSize;
    UA_DiagnosticInfo *diagnosticInfos;
} UA_WriteResponse;
#define UA_TYPES_WRITERESPONSE 106
#define UA_WriteResponse_new() (UA_WriteResponse*)UA_new(&UA_TYPES[UA_TYPES_WRITERESPONSE])
#define UA_WriteResponse_init(p) UA_init(p, &UA_TYPES[UA_TYPES_WRITERESPONSE])
#define UA_WriteResponse_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_WRITERESPONSE])
#define UA_WriteResponse_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_WRITERESPONSE])
#define UA_WriteResponse_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_WRITERESPONSE])
#define UA_WriteResponse_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_WRITERESPONSE], dst, offset)
#define UA_WriteResponse_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_WRITERESPONSE])

typedef struct {
    UA_NodeId objectId;
    UA_NodeId methodId;
    UA_Int32 inputArgumentsSize;
    UA_Variant *inputArguments;
} UA_CallMethodRequest;
#define UA_TYPES_CALLMETHODREQUEST 107
#define UA_CallMethodRequest_new() (UA_CallMethodRequest*)UA_new(&UA_TYPES[UA_TYPES_CALLMETHODREQUEST])
#define UA_CallMethodRequest_init(p) UA_init(p, &UA_TYPES[UA_TYPES_CALLMETHODREQUEST])
#define UA_CallMethodRequest_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_CALLMETHODREQUEST])
#define UA_CallMethodRequest_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_CALLMETHODREQUEST])
#define UA_CallMethodRequest_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_CALLMETHODREQUEST])
#define UA_CallMethodRequest_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CALLMETHODREQUEST], dst, offset)
#define UA_CallMethodRequest_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CALLMETHODREQUEST])

typedef struct {
    UA_StatusCode statusCode;
    UA_Int32 inputArgumentResultsSize;
    UA_StatusCode *inputArgumentResults;
    UA_Int32 inputArgumentDiagnosticInfosSize;
    UA_DiagnosticInfo *inputArgumentDiagnosticInfos;
    UA_Int32 outputArgumentsSize;
    UA_Variant *outputArguments;
} UA_CallMethodResult;
#define UA_TYPES_CALLMETHODRESULT 108
#define UA_CallMethodResult_new() (UA_CallMethodResult*)UA_new(&UA_TYPES[UA_TYPES_CALLMETHODRESULT])
#define UA_CallMethodResult_init(p) UA_init(p, &UA_TYPES[UA_TYPES_CALLMETHODRESULT])
#define UA_CallMethodResult_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_CALLMETHODRESULT])
#define UA_CallMethodResult_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_CALLMETHODRESULT])
#define UA_CallMethodResult_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_CALLMETHODRESULT])
#define UA_CallMethodResult_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CALLMETHODRESULT], dst, offset)
#define UA_CallMethodResult_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CALLMETHODRESULT])

typedef struct {
    UA_RequestHeader requestHeader;
    UA_Int32 methodsToCallSize;
    UA_CallMethodRequest *methodsToCall;
} UA_CallRequest;
#define UA_TYPES_CALLREQUEST 109
#define UA_CallRequest_new() (UA_CallRequest*)UA_new(&UA_TYPES[UA_TYPES_CALLREQUEST])
#define UA_CallRequest_init(p) UA_init(p, &UA_TYPES[UA_TYPES_CALLREQUEST])
#define UA_CallRequest_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_CALLREQUEST])
#define UA_CallRequest_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_CALLREQUEST])
#define UA_CallRequest_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_CALLREQUEST])
#define UA_CallRequest_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CALLREQUEST], dst, offset)
#define UA_CallRequest_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CALLREQUEST])

typedef struct {
    UA_ResponseHeader responseHeader;
    UA_Int32 resultsSize;
    UA_CallMethodResult *results;
    UA_Int32 diagnosticInfosSize;
    UA_DiagnosticInfo *diagnosticInfos;
} UA_CallResponse;
#define UA_TYPES_CALLRESPONSE 110
#define UA_CallResponse_new() (UA_CallResponse*)UA_new(&UA_TYPES[UA_TYPES_CALLRESPONSE])
#define UA_CallResponse_init(p) UA_init(p, &UA_TYPES[UA_TYPES_CALLRESPONSE])
#define UA_CallResponse_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_CALLRESPONSE])
#define UA_CallResponse_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_CALLRESPONSE])
#define UA_CallResponse_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_CALLRESPONSE])
#define UA_CallResponse_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CALLRESPONSE], dst, offset)
#define UA_CallResponse_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CALLRESPONSE])

typedef enum { 
    UA_MONITORINGMODE_DISABLED = 0,
    UA_MONITORINGMODE_SAMPLING = 1,
    UA_MONITORINGMODE_REPORTING = 2
} UA_MonitoringMode;
#define UA_TYPES_MONITORINGMODE 111
#define UA_MonitoringMode_new (UA_MonitoringMode*)UA_Int32_new
#define UA_MonitoringMode_init(p) UA_Int32_init((UA_Int32*)p)
#define UA_MonitoringMode_delete(p) UA_Int32_delete((UA_Int32*)p)
#define UA_MonitoringMode_deleteMembers(p) UA_Int32_deleteMembers((UA_Int32*)p)
#define UA_MonitoringMode_copy(src, dst) UA_Int32_copy((const UA_Int32*)src, (UA_Int32*)dst)
#define UA_MonitoringMode_encodeBinary(src, dst, offset) UA_Int32_encodeBinary((UA_Int32*)src, dst, offset)
#define UA_MonitoringMode_decodeBinary(src, offset, dst) UA_Int32_decodeBinary(src, offset, (UA_Int32*)dst)

typedef struct {
    UA_UInt32 clientHandle;
    UA_Double samplingInterval;
    UA_ExtensionObject filter;
    UA_UInt32 queueSize;
    UA_Boolean discardOldest;
} UA_MonitoringParameters;
#define UA_TYPES_MONITORINGPARAMETERS 112
#define UA_MonitoringParameters_new() (UA_MonitoringParameters*)UA_new(&UA_TYPES[UA_TYPES_MONITORINGPARAMETERS])
#define UA_MonitoringParameters_init(p) UA_init(p, &UA_TYPES[UA_TYPES_MONITORINGPARAMETERS])
#define UA_MonitoringParameters_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_MONITORINGPARAMETERS])
#define UA_MonitoringParameters_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_MONITORINGPARAMETERS])
#define UA_MonitoringParameters_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_MONITORINGPARAMETERS])
#define UA_MonitoringParameters_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_MONITORINGPARAMETERS], dst, offset)
#define UA_MonitoringParameters_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_MONITORINGPARAMETERS])

typedef struct {
    UA_ReadValueId itemToMonitor;
    UA_MonitoringMode monitoringMode;
    UA_MonitoringParameters requestedParameters;
} UA_MonitoredItemCreateRequest;
#define UA_TYPES_MONITOREDITEMCREATEREQUEST 113
#define UA_MonitoredItemCreateRequest_new() (UA_MonitoredItemCreateRequest*)UA_new(&UA_TYPES[UA_TYPES_MONITOREDITEMCREATEREQUEST])
#define UA_MonitoredItemCreateRequest_init(p) UA_init(p, &UA_TYPES[UA_TYPES_MONITOREDITEMCREATEREQUEST])
#define UA_MonitoredItemCreateRequest_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_MONITOREDITEMCREATEREQUEST])
#define UA_MonitoredItemCreateRequest_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_MONITOREDITEMCREATEREQUEST])
#define UA_MonitoredItemCreateRequest_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_MONITOREDITEMCREATEREQUEST])
#define UA_MonitoredItemCreateRequest_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_MONITOREDITEMCREATEREQUEST], dst, offset)
#define UA_MonitoredItemCreateRequest_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_MONITOREDITEMCREATEREQUEST])

typedef struct {
    UA_StatusCode statusCode;
    UA_UInt32 monitoredItemId;
    UA_Double revisedSamplingInterval;
    UA_UInt32 revisedQueueSize;
    UA_ExtensionObject filterResult;
} UA_MonitoredItemCreateResult;
#define UA_TYPES_MONITOREDITEMCREATERESULT 114
#define UA_MonitoredItemCreateResult_new() (UA_MonitoredItemCreateResult*)UA_new(&UA_TYPES[UA_TYPES_MONITOREDITEMCREATERESULT])
#define UA_MonitoredItemCreateResult_init(p) UA_init(p, &UA_TYPES[UA_TYPES_MONITOREDITEMCREATERESULT])
#define UA_MonitoredItemCreateResult_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_MONITOREDITEMCREATERESULT])
#define UA_MonitoredItemCreateResult_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_MONITOREDITEMCREATERESULT])
#define UA_MonitoredItemCreateResult_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_MONITOREDITEMCREATERESULT])
#define UA_MonitoredItemCreateResult_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_MONITOREDITEMCREATERESULT], dst, offset)
#define UA_MonitoredItemCreateResult_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_MONITOREDITEMCREATERESULT])

typedef struct {
    UA_RequestHeader requestHeader;
    UA_UInt32 subscriptionId;
    UA_TimestampsToReturn timestampsToReturn;
    UA_Int32 itemsToCreateSize;
    UA_MonitoredItemCreateRequest *itemsToCreate;
} UA_CreateMonitoredItemsRequest;
#define UA_TYPES_CREATEMONITOREDITEMSREQUEST 115
#define UA_CreateMonitoredItemsRequest_new() (UA_CreateMonitoredItemsRequest*)UA_new(&UA_TYPES[UA_TYPES_CREATEMONITOREDITEMSREQUEST])
#define UA_CreateMonitoredItemsRequest_init(p) UA_init(p, &UA_TYPES[UA_TYPES_CREATEMONITOREDITEMSREQUEST])
#define UA_CreateMonitoredItemsRequest_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_CREATEMONITOREDITEMSREQUEST])
#define UA_CreateMonitoredItemsRequest_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_CREATEMONITOREDITEMSREQUEST])
#define UA_CreateMonitoredItemsRequest_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_CREATEMONITOREDITEMSREQUEST])
#define UA_CreateMonitoredItemsRequest_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CREATEMONITOREDITEMSREQUEST], dst, offset)
#define UA_CreateMonitoredItemsRequest_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CREATEMONITOREDITEMSREQUEST])

typedef struct {
    UA_ResponseHeader responseHeader;
    UA_Int32 resultsSize;
    UA_MonitoredItemCreateResult *results;
    UA_Int32 diagnosticInfosSize;
    UA_DiagnosticInfo *diagnosticInfos;
} UA_CreateMonitoredItemsResponse;
#define UA_TYPES_CREATEMONITOREDITEMSRESPONSE 116
#define UA_CreateMonitoredItemsResponse_new() (UA_CreateMonitoredItemsResponse*)UA_new(&UA_TYPES[UA_TYPES_CREATEMONITOREDITEMSRESPONSE])
#define UA_CreateMonitoredItemsResponse_init(p) UA_init(p, &UA_TYPES[UA_TYPES_CREATEMONITOREDITEMSRESPONSE])
#define UA_CreateMonitoredItemsResponse_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_CREATEMONITOREDITEMSRESPONSE])
#define UA_CreateMonitoredItemsResponse_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_CREATEMONITOREDITEMSRESPONSE])
#define UA_CreateMonitoredItemsResponse_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_CREATEMONITOREDITEMSRESPONSE])
#define UA_CreateMonitoredItemsResponse_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CREATEMONITOREDITEMSRESPONSE], dst, offset)
#define UA_CreateMonitoredItemsResponse_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CREATEMONITOREDITEMSRESPONSE])

typedef struct {
    UA_RequestHeader requestHeader;
    UA_UInt32 subscriptionId;
    UA_Int32 monitoredItemIdsSize;
    UA_UInt32 *monitoredItemIds;
} UA_DeleteMonitoredItemsRequest;
#define UA_TYPES_DELETEMONITOREDITEMSREQUEST 117
#define UA_DeleteMonitoredItemsRequest_new() (UA_DeleteMonitoredItemsRequest*)UA_new(&UA_TYPES[UA_TYPES_DELETEMONITOREDITEMSREQUEST])
#define UA_DeleteMonitoredItemsRequest_init(p) UA_init(p, &UA_TYPES[UA_TYPES_DELETEMONITOREDITEMSREQUEST])
#define UA_DeleteMonitoredItemsRequest_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_DELETEMONITOREDITEMSREQUEST])
#define UA_DeleteMonitoredItemsRequest_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_DELETEMONITOREDITEMSREQUEST])
#define UA_DeleteMonitoredItemsRequest_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_DELETEMONITOREDITEMSREQUEST])
#define UA_DeleteMonitoredItemsRequest_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_DELETEMONITOREDITEMSREQUEST], dst, offset)
#define UA_DeleteMonitoredItemsRequest_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_DELETEMONITOREDITEMSREQUEST])

typedef struct {
    UA_ResponseHeader responseHeader;
    UA_Int32 resultsSize;
    UA_StatusCode *results;
    UA_Int32 diagnosticInfosSize;
    UA_DiagnosticInfo *diagnosticInfos;
} UA_DeleteMonitoredItemsResponse;
#define UA_TYPES_DELETEMONITOREDITEMSRESPONSE 118
#define UA_DeleteMonitoredItemsResponse_new() (UA_DeleteMonitoredItemsResponse*)UA_new(&UA_TYPES[UA_TYPES_DELETEMONITOREDITEMSRESPONSE])
#define UA_DeleteMonitoredItemsResponse_init(p) UA_init(p, &UA_TYPES[UA_TYPES_DELETEMONITOREDITEMSRESPONSE])
#define UA_DeleteMonitoredItemsResponse_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_DELETEMONITOREDITEMSRESPONSE])
#define UA_DeleteMonitoredItemsResponse_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_DELETEMONITOREDITEMSRESPONSE])
#define UA_DeleteMonitoredItemsResponse_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_DELETEMONITOREDITEMSRESPONSE])
#define UA_DeleteMonitoredItemsResponse_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_DELETEMONITOREDITEMSRESPONSE], dst, offset)
#define UA_DeleteMonitoredItemsResponse_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_DELETEMONITOREDITEMSRESPONSE])

typedef struct {
    UA_RequestHeader requestHeader;
    UA_Double requestedPublishingInterval;
    UA_UInt32 requestedLifetimeCount;
    UA_UInt32 requestedMaxKeepAliveCount;
    UA_UInt32 maxNotificationsPerPublish;
    UA_Boolean publishingEnabled;
    UA_Byte priority;
} UA_CreateSubscriptionRequest;
#define UA_TYPES_CREATESUBSCRIPTIONREQUEST 119
#define UA_CreateSubscriptionRequest_new() (UA_CreateSubscriptionRequest*)UA_new(&UA_TYPES[UA_TYPES_CREATESUBSCRIPTIONREQUEST])
#define UA_CreateSubscriptionRequest_init(p) UA_init(p, &UA_TYPES[UA_TYPES_CREATESUBSCRIPTIONREQUEST])
#define UA_CreateSubscriptionRequest_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_CREATESUBSCRIPTIONREQUEST])
#define UA_CreateSubscriptionRequest_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_CREATESUBSCRIPTIONREQUEST])
#define UA_CreateSubscriptionRequest_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_CREATESUBSCRIPTIONREQUEST])
#define UA_CreateSubscriptionRequest_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CREATESUBSCRIPTIONREQUEST], dst, offset)
#define UA_CreateSubscriptionRequest_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CREATESUBSCRIPTIONREQUEST])

typedef struct {
    UA_ResponseHeader responseHeader;
    UA_UInt32 subscriptionId;
    UA_Double revisedPublishingInterval;
    UA_UInt32 revisedLifetimeCount;
    UA_UInt32 revisedMaxKeepAliveCount;
} UA_CreateSubscriptionResponse;
#define UA_TYPES_CREATESUBSCRIPTIONRESPONSE 120
#define UA_CreateSubscriptionResponse_new() (UA_CreateSubscriptionResponse*)UA_new(&UA_TYPES[UA_TYPES_CREATESUBSCRIPTIONRESPONSE])
#define UA_CreateSubscriptionResponse_init(p) UA_init(p, &UA_TYPES[UA_TYPES_CREATESUBSCRIPTIONRESPONSE])
#define UA_CreateSubscriptionResponse_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_CREATESUBSCRIPTIONRESPONSE])
#define UA_CreateSubscriptionResponse_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_CREATESUBSCRIPTIONRESPONSE])
#define UA_CreateSubscriptionResponse_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_CREATESUBSCRIPTIONRESPONSE])
#define UA_CreateSubscriptionResponse_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CREATESUBSCRIPTIONRESPONSE], dst, offset)
#define UA_CreateSubscriptionResponse_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CREATESUBSCRIPTIONRESPONSE])

typedef struct {
    UA_RequestHeader requestHeader;
    UA_UInt32 subscriptionId;
    UA_Double requestedPublishingInterval;
    UA_UInt32 requestedLifetimeCount;
    UA_UInt32 requestedMaxKeepAliveCount;
    UA_UInt32 maxNotificationsPerPublish;
    UA_Byte priority;
} UA_ModifySubscriptionRequest;
#define UA_TYPES_MODIFYSUBSCRIPTIONREQUEST 121
#define UA_ModifySubscriptionRequest_new() (UA_ModifySubscriptionRequest*)UA_new(&UA_TYPES[UA_TYPES_MODIFYSUBSCRIPTIONREQUEST])
#define UA_ModifySubscriptionRequest_init(p) UA_init(p, &UA_TYPES[UA_TYPES_MODIFYSUBSCRIPTIONREQUEST])
#define UA_ModifySubscriptionRequest_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_MODIFYSUBSCRIPTIONREQUEST])
#define UA_ModifySubscriptionRequest_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_MODIFYSUBSCRIPTIONREQUEST])
#define UA_ModifySubscriptionRequest_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_MODIFYSUBSCRIPTIONREQUEST])
#define UA_ModifySubscriptionRequest_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_MODIFYSUBSCRIPTIONREQUEST], dst, offset)
#define UA_ModifySubscriptionRequest_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_MODIFYSUBSCRIPTIONREQUEST])

typedef struct {
    UA_ResponseHeader responseHeader;
    UA_Double revisedPublishingInterval;
    UA_UInt32 revisedLifetimeCount;
    UA_UInt32 revisedMaxKeepAliveCount;
} UA_ModifySubscriptionResponse;
#define UA_TYPES_MODIFYSUBSCRIPTIONRESPONSE 122
#define UA_ModifySubscriptionResponse_new() (UA_ModifySubscriptionResponse*)UA_new(&UA_TYPES[UA_TYPES_MODIFYSUBSCRIPTIONRESPONSE])
#define UA_ModifySubscriptionResponse_init(p) UA_init(p, &UA_TYPES[UA_TYPES_MODIFYSUBSCRIPTIONRESPONSE])
#define UA_ModifySubscriptionResponse_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_MODIFYSUBSCRIPTIONRESPONSE])
#define UA_ModifySubscriptionResponse_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_MODIFYSUBSCRIPTIONRESPONSE])
#define UA_ModifySubscriptionResponse_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_MODIFYSUBSCRIPTIONRESPONSE])
#define UA_ModifySubscriptionResponse_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_MODIFYSUBSCRIPTIONRESPONSE], dst, offset)
#define UA_ModifySubscriptionResponse_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_MODIFYSUBSCRIPTIONRESPONSE])

typedef struct {
    UA_RequestHeader requestHeader;
    UA_Boolean publishingEnabled;
    UA_Int32 subscriptionIdsSize;
    UA_UInt32 *subscriptionIds;
} UA_SetPublishingModeRequest;
#define UA_TYPES_SETPUBLISHINGMODEREQUEST 123
#define UA_SetPublishingModeRequest_new() (UA_SetPublishingModeRequest*)UA_new(&UA_TYPES[UA_TYPES_SETPUBLISHINGMODEREQUEST])
#define UA_SetPublishingModeRequest_init(p) UA_init(p, &UA_TYPES[UA_TYPES_SETPUBLISHINGMODEREQUEST])
#define UA_SetPublishingModeRequest_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_SETPUBLISHINGMODEREQUEST])
#define UA_SetPublishingModeRequest_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_SETPUBLISHINGMODEREQUEST])
#define UA_SetPublishingModeRequest_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_SETPUBLISHINGMODEREQUEST])
#define UA_SetPublishingModeRequest_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_SETPUBLISHINGMODEREQUEST], dst, offset)
#define UA_SetPublishingModeRequest_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_SETPUBLISHINGMODEREQUEST])

typedef struct {
    UA_ResponseHeader responseHeader;
    UA_Int32 resultsSize;
    UA_StatusCode *results;
    UA_Int32 diagnosticInfosSize;
    UA_DiagnosticInfo *diagnosticInfos;
} UA_SetPublishingModeResponse;
#define UA_TYPES_SETPUBLISHINGMODERESPONSE 124
#define UA_SetPublishingModeResponse_new() (UA_SetPublishingModeResponse*)UA_new(&UA_TYPES[UA_TYPES_SETPUBLISHINGMODERESPONSE])
#define UA_SetPublishingModeResponse_init(p) UA_init(p, &UA_TYPES[UA_TYPES_SETPUBLISHINGMODERESPONSE])
#define UA_SetPublishingModeResponse_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_SETPUBLISHINGMODERESPONSE])
#define UA_SetPublishingModeResponse_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_SETPUBLISHINGMODERESPONSE])
#define UA_SetPublishingModeResponse_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_SETPUBLISHINGMODERESPONSE])
#define UA_SetPublishingModeResponse_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_SETPUBLISHINGMODERESPONSE], dst, offset)
#define UA_SetPublishingModeResponse_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_SETPUBLISHINGMODERESPONSE])

typedef struct {
    UA_UInt32 sequenceNumber;
    UA_DateTime publishTime;
    UA_Int32 notificationDataSize;
    UA_ExtensionObject *notificationData;
} UA_NotificationMessage;
#define UA_TYPES_NOTIFICATIONMESSAGE 125
#define UA_NotificationMessage_new() (UA_NotificationMessage*)UA_new(&UA_TYPES[UA_TYPES_NOTIFICATIONMESSAGE])
#define UA_NotificationMessage_init(p) UA_init(p, &UA_TYPES[UA_TYPES_NOTIFICATIONMESSAGE])
#define UA_NotificationMessage_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_NOTIFICATIONMESSAGE])
#define UA_NotificationMessage_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_NOTIFICATIONMESSAGE])
#define UA_NotificationMessage_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_NOTIFICATIONMESSAGE])
#define UA_NotificationMessage_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_NOTIFICATIONMESSAGE], dst, offset)
#define UA_NotificationMessage_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_NOTIFICATIONMESSAGE])

typedef struct {
    UA_UInt32 clientHandle;
    UA_DataValue value;
} UA_MonitoredItemNotification;
#define UA_TYPES_MONITOREDITEMNOTIFICATION 126
#define UA_MonitoredItemNotification_new() (UA_MonitoredItemNotification*)UA_new(&UA_TYPES[UA_TYPES_MONITOREDITEMNOTIFICATION])
#define UA_MonitoredItemNotification_init(p) UA_init(p, &UA_TYPES[UA_TYPES_MONITOREDITEMNOTIFICATION])
#define UA_MonitoredItemNotification_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_MONITOREDITEMNOTIFICATION])
#define UA_MonitoredItemNotification_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_MONITOREDITEMNOTIFICATION])
#define UA_MonitoredItemNotification_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_MONITOREDITEMNOTIFICATION])
#define UA_MonitoredItemNotification_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_MONITOREDITEMNOTIFICATION], dst, offset)
#define UA_MonitoredItemNotification_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_MONITOREDITEMNOTIFICATION])

typedef struct {
    UA_UInt32 subscriptionId;
    UA_UInt32 sequenceNumber;
} UA_SubscriptionAcknowledgement;
#define UA_TYPES_SUBSCRIPTIONACKNOWLEDGEMENT 127
#define UA_SubscriptionAcknowledgement_new() (UA_SubscriptionAcknowledgement*)UA_new(&UA_TYPES[UA_TYPES_SUBSCRIPTIONACKNOWLEDGEMENT])
#define UA_SubscriptionAcknowledgement_init(p) UA_init(p, &UA_TYPES[UA_TYPES_SUBSCRIPTIONACKNOWLEDGEMENT])
#define UA_SubscriptionAcknowledgement_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_SUBSCRIPTIONACKNOWLEDGEMENT])
#define UA_SubscriptionAcknowledgement_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_SUBSCRIPTIONACKNOWLEDGEMENT])
#define UA_SubscriptionAcknowledgement_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_SUBSCRIPTIONACKNOWLEDGEMENT])
#define UA_SubscriptionAcknowledgement_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_SUBSCRIPTIONACKNOWLEDGEMENT], dst, offset)
#define UA_SubscriptionAcknowledgement_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_SUBSCRIPTIONACKNOWLEDGEMENT])

typedef struct {
    UA_RequestHeader requestHeader;
    UA_Int32 subscriptionAcknowledgementsSize;
    UA_SubscriptionAcknowledgement *subscriptionAcknowledgements;
} UA_PublishRequest;
#define UA_TYPES_PUBLISHREQUEST 128
#define UA_PublishRequest_new() (UA_PublishRequest*)UA_new(&UA_TYPES[UA_TYPES_PUBLISHREQUEST])
#define UA_PublishRequest_init(p) UA_init(p, &UA_TYPES[UA_TYPES_PUBLISHREQUEST])
#define UA_PublishRequest_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_PUBLISHREQUEST])
#define UA_PublishRequest_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_PUBLISHREQUEST])
#define UA_PublishRequest_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_PUBLISHREQUEST])
#define UA_PublishRequest_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_PUBLISHREQUEST], dst, offset)
#define UA_PublishRequest_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_PUBLISHREQUEST])

typedef struct {
    UA_ResponseHeader responseHeader;
    UA_UInt32 subscriptionId;
    UA_Int32 availableSequenceNumbersSize;
    UA_UInt32 *availableSequenceNumbers;
    UA_Boolean moreNotifications;
    UA_NotificationMessage notificationMessage;
    UA_Int32 resultsSize;
    UA_StatusCode *results;
    UA_Int32 diagnosticInfosSize;
    UA_DiagnosticInfo *diagnosticInfos;
} UA_PublishResponse;
#define UA_TYPES_PUBLISHRESPONSE 129
#define UA_PublishResponse_new() (UA_PublishResponse*)UA_new(&UA_TYPES[UA_TYPES_PUBLISHRESPONSE])
#define UA_PublishResponse_init(p) UA_init(p, &UA_TYPES[UA_TYPES_PUBLISHRESPONSE])
#define UA_PublishResponse_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_PUBLISHRESPONSE])
#define UA_PublishResponse_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_PUBLISHRESPONSE])
#define UA_PublishResponse_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_PUBLISHRESPONSE])
#define UA_PublishResponse_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_PUBLISHRESPONSE], dst, offset)
#define UA_PublishResponse_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_PUBLISHRESPONSE])

typedef struct {
    UA_RequestHeader requestHeader;
    UA_Int32 subscriptionIdsSize;
    UA_UInt32 *subscriptionIds;
} UA_DeleteSubscriptionsRequest;
#define UA_TYPES_DELETESUBSCRIPTIONSREQUEST 130
#define UA_DeleteSubscriptionsRequest_new() (UA_DeleteSubscriptionsRequest*)UA_new(&UA_TYPES[UA_TYPES_DELETESUBSCRIPTIONSREQUEST])
#define UA_DeleteSubscriptionsRequest_init(p) UA_init(p, &UA_TYPES[UA_TYPES_DELETESUBSCRIPTIONSREQUEST])
#define UA_DeleteSubscriptionsRequest_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_DELETESUBSCRIPTIONSREQUEST])
#define UA_DeleteSubscriptionsRequest_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_DELETESUBSCRIPTIONSREQUEST])
#define UA_DeleteSubscriptionsRequest_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_DELETESUBSCRIPTIONSREQUEST])
#define UA_DeleteSubscriptionsRequest_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_DELETESUBSCRIPTIONSREQUEST], dst, offset)
#define UA_DeleteSubscriptionsRequest_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_DELETESUBSCRIPTIONSREQUEST])

typedef struct {
    UA_ResponseHeader responseHeader;
    UA_Int32 resultsSize;
    UA_StatusCode *results;
    UA_Int32 diagnosticInfosSize;
    UA_DiagnosticInfo *diagnosticInfos;
} UA_DeleteSubscriptionsResponse;
#define UA_TYPES_DELETESUBSCRIPTIONSRESPONSE 131
#define UA_DeleteSubscriptionsResponse_new() (UA_DeleteSubscriptionsResponse*)UA_new(&UA_TYPES[UA_TYPES_DELETESUBSCRIPTIONSRESPONSE])
#define UA_DeleteSubscriptionsResponse_init(p) UA_init(p, &UA_TYPES[UA_TYPES_DELETESUBSCRIPTIONSRESPONSE])
#define UA_DeleteSubscriptionsResponse_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_DELETESUBSCRIPTIONSRESPONSE])
#define UA_DeleteSubscriptionsResponse_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_DELETESUBSCRIPTIONSRESPONSE])
#define UA_DeleteSubscriptionsResponse_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_DELETESUBSCRIPTIONSRESPONSE])
#define UA_DeleteSubscriptionsResponse_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_DELETESUBSCRIPTIONSRESPONSE], dst, offset)
#define UA_DeleteSubscriptionsResponse_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_DELETESUBSCRIPTIONSRESPONSE])

typedef struct {
    UA_String productUri;
    UA_String manufacturerName;
    UA_String productName;
    UA_String softwareVersion;
    UA_String buildNumber;
    UA_DateTime buildDate;
} UA_BuildInfo;
#define UA_TYPES_BUILDINFO 132
#define UA_BuildInfo_new() (UA_BuildInfo*)UA_new(&UA_TYPES[UA_TYPES_BUILDINFO])
#define UA_BuildInfo_init(p) UA_init(p, &UA_TYPES[UA_TYPES_BUILDINFO])
#define UA_BuildInfo_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_BUILDINFO])
#define UA_BuildInfo_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_BUILDINFO])
#define UA_BuildInfo_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_BUILDINFO])
#define UA_BuildInfo_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_BUILDINFO], dst, offset)
#define UA_BuildInfo_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_BUILDINFO])

typedef enum { 
    UA_SERVERSTATE_RUNNING = 0,
    UA_SERVERSTATE_FAILED = 1,
    UA_SERVERSTATE_NOCONFIGURATION = 2,
    UA_SERVERSTATE_SUSPENDED = 3,
    UA_SERVERSTATE_SHUTDOWN = 4,
    UA_SERVERSTATE_TEST = 5,
    UA_SERVERSTATE_COMMUNICATIONFAULT = 6,
    UA_SERVERSTATE_UNKNOWN = 7
} UA_ServerState;
#define UA_TYPES_SERVERSTATE 133
#define UA_ServerState_new (UA_ServerState*)UA_Int32_new
#define UA_ServerState_init(p) UA_Int32_init((UA_Int32*)p)
#define UA_ServerState_delete(p) UA_Int32_delete((UA_Int32*)p)
#define UA_ServerState_deleteMembers(p) UA_Int32_deleteMembers((UA_Int32*)p)
#define UA_ServerState_copy(src, dst) UA_Int32_copy((const UA_Int32*)src, (UA_Int32*)dst)
#define UA_ServerState_encodeBinary(src, dst, offset) UA_Int32_encodeBinary((UA_Int32*)src, dst, offset)
#define UA_ServerState_decodeBinary(src, offset, dst) UA_Int32_decodeBinary(src, offset, (UA_Int32*)dst)

typedef struct {
    UA_DateTime startTime;
    UA_DateTime currentTime;
    UA_ServerState state;
    UA_BuildInfo buildInfo;
    UA_UInt32 secondsTillShutdown;
    UA_LocalizedText shutdownReason;
} UA_ServerStatusDataType;
#define UA_TYPES_SERVERSTATUSDATATYPE 134
#define UA_ServerStatusDataType_new() (UA_ServerStatusDataType*)UA_new(&UA_TYPES[UA_TYPES_SERVERSTATUSDATATYPE])
#define UA_ServerStatusDataType_init(p) UA_init(p, &UA_TYPES[UA_TYPES_SERVERSTATUSDATATYPE])
#define UA_ServerStatusDataType_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_SERVERSTATUSDATATYPE])
#define UA_ServerStatusDataType_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_SERVERSTATUSDATATYPE])
#define UA_ServerStatusDataType_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_SERVERSTATUSDATATYPE])
#define UA_ServerStatusDataType_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_SERVERSTATUSDATATYPE], dst, offset)
#define UA_ServerStatusDataType_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_SERVERSTATUSDATATYPE])

typedef struct {
    UA_Int32 monitoredItemsSize;
    UA_MonitoredItemNotification *monitoredItems;
    UA_Int32 diagnosticInfosSize;
    UA_DiagnosticInfo *diagnosticInfos;
} UA_DataChangeNotification;
#define UA_TYPES_DATACHANGENOTIFICATION 135
#define UA_DataChangeNotification_new() (UA_DataChangeNotification*)UA_new(&UA_TYPES[UA_TYPES_DATACHANGENOTIFICATION])
#define UA_DataChangeNotification_init(p) UA_init(p, &UA_TYPES[UA_TYPES_DATACHANGENOTIFICATION])
#define UA_DataChangeNotification_delete(p) UA_delete(p, &UA_TYPES[UA_TYPES_DATACHANGENOTIFICATION])
#define UA_DataChangeNotification_deleteMembers(p) UA_deleteMembers(p, &UA_TYPES[UA_TYPES_DATACHANGENOTIFICATION])
#define UA_DataChangeNotification_copy(src, dst) UA_copy(src, dst, &UA_TYPES[UA_TYPES_DATACHANGENOTIFICATION])
#define UA_DataChangeNotification_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TYPES[UA_TYPES_DATACHANGENOTIFICATION], dst, offset)
#define UA_DataChangeNotification_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_DATACHANGENOTIFICATION])

/// @} /* end of group */

#ifdef __cplusplus
} // extern "C"
#endif


/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/include/ua_connection.h" ***********************************/

/*
 * Copyright (C) 2014 the contributors as stated in the AUTHORS file
 *
 * This file is part of open62541. open62541 is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser General
 * Public License, version 3 (as published by the Free Software Foundation) with
 * a static linking exception as stated in the LICENSE file provided with
 * open62541.
 *
 * open62541 is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 */


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @defgroup communication Communication
 *
 * @{
 */

typedef enum UA_ConnectionState {
    UA_CONNECTION_OPENING, ///< The socket is open, but the HEL/ACK handshake is not done
    UA_CONNECTION_ESTABLISHED, ///< The socket is open and the connection configured
    UA_CONNECTION_CLOSED, ///< The socket has been closed and the connection will be deleted
} UA_ConnectionState;

typedef struct UA_ConnectionConfig {
    UA_UInt32 protocolVersion;
    UA_UInt32 sendBufferSize;
    UA_UInt32 recvBufferSize;
    UA_UInt32 maxMessageSize;
    UA_UInt32 maxChunkCount;
} UA_ConnectionConfig;

extern const UA_EXPORT UA_ConnectionConfig UA_ConnectionConfig_standard;

/* Forward declaration */
struct UA_SecureChannel;
typedef struct UA_SecureChannel UA_SecureChannel;

struct UA_Connection;
typedef struct UA_Connection UA_Connection;

/**
 * The connection to a single client (or server). The connection is defined independent of the
 * underlying network layer implementation. This allows a plugging-in custom implementations (e.g.
 * an embedded TCP stack)
 */
struct UA_Connection {
    UA_ConnectionState state;
    UA_ConnectionConfig localConf;
    UA_ConnectionConfig remoteConf;
    UA_SecureChannel *channel; ///> The securechannel that is attached to this connection (or null)
    UA_Int32 sockfd; ///> Most connectivity solutions run on sockets. Having the socket id here simplifies the design.
    void *handle; ///> A pointer to the networklayer
    UA_ByteString incompleteMessage; ///> Half-received messages (tcp is a streaming protocol) get stored here
    UA_StatusCode (*getBuffer)(UA_Connection *connection, UA_ByteString *buf); ///> Get a buffer of the maximum remote recv size
    void (*releaseBuffer)(UA_Connection *connection, UA_ByteString *buf); ///> Release the buffer manually
    /**
     * Sends a message over the connection.
     * @param connection The connection
     * @param buf The message buffer is potentially reused (or freed) internally if sending succeeds.
     * @param buflen Since the buffer is potentially reused, we provide a separate content length.
     * @return Returns an error code or UA_STATUSCODE_GOOD.
     */
    UA_StatusCode (*write)(UA_Connection *connection, UA_ByteString *buf, size_t buflen);
   /**
     * Receive a message from the remote connection
	 * @param connection The connection
	 * @param response The response string. It is allocated by the connection and needs to be freed with connection->releaseBuffer
     * @param timeout Timeout of the recv operation in milliseconds
     * @return Returns UA_STATUSCODE_BADCOMMUNICATIONERROR if the recv operation can be repeated, UA_STATUSCODE_GOOD if it succeeded and
     * UA_STATUSCODE_BADCONNECTIONCLOSED if the connection was closed.
	 */
    UA_StatusCode (*recv)(UA_Connection *connection, UA_ByteString *response, UA_UInt32 timeout);
    void (*close)(UA_Connection *connection);
};

void UA_EXPORT UA_Connection_init(UA_Connection *connection);
void UA_EXPORT UA_Connection_deleteMembers(UA_Connection *connection);

void UA_EXPORT UA_Connection_detachSecureChannel(UA_Connection *connection);
void UA_EXPORT UA_Connection_attachSecureChannel(UA_Connection *connection, UA_SecureChannel *channel);

/** Returns a string of complete message (the length entry is decoded for that).
    If the received message is incomplete, it is retained in the connection. */
UA_ByteString UA_EXPORT UA_Connection_completeMessages(UA_Connection *connection, UA_ByteString received);

/** @} */

#ifdef __cplusplus
} // extern "C"
#endif


/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/include/ua_log.h" ***********************************/

/*
 * Copyright (C) 2014 the contributors as stated in the AUTHORS file
 *
 * This file is part of open62541. open62541 is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser General
 * Public License, version 3 (as published by the Free Software Foundation) with
 * a static linking exception as stated in the LICENSE file provided with
 * open62541.
 *
 * open62541 is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 */


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @defgroup logging Logging
 *
 * @brief Custom logging solutions can be "plugged in" with this interface
 *
 * @{
 */

typedef enum {
    UA_LOGLEVEL_TRACE,
    UA_LOGLEVEL_DEBUG,
    UA_LOGLEVEL_INFO,
    UA_LOGLEVEL_WARNING,
    UA_LOGLEVEL_ERROR,
    UA_LOGLEVEL_FATAL
} UA_LogLevel;

typedef enum {
    UA_LOGCATEGORY_COMMUNICATION,
    UA_LOGCATEGORY_SERVER,
    UA_LOGCATEGORY_CLIENT,
    UA_LOGCATEGORY_USERLAND
} UA_LogCategory;
    
typedef void (*UA_Logger)(UA_LogLevel level, UA_LogCategory category, const char *msg, ...);

#if UA_LOGLEVEL <= 100
#define UA_LOG_TRACE(LOGGER, CATEGORY, ...) do { \
        if(LOGGER) LOGGER(UA_LOGLEVEL_TRACE, CATEGORY, __VA_ARGS__); } while(0)
#else
#define UA_LOG_TRACE(LOGGER, CATEGORY, ...) do {} while(0)
#endif

#if UA_LOGLEVEL <= 200
#define UA_LOG_DEBUG(LOGGER, CATEGORY, ...) do { \
        if(LOGGER) LOGGER(UA_LOGLEVEL_DEBUG, CATEGORY, __VA_ARGS__); } while(0)
#else
#define UA_LOG_DEBUG(LOGGER, CATEGORY, ...) do {} while(0)
#endif

#if UA_LOGLEVEL <= 300
#define UA_LOG_INFO(LOGGER, CATEGORY, ...) do { \
        if(LOGGER) LOGGER(UA_LOGLEVEL_INFO, CATEGORY, __VA_ARGS__); } while(0)
#else
#define UA_LOG_INFO(LOGGER, CATEGORY, ...) do {} while(0)
#endif

#if UA_LOGLEVEL <= 400
#define UA_LOG_WARNING(LOGGER, CATEGORY, ...) do { \
        if(LOGGER) LOGGER(UA_LOGLEVEL_WARNING, CATEGORY, __VA_ARGS__); } while(0)
#else
#define UA_LOG_WARNING(LOGGER, CATEGORY, ...) do {} while(0)
#endif

#if UA_LOGLEVEL <= 500
#define UA_LOG_ERROR(LOGGER, CATEGORY, ...) do { \
        if(LOGGER) LOGGER(UA_LOGLEVEL_ERROR, CATEGORY, __VA_ARGS__); } while(0)
#else
#define UA_LOG_ERROR(LOGGER, CATEGORY, ...) do {} while(0)
#endif

#if UA_LOGLEVEL <= 600
#define UA_LOG_FATAL(LOGGER, CATEGORY, ...) do { \
        if(LOGGER) LOGGER(UA_LOGLEVEL_FATAL, CATEGORY, __VA_ARGS__); } while(0)
#else
#define UA_LOG_FATAL(LOGGER, CATEGORY, ...) do {} while(0)
#endif

/** @} */

#ifdef __cplusplus
} // extern "C"
#endif


/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/include/ua_server.h" ***********************************/

 /*
 * Copyright (C) 2014 the contributors as stated in the AUTHORS file
 *
 * This file is part of open62541. open62541 is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser General
 * Public License, version 3 (as published by the Free Software Foundation) with
 * a static linking exception as stated in the LICENSE file provided with
 * open62541.
 *
 * open62541 is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 */


#ifdef __cplusplus
extern "C" {
#endif

  
/**
 * @defgroup server Server
 *
 * @{
 */

typedef struct UA_ServerConfig {
    UA_Boolean  Login_enableAnonymous;

    UA_Boolean  Login_enableUsernamePassword;
    char**      Login_usernames;
    char**      Login_passwords;
    UA_UInt32   Login_loginsCount;

    char*       Application_applicationURI;
    char*       Application_applicationName;
} UA_ServerConfig;

extern UA_EXPORT const UA_ServerConfig UA_ServerConfig_standard;

struct UA_Server;
typedef struct UA_Server UA_Server;

UA_Server UA_EXPORT * UA_Server_new(UA_ServerConfig config);
void UA_EXPORT UA_Server_setServerCertificate(UA_Server *server, UA_ByteString certificate);
void UA_EXPORT UA_Server_delete(UA_Server *server);

/** Sets the logger used by the server */
void UA_EXPORT UA_Server_setLogger(UA_Server *server, UA_Logger logger);
UA_Logger UA_EXPORT UA_Server_getLogger(UA_Server *server);

/**
 * Runs the main loop of the server. In each iteration, this calls into the networklayers to see if
 * jobs have arrived and checks if repeated jobs need to be triggered.
 *
 * @param server The server object
 *
 * @param nThreads The number of worker threads. Is ignored if MULTITHREADING is not activated.
 *
 * @param running Points to a boolean value on the heap. When running is set to false, the worker
 * threads and the main loop close and the server is shut down.
 *
 * @return Indicates whether the server shut down cleanly
 *
 */
UA_StatusCode UA_EXPORT UA_Server_run(UA_Server *server, UA_UInt16 nThreads, UA_Boolean *running);

/* The prologue part of UA_Server_run (no need to use if you call UA_Server_run) */
UA_StatusCode UA_EXPORT UA_Server_run_startup(UA_Server *server, UA_UInt16 nThreads, UA_Boolean *running);
/* The epilogue part of UA_Server_run (no need to use if you call UA_Server_run) */
UA_StatusCode UA_EXPORT UA_Server_run_shutdown(UA_Server *server, UA_UInt16 nThreads);
/* One iteration of UA_Server_run (no need to use if you call UA_Server_run) */
UA_StatusCode UA_EXPORT UA_Server_run_mainloop(UA_Server *server, UA_Boolean *running);

/**
 * Datasources are the interface to local data providers. It is expected that
 * the read and release callbacks are implemented. The write callback can be set
 * to null.
 */
typedef struct {
    void *handle; ///> A custom pointer to reuse the same datasource functions for multiple sources

    /**
     * Copies the data from the source into the provided value.
     *
     * @param handle An optional pointer to user-defined data for the specific data source
     * @param includeSourceTimeStamp If true, then the datasource is expected to set the source
     *        timestamp in the returned value
     * @param range If not null, then the datasource shall return only a selection of the (nonscalar)
     *        data. Set UA_STATUSCODE_BADINDEXRANGEINVALID in the value if this does not apply.
     * @param value The (non-null) DataValue that is returned to the client. The data source sets the
     *        read data, the result status and optionally a sourcetimestamp.
     * @return Returns a status code for logging. Error codes intended for the original caller are set
     *         in the value. If an error is returned, then no releasing of the value is done.
     */
    UA_StatusCode (*read)(void *handle, UA_Boolean includeSourceTimeStamp, const UA_NumericRange *range, UA_DataValue *value);

    /**
     * Write into a data source. The write member of UA_DataSource can be empty if the operation
     * is unsupported.
     *
     * @param handle An optional pointer to user-defined data for the specific data source
     * @param data The data to be written into the data source
     * @param range An optional data range. If the data source is scalar or does not support writing
     *        of ranges, then an error code is returned.
     * @return Returns a status code that is returned to the user
     */
    UA_StatusCode (*write)(void *handle, const UA_Variant *data, const UA_NumericRange *range);
} UA_DataSource;

/** @brief Add a new namespace to the server. Returns the index of the new namespace */
UA_UInt16 UA_EXPORT UA_Server_addNamespace(UA_Server *server, const char* name);

/** Add a reference to the server's address space */
UA_StatusCode UA_EXPORT UA_Server_addReference(UA_Server *server, const UA_NodeId sourceId,
                                               const UA_NodeId refTypeId, const UA_ExpandedNodeId targetId);

/** Deletes a node from the nodestore.
 *
 * @param server The server object
 * @param nodeId ID of the node to be deleted

 * @return Return UA_STATUSCODE_GOOD if the node was deleted or an appropriate errorcode if the node was not found
 *         or cannot be deleted.
 */
UA_StatusCode UA_EXPORT
UA_Server_deleteNode(UA_Server *server, UA_NodeId nodeId);

#define UA_SERVER_DELETENODEALIAS_DECL(TYPE) \
UA_StatusCode UA_EXPORT UA_Server_delete##TYPE##Node(UA_Server *server, UA_NodeId nodeId);

UA_SERVER_DELETENODEALIAS_DECL(Object)
UA_SERVER_DELETENODEALIAS_DECL(Variable)
UA_SERVER_DELETENODEALIAS_DECL(ReferenceType)
UA_SERVER_DELETENODEALIAS_DECL(View)
UA_SERVER_DELETENODEALIAS_DECL(VariableType)
UA_SERVER_DELETENODEALIAS_DECL(DataType)

#ifdef ENABLE_METHODCALLS
UA_SERVER_DELETENODEALIAS_DECL(Method)
#endif

/** Deletes a copied instance of a node by deallocating it and all its attributes. This assumes that the node was
 * priorly copied using getNodeCopy. To delete nodes that are located in the nodestore, use UA_Server_deleteNode()
 * instead.
 *
 * @param server The server object
 * @param node   A copy of any node-type struct created with getNodeCopy; must *not* be managed by the nodestore.
 * 
 * @return Return UA_STATUSCODE_GOOD if the node was deleted or an appropriate errorcode if the node was not found
 *         or cannot be deleted.
 */
UA_StatusCode UA_EXPORT 
UA_Server_deleteNodeCopy(UA_Server *server, void **node);

/** Creates a deep copy of a node located in the nodestore and returns it to the userspace. Note that any manipulation
 * of this copied node is not reflected by the server, but otherwise not accessible attributes of the node's struct
 * can be examined in bulk. node->nodeClass can be used to cast the node to a specific node type. Use 
 * UA_Server_deleteNodeCopy() to deallocate this node.
 *
 * @param server The server object
 * @param nodeId ID of the node copy to be copied
 * @param copyInto Pointer to a NULL pointer that will hold the copy of the node on a successfull return.
 * 
 * @return Return UA_STATUSCODE_GOOD if the node was copied or an appropriate errorcode if the node was not found
 *         or cannot be copied.
 */
UA_StatusCode UA_EXPORT 
UA_Server_getNodeCopy(UA_Server *server, UA_NodeId nodeId, void **copyInto);

/** A new variable Node with a value passed in variant.
 *
 * @param server The server object
 * @param nodeId        The requested nodeId of the new node. Use the numeric id with i=0 to get a new ID from the server.
 * @param browseName    The qualified name of this node
 * @param displayName   The localized text shown when displaying the node
 * @param description   The localized human readable description
 * @param userWriteMask Bitmask defining the user write permissions
 * @param writeMask     Bitmask defining the write permissions
 * @param parentNodeId  The node under which this node exists ("parent")
 * @param referenceTypeId Reference type used by the parent to reference this node
 * @param value         A variant containing the value to be assigned to this node.
 * @param createdNodeId Pointer to a NULL pointer that will hold the copy of the nodeId on a successfull return.
 * 
 * @return Return UA_STATUSCODE_GOOD if the node was created or an appropriate error code if not.
 */
UA_StatusCode UA_EXPORT
UA_Server_addVariableNode(UA_Server *server, const UA_NodeId nodeId, const UA_QualifiedName browseName,
                          const UA_LocalizedText displayName, const UA_LocalizedText description, const UA_UInt32 userWriteMask, const UA_UInt32 writeMask,

                          const UA_NodeId parentNodeId, const UA_NodeId referenceTypeId,

                          UA_Variant *value,

                          UA_NodeId *createdNodeId);

// Missing: eventNotifier
UA_StatusCode UA_EXPORT
UA_Server_addObjectNode(UA_Server *server, const UA_NodeId nodeId, const UA_QualifiedName browseName,
                        const UA_LocalizedText displayName, const UA_LocalizedText description, const UA_UInt32 userWriteMask, const UA_UInt32 writeMask,

                        const UA_NodeId parentNodeId, const UA_NodeId referenceTypeId,

                        const UA_ExpandedNodeId typeDefinition,

                        UA_NodeId *createdNodeId);

// Missing: isAbstract, symmetric
UA_StatusCode UA_EXPORT 
UA_Server_addReferenceTypeNode(UA_Server *server, const UA_NodeId nodeId, const UA_QualifiedName browseName,
                               const UA_LocalizedText displayName, const UA_LocalizedText description, const UA_UInt32 userWriteMask, const UA_UInt32 writeMask,

                               const UA_NodeId parentNodeId, const UA_NodeId referenceTypeId,

                               const UA_ExpandedNodeId typeDefinition,
                               const UA_LocalizedText inverseName,

                               UA_NodeId *createdNodeId );

UA_StatusCode UA_EXPORT
UA_Server_addObjectTypeNode(UA_Server *server, const UA_NodeId nodeId, const UA_QualifiedName browseName,
                            const UA_LocalizedText displayName, const UA_LocalizedText description,  const UA_UInt32 userWriteMask, const UA_UInt32 writeMask,

                            const UA_NodeId parentNodeId, const UA_NodeId referenceTypeId,

                            const UA_ExpandedNodeId typeDefinition,
                            const UA_Boolean isAbstract,

                            UA_NodeId *createdNodeId );

UA_StatusCode UA_EXPORT 
UA_Server_addVariableTypeNode(UA_Server *server, const UA_NodeId nodeId, const UA_QualifiedName browseName,
                              const UA_LocalizedText displayName, const UA_LocalizedText description, const UA_UInt32 userWriteMask, const UA_UInt32 writeMask,

                              const UA_NodeId parentNodeId,
                              const UA_NodeId referenceTypeId,

                              UA_Variant *value,
                              const UA_Int32 valueRank,
                              const UA_Boolean isAbstract,

                              UA_NodeId *createdNodeId);

UA_StatusCode UA_EXPORT
UA_Server_addDataTypeNode(UA_Server *server, const UA_NodeId nodeId, const UA_QualifiedName browseName,
                          const UA_LocalizedText displayName, const UA_LocalizedText description, const UA_UInt32 userWriteMask, const UA_UInt32 writeMask,

                          const UA_NodeId parentNodeId,
                          const UA_NodeId referenceTypeId,

                          const UA_ExpandedNodeId typeDefinition,
                          const UA_Boolean isAbstract,

                          UA_NodeId *createdNodeId);


UA_StatusCode UA_EXPORT
UA_Server_addViewNode(UA_Server *server, const UA_NodeId nodeId, const UA_QualifiedName browseName,
                      const UA_LocalizedText displayName, const UA_LocalizedText description, const UA_UInt32 userWriteMask, const UA_UInt32 writeMask,

                      const UA_NodeId parentNodeId,
                      const UA_NodeId referenceTypeId,

                      const UA_ExpandedNodeId typeDefinition,

                      UA_NodeId *createdNodeId);

UA_StatusCode UA_EXPORT
UA_Server_addDataSourceVariableNode(UA_Server *server, const UA_NodeId nodeId, const UA_QualifiedName browseName,
                                    const UA_LocalizedText displayName, const UA_LocalizedText description,  const UA_UInt32 userWriteMask, const UA_UInt32 writeMask,

                                    const UA_NodeId parentNodeId,
                                    const UA_NodeId referenceTypeId,

                                    const UA_DataSource dataSource,

                                    UA_NodeId *createdNodeId);

/* --------------------- */
UA_StatusCode UA_EXPORT
UA_Server_addMonodirectionalReference(UA_Server *server, UA_NodeId sourceNodeId,
                                      UA_ExpandedNodeId targetNodeId, UA_NodeId referenceTypeId,
                                      UA_Boolean isforward);

#ifdef ENABLE_METHODCALLS
typedef UA_StatusCode (*UA_MethodCallback)(const UA_NodeId objectId, const UA_Variant *input,
                                           UA_Variant *output, void *handle);
/** Creates a serverside method including input- and output variable descriptions
 * 
 * @param server The server object.
 * 
 * @param browseName BrowseName to be used for the new method.
 * 
 * @param nodeId Requested NodeId for the new method. If a numeric ID with i=0 is used, the server will assign a random unused id.
 * 
 * @param parentNodeId Parent node containing this method. Note that an ObjectNode needs to reference the method with hasProperty in order for the method to be callable.
 * 
 * @param referenceTypeId Reference type ID to be used by the parent to reference the new method.
 * 
 * @param method Userspace Method/Function of type UA_MethodCallback to be called when a client invokes the method using the Call Service Set.
 * 
 * @param inputArgumentsSize Number of input arguments expected to be passed by a calling client.
 * 
 * @param inputArguments Description of input arguments expected to be passed by a calling client.
 * 
 * @param outputArgumentsSize Description of output arguments expected to be passed by a calling client.
 * 
 * @param outputArguments Description of output arguments expected to be passed by a calling client.
 * 
 * @param createdNodeId Actual nodeId of the new method node if UA_StatusCode indicates success. Can be used to determine the random unique ID assigned by the server if i=0 was passed as a nodeId.
 * 
 */
UA_StatusCode UA_EXPORT
UA_Server_addMethodNode(UA_Server *server, const UA_NodeId nodeId, const UA_QualifiedName browseName,
                        UA_LocalizedText displayName, UA_LocalizedText description, const UA_NodeId parentNodeId, 
                        const UA_NodeId referenceTypeId, UA_UInt32 userWriteMask, UA_UInt32 writeMask, 
                        UA_MethodCallback method, void *handle, UA_Int32 inputArgumentsSize, const UA_Argument *inputArguments, 
                        UA_Int32 outputArgumentsSize, const UA_Argument *outputArguments,
                        UA_NodeId *createdNodeId);
#endif

#ifndef _HAVE_UA_NODEITERATORCALLBACK_D
#define _HAVE_UA_NODEITERATORCALLBACK_D
typedef UA_StatusCode (*UA_NodeIteratorCallback)(UA_NodeId childId, UA_Boolean isInverse, UA_NodeId referenceTypeId, void *handle);
#endif

/** Iterate over all nodes referenced by parentNodeId by calling the callback function for each child node
 * 
 * @param server The server object.
 *
 * @param parentNodeId The NodeId of the parent whose references are to be iterated over
 *
 * @param callback The function of type UA_NodeIteratorCallback to be called for each referenced child
 *
 * @return Upon success, UA_STATUSCODE_GOOD is returned. An error code otherwise.
 */
UA_StatusCode UA_EXPORT UA_Server_forEachChildNodeCall(UA_Server *server, UA_NodeId parentNodeId, UA_NodeIteratorCallback callback, void *handle);

UA_StatusCode UA_EXPORT UA_Server_setAttributeValue(UA_Server *server, UA_NodeId nodeId, UA_AttributeId attributeId, void *value);
// Attribute specific macros for setAttribute_are defined in ua_server_addressspace.c
#define UA_Server_setAttribute_nodeId(SERVER, NODEID, VALUE) UA_Server_setAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_NODEID, (UA_NodeId *) VALUE);
#define UA_Server_setAttribute_nodeClass(SERVER, NODEID, VALUE) UA_Server_setAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_NODECLASS, (UA_NodeClass *) VALUE);
#define UA_Server_setAttribute_browseName(SERVER, NODEID, VALUE) UA_Server_setAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_BROWSENAME, (UA_QualifiedName *) VALUE);
#define UA_Server_setAttribute_displayName(SERVER, NODEID, VALUE) UA_Server_setAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_DISPLAYNAME, (UA_LocalizedText *) VALUE);
#define UA_Server_setAttribute_description(SERVER, NODEID, VALUE) UA_Server_setAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_DESCRIPTION, (UA_LocalizedText *) VALUE);
#define UA_Server_setAttribute_writeMask(SERVER, NODEID, VALUE) UA_Server_setAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_WRITEMASK, (UA_UInt32 *) VALUE);
#define UA_Server_setAttribute_userWriteMask(SERVER, NODEID, VALUE) UA_Server_setAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_USERWRITEMASK, (UA_UInt32 *) VALUE);
#define UA_Server_setAttribute_isAbstract(SERVER, NODEID, VALUE) UA_Server_setAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_ISABSTRACT, (UA_Boolean *) VALUE);
#define UA_Server_setAttribute_symmetric(SERVER, NODEID, VALUE) UA_Server_setAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_SYMMETRIC, (UA_Boolean *) VALUE);
#define UA_Server_setAttribute_inverseName(SERVER, NODEID, VALUE) UA_Server_setAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_INVERSENAME, (UA_LocalizedText *) VALUE);
#define UA_Server_setAttribute_containsNoLoops(SERVER, NODEID, VALUE) UA_Server_setAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_CONTAINSNOLOOPS, (UA_Boolean *) VALUE);
#define UA_Server_setAttribute_eventNotifier(SERVER, NODEID, VALUE) UA_Server_setAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_EVENTNOTIFIER, (UA_Byte *) VALUE);
#define UA_Server_setAttribute_value(SERVER, NODEID, VALUE) UA_Server_setAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_VALUE, (UA_Variant *) VALUE);
#define UA_Server_setAttribute_dataType(SERVER, NODEID, VALUE) UA_Server_setAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_DATATYPE, (UA_NodeId *) VALUE);
#define UA_Server_setAttribute_valueRank(SERVER, NODEID, VALUE) UA_Server_setAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_VALUERANK, (UA_Int32 *) VALUE);
#define UA_Server_setAttribute_arrayDimensions(SERVER, NODEID, VALUE) UA_Server_setAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_ARRAYDIMENSIONS, (UA_Int32 *) VALUE);
#define UA_Server_setAttribute_accessLevel(SERVER, NODEID, VALUE) UA_Server_setAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_ACCESSLEVEL, (UA_UInt32 *) VALUE);
#define UA_Server_setAttribute_userAccessLevel(SERVER, NODEID, VALUE) UA_Server_setAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_USERACCESSLEVEL, (UA_UInt32 *) VALUE);
#define UA_Server_setAttribute_minimumSamplingInterval(SERVER, NODEID, VALUE) UA_Server_setAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_MINIMUMSAMPLINGINTERVAL, (UA_Double *) VALUE);
#define UA_Server_setAttribute_historizing(SERVER, NODEID, VALUE) UA_Server_setAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_HISTORIZING, (UA_Boolean *) VALUE);
#define UA_Server_setAttribute_executable(SERVER, NODEID, VALUE) UA_Server_setAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_EXECUTABLE, (UA_Boolean *) VALUE);
#define UA_Server_setAttribute_userExecutable(SERVER, NODEID, VALUE) UA_Server_setAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_USEREXECUTABLE, (UA_Boolean *) VALUE);

#ifdef ENABLE_METHODCALLS
UA_StatusCode UA_EXPORT
UA_Server_setAttribute_method(UA_Server *server, UA_NodeId methodNodeId, UA_MethodCallback method, void *handle);
#endif

UA_StatusCode UA_EXPORT
UA_Server_setAttribute_DataSource(UA_Server *server, UA_NodeId nodeId, UA_DataSource *value);

UA_StatusCode UA_EXPORT
UA_Server_getAttributeValue(UA_Server *server, UA_NodeId nodeId, UA_AttributeId attributeId, void **value);
#define UA_Server_getAttribute_nodeId(SERVER, NODEID, VALUE) UA_Server_getAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_NODEID, (UA_NodeId **) VALUE);
#define UA_Server_getAttribute_nodeClass(SERVER, NODEID, VALUE) UA_Server_getAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_NODECLASS, (UA_NodeClass **) VALUE);
#define UA_Server_getAttribute_browseName(SERVER, NODEID, VALUE) UA_Server_getAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_BROWSENAME, (UA_QualifiedName **) VALUE);
#define UA_Server_getAttribute_displayName(SERVER, NODEID, VALUE) UA_Server_getAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_DISPLAYNAME, (UA_LocalizedText **) VALUE);
#define UA_Server_getAttribute_description(SERVER, NODEID, VALUE) UA_Server_getAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_DESCRIPTION, (UA_LocalizedText **) VALUE);
#define UA_Server_getAttribute_writeMask(SERVER, NODEID, VALUE) UA_Server_getAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_WRITEMASK, (UA_UInt32 **) VALUE);
#define UA_Server_getAttribute_userWriteMask(SERVER, NODEID, VALUE) UA_Server_getAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_USERWRITEMASK, (UA_UInt32 **) VALUE);
#define UA_Server_getAttribute_isAbstract(SERVER, NODEID, VALUE) UA_Server_getAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_ISABSTRACT, (UA_Boolean **) VALUE);
#define UA_Server_getAttribute_symmetric(SERVER, NODEID, VALUE) UA_Server_getAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_SYMMETRIC, (UA_Boolean **) VALUE);
#define UA_Server_getAttribute_inverseName(SERVER, NODEID, VALUE) UA_Server_getAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_INVERSENAME, (UA_LocalizedText **) VALUE);
#define UA_Server_getAttribute_containsNoLoops(SERVER, NODEID, VALUE) UA_Server_getAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_CONTAINSNOLOOPS, (UA_Boolean **) VALUE);
#define UA_Server_getAttribute_eventNotifier(SERVER, NODEID, VALUE) UA_Server_getAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_EVENTNOTIFIER, (UA_Byte **) VALUE);
#define UA_Server_getAttribute_value(SERVER, NODEID, VALUE) UA_Server_getAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_VALUE, (UA_Variant **) VALUE);
#define UA_Server_getAttribute_dataType(SERVER, NODEID, VALUE) UA_Server_getAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_DATATYPE, (UA_NodeId **) VALUE);
#define UA_Server_getAttribute_valueRank(SERVER, NODEID, VALUE) UA_Server_getAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_VALUERANK, (UA_Int32 **) VALUE);
#define UA_Server_getAttribute_arrayDimensions(SERVER, NODEID, VALUE) UA_Server_getAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_ARRAYDIMENSIONS, (UA_Int32 **) VALUE);
#define UA_Server_getAttribute_accessLevel(SERVER, NODEID, VALUE) UA_Server_getAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_ACCESSLEVEL, (UA_UInt32 **) VALUE);
#define UA_Server_getAttribute_userAccessLevel(SERVER, NODEID, VALUE) UA_Server_getAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_USERACCESSLEVEL, (UA_UInt32 **) VALUE);
#define UA_Server_getAttribute_minimumSamplingInterval(SERVER, NODEID, VALUE) UA_Server_getAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_MINIMUMSAMPLINGINTERVAL, (UA_Double **) VALUE);
#define UA_Server_getAttribute_historizing(SERVER, NODEID, VALUE) UA_Server_getAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_HISTORIZING, (UA_Boolean **) VALUE);
#define UA_Server_getAttribute_executable(SERVER, NODEID, VALUE) UA_Server_getAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_EXECUTABLE, (UA_Boolean **) VALUE);
#define UA_Server_getAttribute_userExecutable(SERVER, NODEID, VALUE) UA_Server_getAttributeValue(SERVER, NODEID, UA_ATTRIBUTEID_USEREXECUTABLE, (UA_Boolean **) VALUE);

#ifdef ENABLE_METHODCALLS
UA_StatusCode UA_EXPORT
UA_Server_getAttribute_method(UA_Server *server, UA_NodeId methodNodeId, UA_MethodCallback *method);
#endif

UA_StatusCode UA_EXPORT
UA_Server_getAttribute_DataSource(UA_Server *server, UA_NodeId nodeId, UA_DataSource **value);

/** Jobs describe work that is executed once or repeatedly. */
typedef struct {
    enum {
        UA_JOBTYPE_NOTHING,
        UA_JOBTYPE_DETACHCONNECTION,
        UA_JOBTYPE_BINARYMESSAGE,
        UA_JOBTYPE_METHODCALL,
        UA_JOBTYPE_DELAYEDMETHODCALL,
    } type;
    union {
        UA_Connection *closeConnection;
        struct {
            UA_Connection *connection;
            UA_ByteString message;
        } binaryMessage;
        struct {
            void *data;
            void (*method)(UA_Server *server, void *data);
        } methodCall;
    } job;
} UA_Job;

/**
 * @param server The server object.
 *
 * @param job Pointer to the job that shall be added. The pointer is not freed but copied to an
 *        internal representation.
 *
 * @param interval The job shall be repeatedly executed with the given interval (in ms). The
 *        interval must be larger than 5ms. The first execution occurs at now() + interval at the
 *        latest.
 *
 * @param jobId Set to the guid of the repeated job. This can be used to cancel the job later on. If
 *        the pointer is null, the guid is not set.
 *
 * @return Upon success, UA_STATUSCODE_GOOD is returned. An error code otherwise.
 */
UA_StatusCode UA_EXPORT UA_Server_addRepeatedJob(UA_Server *server, UA_Job job, UA_UInt32 interval,
                                                 UA_Guid *jobId);

/**
 * Remove repeated job. The entry will be removed asynchronously during the
 * next iteration of the server main loop.
 *
 * @param server The server object.
 *
 * @param jobId The id of the job that shall be removed.
 *
 * @return Upon sucess, UA_STATUSCODE_GOOD is returned. An error code otherwise.
 */
UA_StatusCode UA_EXPORT UA_Server_removeRepeatedJob(UA_Server *server, UA_Guid jobId);

/**
 * Interface to the binary network layers. This structure is returned from the
 * function that initializes the network layer. The layer is already bound to a
 * specific port and listening. The functions in the structure are never called
 * in parallel but only sequentially from the server's main loop. So the network
 * layer does not need to be thread-safe.
 */
typedef struct UA_ServerNetworkLayer {
    void *handle;
    UA_String discoveryUrl;

    /**
     * Starts listening on the the networklayer.
     *
     * @param nl The network layer
     * @param logger The logger
     * @return Returns UA_STATUSCODE_GOOD or an error code.
     */
    UA_StatusCode (*start)(struct UA_ServerNetworkLayer *nl, UA_Logger *logger);
    
    /**
     * Gets called from the main server loop and returns the jobs (accumulated messages and close
     * events) for dispatch.
     *
     * @param nl The network layer
     * @param jobs When the returned integer is positive, *jobs points to an array of UA_Job of the
     * returned size.
     * @param timeout The timeout during which an event must arrive in microseconds
     * @return The size of the jobs array. If the result is negative, an error has occurred.
     */
    UA_Int32 (*getJobs)(struct UA_ServerNetworkLayer *nl, UA_Job **jobs, UA_UInt16 timeout);

    /**
     * Closes the network connection and returns all the jobs that need to be finished before the
     * network layer can be safely deleted.
     *
     * @param nl The network layer
     * @param jobs When the returned integer is positive, jobs points to an array of UA_Job of the
     * returned size.
     * @return The size of the jobs array. If the result is negative, an error has occurred.
     */
    UA_Int32 (*stop)(struct UA_ServerNetworkLayer *nl, UA_Job **jobs);

    /** Deletes the network layer. Call only after a successful shutdown. */
    void (*deleteMembers)(struct UA_ServerNetworkLayer *nl);
} UA_ServerNetworkLayer;

/**
 * Adds a network layer to the server. The network layer is destroyed together
 * with the server. Do not use it after adding it as it might be moved around on
 * the heap.
 */
void UA_EXPORT UA_Server_addNetworkLayer(UA_Server *server, UA_ServerNetworkLayer networkLayer);

/** @} */

#ifndef __cplusplus /* the external nodestore does not work with c++ so far */

/**
 * @ingroup nodestore
 *
 * @defgroup external_nodestore External Nodestore
 *
 * @brief An external application that manages its own data and data model
 *
 * To plug in outside data sources, one can use
 *
 * - VariableNodes with a data source (functions that are called for read and write access)
 * - An external nodestore that is mapped to specific namespaces
 *
 * If no external nodestore is defined for a nodeid, it is always looked up in
 * the "local" nodestore of open62541. Namespace Zero is always in the local
 * nodestore.
 *
 * @{
 */

typedef UA_Int32 (*UA_ExternalNodeStore_addNodes)
(void *ensHandle, const UA_RequestHeader *requestHeader, UA_AddNodesItem *nodesToAdd, UA_UInt32 *indices,
 UA_UInt32 indicesSize, UA_AddNodesResult* addNodesResults, UA_DiagnosticInfo *diagnosticInfos);

typedef UA_Int32 (*UA_ExternalNodeStore_addReferences)
(void *ensHandle, const UA_RequestHeader *requestHeader, UA_AddReferencesItem* referencesToAdd,
 UA_UInt32 *indices,UA_UInt32 indicesSize, UA_StatusCode *addReferencesResults,
 UA_DiagnosticInfo *diagnosticInfos);

typedef UA_Int32 (*UA_ExternalNodeStore_deleteNodes)
(void *ensHandle, const UA_RequestHeader *requestHeader, UA_DeleteNodesItem *nodesToDelete, UA_UInt32 *indices,
 UA_UInt32 indicesSize, UA_StatusCode *deleteNodesResults, UA_DiagnosticInfo *diagnosticInfos);

typedef UA_Int32 (*UA_ExternalNodeStore_deleteReferences)
(void *ensHandle, const UA_RequestHeader *requestHeader, UA_DeleteReferencesItem *referenceToDelete,
 UA_UInt32 *indices, UA_UInt32 indicesSize, UA_StatusCode deleteReferencesresults,
 UA_DiagnosticInfo *diagnosticInfos);

typedef UA_Int32 (*UA_ExternalNodeStore_readNodes)
(void *ensHandle, const UA_RequestHeader *requestHeader, UA_ReadValueId *readValueIds, UA_UInt32 *indices,
 UA_UInt32 indicesSize,UA_DataValue *readNodesResults, UA_Boolean timeStampToReturn,
 UA_DiagnosticInfo *diagnosticInfos);

typedef UA_Int32 (*UA_ExternalNodeStore_writeNodes)
(void *ensHandle, const UA_RequestHeader *requestHeader, UA_WriteValue *writeValues, UA_UInt32 *indices,
 UA_UInt32 indicesSize, UA_StatusCode *writeNodesResults, UA_DiagnosticInfo *diagnosticInfo);

typedef UA_Int32 (*UA_ExternalNodeStore_browseNodes)
(void *ensHandle, const UA_RequestHeader *requestHeader, UA_BrowseDescription *browseDescriptions,
 UA_UInt32 *indices, UA_UInt32 indicesSize, UA_UInt32 requestedMaxReferencesPerNode,
 UA_BrowseResult *browseResults, UA_DiagnosticInfo *diagnosticInfos);

typedef UA_Int32 (*UA_ExternalNodeStore_translateBrowsePathsToNodeIds)
(void *ensHandle, const UA_RequestHeader *requestHeader, UA_BrowsePath *browsePath,
 UA_UInt32 *indices, UA_UInt32 indicesSize, UA_BrowsePathResult *browsePathResults, UA_DiagnosticInfo *diagnosticInfos);

typedef UA_Int32 (*UA_ExternalNodeStore_delete)(void *ensHandle);

typedef struct UA_ExternalNodeStore {
    void *ensHandle;
	UA_ExternalNodeStore_addNodes addNodes;
	UA_ExternalNodeStore_deleteNodes deleteNodes;
	UA_ExternalNodeStore_writeNodes writeNodes;
	UA_ExternalNodeStore_readNodes readNodes;
	UA_ExternalNodeStore_browseNodes browseNodes;
	UA_ExternalNodeStore_translateBrowsePathsToNodeIds translateBrowsePathsToNodeIds;
	UA_ExternalNodeStore_addReferences addReferences;
	UA_ExternalNodeStore_deleteReferences deleteReferences;
	UA_ExternalNodeStore_delete destroy;
} UA_ExternalNodeStore;

#ifdef UA_EXTERNAL_NAMESPACES
UA_StatusCode UA_EXPORT
UA_Server_addExternalNamespace(UA_Server *server, UA_UInt16 namespaceIndex, const UA_String *url, UA_ExternalNodeStore *nodeStore);
#endif /* UA_EXTERNAL_NAMESPACES*/
/** @} */

#endif /* external nodestore */

#ifndef _HAVE_UA_INSTANTIONCALLBACK_D
#define _HAVE_UA_INSTANTIONCALLBACK_D
typedef UA_StatusCode (*UA_InstantiationCallback)(UA_NodeId objectId, UA_NodeId definitionId, void *handle);
#endif

typedef struct arrayOfNodeIds_s {
  UA_Int32  size;
  UA_NodeId *ids;
} arrayOfNodeIds;

UA_StatusCode UA_EXPORT
UA_Server_appendInstanceOfSupertype(UA_Server *server, UA_NodeId nodeId, UA_NodeId appendToNodeId, 
                                    arrayOfNodeIds *subtypeRefs, arrayOfNodeIds *componentRefs, 
                                    UA_InstantiationCallback callback, arrayOfNodeIds *instantiatedTypes, 
                                    void *handle);

void UA_EXPORT
UA_Server_addInstanceOf_instatiateChildNode(UA_Server *server, 
                                                 arrayOfNodeIds *subtypeRefs, arrayOfNodeIds *componentRefs, arrayOfNodeIds *typedefRefs,
                                                 UA_NodeId objectRoot, UA_InstantiationCallback callback, void *typeDefNode,
                                                 UA_Boolean instantiateObjects, arrayOfNodeIds *instantiatedTypes, void *handle);
                                                 
UA_StatusCode UA_EXPORT
UA_Server_addInstanceOf(UA_Server *server, UA_NodeId nodeId, const UA_QualifiedName browseName,
                        UA_LocalizedText displayName, UA_LocalizedText description, const UA_NodeId parentNodeId, 
                        const UA_NodeId referenceTypeId, UA_UInt32 userWriteMask, UA_UInt32 writeMask, 
                        const UA_ExpandedNodeId typeDefinition, UA_InstantiationCallback callback, void *handle, 
                        UA_NodeId *createdNodeId);

#ifdef __cplusplus
} // extern "C"
#endif


/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/include/ua_client.h" ***********************************/


#ifdef __cplusplus
extern "C" {
#endif


struct UA_Client;
typedef struct UA_Client UA_Client;

/**
 * The client networklayer is defined by a single function that fills a UA_Connection struct after
 * successfully connecting.
 */
typedef UA_Connection (*UA_ConnectClientConnection)(UA_ConnectionConfig localConf, char *endpointUrl,
                                                    UA_Logger *logger);

typedef struct UA_ClientConfig {
    UA_Int32 timeout; //sync response timeout
    UA_Int32 secureChannelLifeTime; // lifetime in ms (then the channel needs to be renewed)
    UA_Int32 timeToRenewSecureChannel; //time in ms  before expiration to renew the secure channel
    UA_ConnectionConfig localConnectionConfig;
} UA_ClientConfig;

extern UA_EXPORT const UA_ClientConfig UA_ClientConfig_standard;
UA_Client UA_EXPORT * UA_Client_new(UA_ClientConfig config, UA_Logger logger);

UA_EXPORT void UA_Client_reset(UA_Client* client);
UA_EXPORT void UA_Client_init(UA_Client* client, UA_ClientConfig config, UA_Logger logger);
UA_EXPORT void UA_Client_deleteMembers(UA_Client* client);
UA_EXPORT void UA_Client_delete(UA_Client* client);

UA_StatusCode UA_EXPORT UA_Client_connect(UA_Client *client, UA_ConnectClientConnection connFunc, char *endpointUrl);
UA_StatusCode UA_EXPORT UA_Client_disconnect(UA_Client *client);

UA_StatusCode UA_EXPORT UA_Client_renewSecureChannel(UA_Client *client);

/* Attribute Service Set */
UA_ReadResponse UA_EXPORT UA_Client_read(UA_Client *client, UA_ReadRequest *request);
UA_WriteResponse UA_EXPORT UA_Client_write(UA_Client *client, UA_WriteRequest *request);

/* View Service Set */    
UA_BrowseResponse UA_EXPORT UA_Client_browse(UA_Client *client, UA_BrowseRequest *request);
UA_BrowseNextResponse UA_EXPORT UA_Client_browseNext(UA_Client *client, UA_BrowseNextRequest *request);
UA_TranslateBrowsePathsToNodeIdsResponse UA_EXPORT
    UA_Client_translateTranslateBrowsePathsToNodeIds(UA_Client *client,
                                                     UA_TranslateBrowsePathsToNodeIdsRequest *request);

/* NodeManagement Service Set */
UA_AddNodesResponse UA_EXPORT UA_Client_addNodes(UA_Client *client, UA_AddNodesRequest *request);
UA_AddReferencesResponse UA_EXPORT
    UA_Client_addReferences(UA_Client *client, UA_AddReferencesRequest *request);

UA_DeleteNodesResponse UA_EXPORT UA_Client_deleteNodes(UA_Client *client, UA_DeleteNodesRequest *request);
UA_DeleteReferencesResponse UA_EXPORT
    UA_Client_deleteReferences(UA_Client *client, UA_DeleteReferencesRequest *request);


/* Client-Side Macro/Procy functions */
#ifdef ENABLE_METHODCALLS
UA_CallResponse UA_EXPORT UA_Client_call(UA_Client *client, UA_CallRequest *request);
UA_StatusCode UA_EXPORT UA_Client_CallServerMethod(UA_Client *client, UA_NodeId objectNodeId, UA_NodeId methodNodeId,
                                                   UA_Int32 inputSize, const UA_Variant *input,
                                                   UA_Int32 *outputSize, UA_Variant **output);
#endif
    
UA_AddNodesResponse UA_EXPORT *UA_Client_createObjectNode(  UA_Client *client, UA_ExpandedNodeId reqId, UA_QualifiedName browseName, UA_LocalizedText displayName, 
                                                            UA_LocalizedText description, UA_ExpandedNodeId parentNodeId, UA_NodeId referenceTypeId,
                                                            UA_UInt32 userWriteMask, UA_UInt32 writeMask, UA_ExpandedNodeId typeDefinition);

UA_AddNodesResponse UA_EXPORT *UA_Client_createVariableNode(UA_Client *client, UA_ExpandedNodeId reqId, UA_QualifiedName browseName, UA_LocalizedText displayName, 
                                                            UA_LocalizedText description, UA_ExpandedNodeId parentNodeId, UA_NodeId referenceTypeId,
                                                            UA_UInt32 userWriteMask, UA_UInt32 writeMask, UA_ExpandedNodeId typeDefinition, 
                                                            UA_NodeId dataType, UA_Variant *value );

UA_AddNodesResponse UA_EXPORT *UA_Client_createReferenceTypeNode(UA_Client *client, UA_ExpandedNodeId reqId, UA_QualifiedName browseName, UA_LocalizedText displayName, 
                                                            UA_LocalizedText description, UA_ExpandedNodeId parentNodeId, UA_NodeId referenceTypeId,
                                                            UA_UInt32 userWriteMask, UA_UInt32 writeMask, UA_ExpandedNodeId typeDefinition,
                                                            UA_LocalizedText inverseName );

UA_AddNodesResponse UA_EXPORT *UA_Client_createObjectTypeNode(UA_Client *client, UA_ExpandedNodeId reqId, UA_QualifiedName browseName, UA_LocalizedText displayName, 
                                                            UA_LocalizedText description, UA_ExpandedNodeId parentNodeId, UA_NodeId referenceTypeId,
                                                            UA_UInt32 userWriteMask, UA_UInt32 writeMask, UA_ExpandedNodeId typeDefinition);


#ifdef ENABLE_SUBSCRIPTIONS
UA_Int32      UA_EXPORT UA_Client_newSubscription(UA_Client *client, UA_Int32 publishInterval);
UA_StatusCode UA_EXPORT UA_Client_removeSubscription(UA_Client *client, UA_UInt32 subscriptionId);
//void UA_EXPORT UA_Client_modifySubscription(UA_Client *client);
void UA_EXPORT UA_Client_doPublish(UA_Client *client);

UA_UInt32     UA_EXPORT UA_Client_monitorItemChanges(UA_Client *client, UA_UInt32 subscriptionId,
                                                     UA_NodeId nodeId, UA_UInt32 attributeID,
                                                     void *handlingFunction);
UA_StatusCode UA_EXPORT UA_Client_unMonitorItemChanges(UA_Client *client, UA_UInt32 subscriptionId,
                                                       UA_UInt32 monitoredItemId );
#endif

#ifdef __cplusplus
} // extern "C"
#endif


/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/examples/networklayer_tcp.h" ***********************************/

/*
 * This work is licensed under a Creative Commons CCZero 1.0 Universal License.
 * See http://creativecommons.org/publicdomain/zero/1.0/ for more information.
 */


#ifdef __cplusplus
extern "C" {
#endif


/** @brief Create the TCP networklayer and listen to the specified port */
UA_EXPORT UA_ServerNetworkLayer ServerNetworkLayerTCP_new(UA_ConnectionConfig conf, UA_UInt32 port);
UA_EXPORT UA_Connection ClientNetworkLayerTCP_connect(UA_ConnectionConfig conf, char *endpointUrl, UA_Logger *logger);

#ifdef __cplusplus
} // extern "C"
#endif


/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/examples/logger_stdout.h" ***********************************/

/*
 * This work is licensed under a Creative Commons CCZero 1.0 Universal License.
 * See http://creativecommons.org/publicdomain/zero/1.0/ for more information.
 */



/** Initialises the logger for the current thread. */
UA_EXPORT UA_Logger Logger_Stdout_new(void);


#ifdef __cplusplus
} // extern "C"
#endif

#endif /* OPEN62541_H_ */