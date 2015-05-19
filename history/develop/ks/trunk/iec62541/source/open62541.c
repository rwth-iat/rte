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

/* THIS IS A SINGLE-FILE DISTRIBUTION CONCATENATED FROM THE OPEN62541 SOURCES */

#define UA_AMALGAMATE
#ifndef UA_DYNAMIC_LINKING
# define UA_DYNAMIC_LINKING
#endif

#include "open62541.h"

typedef UA_UInt32 hash_t;

static hash_t mod(hash_t h, hash_t size) { return h % size; }
static hash_t mod2(hash_t h, hash_t size) { return 1 + (h % (size - 2)); }

/* Based on Murmur-Hash 3 by Austin Appleby (public domain, freely usable) */
static hash_t hash_array(const UA_Byte *data, UA_UInt32 len, UA_UInt32 seed) {
    if(data == UA_NULL)
        return 0;

    const int32_t   nblocks = len / 4;
    const uint32_t *blocks;
    static const uint32_t c1 = 0xcc9e2d51;
    static const uint32_t c2 = 0x1b873593;
    static const uint32_t r1 = 15;
    static const uint32_t r2 = 13;
    static const uint32_t m  = 5;
    static const uint32_t n  = 0xe6546b64;
    hash_t hash = seed;
    /* Somce compilers emit a warning when casting from a byte array to ints. */
//#if defined(__GNUC__) || defined(__clang__)
//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wcast-align"
//#endif
    blocks = (const uint32_t *)data;
//#if defined(__GNUC__) || defined(__clang__)
//#pragma GCC diagnostic pop
//#endif
    for(int32_t i = 0;i < nblocks;i++) {
        uint32_t k = blocks[i];
        k    *= c1;
        k     = (k << r1) | (k >> (32 - r1));
        k    *= c2;
        hash ^= k;
        hash  = ((hash << r2) | (hash >> (32 - r2))) * m + n;
    }

    const uint8_t *tail = (const uint8_t *)(data + nblocks * 4);
    uint32_t       k1   = 0;
    switch(len & 3) {
    case 3:
        k1 ^= tail[2] << 16;
    case 2:
        k1 ^= tail[1] << 8;
    case 1:
        k1   ^= tail[0];
        k1   *= c1;
        k1    = (k1 << r1) | (k1 >> (32 - r1));
        k1   *= c2;
        hash ^= k1;
    }

    hash ^= len;
    hash ^= (hash >> 16);
    hash *= 0x85ebca6b;
    hash ^= (hash >> 13);
    hash *= 0xc2b2ae35;
    hash ^= (hash >> 16);
    return hash;
}

static hash_t hash(const UA_NodeId *n) {
    switch(n->identifierType) {
    case UA_NODEIDTYPE_NUMERIC:
        /*  Knuth's multiplicative hashing */
        return (n->identifier.numeric + n->namespaceIndex) * 2654435761;   // mod(2^32) is implicit
    case UA_NODEIDTYPE_STRING:
        return hash_array(n->identifier.string.data, n->identifier.string.length, n->namespaceIndex);
    case UA_NODEIDTYPE_GUID:
        return hash_array((const UA_Byte *)&(n->identifier.guid), sizeof(UA_Guid), n->namespaceIndex);
    case UA_NODEIDTYPE_BYTESTRING:
        return hash_array((const UA_Byte *)n->identifier.byteString.data, n->identifier.byteString.length, n->namespaceIndex);
    default:
        UA_assert(UA_FALSE);
        return 0;
    }
}


/*****************/
/* Helper Macros */
/*****************/

#define UA_TYPE_DEFAULT(TYPE)            \
    UA_TYPE_NEW_DEFAULT(TYPE)            \
    UA_TYPE_INIT_DEFAULT(TYPE)           \
    UA_TYPE_DELETEMEMBERS_NOACTION(TYPE) \
    UA_TYPE_DELETE_DEFAULT(TYPE)         \
    UA_TYPE_COPY_DEFAULT(TYPE)           \

#define UA_TYPE_NEW_DEFAULT(TYPE)                              \
    TYPE * TYPE##_new() {                                      \
        TYPE *p = UA_malloc(sizeof(TYPE));                     \
        if(p) TYPE##_init(p);                                  \
        return p;                                              \
    }

#define UA_TYPE_INIT_DEFAULT(TYPE) \
    void TYPE##_init(TYPE * p) {   \
        *p = (TYPE)0;              \
    }

#define UA_TYPE_DELETEMEMBERS_NOACTION(TYPE) \
    void TYPE##_deleteMembers(TYPE *p) {    \
    }

#define UA_TYPE_DELETE_DEFAULT(TYPE) \
    void TYPE##_delete(TYPE *p) {    \
        TYPE##_deleteMembers(p);     \
        UA_free(p);                  \
    }

#define UA_TYPE_COPY_DEFAULT(TYPE)                             \
    UA_StatusCode TYPE##_copy(TYPE const *src, TYPE *dst) {    \
        *dst = *src;                                           \
        return UA_STATUSCODE_GOOD;                             \
    }

/*****************/
/* Builtin Types */
/*****************/

/* Boolean */
UA_TYPE_DEFAULT(UA_Boolean)

/* SByte */
UA_TYPE_DEFAULT(UA_SByte)

/* Byte */
UA_TYPE_DEFAULT(UA_Byte)

/* Int16 */
UA_TYPE_DEFAULT(UA_Int16)

/* UInt16 */
UA_TYPE_DEFAULT(UA_UInt16)

/* Int32 */
UA_TYPE_DEFAULT(UA_Int32)

/* UInt32 */
UA_TYPE_DEFAULT(UA_UInt32)

/* Int64 */
UA_TYPE_DEFAULT(UA_Int64)

/* UInt64 */
UA_TYPE_DEFAULT(UA_UInt64)

/* Float */
UA_TYPE_DEFAULT(UA_Float)

/* Double */
UA_TYPE_DEFAULT(UA_Double)

/* String */
UA_TYPE_NEW_DEFAULT(UA_String)
UA_TYPE_DELETE_DEFAULT(UA_String)

void UA_String_init(UA_String *p) {
    p->length = -1;
    p->data   = UA_NULL;
}

void UA_String_deleteMembers(UA_String *p) {
    UA_free(p->data);
    p->data = UA_NULL;
}

UA_StatusCode UA_String_copy(UA_String const *src, UA_String *dst) {
    UA_String_init(dst);
    if(src->length > 0) {
        if(!(dst->data = UA_malloc((UA_UInt32)src->length)))
            return UA_STATUSCODE_BADOUTOFMEMORY;
        UA_memcpy((void *)dst->data, src->data, (UA_UInt32)src->length);
    }
    dst->length = src->length;
    return UA_STATUSCODE_GOOD;
}

UA_String UA_String_fromChars(char const *src) {
    UA_String str;
    size_t length = strlen(src);
    if(length == 0) {
        str.length = 0;
        str.data = UA_NULL;
        return str;
    }
    str.data = UA_malloc(length);
    if(!str.data) {
        str.length = -1;
        return str;
    }
    UA_memcpy(str.data, src, length);
    str.length = length;
    return str;
}

#define UA_STRING_COPYPRINTF_BUFSIZE 1024
UA_StatusCode UA_String_copyprintf(char const *fmt, UA_String *dst, ...) {
    char src[UA_STRING_COPYPRINTF_BUFSIZE];
    va_list ap;
    va_start(ap, dst);
//#if defined(__GNUC__) || defined(__clang__)
//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wformat-nonliteral"
//#endif
    // vsnprintf should only take a literal and no variable to be secure
    UA_Int32 len = vsnprintf(src, UA_STRING_COPYPRINTF_BUFSIZE, fmt, ap);
//#if defined(__GNUC__) || defined(__clang__)
//#pragma GCC diagnostic pop
//#endif
    va_end(ap);
    if(len < 0)  // FIXME: old glibc 2.0 would return -1 when truncated
        return UA_STATUSCODE_BADINTERNALERROR;
    // since glibc 2.1 vsnprintf returns the len that would have resulted if buf were large enough
    len = ( len > UA_STRING_COPYPRINTF_BUFSIZE ? UA_STRING_COPYPRINTF_BUFSIZE : len );
    if(!(dst->data = UA_malloc((UA_UInt32)len)))
        return UA_STATUSCODE_BADOUTOFMEMORY;
    UA_memcpy((void *)dst->data, src, (UA_UInt32)len);
    dst->length = len;
    return UA_STATUSCODE_GOOD;
}

UA_Boolean UA_String_equal(const UA_String *string1, const UA_String *string2) {
    if(string1->length <= 0 && string2->length <= 0)
        return UA_TRUE;
    if(string1->length != string2->length)
        return UA_FALSE;

    // casts are needed to overcome signed warnings
    UA_Int32 is = strncmp((char const *)string1->data, (char const *)string2->data, (size_t)string1->length);
    return (is == 0) ? UA_TRUE : UA_FALSE;
}

/* DateTime */
#define UNIX_EPOCH_BIAS_SEC 11644473600LL // Number of seconds from 1 Jan. 1601 00:00 to 1 Jan 1970 00:00 UTC
#define HUNDRED_NANOSEC_PER_USEC 10LL
#define HUNDRED_NANOSEC_PER_SEC (HUNDRED_NANOSEC_PER_USEC * 1000000LL)

#ifdef __MINGW32__
#ifndef _TIMEZONE_DEFINED
#define _TIMEZONE_DEFINED
struct timezone {
  int tz_minuteswest;
  int tz_dsttime;
};
#endif
#endif
#ifdef _WIN32
static const unsigned __int64 epoch = 116444736000000000;
int gettimeofday(struct timeval *tp, struct timezone *tzp);
int gettimeofday(struct timeval *tp, struct timezone *tzp) {
    FILETIME       ft;
    SYSTEMTIME     st;
    ULARGE_INTEGER ul;
    GetSystemTime(&st);
    SystemTimeToFileTime(&st, &ft);
    ul.LowPart  = ft.dwLowDateTime;
    ul.HighPart = ft.dwHighDateTime;
    tp->tv_sec  = (ul.QuadPart - epoch) / 10000000L;
    tp->tv_usec = st.wMilliseconds * 1000;
    return 0;
}
#endif

UA_DateTime UA_DateTime_now() {
    UA_DateTime    dateTime;
    struct timeval tv;
    gettimeofday(&tv, UA_NULL);
    dateTime = (tv.tv_sec + UNIX_EPOCH_BIAS_SEC)
               * HUNDRED_NANOSEC_PER_SEC + tv.tv_usec * HUNDRED_NANOSEC_PER_USEC;
    return dateTime;
}

UA_DateTimeStruct UA_DateTime_toStruct(UA_DateTime atime) {
    UA_DateTimeStruct dateTimeStruct;
    //calcualting the the milli-, micro- and nanoseconds
    dateTimeStruct.nanoSec  = (UA_Int16)((atime % 10) * 100);
    dateTimeStruct.microSec = (UA_Int16)((atime % 10000) / 10);
    dateTimeStruct.milliSec = (UA_Int16)((atime % 10000000) / 10000);

    //calculating the unix time with #include <time.h>
    time_t secSinceUnixEpoch = (atime/10000000) - UNIX_EPOCH_BIAS_SEC;
    struct tm ts = *gmtime(&secSinceUnixEpoch);
    dateTimeStruct.sec    = (UA_Int16)ts.tm_sec;
    dateTimeStruct.min    = (UA_Int16)ts.tm_min;
    dateTimeStruct.hour   = (UA_Int16)ts.tm_hour;
    dateTimeStruct.day    = (UA_Int16)ts.tm_mday;
    dateTimeStruct.month  = (UA_Int16)(ts.tm_mon + 1);
    dateTimeStruct.year   = (UA_Int16)(ts.tm_year + 1900);
    return dateTimeStruct;
}

UA_StatusCode UA_DateTime_toString(UA_DateTime atime, UA_String *timeString) {
    // length of the string is 31 (incl. \0 at the end)
    if(!(timeString->data = UA_malloc(32)))
        return UA_STATUSCODE_BADOUTOFMEMORY;
    timeString->length = 31;

    UA_DateTimeStruct tSt = UA_DateTime_toStruct(atime);
    sprintf((char*)timeString->data, "%02d/%02d/%04d %02d:%02d:%02d.%03d.%03d.%03d", tSt.month, tSt.day, tSt.year,
            tSt.hour, tSt.min, tSt.sec, tSt.milliSec, tSt.microSec, tSt.nanoSec);
    return UA_STATUSCODE_GOOD;
}

/* Guid */
UA_TYPE_NEW_DEFAULT(UA_Guid)
UA_TYPE_DELETEMEMBERS_NOACTION(UA_Guid)
UA_TYPE_DELETE_DEFAULT(UA_Guid)

UA_Boolean UA_Guid_equal(const UA_Guid *g1, const UA_Guid *g2) {
    if(memcmp(g1, g2, sizeof(UA_Guid)) == 0)
        return UA_TRUE;
    return UA_FALSE;
}

UA_Guid UA_Guid_random(UA_UInt32 *seed) {
    UA_Guid result;
    result.data1 = RAND(seed);
    UA_UInt32 r = RAND(seed);
    result.data2 = (UA_UInt16) r;
    result.data3 = (UA_UInt16) (r >> 16);
    r = RAND(seed);
    result.data4[0] = (UA_Byte)r;
    result.data4[1] = (UA_Byte)(r >> 4);
    result.data4[2] = (UA_Byte)(r >> 8);
    result.data4[3] = (UA_Byte)(r >> 12);
    r = RAND(seed);
    result.data4[4] = (UA_Byte)r;
    result.data4[5] = (UA_Byte)(r >> 4);
    result.data4[6] = (UA_Byte)(r >> 8);
    result.data4[7] = (UA_Byte)(r >> 12);
    return result;
}

void UA_Guid_init(UA_Guid *p) {
    p->data1 = 0;
    p->data2 = 0;
    p->data3 = 0;
    memset(p->data4, 0, sizeof(UA_Byte)*8);
}

UA_StatusCode UA_Guid_copy(UA_Guid const *src, UA_Guid *dst) {
    UA_memcpy((void *)dst, (const void *)src, sizeof(UA_Guid));
    return UA_STATUSCODE_GOOD;
}

/* ByteString */
UA_StatusCode UA_ByteString_newMembers(UA_ByteString *p, UA_Int32 length) {
    if(length > 0) {
        if(!(p->data = UA_malloc((UA_UInt32)length)))
            return UA_STATUSCODE_BADOUTOFMEMORY;
        p->length = length;
    } else {
        p->data = UA_NULL;
        if(length == 0)
            p->length = 0;
        else
            p->length = -1;
    }
    return UA_STATUSCODE_GOOD;
}

/* XmlElement */

/* NodeId */
UA_TYPE_NEW_DEFAULT(UA_NodeId)
UA_TYPE_DELETE_DEFAULT(UA_NodeId)

void UA_NodeId_init(UA_NodeId *p) {
    p->identifierType = UA_NODEIDTYPE_NUMERIC;
    p->namespaceIndex = 0;
    memset(&p->identifier, 0, sizeof(p->identifier));
}

UA_StatusCode UA_NodeId_copy(UA_NodeId const *src, UA_NodeId *dst) {
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    switch(src->identifierType) {
    case UA_NODEIDTYPE_NUMERIC:
        *dst = *src;
        return UA_STATUSCODE_GOOD;
    case UA_NODEIDTYPE_STRING: // Table 6, second entry
        retval |= UA_String_copy(&src->identifier.string, &dst->identifier.string);
        break;
    case UA_NODEIDTYPE_GUID: // Table 6, third entry
        retval |= UA_Guid_copy(&src->identifier.guid, &dst->identifier.guid);
        break;
    case UA_NODEIDTYPE_BYTESTRING: // Table 6, "OPAQUE"
        retval |= UA_ByteString_copy(&src->identifier.byteString, &dst->identifier.byteString);
        break;
    default:
        UA_NodeId_init(dst);
        return UA_STATUSCODE_BADINTERNALERROR;
    }
    dst->namespaceIndex = src->namespaceIndex;
    dst->identifierType = src->identifierType;
    if(retval) {
        UA_NodeId_deleteMembers(dst);
        UA_NodeId_init(dst);
    }
    return retval;
}

static UA_Boolean UA_NodeId_isBasicType(UA_NodeId const *id) {
    return id ->namespaceIndex == 0 && 1 <= id ->identifier.numeric &&
        id ->identifier.numeric <= 25;
}

void UA_NodeId_deleteMembers(UA_NodeId *p) {
    switch(p->identifierType) {
    case UA_NODEIDTYPE_STRING:
    case UA_NODEIDTYPE_BYTESTRING:
        UA_ByteString_deleteMembers(&p->identifier.byteString);
        break;
    default:
        break;
    }
}

UA_Boolean UA_NodeId_equal(const UA_NodeId *n1, const UA_NodeId *n2) {
	if(n1->namespaceIndex != n2->namespaceIndex || n1->identifierType!=n2->identifierType)
        return UA_FALSE;

    switch(n1->identifierType) {
    case UA_NODEIDTYPE_NUMERIC:
        if(n1->identifier.numeric == n2->identifier.numeric)
            return UA_TRUE;
        else
            return UA_FALSE;

    case UA_NODEIDTYPE_STRING:
        return UA_String_equal(&n1->identifier.string, &n2->identifier.string);

    case UA_NODEIDTYPE_GUID:
        return UA_Guid_equal(&n1->identifier.guid, &n2->identifier.guid);

    case UA_NODEIDTYPE_BYTESTRING:
        return UA_ByteString_equal(&n1->identifier.byteString, &n2->identifier.byteString);
    }
    return UA_FALSE;
}

UA_Boolean UA_NodeId_isNull(const UA_NodeId *p) {
    switch(p->identifierType) {
    case UA_NODEIDTYPE_NUMERIC:
        if(p->namespaceIndex != 0 || p->identifier.numeric != 0)
            return UA_FALSE;
        break;

    case UA_NODEIDTYPE_STRING:
        if(p->namespaceIndex != 0 || p->identifier.string.length > 0)
            return UA_FALSE;
        break;

    case UA_NODEIDTYPE_GUID:
        if(p->namespaceIndex != 0 ||
           memcmp(&p->identifier.guid, (char[sizeof(UA_Guid)]) { 0 }, sizeof(UA_Guid)) != 0)
            return UA_FALSE;
        break;

    case UA_NODEIDTYPE_BYTESTRING:
        if(p->namespaceIndex != 0 || p->identifier.byteString.length > 0)
            return UA_FALSE;
        break;

    default:
        return UA_FALSE;
    }
    return UA_TRUE;
}

/* ExpandedNodeId */
UA_TYPE_NEW_DEFAULT(UA_ExpandedNodeId)
UA_TYPE_DELETE_DEFAULT(UA_ExpandedNodeId)

void UA_ExpandedNodeId_deleteMembers(UA_ExpandedNodeId *p) {
    UA_NodeId_deleteMembers(&p->nodeId);
    UA_String_deleteMembers(&p->namespaceUri);
}

void UA_ExpandedNodeId_init(UA_ExpandedNodeId *p) {
    UA_NodeId_init(&p->nodeId);
    UA_String_init(&p->namespaceUri);
    p->serverIndex = 0;
}

UA_StatusCode UA_ExpandedNodeId_copy(UA_ExpandedNodeId const *src, UA_ExpandedNodeId *dst) {
    UA_StatusCode retval = UA_NodeId_copy(&src->nodeId, &dst->nodeId);
    retval |= UA_String_copy(&src->namespaceUri, &dst->namespaceUri);
    dst->serverIndex = src->serverIndex;
    if(retval) {
        UA_ExpandedNodeId_deleteMembers(dst);
        UA_ExpandedNodeId_init(dst);
    }
    return retval;
}

UA_Boolean UA_ExpandedNodeId_isNull(const UA_ExpandedNodeId *p) {
    return UA_NodeId_isNull(&p->nodeId);
}

/* StatusCode */

/* QualifiedName */
UA_TYPE_NEW_DEFAULT(UA_QualifiedName)
UA_TYPE_DELETE_DEFAULT(UA_QualifiedName)

void UA_QualifiedName_deleteMembers(UA_QualifiedName *p) {
    UA_String_deleteMembers(&p->name);
}

void UA_QualifiedName_init(UA_QualifiedName *p) {
    UA_String_init(&p->name);
    p->namespaceIndex = 0;
}

UA_StatusCode UA_QualifiedName_copy(UA_QualifiedName const *src, UA_QualifiedName *dst) {
    UA_StatusCode retval = UA_String_copy(&src->name, &dst->name);
    dst->namespaceIndex = src->namespaceIndex;
    if(retval) {
        UA_QualifiedName_deleteMembers(dst);
        UA_QualifiedName_init(dst);
    }
    return retval;
}

/* LocalizedText */
UA_TYPE_NEW_DEFAULT(UA_LocalizedText)
UA_TYPE_DELETE_DEFAULT(UA_LocalizedText)

void UA_LocalizedText_deleteMembers(UA_LocalizedText *p) {
    UA_String_deleteMembers(&p->locale);
    UA_String_deleteMembers(&p->text);
}

void UA_LocalizedText_init(UA_LocalizedText *p) {
    UA_String_init(&p->locale);
    UA_String_init(&p->text);
}

UA_StatusCode UA_LocalizedText_copy(UA_LocalizedText const *src, UA_LocalizedText *dst) {
    UA_StatusCode retval = UA_String_copy(&src->locale, &dst->locale);
    retval |= UA_String_copy(&src->text, &dst->text);
    if(retval) {
        UA_LocalizedText_deleteMembers(dst);
        UA_LocalizedText_init(dst);
    }
    return retval;
}

/* ExtensionObject */
UA_TYPE_NEW_DEFAULT(UA_ExtensionObject)
UA_TYPE_DELETE_DEFAULT(UA_ExtensionObject)

void UA_ExtensionObject_deleteMembers(UA_ExtensionObject *p) {
    UA_NodeId_deleteMembers(&p->typeId);
    UA_ByteString_deleteMembers(&p->body);
}

void UA_ExtensionObject_init(UA_ExtensionObject *p) {
    UA_NodeId_init(&p->typeId);
    p->encoding = UA_EXTENSIONOBJECT_ENCODINGMASK_NOBODYISENCODED;
    UA_ByteString_init(&p->body);
}

UA_StatusCode UA_ExtensionObject_copy(UA_ExtensionObject const *src, UA_ExtensionObject *dst) {
    UA_StatusCode retval = UA_ByteString_copy(&src->body, &dst->body);
    retval |= UA_NodeId_copy(&src->typeId, &dst->typeId);
    dst->encoding = src->encoding;
    if(retval) {
        UA_ExtensionObject_deleteMembers(dst);
        UA_ExtensionObject_init(dst);
    }
    return retval;
}

/* DataValue */
UA_TYPE_NEW_DEFAULT(UA_DataValue)
UA_TYPE_DELETE_DEFAULT(UA_DataValue)

void UA_DataValue_deleteMembers(UA_DataValue *p) {
    UA_Variant_deleteMembers(&p->value);
}

void UA_DataValue_init(UA_DataValue *p) {
    *((UA_Byte*)p) = 0; // zero out the bitfield
    p->serverPicoseconds = 0;
    UA_DateTime_init(&p->serverTimestamp);
    p->sourcePicoseconds = 0;
    UA_DateTime_init(&p->sourceTimestamp);
    UA_StatusCode_init(&p->status);
    UA_Variant_init(&p->value);
}

UA_StatusCode UA_DataValue_copy(UA_DataValue const *src, UA_DataValue *dst) {
    UA_DataValue_init(dst);
    *((UA_Byte*)dst) = *((const UA_Byte*)src); // the bitfield
    UA_StatusCode retval = UA_DateTime_copy(&src->serverTimestamp, &dst->serverTimestamp);
    retval |= UA_DateTime_copy(&src->sourceTimestamp, &dst->sourceTimestamp);
    retval |= UA_Variant_copy(&src->value, &dst->value);
    dst->serverPicoseconds = src->serverPicoseconds;
    dst->sourcePicoseconds = src->sourcePicoseconds;
    dst->status = src->status;
    if(retval) {
        UA_DataValue_deleteMembers(dst);
        UA_DataValue_init(dst);
    }
    return retval;
}

/* Variant */
UA_TYPE_NEW_DEFAULT(UA_Variant)
UA_TYPE_DELETE_DEFAULT(UA_Variant)

void UA_Variant_init(UA_Variant *p) {
    p->storageType = UA_VARIANT_DATA;
    p->arrayLength = -1;
    p->data = UA_NULL;
    p->arrayDimensions = UA_NULL;
    p->arrayDimensionsSize = -1;
    p->type = &UA_TYPES[UA_TYPES_BOOLEAN];
}

void UA_Variant_deleteMembers(UA_Variant *p) {
    if(p->storageType != UA_VARIANT_DATA)
        return;
    if(p->data) {
        if(p->arrayLength == -1)
            p->arrayLength = 1;
        UA_Array_delete(p->data, p->type, p->arrayLength);
        p->data = UA_NULL;
        p->arrayLength = -1;
    }
    if(p->arrayDimensions) {
        UA_free(p->arrayDimensions);
        p->arrayDimensions = UA_NULL;
    }
}

UA_StatusCode UA_Variant_copy(UA_Variant const *src, UA_Variant *dst) {
    UA_Variant_init(dst);
    UA_Int32 tmp = src->arrayLength;
    if(src->arrayLength == -1 && src->data)
        tmp = 1;
    UA_StatusCode retval = UA_Array_copy(src->data, &dst->data, src->type, tmp);
    if(retval != UA_STATUSCODE_GOOD) {
        UA_Variant_deleteMembers(dst);
        UA_Variant_init(dst);
        return retval;
    }
    dst->arrayLength = src->arrayLength;
    dst->type = src->type;
    dst->storageType = UA_VARIANT_DATA;

    if(src->arrayDimensions) {
        retval |= UA_Array_copy(src->arrayDimensions, (void **)&dst->arrayDimensions,
                                &UA_TYPES[UA_TYPES_INT32], src->arrayDimensionsSize);
        if(retval != UA_STATUSCODE_GOOD) {
            UA_Variant_deleteMembers(dst);
            UA_Variant_init(dst);
            return retval;
        }
        dst->arrayDimensionsSize = src->arrayDimensionsSize;
    }
    return retval;
}

UA_Boolean UA_Variant_isScalar(UA_Variant *v) {
    return (v->data != UA_NULL && v->arrayLength == -1);
}

/**
 * Tests if a range is compatible with a variant. If yes, the following values are set:
 * - total: how many elements are indicated by the range
 * - block_size: how big is each contiguous block of elements in the variant denoted by the range
 * - block_distance: how many elements are between the blocks (beginning to beginning)
 * - first_elem: where does the first block begin
 */
static UA_StatusCode
testRangeWithVariant(const UA_Variant *v, const UA_NumericRange range, size_t *total,
                     size_t *block_size, size_t *block_distance, size_t *first_elem)
{
    /* Test the integrity of the source variant dimensions */
    UA_Int32 dims_count = 1;
    const UA_Int32 *dims = &v->arrayLength; // default: the array has only one dimension
    if(v->arrayDimensionsSize > 0) {
        dims_count = v->arrayDimensionsSize;
        dims = v->arrayDimensions;
        UA_Int32 elements = 1;
        for(UA_Int32 i = 0; i < dims_count; i++)
            elements *= dims[i];
        if(elements != v->arrayLength)
            return UA_STATUSCODE_BADINTERNALERROR;
    }

    /* Test the integrity of the range */
    size_t count = 1;
    if(range.dimensionsSize != dims_count)
        return UA_STATUSCODE_BADINTERNALERROR;
    for(UA_Int32 i = 0; i < dims_count; i++) {
        if(range.dimensions[i].min > range.dimensions[i].max)
            return UA_STATUSCODE_BADINDEXRANGEINVALID;
        if(range.dimensions[i].max > (UA_UInt32)*(dims+i))
            return UA_STATUSCODE_BADINDEXRANGENODATA;
        count *= (range.dimensions[i].max - range.dimensions[i].min) + 1;
    }

    /* Compute the block size and the position of the first element */
    size_t bs = 0;
    size_t bd = 0;
    size_t fe = 0;
    size_t running_dimssize = 1; // elements per block of dimensions k to k_max
    UA_Boolean found_contiguous = UA_FALSE;
    for(UA_Int32 k = dims_count - 1; k >= 0; k--) {
        if(!found_contiguous && (range.dimensions[k].min != 0 ||
                                 range.dimensions[k].max + 1 != (UA_UInt32)dims[k])) {
            found_contiguous = UA_TRUE;
            bs = (range.dimensions[k].max - range.dimensions[k].min + 1) * running_dimssize;
            bd = dims[k] * running_dimssize;
        } 
        fe += running_dimssize * range.dimensions[k].min;
        running_dimssize *= dims[k];
    }
    *total = count;
    *block_size = bs;
    *block_distance = bd;
    *first_elem = fe;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_Variant_copyRange(const UA_Variant *src, UA_Variant *dst, const UA_NumericRange range) {
    size_t count, block_size, block_distance, first_elem;
    UA_StatusCode retval = testRangeWithVariant(src, range, &count, &block_size, &block_distance, &first_elem);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;

    UA_Variant_init(dst);
    size_t elem_size = src->type->memSize;
    dst->data = UA_malloc(elem_size * count);
    if(!dst->data)
        return UA_STATUSCODE_BADOUTOFMEMORY;

    /* Copy the range */
    size_t block_count = count / block_size;
    uintptr_t nextdst = (uintptr_t)dst->data;
    uintptr_t nextsrc = (uintptr_t)src->data + (elem_size * first_elem);
    if(src->type->fixedSize) {
        for(size_t i = 0; i < block_count; i++) {
            memcpy((void*)nextdst, (void*)nextsrc, elem_size * block_size);
            nextdst += block_size * elem_size;
            nextsrc += block_distance * elem_size;
        }
    } else {
        for(size_t i = 0; i < block_count; i++) {
            for(size_t j = 0; j < block_size && retval == UA_STATUSCODE_GOOD; j++) {
                retval = UA_copy((const void*)nextsrc, (void*)nextdst, src->type);
                nextdst += elem_size;
                nextsrc += elem_size;
            }
            nextsrc += (block_distance - block_size) * elem_size;
        }
        if(retval != UA_STATUSCODE_GOOD) {
            size_t copied = ((nextdst - elem_size) - (uintptr_t)dst->data) / elem_size;
            UA_Array_delete(dst->data, src->type, copied);
            return retval;
        }
    }

    /* Copy the range dimensions*/
    if(src->arrayDimensionsSize > 0) {
        dst->arrayDimensions = UA_malloc(sizeof(UA_Int32) * src->arrayDimensionsSize);
        if(!dst->arrayDimensions) {
            UA_Array_delete(dst->data, src->type, count);
            return UA_STATUSCODE_BADOUTOFMEMORY;
        }
        for(UA_Int32 k = 0; k < src->arrayDimensionsSize; k++)
            dst->arrayDimensions[k] = range.dimensions[k].max - range.dimensions[k].min + 1;
        dst->arrayDimensionsSize = src->arrayDimensionsSize;
    }
    dst->arrayLength = count;
    dst->type = src->type;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_Variant_setRange(UA_Variant *v, void *data, const UA_NumericRange range) {
    size_t count, block_size, block_distance, first_elem;
    UA_StatusCode retval = testRangeWithVariant(v, range, &count, &block_size, &block_distance, &first_elem);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;

    size_t block_count = count / block_size;
    size_t elem_size = v->type->memSize;
    uintptr_t nextdst = (uintptr_t)v->data + (first_elem * elem_size);
    uintptr_t nextsrc = (uintptr_t)data;
    if(v->type->fixedSize) {
        for(size_t i = 0; i < block_count; i++) {
            memcpy((void*)nextdst, (void*)nextsrc, elem_size * block_size);
            nextdst += block_size * elem_size;
            nextsrc += block_distance * elem_size;
        }
    } else {
        for(size_t i = 0; i < block_count; i++) {
            for(size_t j = 0; j < block_size; j++) {
                UA_deleteMembers((void*)nextdst, v->type);
                nextdst += elem_size;
            }
            nextdst -= block_size * elem_size;
            memcpy((void*)nextdst, (void*)nextsrc, elem_size * block_size);
            nextdst += block_size * elem_size;
            nextsrc += block_distance * elem_size;
        }
    }
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_Variant_setCopyRange(const UA_Variant *src, UA_Variant *dst, void *data,
                                      const UA_NumericRange range) {
    // todo: don't copy the entire variant but only the retained parts
    UA_StatusCode retval = UA_Variant_copy(src, dst);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;
    retval = UA_Variant_setRange(dst, data, range);
    if(retval != UA_STATUSCODE_GOOD) {
        UA_Variant_deleteMembers(dst);
        return retval;
    }
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_Variant_setScalar(UA_Variant *v, void *p, const UA_DataType *type) {
    return UA_Variant_setArray(v, p, -1, type);
}

UA_StatusCode UA_Variant_setScalarCopy(UA_Variant *v, const void *p, const UA_DataType *type) {
    void *new = UA_malloc(type->memSize);
    if(!new)
        return UA_STATUSCODE_BADOUTOFMEMORY;
    UA_StatusCode retval = UA_copy(p, new, type);
	if(retval != UA_STATUSCODE_GOOD) {
		UA_delete(new, type);
		return retval;
	}
    return UA_Variant_setArray(v, new, -1, type);
}

UA_StatusCode UA_Variant_setArray(UA_Variant *v, void *array, UA_Int32 noElements,
                                  const UA_DataType *type) {
    v->type = type;
    v->arrayLength = noElements;
    v->data = array;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_Variant_setArrayCopy(UA_Variant *v, const void *array, UA_Int32 noElements,
                                      const UA_DataType *type) {
    void *new;
    UA_StatusCode retval = UA_Array_copy(array, &new, type, noElements);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;
    return UA_Variant_setArray(v, new, noElements, type);
}

/* DiagnosticInfo */
UA_TYPE_NEW_DEFAULT(UA_DiagnosticInfo)
UA_TYPE_DELETE_DEFAULT(UA_DiagnosticInfo)

void UA_DiagnosticInfo_deleteMembers(UA_DiagnosticInfo *p) {
    UA_String_deleteMembers(&p->additionalInfo);
    if(p->hasInnerDiagnosticInfo && p->innerDiagnosticInfo) {
        UA_DiagnosticInfo_delete(p->innerDiagnosticInfo);
        p->innerDiagnosticInfo = UA_NULL;
    }
}

void UA_DiagnosticInfo_init(UA_DiagnosticInfo *p) {
	*((UA_Byte*)p) = 0; // zero out the bitfield
    p->symbolicId          = 0;
    p->namespaceUri        = 0;
    p->localizedText       = 0;
    p->locale              = 0;
    UA_String_init(&p->additionalInfo);
    p->innerDiagnosticInfo = UA_NULL;
    UA_StatusCode_init(&p->innerStatusCode);
}

UA_StatusCode UA_DiagnosticInfo_copy(UA_DiagnosticInfo const *src, UA_DiagnosticInfo *dst) {
    UA_DiagnosticInfo_init(dst);
    *((UA_Byte*)dst) = *((const UA_Byte*)src); // the bitfield
    dst->symbolicId = src->symbolicId;
    dst->namespaceUri = src->namespaceUri;
    dst->localizedText = src->localizedText;
    dst->locale = src->locale;
    dst->innerStatusCode = src->innerStatusCode;
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    if(src->hasAdditionalInfo)
       retval = UA_String_copy(&src->additionalInfo, &dst->additionalInfo);
    if(src->hasInnerDiagnosticInfo && src->innerDiagnosticInfo) {
        if((dst->innerDiagnosticInfo = UA_malloc(sizeof(UA_DiagnosticInfo)))) {
            retval |= UA_DiagnosticInfo_copy(src->innerDiagnosticInfo, dst->innerDiagnosticInfo);
            dst->hasInnerDiagnosticInfo = src->hasInnerDiagnosticInfo;
        }
        else
            retval |= UA_STATUSCODE_BADOUTOFMEMORY;
    }
    if(retval) {
        UA_DiagnosticInfo_deleteMembers(dst);
        UA_DiagnosticInfo_init(dst);
    }
    return retval;
}

/*******************/
/* Structure Types */
/*******************/

void UA_init(void *p, const UA_DataType *dataType) {
    /* Do not check if the index is a builtin-type here. Builtins will be called
       with their very own _init functions normally. In the off case, that the
       generic function is called with the index of a builtin, their layout
       contains a single member of the builtin type, that will be inited in the
       for loop. */

    uintptr_t ptr = (uintptr_t)p;
    UA_Byte membersSize = dataType->membersSize;
    for(size_t i=0;i<membersSize; i++) {
        const UA_DataTypeMember *member = &dataType->members[i];
        if(member->isArray) {
            /* Padding contains bit-magic to split into padding before and after
               the length integer */
            ptr += (member->padding >> 3);
            *((UA_Int32*)ptr) = -1;
            ptr += sizeof(UA_Int32) + (member->padding & 0x07);
            *((void**)ptr) = UA_NULL;
            ptr += sizeof(void*);
            continue;
        }

        ptr += member->padding;
        if(!member->namespaceZero) {
            // pointer arithmetic
            const UA_DataType *memberType = dataType - dataType->typeIndex + member->memberTypeIndex;
            UA_init((void*)ptr, memberType);
            ptr += memberType->memSize;
            continue;
        }

        switch(member->memberTypeIndex) {
        case UA_TYPES_BOOLEAN:
        case UA_TYPES_SBYTE:
        case UA_TYPES_BYTE:
            *(UA_Byte*)ptr = 0;
            break;
        case UA_TYPES_INT16:
        case UA_TYPES_UINT16:
            *(UA_Int16*)ptr = 0;
            break;
        case UA_TYPES_INT32:
        case UA_TYPES_UINT32:
        case UA_TYPES_STATUSCODE:
        case UA_TYPES_FLOAT:
            *(UA_Int32*)ptr = 0;
            break;
        case UA_TYPES_INT64:
        case UA_TYPES_UINT64:
        case UA_TYPES_DOUBLE:
        case UA_TYPES_DATETIME:
            *(UA_Int64*)ptr = 0;
            break;
        case UA_TYPES_GUID:
            UA_Guid_init((UA_Guid*)ptr);
            break;
        case UA_TYPES_NODEID:
            UA_NodeId_init((UA_NodeId*)ptr);
            break;
        case UA_TYPES_EXPANDEDNODEID:
            UA_ExpandedNodeId_init((UA_ExpandedNodeId*)ptr);
            break;
        case UA_TYPES_LOCALIZEDTEXT:
            UA_LocalizedText_init((UA_LocalizedText*)ptr);
            break;
        case UA_TYPES_EXTENSIONOBJECT:
            UA_ExtensionObject_init((UA_ExtensionObject*)ptr);
            break;
        case UA_TYPES_DATAVALUE:
            UA_DataValue_init((UA_DataValue*)ptr);
            break;
        case UA_TYPES_VARIANT:
            UA_Variant_init((UA_Variant*)ptr);
            break;
        case UA_TYPES_DIAGNOSTICINFO:
            UA_DiagnosticInfo_init((UA_DiagnosticInfo*)ptr);
            break;
        default:
            // QualifiedName, LocalizedText and strings are treated as structures, also
            UA_init((void*)ptr, &UA_TYPES[member->memberTypeIndex]);
        }
        ptr += UA_TYPES[member->memberTypeIndex].memSize;
    }
}

void * UA_new(const UA_DataType *dataType) {
    void *p = UA_malloc(dataType->memSize);
    if(p)
        UA_init(p, dataType);
    return p;
}

UA_StatusCode UA_copy(const void *src, void *dst, const UA_DataType *dataType) {
    if(dataType->fixedSize) {
        memcpy(dst, src, dataType->memSize);
        return UA_STATUSCODE_GOOD;
    }
    UA_init(dst, dataType);
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    uintptr_t ptrs = (uintptr_t)src;
    uintptr_t ptrd = (uintptr_t)dst;
    UA_Byte membersSize = dataType->membersSize;
    for(size_t i=0;i<membersSize; i++) {
        const UA_DataTypeMember *member = &dataType->members[i];
        const UA_DataType *memberType;
        if(member->namespaceZero)
            memberType = &UA_TYPES[member->memberTypeIndex];
        else
            memberType = dataType - dataType->typeIndex + member->memberTypeIndex;

        if(member->isArray) {
            ptrs += (member->padding >> 3);
            ptrd += (member->padding >> 3);
            UA_Int32 *dstNoElements = (UA_Int32*)ptrd;
            const UA_Int32 noElements = *((const UA_Int32*)ptrs);
            ptrs += sizeof(UA_Int32) + (member->padding & 0x07);
            ptrd += sizeof(UA_Int32) + (member->padding & 0x07);
            retval = UA_Array_copy(*(void* const*)ptrs, (void**)ptrd, memberType, noElements);
            if(retval != UA_STATUSCODE_GOOD) {
                UA_deleteMembers(dst, dataType);
                UA_init(dst, dataType);
                return retval;
            }
            *dstNoElements = noElements;
            ptrs += sizeof(void*);
            ptrd += sizeof(void*);
            continue;
        }

        ptrs += member->padding;
        ptrd += member->padding;
        if(!member->namespaceZero) {
            retval = UA_copy((const void*)ptrs, (void*)ptrd, memberType);
            if(retval != UA_STATUSCODE_GOOD) {
                UA_deleteMembers(dst, dataType);
                UA_init(dst, dataType);
                return retval;
            }
            ptrs += memberType->memSize;
            ptrd += memberType->memSize;
            continue;
        }

        switch(member->memberTypeIndex) {
        case UA_TYPES_BOOLEAN:
        case UA_TYPES_SBYTE:
        case UA_TYPES_BYTE:
            *((UA_Byte*)ptrd) = *((const UA_Byte*)ptrs);
            break;
        case UA_TYPES_INT16:
        case UA_TYPES_UINT16:
            *((UA_Int16*)ptrd) = *((const UA_Byte*)ptrs);
            break;
        case UA_TYPES_INT32:
        case UA_TYPES_UINT32:
        case UA_TYPES_STATUSCODE:
        case UA_TYPES_FLOAT:
            *((UA_Int32*)ptrd) = *((const UA_Int32*)ptrs);
            break;
        case UA_TYPES_INT64:
        case UA_TYPES_UINT64:
        case UA_TYPES_DOUBLE:
        case UA_TYPES_DATETIME:
            *((UA_Int64*)ptrd) = *((const UA_Int64*)ptrs);
            break;
        case UA_TYPES_GUID:
            *((UA_Guid*)ptrd) = *((const UA_Guid*)ptrs);
            break;
        case UA_TYPES_NODEID:
            retval |= UA_NodeId_copy((const UA_NodeId*)ptrs, (UA_NodeId*)ptrd);
            break;
        case UA_TYPES_EXPANDEDNODEID:
            retval |= UA_ExpandedNodeId_copy((const UA_ExpandedNodeId*)ptrs, (UA_ExpandedNodeId*)ptrd);
            break;
        case UA_TYPES_LOCALIZEDTEXT:
            retval |= UA_LocalizedText_copy((const UA_LocalizedText*)ptrs, (UA_LocalizedText*)ptrd);
            break;
        case UA_TYPES_EXTENSIONOBJECT:
            retval |= UA_ExtensionObject_copy((const UA_ExtensionObject*)ptrs, (UA_ExtensionObject*)ptrd);
            break;
        case UA_TYPES_DATAVALUE:
            retval |= UA_DataValue_copy((const UA_DataValue*)ptrs, (UA_DataValue*)ptrd);
            break;
        case UA_TYPES_VARIANT:
            retval |= UA_Variant_copy((const UA_Variant*)ptrs, (UA_Variant*)ptrd);
            break;
        case UA_TYPES_DIAGNOSTICINFO:
            retval |= UA_DiagnosticInfo_copy((const UA_DiagnosticInfo*)ptrs, (UA_DiagnosticInfo*)ptrd);
            break;
        default:
            // QualifiedName, LocalizedText and strings are treated as structures, also
            retval |= UA_copy((const void *)ptrs, (void*)ptrd, memberType);
        }
        ptrs += memberType->memSize;
        ptrd += memberType->memSize;
    }
    if(retval != UA_STATUSCODE_GOOD) {
        UA_deleteMembers(dst, dataType);
        UA_init(dst, dataType);
    }
    return retval;
}

void UA_deleteMembers(void *p, const UA_DataType *dataType) {
    uintptr_t ptr = (uintptr_t)p;
    if(dataType->fixedSize)
        return;
    UA_Byte membersSize = dataType->membersSize;
    for(size_t i=0;i<membersSize; i++) {
        const UA_DataTypeMember *member = &dataType->members[i];
        const UA_DataType *memberType;
        if(member->namespaceZero)
            memberType = &UA_TYPES[member->memberTypeIndex];
        else
            memberType = dataType - dataType->typeIndex + member->memberTypeIndex;

        if(member->isArray) {
            ptr += (member->padding >> 3);
            UA_Int32 noElements = *((UA_Int32*)ptr);
            ptr += sizeof(UA_Int32) + (member->padding & 0x07);
            UA_Array_delete(*(void**)ptr, memberType, noElements);
            *(void**)ptr = UA_NULL;
            ptr += sizeof(void*);
            continue;
        }

        ptr += member->padding;
        if(!member->namespaceZero) {
            UA_deleteMembers((void*)ptr, memberType);
            ptr += memberType->memSize;
            continue;
        }

        switch(member->memberTypeIndex) {
        case UA_TYPES_BOOLEAN:
        case UA_TYPES_SBYTE:
        case UA_TYPES_BYTE:
        case UA_TYPES_INT16:
        case UA_TYPES_UINT16:
        case UA_TYPES_INT32:
        case UA_TYPES_UINT32:
        case UA_TYPES_STATUSCODE:
        case UA_TYPES_FLOAT:
        case UA_TYPES_INT64:
        case UA_TYPES_UINT64:
        case UA_TYPES_DOUBLE:
        case UA_TYPES_DATETIME:
        case UA_TYPES_GUID:
            break;
        case UA_TYPES_NODEID:
            UA_NodeId_deleteMembers((UA_NodeId*)ptr);
            break;
        case UA_TYPES_EXPANDEDNODEID:
            UA_ExpandedNodeId_deleteMembers((UA_ExpandedNodeId*)ptr);
            break;
        case UA_TYPES_LOCALIZEDTEXT:
            UA_LocalizedText_deleteMembers((UA_LocalizedText*)ptr);
            break;
        case UA_TYPES_EXTENSIONOBJECT:
            UA_ExtensionObject_deleteMembers((UA_ExtensionObject*)ptr);
            break;
        case UA_TYPES_DATAVALUE:
            UA_DataValue_deleteMembers((UA_DataValue*)ptr);
            break;
        case UA_TYPES_VARIANT:
            UA_Variant_deleteMembers((UA_Variant*)ptr);
            break;
        case UA_TYPES_DIAGNOSTICINFO:
            UA_DiagnosticInfo_deleteMembers((UA_DiagnosticInfo*)ptr);
            break;
        default:
            // QualifiedName, LocalizedText and strings are treated as structures, also
            UA_deleteMembers((void*)ptr, memberType);
        }
        ptr += memberType->memSize;
    }
}

void UA_delete(void *p, const UA_DataType *dataType) {
    UA_deleteMembers(p, dataType);
    UA_free(p);
}

/******************/
/* Array Handling */
/******************/

void* UA_Array_new(const UA_DataType *dataType, UA_Int32 noElements) {
    if(noElements <= 0)
        return UA_NULL;

    if((UA_Int32)dataType->memSize * noElements < 0 || dataType->memSize * noElements > MAX_ARRAY_SIZE )
        return UA_NULL;

    if(dataType->fixedSize)
        return calloc(noElements, dataType->memSize);

    void *p = malloc(dataType->memSize * (size_t)noElements);
    if(!p)
        return p;

    uintptr_t ptr = (uintptr_t)p;
    for(int i = 0; i<noElements; i++) {
        UA_init((void*)ptr, dataType);
        ptr += dataType->memSize;
    }
    return p;
}

UA_StatusCode UA_Array_copy(const void *src, void **dst, const UA_DataType *dataType, UA_Int32 noElements) {
    if(noElements <= 0) {
        *dst = UA_NULL;
        return UA_STATUSCODE_GOOD;
    }

    if(!(*dst = UA_malloc((size_t)noElements * dataType->memSize)))
        return UA_STATUSCODE_BADOUTOFMEMORY;

    if(dataType->fixedSize) {
        memcpy(*dst, src, dataType->memSize * (size_t)noElements);
        return UA_STATUSCODE_GOOD;
    }

    uintptr_t ptrs = (uintptr_t)src;
    uintptr_t ptrd = (uintptr_t)*dst;
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    for(int i=0;i<noElements; i++) {
        retval = UA_copy((void*)ptrs, (void*)ptrd, dataType);
        ptrs += dataType->memSize;
        ptrd += dataType->memSize;
    }

    if(retval != UA_STATUSCODE_GOOD)
        UA_Array_delete(*dst, dataType, noElements);

    return retval;
}

void UA_Array_delete(void *p, const UA_DataType *dataType, UA_Int32 noElements) {
    if(!dataType->fixedSize) {
        uintptr_t ptr = (uintptr_t)p;
        for(UA_Int32 i = 0; i<noElements; i++) {
            UA_deleteMembers((void*)ptr, dataType);
            ptr += dataType->memSize;
        }
    }
    UA_free(p);
}

#define UA_TYPE_CALCSIZEBINARY_MEMSIZE(TYPE) \
    size_t TYPE##_calcSizeBinary(TYPE const *p) { return sizeof(TYPE); }

#define UA_TYPE_BINARY_ENCODING_AS(TYPE, TYPE_AS)                       \
    size_t TYPE##_calcSizeBinary(TYPE const *p) {                       \
        return TYPE_AS##_calcSizeBinary((const TYPE_AS *)p);            \
    }                                                                   \
    UA_StatusCode TYPE##_encodeBinary(TYPE const *src, UA_ByteString *dst, size_t *offset) { \
        return TYPE_AS##_encodeBinary((const TYPE_AS *)src, dst, offset); \
    }                                                                   \
    UA_StatusCode TYPE##_decodeBinary(UA_ByteString const *src, size_t *offset, TYPE *dst) { \
        return TYPE_AS##_decodeBinary(src, offset, (TYPE_AS *)dst);     \
    }

/* Boolean */
UA_TYPE_CALCSIZEBINARY_MEMSIZE(UA_Boolean)
UA_StatusCode UA_Boolean_encodeBinary(const UA_Boolean *src, UA_ByteString *dst, size_t *offset) {
    if(*offset + sizeof(UA_Boolean) > (size_t)dst->length)
        return UA_STATUSCODE_BADENCODINGERROR;
    dst->data[*offset] = (UA_Byte)*src;
    ++(*offset);
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_Boolean_decodeBinary(UA_ByteString const *src, size_t *offset, UA_Boolean *dst) {
    if(*offset + sizeof(UA_Boolean) > (size_t)src->length )
        return UA_STATUSCODE_BADDECODINGERROR;
    *dst = (src->data[*offset] > 0) ? UA_TRUE : UA_FALSE;
    ++(*offset);
    return UA_STATUSCODE_GOOD;
}

/* SByte */
UA_TYPE_BINARY_ENCODING_AS(UA_SByte, UA_Byte)

/* Byte */
UA_TYPE_CALCSIZEBINARY_MEMSIZE(UA_Byte)
UA_StatusCode UA_Byte_encodeBinary(const UA_Byte *src, UA_ByteString *dst, size_t *offset) {
    if(*offset + sizeof(UA_Byte) > (size_t)dst->length)
        return UA_STATUSCODE_BADENCODINGERROR;
    dst->data[*offset] = (UA_Byte)*src;
    ++(*offset);
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_Byte_decodeBinary(UA_ByteString const *src, size_t *offset, UA_Byte *dst) {
    if(*offset + sizeof(UA_Byte) > (size_t)src->length )
        return UA_STATUSCODE_BADDECODINGERROR;
    *dst = src->data[*offset];
    ++(*offset);
    return UA_STATUSCODE_GOOD;
}

/* Int16 */
UA_TYPE_BINARY_ENCODING_AS(UA_Int16, UA_UInt16)

/* UInt16 */
UA_TYPE_CALCSIZEBINARY_MEMSIZE(UA_UInt16)
UA_StatusCode UA_UInt16_encodeBinary(UA_UInt16 const *src, UA_ByteString * dst, size_t *offset) {
    if(*offset + sizeof(UA_UInt16) > (size_t)dst->length )
        return UA_STATUSCODE_BADENCODINGERROR;
    UA_UInt16 *dst_ptr = (UA_UInt16*)&dst->data[*offset];
#ifndef _WIN32
    *dst_ptr = htole16(*src);
#else
    *dst_ptr = *src;
#endif
    *offset += 2;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_UInt16_decodeBinary(UA_ByteString const *src, size_t *offset, UA_UInt16 *dst) {
    if(*offset + sizeof(UA_UInt16) > (size_t)src->length)
        return UA_STATUSCODE_BADDECODINGERROR;
    UA_UInt16 value = *((UA_UInt16*)&src->data[*offset]);
#ifndef _WIN32
    value = le16toh(value);
#endif
    *dst = value;
    *offset += 2;
    return UA_STATUSCODE_GOOD;
}

/* Int32 */
UA_TYPE_BINARY_ENCODING_AS(UA_Int32, UA_UInt32)

/* UInt32 */
UA_TYPE_CALCSIZEBINARY_MEMSIZE(UA_UInt32)
UA_StatusCode UA_UInt32_encodeBinary(UA_UInt32 const *src, UA_ByteString * dst, size_t *offset) {
    if(*offset + sizeof(UA_UInt32) > (size_t)dst->length )
        return UA_STATUSCODE_BADENCODINGERROR;
    UA_UInt32 *dst_ptr = (UA_UInt32*)&dst->data[*offset];
#ifndef _WIN32
    *dst_ptr = htole32(*src);
#else
    *dst_ptr = *src;
#endif
    *offset += 4;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_UInt32_decodeBinary(UA_ByteString const *src, size_t *offset, UA_UInt32 * dst) {
    if(*offset + sizeof(UA_UInt32) > (size_t)src->length)
        return UA_STATUSCODE_BADDECODINGERROR;
    UA_UInt32 value = *((UA_UInt32*)&src->data[*offset]);
#ifndef _WIN32
    value = le32toh(value);
#endif
    *dst = value;
    *offset += 4;
    return UA_STATUSCODE_GOOD;
}

/* Int64 */
UA_TYPE_BINARY_ENCODING_AS(UA_Int64, UA_UInt64)

/* UInt64 */
UA_TYPE_CALCSIZEBINARY_MEMSIZE(UA_UInt64)
UA_StatusCode UA_UInt64_encodeBinary(UA_UInt64 const *src, UA_ByteString *dst, size_t *offset) {
    if(*offset + sizeof(UA_UInt64) > (size_t)dst->length )
        return UA_STATUSCODE_BADENCODINGERROR;
    UA_UInt64 *dst_ptr = (UA_UInt64*)&dst->data[*offset];
#ifndef _WIN32
    *dst_ptr = htole64(*src);
#else
    *dst_ptr = *src;
#endif
    *offset += 8;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_UInt64_decodeBinary(UA_ByteString const *src, size_t *offset, UA_UInt64 * dst) {
    if(*offset + sizeof(UA_UInt64) > (size_t)src->length)
        return UA_STATUSCODE_BADDECODINGERROR;
    UA_UInt64 value = *((UA_UInt64*)&src->data[*offset]);
#ifndef _WIN32
    value = le64toh(value);
#endif
    *dst = value;
    *offset += 8;
    return UA_STATUSCODE_GOOD;
}

/* Float */
/* In case that the float endiannes is different from the integer endianness (very unlikely on
   modern cpus), use the following code:

   UA_Byte UA_FLOAT_ZERO[] = { 0x00, 0x00, 0x00, 0x00 };
   UA_Float mantissa;
   UA_UInt32 biasedExponent;
   UA_Float sign;
   if(memcmp(&src->data[*offset], UA_FLOAT_ZERO, 4) == 0)
       return UA_Int32_decodeBinary(src, offset, (UA_Int32 *)dst);
   mantissa = (UA_Float)(src->data[*offset] & 0xFF);                                   // bits 0-7
   mantissa = (mantissa / (UA_Float)256.0 ) + (UA_Float)(src->data[*offset+1] & 0xFF); // bits 8-15
   mantissa = (mantissa / (UA_Float)256.0 ) + (UA_Float)(src->data[*offset+2] & 0x7F); // bits 16-22
   biasedExponent  = (src->data[*offset+2] & 0x80) >>  7;                              // bits 23
   biasedExponent |= (UA_UInt32)(src->data[*offset+3] & 0x7F) <<  1;                   // bits 24-30
   sign = ( src->data[*offset+ 3] & 0x80 ) ? -1.0 : 1.0;                               // bit 31
   if(biasedExponent >= 127)
       *dst = (UA_Float)sign * (1ULL << (biasedExponent-127)) * (1.0 + mantissa / 128.0 );
   else
       *dst = (UA_Float)sign * 2.0 * (1.0 + mantissa / 128.0 ) / ((UA_Float)(biasedExponent-127));}
 */
UA_TYPE_BINARY_ENCODING_AS(UA_Float, UA_UInt32)

/* Double */
/* UA_Byte UA_DOUBLE_ZERO[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
   UA_Double sign;
   UA_Double mantissa;
   UA_UInt32 biasedExponent;
   if(memcmp(&src->data[*offset], UA_DOUBLE_ZERO, 8) == 0)
       return UA_Int64_decodeBinary(src, offset, (UA_Int64 *)dst);
   mantissa = (UA_Double)(src->data[*offset] & 0xFF);                         // bits 0-7
   mantissa = (mantissa / 256.0 ) + (UA_Double)(src->data[*offset+1] & 0xFF); // bits 8-15
   mantissa = (mantissa / 256.0 ) + (UA_Double)(src->data[*offset+2] & 0xFF); // bits 16-23
   mantissa = (mantissa / 256.0 ) + (UA_Double)(src->data[*offset+3] & 0xFF); // bits 24-31
   mantissa = (mantissa / 256.0 ) + (UA_Double)(src->data[*offset+4] & 0xFF); // bits 32-39
   mantissa = (mantissa / 256.0 ) + (UA_Double)(src->data[*offset+5] & 0xFF); // bits 40-47
   mantissa = (mantissa / 256.0 ) + (UA_Double)(src->data[*offset+6] & 0x0F); // bits 48-51
   biasedExponent  = (src->data[*offset+6] & 0xF0) >>  4; // bits 52-55
   biasedExponent |= ((UA_UInt32)(src->data[*offset+7] & 0x7F)) <<  4; // bits 56-62
   sign = ( src->data[*offset+7] & 0x80 ) ? -1.0 : 1.0; // bit 63
   if(biasedExponent >= 1023)
       *dst = (UA_Double)sign * (1ULL << (biasedExponent-1023)) * (1.0 + mantissa / 8.0 );
   else
     *dst = (UA_Double)sign * 2.0 * (1.0 + mantissa / 8.0 ) / ((UA_Double)(biasedExponent-1023));
*/
UA_TYPE_BINARY_ENCODING_AS(UA_Double, UA_UInt64)

/* String */
size_t UA_String_calcSizeBinary(UA_String const *string) {
    if(string->length > 0)
        return sizeof(UA_Int32) + (string->length * sizeof(UA_Byte));
    else
        return sizeof(UA_Int32);
}

UA_StatusCode UA_String_encodeBinary(UA_String const *src, UA_ByteString *dst, size_t *offset) {
    if(*offset + UA_String_calcSizeBinary(src) > (size_t)dst->length)
        return UA_STATUSCODE_BADENCODINGERROR;

    UA_StatusCode retval = UA_Int32_encodeBinary(&src->length, dst, offset);
    if(src->length > 0) {
        UA_memcpy(&dst->data[*offset], src->data, src->length);
        *offset += src->length;
    }
    return retval;
}

UA_StatusCode UA_String_decodeBinary(UA_ByteString const *src, size_t *offset, UA_String *dst) {
    UA_String_init(dst);
    UA_Int32 length;
    if(UA_Int32_decodeBinary(src, offset, &length))
        return UA_STATUSCODE_BADINTERNALERROR;
    if(length <= 0) {
        if(length == 0)
            dst->length = 0;
        else
            dst->length = -1;
        return UA_STATUSCODE_GOOD;
    }
        
    if(*offset + (size_t)length > (size_t)src->length)
        return UA_STATUSCODE_BADINTERNALERROR;
    
    if(!(dst->data = UA_malloc(length)))
        return UA_STATUSCODE_BADOUTOFMEMORY;

    UA_memcpy(dst->data, &src->data[*offset], length);
    dst->length = length;
    *offset += length;
    return UA_STATUSCODE_GOOD;
}

/* DateTime */
UA_TYPE_BINARY_ENCODING_AS(UA_DateTime, UA_Int64)

/* Guid */
size_t UA_Guid_calcSizeBinary(UA_Guid const *p) {
    return 16;
}

UA_StatusCode UA_Guid_encodeBinary(UA_Guid const *src, UA_ByteString * dst, size_t *offset) {
    UA_StatusCode retval = UA_UInt32_encodeBinary(&src->data1, dst, offset);
    retval |= UA_UInt16_encodeBinary(&src->data2, dst, offset);
    retval |= UA_UInt16_encodeBinary(&src->data3, dst, offset);
    for(UA_Int32 i = 0;i < 8;i++)
        retval |= UA_Byte_encodeBinary(&src->data4[i], dst, offset);
    return retval;
}

UA_StatusCode UA_Guid_decodeBinary(UA_ByteString const *src, size_t *offset, UA_Guid * dst) {
    // This could be done with a single memcpy (if the compiler does no fancy realigning of structs)
    UA_StatusCode retval = UA_UInt32_decodeBinary(src, offset, &dst->data1);
    retval |= UA_UInt16_decodeBinary(src, offset, &dst->data2);
    retval |= UA_UInt16_decodeBinary(src, offset, &dst->data3);
    for(size_t i = 0;i < 8;i++)
        retval |= UA_Byte_decodeBinary(src, offset, &dst->data4[i]);
    if(retval)
        UA_Guid_deleteMembers(dst);
    return retval;
}

/* ByteString */
UA_TYPE_BINARY_ENCODING_AS(UA_ByteString, UA_String)

/* XmlElement */
UA_TYPE_BINARY_ENCODING_AS(UA_XmlElement, UA_String)

/* NodeId */

/* The shortened numeric nodeid types. */
#define UA_NODEIDTYPE_TWOBYTE 0
#define UA_NODEIDTYPE_FOURBYTE 1

size_t UA_NodeId_calcSizeBinary(UA_NodeId const *p) {
    size_t length = 0;
    switch(p->identifierType) {
    case UA_NODEIDTYPE_NUMERIC:
        if(p->identifier.numeric > UA_UINT16_MAX || p->namespaceIndex > UA_BYTE_MAX)
            length = sizeof(UA_Byte) + sizeof(UA_UInt16) + sizeof(UA_UInt32);
        else if(p->identifier.numeric > UA_BYTE_MAX || p->namespaceIndex > 0)
            length = 4;  /* UA_NODEIDTYPE_FOURBYTE */
        else
            length = 2;  /* UA_NODEIDTYPE_TWOBYTE*/
        break;
    case UA_NODEIDTYPE_STRING:
        length = sizeof(UA_Byte) + sizeof(UA_UInt16) + UA_String_calcSizeBinary(&p->identifier.string);
        break;
    case UA_NODEIDTYPE_GUID:
        length = sizeof(UA_Byte) + sizeof(UA_UInt16) + UA_Guid_calcSizeBinary(&p->identifier.guid);
        break;
    case UA_NODEIDTYPE_BYTESTRING:
        length = sizeof(UA_Byte) + sizeof(UA_UInt16) + UA_ByteString_calcSizeBinary(&p->identifier.byteString);
        break;
    default:
        UA_assert(UA_FALSE); // this must never happen
        break;
    }
    return length;
}

static UA_StatusCode UA_NodeId_encodeBinary_nodeid_offset(UA_NodeId const *src, UA_ByteString * dst, size_t *offset, UA_Int32 nodeid_offset) {
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    // temporary variables for endian-save code
    UA_Byte srcByte;
    UA_UInt16 srcUInt16;
    UA_UInt32 srcUInt32;
    switch(src->identifierType) {
    case UA_NODEIDTYPE_NUMERIC:
        if(src->identifier.numeric+nodeid_offset > UA_UINT16_MAX || src->namespaceIndex > UA_BYTE_MAX) {
            srcByte = UA_NODEIDTYPE_NUMERIC;
            retval |= UA_Byte_encodeBinary(&srcByte, dst, offset);
            retval |= UA_UInt16_encodeBinary(&src->namespaceIndex, dst, offset);
            srcUInt32 = src->identifier.numeric+nodeid_offset;
            retval |= UA_UInt32_encodeBinary(&srcUInt32, dst, offset);
        } else if(src->identifier.numeric+nodeid_offset > UA_BYTE_MAX || src->namespaceIndex > 0) { /* UA_NODEIDTYPE_FOURBYTE */
            srcByte = UA_NODEIDTYPE_FOURBYTE;
            retval |= UA_Byte_encodeBinary(&srcByte, dst, offset);
            srcByte = src->namespaceIndex;
            srcUInt16 = src->identifier.numeric+nodeid_offset;
            retval |= UA_Byte_encodeBinary(&srcByte, dst, offset);
            retval |= UA_UInt16_encodeBinary(&srcUInt16, dst, offset);
        } else { /* UA_NODEIDTYPE_TWOBYTE */
            srcByte = UA_NODEIDTYPE_TWOBYTE;
            retval |= UA_Byte_encodeBinary(&srcByte, dst, offset);
            srcByte = src->identifier.numeric+nodeid_offset;
            retval |= UA_Byte_encodeBinary(&srcByte, dst, offset);
        }
        break;
    case UA_NODEIDTYPE_STRING:
        srcByte = UA_NODEIDTYPE_STRING;
        retval |= UA_Byte_encodeBinary(&srcByte, dst, offset);
        retval |= UA_UInt16_encodeBinary(&src->namespaceIndex, dst, offset);
        retval |= UA_String_encodeBinary(&src->identifier.string, dst, offset);
        break;
    case UA_NODEIDTYPE_GUID:
        srcByte = UA_NODEIDTYPE_GUID;
        retval |= UA_Byte_encodeBinary(&srcByte, dst, offset);
        retval |= UA_UInt16_encodeBinary(&src->namespaceIndex, dst, offset);
        retval |= UA_Guid_encodeBinary(&src->identifier.guid, dst, offset);
        break;
    case UA_NODEIDTYPE_BYTESTRING:
        srcByte = UA_NODEIDTYPE_BYTESTRING;
        retval |= UA_Byte_encodeBinary(&srcByte, dst, offset);
        retval |= UA_UInt16_encodeBinary(&src->namespaceIndex, dst, offset);
        retval |= UA_ByteString_encodeBinary(&src->identifier.byteString, dst, offset);
        break;
    default:
        UA_assert(UA_FALSE);
    }
    return retval;
}

UA_StatusCode UA_NodeId_encodeBinary(UA_NodeId const *src, UA_ByteString * dst, size_t *offset) {
	return UA_NodeId_encodeBinary_nodeid_offset(src, dst, offset, 0);
}

UA_StatusCode UA_NodeId_decodeBinary(UA_ByteString const *src, size_t *offset, UA_NodeId *dst) {
    // temporary variables to overcome decoder's non-endian-saveness for datatypes with different length
    UA_Byte   dstByte = 0;
    UA_UInt16 dstUInt16 = 0;
    UA_Byte   encodingByte = 0;

    UA_StatusCode retval = UA_Byte_decodeBinary(src, offset, &encodingByte);
    if(retval) {
        UA_NodeId_init(dst);
        return retval;
    }
    
    switch(encodingByte) {
    case UA_NODEIDTYPE_TWOBYTE: // Table 7
        dst->identifierType     = UA_NODEIDTYPE_NUMERIC;
        retval = UA_Byte_decodeBinary(src, offset, &dstByte);
        dst->identifier.numeric = dstByte;
        dst->namespaceIndex     = 0; // default namespace
        break;
    case UA_NODEIDTYPE_FOURBYTE: // Table 8
        dst->identifierType     = UA_NODEIDTYPE_NUMERIC;
        retval |= UA_Byte_decodeBinary(src, offset, &dstByte);
        dst->namespaceIndex     = dstByte;
        retval |= UA_UInt16_decodeBinary(src, offset, &dstUInt16);
        dst->identifier.numeric = dstUInt16;
        break;
    case UA_NODEIDTYPE_NUMERIC: // Table 6, first entry
        dst->identifierType = UA_NODEIDTYPE_NUMERIC;
        retval |= UA_UInt16_decodeBinary(src, offset, &dst->namespaceIndex);
        retval |= UA_UInt32_decodeBinary(src, offset, &dst->identifier.numeric);
        break;
    case UA_NODEIDTYPE_STRING: // Table 6, second entry
        dst->identifierType = UA_NODEIDTYPE_STRING;
        retval |= UA_UInt16_decodeBinary(src, offset, &dst->namespaceIndex);
        retval |= UA_String_decodeBinary(src, offset, &dst->identifier.string);
        break;
    case UA_NODEIDTYPE_GUID: // Table 6, third entry
        dst->identifierType = UA_NODEIDTYPE_GUID;
        retval |= UA_UInt16_decodeBinary(src, offset, &dst->namespaceIndex);
        retval |= UA_Guid_decodeBinary(src, offset, &dst->identifier.guid);
        break;
    case UA_NODEIDTYPE_BYTESTRING: // Table 6, "OPAQUE"
        dst->identifierType = UA_NODEIDTYPE_BYTESTRING;
        retval |= UA_UInt16_decodeBinary(src, offset, &dst->namespaceIndex);
        retval |= UA_ByteString_decodeBinary(src, offset, &dst->identifier.byteString);
        break;
    default:
        UA_NodeId_init(dst);
        retval |= UA_STATUSCODE_BADINTERNALERROR; // the client sends an encodingByte we do not recognize
        break;
    }
    if(retval)
        UA_NodeId_deleteMembers(dst);
    return retval;
}

/* ExpandedNodeId */
size_t UA_ExpandedNodeId_calcSizeBinary(UA_ExpandedNodeId const *p) {
    size_t length = UA_NodeId_calcSizeBinary(&p->nodeId);
    if(p->namespaceUri.length > 0)
        length += UA_String_calcSizeBinary(&p->namespaceUri);
    if(p->serverIndex > 0)
        length += sizeof(UA_UInt32);
    return length;
}

#define UA_EXPANDEDNODEID_NAMESPACEURI_FLAG 0x80
#define UA_EXPANDEDNODEID_SERVERINDEX_FLAG 0x40

UA_StatusCode UA_ExpandedNodeId_encodeBinary(UA_ExpandedNodeId const *src, UA_ByteString * dst, size_t *offset) {
    UA_Byte flags = 0;
    UA_UInt32 start = *offset;
    UA_StatusCode retval = UA_NodeId_encodeBinary(&src->nodeId, dst, offset);
    if(src->namespaceUri.length > 0) {
        // TODO: Set namespaceIndex to 0 in the nodeid as the namespaceUri takes precedence
        retval |= UA_String_encodeBinary(&src->namespaceUri, dst, offset);
        flags |= UA_EXPANDEDNODEID_NAMESPACEURI_FLAG;
    }
    if(src->serverIndex > 0) {
        retval |= UA_UInt32_encodeBinary(&src->serverIndex, dst, offset);
        flags |= UA_EXPANDEDNODEID_SERVERINDEX_FLAG;
    }
    if(flags != 0)
        dst->data[start] |= flags;
    return retval;
}

UA_StatusCode UA_ExpandedNodeId_decodeBinary(UA_ByteString const *src, size_t *offset, UA_ExpandedNodeId *dst) {
    UA_ExpandedNodeId_init(dst);
    // get encodingflags and leave a "clean" nodeidtype
    if(*offset >= (size_t)src->length)
        return UA_STATUSCODE_BADDECODINGERROR;
    UA_Byte encodingByte = src->data[*offset];
    src->data[*offset] = encodingByte & ~(UA_EXPANDEDNODEID_NAMESPACEURI_FLAG | UA_EXPANDEDNODEID_SERVERINDEX_FLAG);

    UA_StatusCode retval = UA_NodeId_decodeBinary(src, offset, &dst->nodeId);
    if(encodingByte & UA_EXPANDEDNODEID_NAMESPACEURI_FLAG) {
        dst->nodeId.namespaceIndex = 0;
        retval |= UA_String_decodeBinary(src, offset, &dst->namespaceUri);
    }
    if(encodingByte & UA_EXPANDEDNODEID_SERVERINDEX_FLAG)
        retval |= UA_UInt32_decodeBinary(src, offset, &dst->serverIndex);
    if(retval)
        UA_ExpandedNodeId_deleteMembers(dst);
    return retval;
}

/* StatusCode */
UA_TYPE_BINARY_ENCODING_AS(UA_StatusCode, UA_UInt32)

/* QualifiedName */
size_t UA_QualifiedName_calcSizeBinary(UA_QualifiedName const *p) {
    size_t length = sizeof(UA_UInt16); //qualifiedName->namespaceIndex
    // length += sizeof(UA_UInt16); //qualifiedName->reserved
    length += UA_String_calcSizeBinary(&p->name); //qualifiedName->name
    return length;
}

UA_StatusCode UA_QualifiedName_decodeBinary(UA_ByteString const *src, size_t *offset, UA_QualifiedName *dst) {
    UA_QualifiedName_init(dst);
    UA_StatusCode retval = UA_UInt16_decodeBinary(src, offset, &dst->namespaceIndex);
    retval |= UA_String_decodeBinary(src, offset, &dst->name);
    if(retval)
        UA_QualifiedName_deleteMembers(dst);
    return retval;
}

UA_StatusCode UA_QualifiedName_encodeBinary(UA_QualifiedName const *src, UA_ByteString* dst, size_t *offset) {
    UA_StatusCode retval = UA_UInt16_encodeBinary(&src->namespaceIndex, dst, offset);
    retval |= UA_String_encodeBinary(&src->name, dst, offset);
    return retval;
}

/* LocalizedText */
#define UA_LOCALIZEDTEXT_ENCODINGMASKTYPE_LOCALE 0x01
#define UA_LOCALIZEDTEXT_ENCODINGMASKTYPE_TEXT 0x02

size_t UA_LocalizedText_calcSizeBinary(UA_LocalizedText const *p) {
    size_t length = 1; // for encodingMask
    if(p->locale.data != UA_NULL)
        length += UA_String_calcSizeBinary(&p->locale);
    if(p->text.data != UA_NULL)
        length += UA_String_calcSizeBinary(&p->text);
    return length;
}

UA_StatusCode UA_LocalizedText_encodeBinary(UA_LocalizedText const *src, UA_ByteString * dst, size_t *offset) {
    UA_Byte encodingMask = 0;
    if(src->locale.data != UA_NULL)
        encodingMask |= UA_LOCALIZEDTEXT_ENCODINGMASKTYPE_LOCALE;
    if(src->text.data != UA_NULL)
        encodingMask |= UA_LOCALIZEDTEXT_ENCODINGMASKTYPE_TEXT;
    UA_StatusCode retval = UA_Byte_encodeBinary(&encodingMask, dst, offset);
    if(encodingMask & UA_LOCALIZEDTEXT_ENCODINGMASKTYPE_LOCALE)
        retval |= UA_String_encodeBinary(&src->locale, dst, offset);
    if(encodingMask & UA_LOCALIZEDTEXT_ENCODINGMASKTYPE_TEXT)
        retval |= UA_String_encodeBinary(&src->text, dst, offset);
    return retval;
}

UA_StatusCode UA_LocalizedText_decodeBinary(UA_ByteString const *src, size_t *offset, UA_LocalizedText *dst) {
    UA_LocalizedText_init(dst);
    UA_Byte encodingMask = 0;
    UA_StatusCode retval = UA_Byte_decodeBinary(src, offset, &encodingMask);
    if(encodingMask & UA_LOCALIZEDTEXT_ENCODINGMASKTYPE_LOCALE)
        retval |= UA_String_decodeBinary(src, offset, &dst->locale);
    if(encodingMask & UA_LOCALIZEDTEXT_ENCODINGMASKTYPE_TEXT)
        retval |= UA_String_decodeBinary(src, offset, &dst->text);
    if(retval)
        UA_LocalizedText_deleteMembers(dst);
    return retval;
}

/* ExtensionObject */
size_t UA_ExtensionObject_calcSizeBinary(UA_ExtensionObject const *p) {
    size_t length = UA_NodeId_calcSizeBinary(&p->typeId);
    length += 1; // encoding
    switch(p->encoding) {
    case UA_EXTENSIONOBJECT_ENCODINGMASK_NOBODYISENCODED:
        break;
    case UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISBYTESTRING:
        length += UA_ByteString_calcSizeBinary(&p->body);
        break;
    case UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISXML:
        length += UA_XmlElement_calcSizeBinary((const UA_XmlElement *)&p->body);
        break;
    default:
        UA_assert(UA_FALSE);
    }
    return length;
}

UA_StatusCode UA_ExtensionObject_encodeBinary(UA_ExtensionObject const *src, UA_ByteString * dst, size_t *offset) {
    UA_StatusCode retval = UA_NodeId_encodeBinary(&src->typeId, dst, offset);
    retval |= UA_Byte_encodeBinary((const UA_Byte*)&src->encoding, dst, offset);
    switch(src->encoding) {
    case UA_EXTENSIONOBJECT_ENCODINGMASK_NOBODYISENCODED:
        break;
    case UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISBYTESTRING:
        retval |= UA_ByteString_encodeBinary(&src->body, dst, offset);
        break;
    case UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISXML:
        retval |= UA_ByteString_encodeBinary(&src->body, dst, offset);
        break;
    default:
        UA_assert(UA_FALSE);
    }
    return retval;
}

UA_StatusCode UA_ExtensionObject_decodeBinary(UA_ByteString const *src, size_t *offset, UA_ExtensionObject *dst) {
    UA_ExtensionObject_init(dst);
    UA_Byte encoding = 0;
    UA_StatusCode retval = UA_NodeId_decodeBinary(src, offset, &dst->typeId);
    retval |= UA_Byte_decodeBinary(src, offset, &encoding);
    dst->encoding = encoding;
    retval |= UA_String_copy(&UA_STRING_NULL, (UA_String *)&dst->body);
    if(retval) {
        UA_ExtensionObject_init(dst);
        return retval;
    }
    switch(dst->encoding) {
    case UA_EXTENSIONOBJECT_ENCODINGMASK_NOBODYISENCODED:
        break;
    case UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISBYTESTRING:
    case UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISXML:
        retval |= UA_ByteString_decodeBinary(src, offset, &dst->body);
        break;
    default:
        UA_ExtensionObject_deleteMembers(dst);
        return UA_STATUSCODE_BADDECODINGERROR;
    }
    if(retval)
        UA_ExtensionObject_deleteMembers(dst);
    return retval;
}

/* DataValue */
size_t UA_DataValue_calcSizeBinary(UA_DataValue const *p) {
    size_t length = sizeof(UA_Byte);
    if(p->hasValue)
        length += UA_Variant_calcSizeBinary(&p->value);
    if(p->hasStatus)
        length += sizeof(UA_UInt32);
    if(p->hasSourceTimestamp)
        length += sizeof(UA_DateTime);
    if(p->hasSourcePicoseconds)
        length += sizeof(UA_Int16);
    if(p->hasServerTimestamp)
        length += sizeof(UA_DateTime);
    if(p->hasServerPicoseconds)
        length += sizeof(UA_Int16);
    return length;
}

UA_StatusCode UA_DataValue_encodeBinary(UA_DataValue const *src, UA_ByteString *dst, size_t *offset) {
    UA_StatusCode retval = UA_Byte_encodeBinary((const UA_Byte*)src, dst, offset);
    if(src->hasValue)
        retval |= UA_Variant_encodeBinary(&src->value, dst, offset);
    if(src->hasStatus)
        retval |= UA_StatusCode_encodeBinary(&src->status, dst, offset);
    if(src->hasSourceTimestamp)
        retval |= UA_DateTime_encodeBinary(&src->sourceTimestamp, dst, offset);
    if(src->hasSourcePicoseconds)
        retval |= UA_Int16_encodeBinary(&src->sourcePicoseconds, dst, offset);
    if(src->hasServerTimestamp)
        retval |= UA_DateTime_encodeBinary(&src->serverTimestamp, dst, offset);
    if(src->hasServerPicoseconds)
        retval |= UA_Int16_encodeBinary(&src->serverPicoseconds, dst, offset);
    return retval;
}

#define MAX_PICO_SECONDS 1000
UA_StatusCode UA_DataValue_decodeBinary(UA_ByteString const *src, size_t *offset, UA_DataValue *dst) {
    UA_DataValue_init(dst);
    UA_StatusCode retval = UA_Byte_decodeBinary(src, offset, (UA_Byte*)dst);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;
    if(dst->hasValue)
        retval |= UA_Variant_decodeBinary(src, offset, &dst->value);
    if(dst->hasStatus)
        retval |= UA_StatusCode_decodeBinary(src, offset, &dst->status);
    if(dst->hasSourceTimestamp)
        retval |= UA_DateTime_decodeBinary(src, offset, &dst->sourceTimestamp);
    if(dst->hasSourcePicoseconds) {
        retval |= UA_Int16_decodeBinary(src, offset, &dst->sourcePicoseconds);
        if(dst->sourcePicoseconds > MAX_PICO_SECONDS)
            dst->sourcePicoseconds = MAX_PICO_SECONDS;
    }
    if(dst->hasServerTimestamp)
        retval |= UA_DateTime_decodeBinary(src, offset, &dst->serverTimestamp);
    if(dst->hasServerPicoseconds) {
        retval |= UA_Int16_decodeBinary(src, offset, &dst->serverPicoseconds);
        if(dst->serverPicoseconds > MAX_PICO_SECONDS)
            dst->serverPicoseconds = MAX_PICO_SECONDS;
    }
    if(retval)
        UA_DataValue_deleteMembers(dst);
    return retval;
}

/* Variant */
/* We can store all data types in a variant internally. But for communication we
 * encode them in an ExtensionObject if they are not one of the built in types.
 * Officially, only builtin types are contained in a variant.
 *
 * Every ExtensionObject incurrs an overhead of 4 byte (nodeid) + 1 byte (encoding) */

/* VariantBinaryEncoding - Part: 6, Chapter: 5.2.2.16, Page: 22 */
enum UA_VARIANT_ENCODINGMASKTYPE_enum {
    UA_VARIANT_ENCODINGMASKTYPE_TYPEID_MASK = 0x3F,            // bits 0:5
    UA_VARIANT_ENCODINGMASKTYPE_DIMENSIONS  = (0x01 << 6),     // bit 6
    UA_VARIANT_ENCODINGMASKTYPE_ARRAY       = (0x01 << 7)      // bit 7
};

size_t UA_Variant_calcSizeBinary(UA_Variant const *p) {
    UA_Boolean isArray = p->arrayLength != -1 || !p->data;  // a single element is not an array
    UA_Boolean hasDimensions = isArray && p->arrayDimensions != UA_NULL;
    UA_Boolean isBuiltin = p->type->namespaceZero && UA_IS_BUILTIN(p->type->typeIndex);
    size_t length = sizeof(UA_Byte); //p->encodingMask
    UA_Int32 arrayLength = p->arrayLength;
    if(!isArray) {
    	arrayLength = 1;
        length += UA_calcSizeBinary(p->data, p->type);
    }
    else
        length += UA_Array_calcSizeBinary(p->data, arrayLength, p->type);
        
    // if the type is not builtin, we encode it as an extensionobject
    if(!isBuiltin) {
        if(arrayLength > 0)
            length += 9 * arrayLength;  // overhead for extensionobjects: 4 byte nodeid + 1 byte
                                        // encoding + 4 byte bytestring length
    }
    if(hasDimensions)
        length += UA_Array_calcSizeBinary(p->arrayDimensions, p->arrayDimensionsSize,
                                          &UA_TYPES[UA_TYPES_INT32]);
    return length;
}

UA_StatusCode UA_Variant_encodeBinary(UA_Variant const *src, UA_ByteString *dst, size_t *offset) {
    UA_Byte encodingByte = 0;
    UA_Boolean isArray = src->arrayLength != -1 || !src->data;  // a single element is not an array
    UA_Boolean hasDimensions = isArray && src->arrayDimensions != UA_NULL;
    UA_Boolean isBuiltin = src->type->namespaceZero && UA_IS_BUILTIN(src->type->typeIndex);

    if(isArray) {
        encodingByte |= UA_VARIANT_ENCODINGMASKTYPE_ARRAY;
        if(hasDimensions)
            encodingByte |= UA_VARIANT_ENCODINGMASKTYPE_DIMENSIONS;
    }

    if(isBuiltin)
        encodingByte |= UA_VARIANT_ENCODINGMASKTYPE_TYPEID_MASK &
            (UA_Byte)UA_TYPES[src->type->typeIndex].typeId.identifier.numeric;
    else
        encodingByte |= UA_VARIANT_ENCODINGMASKTYPE_TYPEID_MASK & (UA_Byte)22; // wrap in an extensionobject

    UA_StatusCode retval = UA_Byte_encodeBinary(&encodingByte, dst, offset);

    if(isArray)
        retval |= UA_Int32_encodeBinary(&src->arrayLength, dst, offset);
    uintptr_t ptr = (uintptr_t)src->data;
    ptrdiff_t memSize = src->type->memSize;
    UA_Int32 numToEncode = src->arrayLength;
    if(!isArray)
        numToEncode = 1;
    for(UA_Int32 i=0;i<numToEncode;i++) {
        if(!isBuiltin) {
            /* The type is wrapped inside an extensionobject*/
            // todo: offest holds only for namespace 0
            retval |= UA_NodeId_encodeBinary_nodeid_offset(&src->type->typeId, dst, offset, UA_ENCODINGOFFSET_BINARY);
            UA_Byte eoEncoding = UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISBYTESTRING;
            retval |= UA_Byte_encodeBinary(&eoEncoding, dst, offset);
            UA_Int32 eoEncodingLength = UA_calcSizeBinary(src->data, src->type);
            retval |= UA_Int32_encodeBinary(&eoEncodingLength, dst, offset);
        }
        retval |= UA_encodeBinary((void*)ptr, src->type, dst, offset);
        ptr += memSize;
    }
    if(hasDimensions)
        retval |= UA_Array_encodeBinary(src->arrayDimensions, src->arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32], dst, offset);
    return retval;
}

/* The resulting variant always has the storagetype UA_VARIANT_DATA. Currently,
   we only support ns0 types (todo: attach typedescriptions to datatypenodes) */
UA_StatusCode UA_Variant_decodeBinary(UA_ByteString const *src, size_t *offset, UA_Variant *dst) {
    UA_Variant_init(dst);
    UA_Byte encodingByte;
    UA_StatusCode retval = UA_Byte_decodeBinary(src, offset, &encodingByte);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;

    UA_Boolean isArray = encodingByte & UA_VARIANT_ENCODINGMASKTYPE_ARRAY;
    size_t typeIndex = (encodingByte & UA_VARIANT_ENCODINGMASKTYPE_TYPEID_MASK) - 1;
    if(typeIndex > 24) /* must be builtin */
        return UA_STATUSCODE_BADDECODINGERROR;

    if(isArray || typeIndex != UA_TYPES_EXTENSIONOBJECT) {
        /* an array or a single builtin */
        const UA_DataType *dataType = &UA_TYPES[typeIndex];
        UA_Int32 arraySize = -1;
        if(isArray) {
            retval |= UA_Int32_decodeBinary(src, offset, &arraySize);
            if(retval != UA_STATUSCODE_GOOD)
                return retval;
        }
        retval |= UA_Array_decodeBinary(src, offset, (!isArray && arraySize==-1) ? 1: arraySize, &dst->data, dataType);
        if(retval != UA_STATUSCODE_GOOD)
            return retval;
        dst->arrayLength = arraySize; // for deleteMembers
        dst->type = dataType;
    } else {
        /* a single extensionobject */
        size_t oldoffset = *offset;
        UA_NodeId typeId;
        if((retval |= UA_NodeId_decodeBinary(src, offset, &typeId)) != UA_STATUSCODE_GOOD)
            return retval;
        UA_Byte EOencodingByte;
        if((retval |= UA_Byte_decodeBinary(src, offset, &EOencodingByte)) != UA_STATUSCODE_GOOD) {
            UA_NodeId_deleteMembers(&typeId);
            return retval;
        }
        const UA_DataType *dataType = UA_NULL;
        if(typeId.namespaceIndex == 0 && EOencodingByte == UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISBYTESTRING) {
            for(typeIndex = 0;typeIndex < UA_TYPES_COUNT; typeIndex++) {
                if(UA_NodeId_equal(&typeId, &UA_TYPES[typeIndex].typeId)) {
                    dataType = &UA_TYPES[typeIndex];
                    break;
                }
            }
        }
        UA_NodeId_deleteMembers(&typeId);
        if(!dataType) {
            *offset = oldoffset;
            dataType = &UA_TYPES[UA_TYPES_EXTENSIONOBJECT];
        }
        if((retval |= UA_decodeBinary(src, offset, dst->data, dataType)) != UA_STATUSCODE_GOOD)
            return retval;
        dst->type = dataType;
        dst->arrayLength = -1;
    }
    
    /* array dimensions */
    if(isArray && (encodingByte & UA_VARIANT_ENCODINGMASKTYPE_DIMENSIONS)) {
        retval |= UA_Int32_decodeBinary(src, offset, &dst->arrayDimensionsSize);
        if(retval == UA_STATUSCODE_GOOD)
            retval |= UA_Array_decodeBinary(src, offset, dst->arrayDimensionsSize,
                                            &dst->data, &UA_TYPES[UA_TYPES_INT32]);
        if(retval != UA_STATUSCODE_GOOD) {
            dst->arrayDimensionsSize = -1;
            UA_Variant_deleteMembers(dst);
            return retval;
        }
    }
    return UA_STATUSCODE_GOOD;
}

/* DiagnosticInfo */
size_t UA_DiagnosticInfo_calcSizeBinary(UA_DiagnosticInfo const *ptr) {
    size_t length = sizeof(UA_Byte);
    if(ptr->hasSymbolicId)
        length += sizeof(UA_Int32);
    if(ptr->hasNamespaceUri)
        length += sizeof(UA_Int32);
    if(ptr->hasLocalizedText)
        length += sizeof(UA_Int32);
    if(ptr->hasLocale)
        length += sizeof(UA_Int32);
    if(ptr->hasAdditionalInfo)
        length += UA_String_calcSizeBinary(&ptr->additionalInfo);
    if(ptr->hasInnerStatusCode)
        length += sizeof(UA_StatusCode);
    if(ptr->hasInnerDiagnosticInfo)
        length += UA_DiagnosticInfo_calcSizeBinary(ptr->innerDiagnosticInfo);
    return length;
}

UA_StatusCode UA_DiagnosticInfo_encodeBinary(const UA_DiagnosticInfo *src, UA_ByteString * dst, size_t *offset) {
    UA_StatusCode retval = UA_Byte_encodeBinary((const UA_Byte *)src, dst, offset);
    if(src->hasSymbolicId)
        retval |= UA_Int32_encodeBinary(&src->symbolicId, dst, offset);
    if(src->hasNamespaceUri)
        retval |= UA_Int32_encodeBinary( &src->namespaceUri, dst, offset);
    if(src->hasLocalizedText)
        retval |= UA_Int32_encodeBinary(&src->localizedText, dst, offset);
    if(src->hasLocale)
        retval |= UA_Int32_encodeBinary(&src->locale, dst, offset);
    if(src->hasAdditionalInfo)
        retval |= UA_String_encodeBinary(&src->additionalInfo, dst, offset);
    if(src->hasInnerStatusCode)
        retval |= UA_StatusCode_encodeBinary(&src->innerStatusCode, dst, offset);
    if(src->hasInnerDiagnosticInfo)
        retval |= UA_DiagnosticInfo_encodeBinary(src->innerDiagnosticInfo, dst, offset);
    return retval;
}

UA_StatusCode UA_DiagnosticInfo_decodeBinary(UA_ByteString const *src, size_t *offset, UA_DiagnosticInfo *dst) {
    UA_DiagnosticInfo_init(dst);
    UA_StatusCode retval = UA_Byte_decodeBinary(src, offset, (UA_Byte*)dst);
    if(!retval)
        return retval;
    if(dst->hasSymbolicId)
        retval |= UA_Int32_decodeBinary(src, offset, &dst->symbolicId);
    if(dst->hasNamespaceUri)
        retval |= UA_Int32_decodeBinary(src, offset, &dst->namespaceUri);
    if(dst->hasLocalizedText)
        retval |= UA_Int32_decodeBinary(src, offset, &dst->localizedText);
    if(dst->hasLocale)
        retval |= UA_Int32_decodeBinary(src, offset, &dst->locale);
    if(dst->hasAdditionalInfo)
        retval |= UA_String_decodeBinary(src, offset, &dst->additionalInfo);
    if(dst->hasInnerStatusCode)
        retval |= UA_StatusCode_decodeBinary(src, offset, &dst->innerStatusCode);
    if(dst->hasInnerDiagnosticInfo) {
        // innerDiagnosticInfo is a pointer to struct, therefore allocate
        if((dst->innerDiagnosticInfo = UA_malloc(sizeof(UA_DiagnosticInfo)))) {
            if(UA_DiagnosticInfo_decodeBinary(src, offset, dst->innerDiagnosticInfo) != UA_STATUSCODE_GOOD) {
                UA_free(dst->innerDiagnosticInfo);
                dst->innerDiagnosticInfo = UA_NULL;
                retval |= UA_STATUSCODE_BADINTERNALERROR;
            }
        } else {
            retval |= UA_STATUSCODE_BADOUTOFMEMORY;
        }
    }
    if(retval != UA_STATUSCODE_GOOD)
        UA_DiagnosticInfo_deleteMembers(dst);
    return retval;
}

/********************/
/* Structured Types */
/********************/

size_t UA_calcSizeBinary(const void *p, const UA_DataType *dataType) {
    size_t size = 0;
    uintptr_t ptr = (uintptr_t)p;
    UA_Byte membersSize = dataType->membersSize;
    for(size_t i=0;i<membersSize; i++) {
        const UA_DataTypeMember *member = &dataType->members[i];
        const UA_DataType *memberType;
        if(member->namespaceZero)
            memberType = &UA_TYPES[member->memberTypeIndex];
        else
            memberType = dataType - dataType->typeIndex + member->memberTypeIndex;

        if(member->isArray) {
            ptr += (member->padding >> 3);
            const UA_Int32 noElements = *((const UA_Int32*)ptr);
            ptr += sizeof(UA_Int32) + (member->padding & 0x07);
            size += UA_Array_calcSizeBinary(*(void * const *)ptr, noElements, memberType);
            ptr += sizeof(void*);
            continue;
        }

        ptr += member->padding;
        if(!member->namespaceZero) {
            size += UA_calcSizeBinary((const void*)ptr, memberType);
            ptr += memberType->memSize;
            continue;
        }

        switch(member->memberTypeIndex) {
        case UA_TYPES_BOOLEAN:
        case UA_TYPES_SBYTE:
        case UA_TYPES_BYTE:
            size += 1;
            break;
        case UA_TYPES_INT16:
        case UA_TYPES_UINT16:
            size += 2;
            break;
        case UA_TYPES_INT32:
        case UA_TYPES_UINT32:
        case UA_TYPES_STATUSCODE:
        case UA_TYPES_FLOAT:
            size += 4;
            break;
        case UA_TYPES_INT64:
        case UA_TYPES_UINT64:
        case UA_TYPES_DOUBLE:
        case UA_TYPES_DATETIME:
            size += 8;
            break;
        case UA_TYPES_GUID:
            size += 16;
            break;
        case UA_TYPES_NODEID:
            size += UA_NodeId_calcSizeBinary((const UA_NodeId*)ptr);
            break;
        case UA_TYPES_EXPANDEDNODEID:
            size += UA_ExpandedNodeId_calcSizeBinary((const UA_ExpandedNodeId*)ptr);
            break;
        case UA_TYPES_LOCALIZEDTEXT:
            size += UA_LocalizedText_calcSizeBinary((const UA_LocalizedText*)ptr);
            break;
        case UA_TYPES_EXTENSIONOBJECT:
            size += UA_ExtensionObject_calcSizeBinary((const UA_ExtensionObject*)ptr);
            break;
        case UA_TYPES_DATAVALUE:
            size += UA_DataValue_calcSizeBinary((const UA_DataValue*)ptr);
            break;
        case UA_TYPES_VARIANT:
            size += UA_Variant_calcSizeBinary((const UA_Variant*)ptr);
            break;
        case UA_TYPES_DIAGNOSTICINFO:
            size += UA_DiagnosticInfo_calcSizeBinary((const UA_DiagnosticInfo*)ptr);
            break;
        default:
            // UA_TYPES_QUALIFIEDNAME, UA_TYPES_STRING, UA_TYPES_BYTESTRING, UA_TYPES_XMLELEMENT:
            size += UA_calcSizeBinary((const void*)ptr, memberType);
        }
        ptr += memberType->memSize;
    }
    return size;
}

UA_StatusCode UA_encodeBinary(const void *src, const UA_DataType *dataType, UA_ByteString *dst, size_t *offset) {
    uintptr_t ptr = (uintptr_t)src;
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    UA_Byte membersSize = dataType->membersSize;
    for(size_t i=0;i<membersSize && retval == UA_STATUSCODE_GOOD; i++) {
        const UA_DataTypeMember *member = &dataType->members[i];
        const UA_DataType *memberType;
        if(member->namespaceZero)
            memberType = &UA_TYPES[member->memberTypeIndex];
        else
            memberType = dataType - dataType->typeIndex + member->memberTypeIndex;

        if(member->isArray) {
            ptr += (member->padding >> 3);
            const UA_Int32 noElements = *((const UA_Int32*)ptr);
            ptr += sizeof(UA_Int32) + (member->padding & 0x07);
            retval = UA_Array_encodeBinary(*(void * const *)ptr, noElements, memberType, dst, offset);
            ptr += sizeof(void*);
            continue;
        }

        ptr += member->padding;
        if(!member->namespaceZero) {
            UA_encodeBinary((const void*)ptr, memberType, dst, offset);
            ptr += memberType->memSize;
            continue;
        }
        
        switch(member->memberTypeIndex) {
        case UA_TYPES_BOOLEAN:
        case UA_TYPES_SBYTE:
        case UA_TYPES_BYTE:
            retval = UA_Byte_encodeBinary((const UA_Byte*)ptr, dst, offset);
            break;
        case UA_TYPES_INT16:
        case UA_TYPES_UINT16:
            retval = UA_UInt16_encodeBinary((const UA_UInt16*)ptr, dst, offset);
            break;
        case UA_TYPES_INT32:
        case UA_TYPES_UINT32:
        case UA_TYPES_FLOAT:
        case UA_TYPES_STATUSCODE:
            retval = UA_UInt32_encodeBinary((const UA_UInt32*)ptr, dst, offset);
            break;
        case UA_TYPES_INT64:
        case UA_TYPES_UINT64:
        case UA_TYPES_DOUBLE:
        case UA_TYPES_DATETIME:
            retval = UA_UInt64_encodeBinary((const UA_UInt64*)ptr, dst, offset);
            break;
        case UA_TYPES_GUID:
            retval = UA_Guid_encodeBinary((const UA_Guid*)ptr, dst, offset);
            break;
        case UA_TYPES_NODEID:
            retval = UA_NodeId_encodeBinary((const UA_NodeId*)ptr, dst, offset);
            break;
        case UA_TYPES_EXPANDEDNODEID:
            retval = UA_ExpandedNodeId_encodeBinary((const UA_ExpandedNodeId*)ptr, dst, offset);
            break;
        case UA_TYPES_LOCALIZEDTEXT:
            retval = UA_LocalizedText_encodeBinary((const UA_LocalizedText*)ptr, dst, offset);
            break;
        case UA_TYPES_EXTENSIONOBJECT:
            retval = UA_ExtensionObject_encodeBinary((const UA_ExtensionObject*)ptr, dst, offset);
            break;
        case UA_TYPES_DATAVALUE:
            retval = UA_DataValue_encodeBinary((const UA_DataValue*)ptr, dst, offset);
            break;
        case UA_TYPES_VARIANT:
            retval = UA_Variant_encodeBinary((const UA_Variant*)ptr, dst, offset);
            break;
        case UA_TYPES_DIAGNOSTICINFO:
            retval = UA_DiagnosticInfo_encodeBinary((const UA_DiagnosticInfo*)ptr, dst, offset);
            break;
        default:
            // UA_TYPES_QUALIFIEDNAME, UA_TYPES_STRING, UA_TYPES_BYTESTRING, UA_TYPES_XMLELEMENT:
            retval = UA_encodeBinary((const void*)ptr, memberType, dst, offset);
        }
        ptr += memberType->memSize;
    }
    return retval;
}

UA_StatusCode UA_decodeBinary(const UA_ByteString *src, size_t *offset, void *dst, const UA_DataType *dataType) {
    UA_init(dst, dataType);
    uintptr_t ptr = (uintptr_t)dst;
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    UA_Byte membersSize = dataType->membersSize;
    for(size_t i=0;i<membersSize && retval == UA_STATUSCODE_GOOD; i++) {
        const UA_DataTypeMember *member = &dataType->members[i];
        const UA_DataType *memberType;
        if(member->namespaceZero)
            memberType = &UA_TYPES[member->memberTypeIndex];
        else
            memberType = dataType - dataType->typeIndex + member->memberTypeIndex;

        if(member->isArray) {
            ptr += (member->padding >> 3);
            UA_Int32 *noElements = (UA_Int32*)ptr;
            UA_Int32 tempNoElements;
            retval |= UA_Int32_decodeBinary(src, offset, &tempNoElements);
            if(retval)
                continue;
            ptr += sizeof(UA_Int32) + (member->padding & 0x07);
            retval = UA_Array_decodeBinary(src, offset, tempNoElements, (void**)ptr, memberType);
            if(retval == UA_STATUSCODE_GOOD)
                *noElements = tempNoElements;
            ptr += sizeof(void*);
            continue;
        }

        ptr += member->padding;
        if(!member->namespaceZero) {
            UA_decodeBinary(src, offset, (void*)ptr, memberType);
            ptr += memberType->memSize;
            continue;
        }

        switch(member->memberTypeIndex) {
        case UA_TYPES_BOOLEAN:
        case UA_TYPES_SBYTE:
        case UA_TYPES_BYTE:
            retval = UA_Byte_decodeBinary(src, offset, (UA_Byte*)ptr);
            break;
        case UA_TYPES_INT16:
        case UA_TYPES_UINT16:
            retval = UA_Int16_decodeBinary(src, offset, (UA_Int16*)ptr);
            break;
        case UA_TYPES_INT32:
        case UA_TYPES_UINT32:
        case UA_TYPES_FLOAT:
        case UA_TYPES_STATUSCODE:
            retval = UA_UInt32_decodeBinary(src, offset, (UA_UInt32*)ptr);
            break;
        case UA_TYPES_INT64:
        case UA_TYPES_UINT64:
        case UA_TYPES_DOUBLE:
        case UA_TYPES_DATETIME:
            retval = UA_UInt64_decodeBinary(src, offset, (UA_UInt64*)ptr);
            break;
        case UA_TYPES_GUID:
            retval = UA_Guid_decodeBinary(src, offset, (UA_Guid*)ptr);
            break;
        case UA_TYPES_NODEID:
            retval = UA_NodeId_decodeBinary(src, offset, (UA_NodeId*)ptr);
            break;
        case UA_TYPES_EXPANDEDNODEID:
            retval = UA_ExpandedNodeId_decodeBinary(src, offset, (UA_ExpandedNodeId*)ptr);
            break;
        case UA_TYPES_LOCALIZEDTEXT:
            retval = UA_LocalizedText_decodeBinary(src, offset, (UA_LocalizedText*)ptr);
            break;
        case UA_TYPES_EXTENSIONOBJECT:
            retval = UA_ExtensionObject_decodeBinary(src, offset, (UA_ExtensionObject*)ptr);
            break;
        case UA_TYPES_DATAVALUE:
            retval = UA_DataValue_decodeBinary(src, offset, (UA_DataValue*)ptr);
            break;
        case UA_TYPES_VARIANT:
            retval = UA_Variant_decodeBinary(src, offset, (UA_Variant*)ptr);
            break;
        case UA_TYPES_DIAGNOSTICINFO:
            retval = UA_DiagnosticInfo_decodeBinary(src, offset, (UA_DiagnosticInfo*)ptr);
            break;
        default:
            // UA_TYPES_QUALIFIEDNAME, UA_TYPES_STRING, UA_TYPES_BYTESTRING, UA_TYPES_XMLELEMENT:
            retval = UA_decodeBinary(src, offset, (void*)ptr, memberType);
        }
        ptr += memberType->memSize;
    }
    if(retval != UA_STATUSCODE_GOOD)
        UA_deleteMembers(dst, dataType);
    return retval;
}

/******************/
/* Array Handling */
/******************/

size_t UA_Array_calcSizeBinary(const void *p, UA_Int32 noElements, const UA_DataType *dataType) {
    if(noElements <= 0)
        return 4;
    size_t size = 4; // the array size encoding
    if(dataType->fixedSize) {
        size += noElements * UA_calcSizeBinary(p, dataType);
        return size;
    }
    uintptr_t ptr = (uintptr_t)p;
    for(int i=0;i<noElements;i++) {
        size += UA_calcSizeBinary((void*)ptr, dataType);
        ptr += dataType->memSize;
    }
    return size;
}

UA_StatusCode UA_Array_encodeBinary(const void *src, UA_Int32 noElements, const UA_DataType *dataType,
                                    UA_ByteString *dst, size_t *offset) {
    if(noElements <= -1)
        noElements = -1;
    UA_Int32_encodeBinary(&noElements, dst, offset);
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    uintptr_t ptr = (uintptr_t)src;
    for(int i=0;i<noElements && retval == UA_STATUSCODE_GOOD;i++) {
        retval = UA_encodeBinary((const void*)ptr, dataType, dst, offset);
        ptr += dataType->memSize;
    }
    return retval;
}

UA_StatusCode UA_Array_decodeBinary(const UA_ByteString *src, size_t *offset, UA_Int32 noElements,
                                    void **dst, const UA_DataType *dataType) {
    if(noElements <= 0) {
        *dst = UA_NULL;
        return UA_STATUSCODE_GOOD;
    }

    if((UA_Int32)dataType->memSize * noElements < 0 || dataType->memSize * noElements > MAX_ARRAY_SIZE )
        return UA_STATUSCODE_BADOUTOFMEMORY;

    // filter out arrays that can obviously not be parsed
    if(*offset + ((dataType->memSize * noElements)/32) > (UA_UInt32)src->length)
        return UA_STATUSCODE_BADDECODINGERROR;

    *dst = UA_malloc(dataType->memSize * noElements);
    if(!*dst)
        return UA_STATUSCODE_BADOUTOFMEMORY;

    uintptr_t ptr = (uintptr_t)*dst;
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    UA_Int32 i;
    for(i=0;i<noElements && retval == UA_STATUSCODE_GOOD;i++) {
        retval = UA_decodeBinary(src, offset, (void*)ptr, dataType);
        ptr += dataType->memSize;
    }
    if(retval != UA_STATUSCODE_GOOD) {
        UA_Array_delete(*dst, dataType, i);
        *dst = UA_NULL;
    }
    return retval;
}
/**
* @file ua_types_generated.c
*
* @brief Autogenerated data types
*
* Generated from Opc.Ua.Types.bsd with script C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/tools/generate_datatypes.py
* on host SPECTRE by user lars at 2015-04-17 10:13:36
*/


const UA_DataType *UA_TYPES = (UA_DataType[]){

/* UA_Boolean */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 1}, .memSize = sizeof(UA_Boolean), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_BOOLEAN,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_BOOLEAN },

/* UA_SByte */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 2}, .memSize = sizeof(UA_SByte), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_SBYTE,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_SBYTE },

/* UA_Byte */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 3}, .memSize = sizeof(UA_Byte), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_BYTE,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_BYTE },

/* UA_Int16 */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 4}, .memSize = sizeof(UA_Int16), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT16,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT16 },

/* UA_UInt16 */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 5}, .memSize = sizeof(UA_UInt16), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_UINT16,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_UINT16 },

/* UA_Int32 */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 6}, .memSize = sizeof(UA_Int32), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_UInt32 */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 7}, .memSize = sizeof(UA_UInt32), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_UINT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_UINT32 },

/* UA_Int64 */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 8}, .memSize = sizeof(UA_Int64), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT64,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT64 },

/* UA_UInt64 */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 9}, .memSize = sizeof(UA_UInt64), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_UINT64,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_UINT64 },

/* UA_Float */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 10}, .memSize = sizeof(UA_Float), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_FLOAT,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_FLOAT },

/* UA_Double */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 11}, .memSize = sizeof(UA_Double), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_DOUBLE,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_DOUBLE },

/* UA_String */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 12}, .memSize = sizeof(UA_String), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_String, data) - sizeof(UA_Int32), .isArray = UA_TRUE }}, .typeIndex = UA_TYPES_STRING },

/* UA_DateTime */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 13}, .memSize = sizeof(UA_DateTime), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_DATETIME,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_DATETIME },

/* UA_Guid */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 14}, .memSize = sizeof(UA_Guid), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_FALSE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_GUID,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_GUID },

/* UA_ByteString */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 15}, .memSize = sizeof(UA_ByteString), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_String, data) - sizeof(UA_Int32), .isArray = UA_TRUE }}, .typeIndex = UA_TYPES_BYTESTRING },

/* UA_XmlElement */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 16}, .memSize = sizeof(UA_XmlElement), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_String, data) - sizeof(UA_Int32), .isArray = UA_TRUE }}, .typeIndex = UA_TYPES_XMLELEMENT },

/* UA_NodeId */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 17}, .memSize = sizeof(UA_NodeId), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_NODEID,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_NODEID },

/* UA_ExpandedNodeId */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 18}, .memSize = sizeof(UA_ExpandedNodeId), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_EXPANDEDNODEID },

/* UA_StatusCode */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 19}, .memSize = sizeof(UA_StatusCode), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_STATUSCODE,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_STATUSCODE },

/* UA_QualifiedName */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 20}, .memSize = sizeof(UA_QualifiedName), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 2, .members = {
	{.memberTypeIndex = UA_TYPES_UINT16, .namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE },
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_QualifiedName, name) - sizeof(UA_UInt16), .isArray = UA_FALSE }},
.typeIndex = UA_TYPES_QUALIFIEDNAME },

/* UA_LocalizedText */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 21}, .memSize = sizeof(UA_LocalizedText), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_LOCALIZEDTEXT },

/* UA_ExtensionObject */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 22}, .memSize = sizeof(UA_ExtensionObject), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_EXTENSIONOBJECT,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_EXTENSIONOBJECT },

/* UA_DataValue */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 23}, .memSize = sizeof(UA_DataValue), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_DATAVALUE,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_DATAVALUE },

/* UA_Variant */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 24}, .memSize = sizeof(UA_Variant), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_VARIANT,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_VARIANT },

/* UA_DiagnosticInfo */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 25}, .memSize = sizeof(UA_DiagnosticInfo), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_DIAGNOSTICINFO },

/* UA_IdType */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 256}, .memSize = sizeof(UA_IdType), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_NodeClass */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 257}, .memSize = sizeof(UA_NodeClass), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_ReferenceNode */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 285}, .memSize = sizeof(UA_ReferenceNode), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_REFERENCENODE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceNode, referenceTypeId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceNode, isInverse) - (offsetof(UA_ReferenceNode, referenceTypeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceNode, targetId) - (offsetof(UA_ReferenceNode, isInverse) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, }},

/* UA_ApplicationType */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 307}, .memSize = sizeof(UA_ApplicationType), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_ApplicationDescription */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 308}, .memSize = sizeof(UA_ApplicationDescription), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_APPLICATIONDESCRIPTION, .membersSize = 7,
	.members={
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ApplicationDescription, applicationUri) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ApplicationDescription, productUri) - (offsetof(UA_ApplicationDescription, applicationUri) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ApplicationDescription, applicationName) - (offsetof(UA_ApplicationDescription, productUri) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_APPLICATIONTYPE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ApplicationDescription, applicationType) - (offsetof(UA_ApplicationDescription, applicationName) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ApplicationDescription, gatewayServerUri) - (offsetof(UA_ApplicationDescription, applicationType) + sizeof(UA_ApplicationType)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ApplicationDescription, discoveryProfileUri) - (offsetof(UA_ApplicationDescription, gatewayServerUri) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ApplicationDescription, discoveryUrlsSize) - (offsetof(UA_ApplicationDescription, discoveryProfileUri) + sizeof(UA_String))) << 3) + (offsetof(UA_ApplicationDescription, discoveryUrls) - sizeof(UA_Int32) - offsetof(UA_ApplicationDescription, discoveryUrlsSize)), .isArray = UA_TRUE }, }},

/* UA_RequestHeader */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 389}, .memSize = sizeof(UA_RequestHeader), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_REQUESTHEADER, .membersSize = 7,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RequestHeader, authenticationToken) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DATETIME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RequestHeader, timestamp) - (offsetof(UA_RequestHeader, authenticationToken) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RequestHeader, requestHandle) - (offsetof(UA_RequestHeader, timestamp) + sizeof(UA_DateTime)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RequestHeader, returnDiagnostics) - (offsetof(UA_RequestHeader, requestHandle) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RequestHeader, auditEntryId) - (offsetof(UA_RequestHeader, returnDiagnostics) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RequestHeader, timeoutHint) - (offsetof(UA_RequestHeader, auditEntryId) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXTENSIONOBJECT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RequestHeader, additionalHeader) - (offsetof(UA_RequestHeader, timeoutHint) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_ResponseHeader */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 392}, .memSize = sizeof(UA_ResponseHeader), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_RESPONSEHEADER, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_DATETIME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ResponseHeader, timestamp) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ResponseHeader, requestHandle) - (offsetof(UA_ResponseHeader, timestamp) + sizeof(UA_DateTime)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ResponseHeader, serviceResult) - (offsetof(UA_ResponseHeader, requestHandle) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ResponseHeader, serviceDiagnostics) - (offsetof(UA_ResponseHeader, serviceResult) + sizeof(UA_StatusCode)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ResponseHeader, stringTableSize) - (offsetof(UA_ResponseHeader, serviceDiagnostics) + sizeof(UA_DiagnosticInfo))) << 3) + (offsetof(UA_ResponseHeader, stringTable) - sizeof(UA_Int32) - offsetof(UA_ResponseHeader, stringTableSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_EXTENSIONOBJECT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ResponseHeader, additionalHeader) - (offsetof(UA_ResponseHeader, stringTable) + sizeof(void*)), .isArray = UA_FALSE }, }},

/* UA_FindServersRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 420}, .memSize = sizeof(UA_FindServersRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_FINDSERVERSREQUEST, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_FindServersRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_FindServersRequest, endpointUrl) - (offsetof(UA_FindServersRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_FindServersRequest, localeIdsSize) - (offsetof(UA_FindServersRequest, endpointUrl) + sizeof(UA_String))) << 3) + (offsetof(UA_FindServersRequest, localeIds) - sizeof(UA_Int32) - offsetof(UA_FindServersRequest, localeIdsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_FindServersRequest, serverUrisSize) - (offsetof(UA_FindServersRequest, localeIds) + sizeof(void*))) << 3) + (offsetof(UA_FindServersRequest, serverUris) - sizeof(UA_Int32) - offsetof(UA_FindServersRequest, serverUrisSize)), .isArray = UA_TRUE }, }},

/* UA_FindServersResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 423}, .memSize = sizeof(UA_FindServersResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_FINDSERVERSRESPONSE, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_FindServersResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_APPLICATIONDESCRIPTION, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_FindServersResponse, serversSize) - (offsetof(UA_FindServersResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_FindServersResponse, servers) - sizeof(UA_Int32) - offsetof(UA_FindServersResponse, serversSize)), .isArray = UA_TRUE }, }},

/* UA_MessageSecurityMode */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 302}, .memSize = sizeof(UA_MessageSecurityMode), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_UserTokenType */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 303}, .memSize = sizeof(UA_UserTokenType), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_UserTokenPolicy */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 304}, .memSize = sizeof(UA_UserTokenPolicy), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_USERTOKENPOLICY, .membersSize = 5,
	.members={
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_UserTokenPolicy, policyId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_USERTOKENTYPE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_UserTokenPolicy, tokenType) - (offsetof(UA_UserTokenPolicy, policyId) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_UserTokenPolicy, issuedTokenType) - (offsetof(UA_UserTokenPolicy, tokenType) + sizeof(UA_UserTokenType)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_UserTokenPolicy, issuerEndpointUrl) - (offsetof(UA_UserTokenPolicy, issuedTokenType) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_UserTokenPolicy, securityPolicyUri) - (offsetof(UA_UserTokenPolicy, issuerEndpointUrl) + sizeof(UA_String)), .isArray = UA_FALSE }, }},

/* UA_EndpointDescription */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 312}, .memSize = sizeof(UA_EndpointDescription), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ENDPOINTDESCRIPTION, .membersSize = 8,
	.members={
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_EndpointDescription, endpointUrl) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_APPLICATIONDESCRIPTION, .namespaceZero = UA_TRUE, .padding = offsetof(UA_EndpointDescription, server) - (offsetof(UA_EndpointDescription, endpointUrl) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_EndpointDescription, serverCertificate) - (offsetof(UA_EndpointDescription, server) + sizeof(UA_ApplicationDescription)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_MESSAGESECURITYMODE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_EndpointDescription, securityMode) - (offsetof(UA_EndpointDescription, serverCertificate) + sizeof(UA_ByteString)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_EndpointDescription, securityPolicyUri) - (offsetof(UA_EndpointDescription, securityMode) + sizeof(UA_MessageSecurityMode)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_USERTOKENPOLICY, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_EndpointDescription, userIdentityTokensSize) - (offsetof(UA_EndpointDescription, securityPolicyUri) + sizeof(UA_String))) << 3) + (offsetof(UA_EndpointDescription, userIdentityTokens) - sizeof(UA_Int32) - offsetof(UA_EndpointDescription, userIdentityTokensSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_EndpointDescription, transportProfileUri) - (offsetof(UA_EndpointDescription, userIdentityTokens) + sizeof(void*)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_EndpointDescription, securityLevel) - (offsetof(UA_EndpointDescription, transportProfileUri) + sizeof(UA_String)), .isArray = UA_FALSE }, }},

/* UA_GetEndpointsRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 426}, .memSize = sizeof(UA_GetEndpointsRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_GETENDPOINTSREQUEST, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_GetEndpointsRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_GetEndpointsRequest, endpointUrl) - (offsetof(UA_GetEndpointsRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_GetEndpointsRequest, localeIdsSize) - (offsetof(UA_GetEndpointsRequest, endpointUrl) + sizeof(UA_String))) << 3) + (offsetof(UA_GetEndpointsRequest, localeIds) - sizeof(UA_Int32) - offsetof(UA_GetEndpointsRequest, localeIdsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_GetEndpointsRequest, profileUrisSize) - (offsetof(UA_GetEndpointsRequest, localeIds) + sizeof(void*))) << 3) + (offsetof(UA_GetEndpointsRequest, profileUris) - sizeof(UA_Int32) - offsetof(UA_GetEndpointsRequest, profileUrisSize)), .isArray = UA_TRUE }, }},

/* UA_GetEndpointsResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 429}, .memSize = sizeof(UA_GetEndpointsResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_GETENDPOINTSRESPONSE, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_GetEndpointsResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_ENDPOINTDESCRIPTION, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_GetEndpointsResponse, endpointsSize) - (offsetof(UA_GetEndpointsResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_GetEndpointsResponse, endpoints) - sizeof(UA_Int32) - offsetof(UA_GetEndpointsResponse, endpointsSize)), .isArray = UA_TRUE }, }},

/* UA_SecurityTokenRequestType */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 315}, .memSize = sizeof(UA_SecurityTokenRequestType), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_ChannelSecurityToken */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 441}, .memSize = sizeof(UA_ChannelSecurityToken), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = sizeof(UA_ChannelSecurityToken) == 20, .typeIndex = UA_TYPES_CHANNELSECURITYTOKEN, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ChannelSecurityToken, channelId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ChannelSecurityToken, tokenId) - (offsetof(UA_ChannelSecurityToken, channelId) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DATETIME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ChannelSecurityToken, createdAt) - (offsetof(UA_ChannelSecurityToken, tokenId) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ChannelSecurityToken, revisedLifetime) - (offsetof(UA_ChannelSecurityToken, createdAt) + sizeof(UA_DateTime)), .isArray = UA_FALSE }, }},

/* UA_OpenSecureChannelRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 444}, .memSize = sizeof(UA_OpenSecureChannelRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_OPENSECURECHANNELREQUEST, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_OpenSecureChannelRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_OpenSecureChannelRequest, clientProtocolVersion) - (offsetof(UA_OpenSecureChannelRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_SECURITYTOKENREQUESTTYPE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_OpenSecureChannelRequest, requestType) - (offsetof(UA_OpenSecureChannelRequest, clientProtocolVersion) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_MESSAGESECURITYMODE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_OpenSecureChannelRequest, securityMode) - (offsetof(UA_OpenSecureChannelRequest, requestType) + sizeof(UA_SecurityTokenRequestType)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_OpenSecureChannelRequest, clientNonce) - (offsetof(UA_OpenSecureChannelRequest, securityMode) + sizeof(UA_MessageSecurityMode)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_OpenSecureChannelRequest, requestedLifetime) - (offsetof(UA_OpenSecureChannelRequest, clientNonce) + sizeof(UA_ByteString)), .isArray = UA_FALSE }, }},

/* UA_OpenSecureChannelResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 447}, .memSize = sizeof(UA_OpenSecureChannelResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_OPENSECURECHANNELRESPONSE, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_OpenSecureChannelResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_OpenSecureChannelResponse, serverProtocolVersion) - (offsetof(UA_OpenSecureChannelResponse, responseHeader) + sizeof(UA_ResponseHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_CHANNELSECURITYTOKEN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_OpenSecureChannelResponse, securityToken) - (offsetof(UA_OpenSecureChannelResponse, serverProtocolVersion) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_OpenSecureChannelResponse, serverNonce) - (offsetof(UA_OpenSecureChannelResponse, securityToken) + sizeof(UA_ChannelSecurityToken)), .isArray = UA_FALSE }, }},

/* UA_CloseSecureChannelRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 450}, .memSize = sizeof(UA_CloseSecureChannelRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CLOSESECURECHANNELREQUEST, .membersSize = 1,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CloseSecureChannelRequest, requestHeader) - 0, .isArray = UA_FALSE }, }},

/* UA_CloseSecureChannelResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 453}, .memSize = sizeof(UA_CloseSecureChannelResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CLOSESECURECHANNELRESPONSE, .membersSize = 1,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CloseSecureChannelResponse, responseHeader) - 0, .isArray = UA_FALSE }, }},

/* UA_SignedSoftwareCertificate */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 344}, .memSize = sizeof(UA_SignedSoftwareCertificate), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_SIGNEDSOFTWARECERTIFICATE, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SignedSoftwareCertificate, certificateData) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SignedSoftwareCertificate, signature) - (offsetof(UA_SignedSoftwareCertificate, certificateData) + sizeof(UA_ByteString)), .isArray = UA_FALSE }, }},

/* UA_SignatureData */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 456}, .memSize = sizeof(UA_SignatureData), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_SIGNATUREDATA, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SignatureData, algorithm) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SignatureData, signature) - (offsetof(UA_SignatureData, algorithm) + sizeof(UA_String)), .isArray = UA_FALSE }, }},

/* UA_CreateSessionRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 459}, .memSize = sizeof(UA_CreateSessionRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CREATESESSIONREQUEST, .membersSize = 9,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_APPLICATIONDESCRIPTION, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionRequest, clientDescription) - (offsetof(UA_CreateSessionRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionRequest, serverUri) - (offsetof(UA_CreateSessionRequest, clientDescription) + sizeof(UA_ApplicationDescription)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionRequest, endpointUrl) - (offsetof(UA_CreateSessionRequest, serverUri) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionRequest, sessionName) - (offsetof(UA_CreateSessionRequest, endpointUrl) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionRequest, clientNonce) - (offsetof(UA_CreateSessionRequest, sessionName) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionRequest, clientCertificate) - (offsetof(UA_CreateSessionRequest, clientNonce) + sizeof(UA_ByteString)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DOUBLE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionRequest, requestedSessionTimeout) - (offsetof(UA_CreateSessionRequest, clientCertificate) + sizeof(UA_ByteString)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionRequest, maxResponseMessageSize) - (offsetof(UA_CreateSessionRequest, requestedSessionTimeout) + sizeof(UA_Double)), .isArray = UA_FALSE }, }},

/* UA_CreateSessionResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 462}, .memSize = sizeof(UA_CreateSessionResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CREATESESSIONRESPONSE, .membersSize = 10,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionResponse, sessionId) - (offsetof(UA_CreateSessionResponse, responseHeader) + sizeof(UA_ResponseHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionResponse, authenticationToken) - (offsetof(UA_CreateSessionResponse, sessionId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DOUBLE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionResponse, revisedSessionTimeout) - (offsetof(UA_CreateSessionResponse, authenticationToken) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionResponse, serverNonce) - (offsetof(UA_CreateSessionResponse, revisedSessionTimeout) + sizeof(UA_Double)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionResponse, serverCertificate) - (offsetof(UA_CreateSessionResponse, serverNonce) + sizeof(UA_ByteString)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_ENDPOINTDESCRIPTION, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_CreateSessionResponse, serverEndpointsSize) - (offsetof(UA_CreateSessionResponse, serverCertificate) + sizeof(UA_ByteString))) << 3) + (offsetof(UA_CreateSessionResponse, serverEndpoints) - sizeof(UA_Int32) - offsetof(UA_CreateSessionResponse, serverEndpointsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_SIGNEDSOFTWARECERTIFICATE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_CreateSessionResponse, serverSoftwareCertificatesSize) - (offsetof(UA_CreateSessionResponse, serverEndpoints) + sizeof(void*))) << 3) + (offsetof(UA_CreateSessionResponse, serverSoftwareCertificates) - sizeof(UA_Int32) - offsetof(UA_CreateSessionResponse, serverSoftwareCertificatesSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_SIGNATUREDATA, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionResponse, serverSignature) - (offsetof(UA_CreateSessionResponse, serverSoftwareCertificates) + sizeof(void*)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionResponse, maxRequestMessageSize) - (offsetof(UA_CreateSessionResponse, serverSignature) + sizeof(UA_SignatureData)), .isArray = UA_FALSE }, }},

/* UA_ActivateSessionRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 465}, .memSize = sizeof(UA_ActivateSessionRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ACTIVATESESSIONREQUEST, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ActivateSessionRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_SIGNATUREDATA, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ActivateSessionRequest, clientSignature) - (offsetof(UA_ActivateSessionRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_SIGNEDSOFTWARECERTIFICATE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ActivateSessionRequest, clientSoftwareCertificatesSize) - (offsetof(UA_ActivateSessionRequest, clientSignature) + sizeof(UA_SignatureData))) << 3) + (offsetof(UA_ActivateSessionRequest, clientSoftwareCertificates) - sizeof(UA_Int32) - offsetof(UA_ActivateSessionRequest, clientSoftwareCertificatesSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ActivateSessionRequest, localeIdsSize) - (offsetof(UA_ActivateSessionRequest, clientSoftwareCertificates) + sizeof(void*))) << 3) + (offsetof(UA_ActivateSessionRequest, localeIds) - sizeof(UA_Int32) - offsetof(UA_ActivateSessionRequest, localeIdsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_EXTENSIONOBJECT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ActivateSessionRequest, userIdentityToken) - (offsetof(UA_ActivateSessionRequest, localeIds) + sizeof(void*)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_SIGNATUREDATA, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ActivateSessionRequest, userTokenSignature) - (offsetof(UA_ActivateSessionRequest, userIdentityToken) + sizeof(UA_ExtensionObject)), .isArray = UA_FALSE }, }},

/* UA_ActivateSessionResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 468}, .memSize = sizeof(UA_ActivateSessionResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ACTIVATESESSIONRESPONSE, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ActivateSessionResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ActivateSessionResponse, serverNonce) - (offsetof(UA_ActivateSessionResponse, responseHeader) + sizeof(UA_ResponseHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ActivateSessionResponse, resultsSize) - (offsetof(UA_ActivateSessionResponse, serverNonce) + sizeof(UA_ByteString))) << 3) + (offsetof(UA_ActivateSessionResponse, results) - sizeof(UA_Int32) - offsetof(UA_ActivateSessionResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ActivateSessionResponse, diagnosticInfosSize) - (offsetof(UA_ActivateSessionResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_ActivateSessionResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_ActivateSessionResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_CloseSessionRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 471}, .memSize = sizeof(UA_CloseSessionRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CLOSESESSIONREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CloseSessionRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CloseSessionRequest, deleteSubscriptions) - (offsetof(UA_CloseSessionRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, }},

/* UA_CloseSessionResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 474}, .memSize = sizeof(UA_CloseSessionResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CLOSESESSIONRESPONSE, .membersSize = 1,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CloseSessionResponse, responseHeader) - 0, .isArray = UA_FALSE }, }},

/* UA_NodeAttributesMask */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 348}, .memSize = sizeof(UA_NodeAttributesMask), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_NodeAttributes */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 349}, .memSize = sizeof(UA_NodeAttributes), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_NODEATTRIBUTES, .membersSize = 5,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_NodeAttributes, specifiedAttributes) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_NodeAttributes, displayName) - (offsetof(UA_NodeAttributes, specifiedAttributes) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_NodeAttributes, description) - (offsetof(UA_NodeAttributes, displayName) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_NodeAttributes, writeMask) - (offsetof(UA_NodeAttributes, description) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_NodeAttributes, userWriteMask) - (offsetof(UA_NodeAttributes, writeMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_ObjectAttributes */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 352}, .memSize = sizeof(UA_ObjectAttributes), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_OBJECTATTRIBUTES, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectAttributes, specifiedAttributes) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectAttributes, displayName) - (offsetof(UA_ObjectAttributes, specifiedAttributes) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectAttributes, description) - (offsetof(UA_ObjectAttributes, displayName) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectAttributes, writeMask) - (offsetof(UA_ObjectAttributes, description) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectAttributes, userWriteMask) - (offsetof(UA_ObjectAttributes, writeMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectAttributes, eventNotifier) - (offsetof(UA_ObjectAttributes, userWriteMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_VariableAttributes */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 355}, .memSize = sizeof(UA_VariableAttributes), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_VARIABLEATTRIBUTES, .membersSize = 13,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, specifiedAttributes) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, displayName) - (offsetof(UA_VariableAttributes, specifiedAttributes) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, description) - (offsetof(UA_VariableAttributes, displayName) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, writeMask) - (offsetof(UA_VariableAttributes, description) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, userWriteMask) - (offsetof(UA_VariableAttributes, writeMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_VARIANT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, value) - (offsetof(UA_VariableAttributes, userWriteMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, dataType) - (offsetof(UA_VariableAttributes, value) + sizeof(UA_Variant)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_INT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, valueRank) - (offsetof(UA_VariableAttributes, dataType) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_VariableAttributes, arrayDimensionsSize) - (offsetof(UA_VariableAttributes, valueRank) + sizeof(UA_Int32))) << 3) + (offsetof(UA_VariableAttributes, arrayDimensions) - sizeof(UA_Int32) - offsetof(UA_VariableAttributes, arrayDimensionsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, accessLevel) - (offsetof(UA_VariableAttributes, arrayDimensions) + sizeof(void*)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, userAccessLevel) - (offsetof(UA_VariableAttributes, accessLevel) + sizeof(UA_Byte)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DOUBLE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, minimumSamplingInterval) - (offsetof(UA_VariableAttributes, userAccessLevel) + sizeof(UA_Byte)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, historizing) - (offsetof(UA_VariableAttributes, minimumSamplingInterval) + sizeof(UA_Double)), .isArray = UA_FALSE }, }},

/* UA_ObjectTypeAttributes */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 361}, .memSize = sizeof(UA_ObjectTypeAttributes), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_OBJECTTYPEATTRIBUTES, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectTypeAttributes, specifiedAttributes) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectTypeAttributes, displayName) - (offsetof(UA_ObjectTypeAttributes, specifiedAttributes) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectTypeAttributes, description) - (offsetof(UA_ObjectTypeAttributes, displayName) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectTypeAttributes, writeMask) - (offsetof(UA_ObjectTypeAttributes, description) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectTypeAttributes, userWriteMask) - (offsetof(UA_ObjectTypeAttributes, writeMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectTypeAttributes, isAbstract) - (offsetof(UA_ObjectTypeAttributes, userWriteMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_ReferenceTypeAttributes */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 367}, .memSize = sizeof(UA_ReferenceTypeAttributes), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_REFERENCETYPEATTRIBUTES, .membersSize = 8,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceTypeAttributes, specifiedAttributes) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceTypeAttributes, displayName) - (offsetof(UA_ReferenceTypeAttributes, specifiedAttributes) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceTypeAttributes, description) - (offsetof(UA_ReferenceTypeAttributes, displayName) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceTypeAttributes, writeMask) - (offsetof(UA_ReferenceTypeAttributes, description) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceTypeAttributes, userWriteMask) - (offsetof(UA_ReferenceTypeAttributes, writeMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceTypeAttributes, isAbstract) - (offsetof(UA_ReferenceTypeAttributes, userWriteMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceTypeAttributes, symmetric) - (offsetof(UA_ReferenceTypeAttributes, isAbstract) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceTypeAttributes, inverseName) - (offsetof(UA_ReferenceTypeAttributes, symmetric) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, }},

/* UA_ViewAttributes */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 373}, .memSize = sizeof(UA_ViewAttributes), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_VIEWATTRIBUTES, .membersSize = 7,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ViewAttributes, specifiedAttributes) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ViewAttributes, displayName) - (offsetof(UA_ViewAttributes, specifiedAttributes) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ViewAttributes, description) - (offsetof(UA_ViewAttributes, displayName) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ViewAttributes, writeMask) - (offsetof(UA_ViewAttributes, description) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ViewAttributes, userWriteMask) - (offsetof(UA_ViewAttributes, writeMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ViewAttributes, containsNoLoops) - (offsetof(UA_ViewAttributes, userWriteMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ViewAttributes, eventNotifier) - (offsetof(UA_ViewAttributes, containsNoLoops) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, }},

/* UA_AddNodesItem */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 376}, .memSize = sizeof(UA_AddNodesItem), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ADDNODESITEM, .membersSize = 7,
	.members={
	{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesItem, parentNodeId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesItem, referenceTypeId) - (offsetof(UA_AddNodesItem, parentNodeId) + sizeof(UA_ExpandedNodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesItem, requestedNewNodeId) - (offsetof(UA_AddNodesItem, referenceTypeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_QUALIFIEDNAME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesItem, browseName) - (offsetof(UA_AddNodesItem, requestedNewNodeId) + sizeof(UA_ExpandedNodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODECLASS, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesItem, nodeClass) - (offsetof(UA_AddNodesItem, browseName) + sizeof(UA_QualifiedName)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXTENSIONOBJECT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesItem, nodeAttributes) - (offsetof(UA_AddNodesItem, nodeClass) + sizeof(UA_NodeClass)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesItem, typeDefinition) - (offsetof(UA_AddNodesItem, nodeAttributes) + sizeof(UA_ExtensionObject)), .isArray = UA_FALSE }, }},

/* UA_AddNodesResult */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 483}, .memSize = sizeof(UA_AddNodesResult), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ADDNODESRESULT, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesResult, statusCode) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesResult, addedNodeId) - (offsetof(UA_AddNodesResult, statusCode) + sizeof(UA_StatusCode)), .isArray = UA_FALSE }, }},

/* UA_AddNodesRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 486}, .memSize = sizeof(UA_AddNodesRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ADDNODESREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_ADDNODESITEM, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_AddNodesRequest, nodesToAddSize) - (offsetof(UA_AddNodesRequest, requestHeader) + sizeof(UA_RequestHeader))) << 3) + (offsetof(UA_AddNodesRequest, nodesToAdd) - sizeof(UA_Int32) - offsetof(UA_AddNodesRequest, nodesToAddSize)), .isArray = UA_TRUE }, }},

/* UA_AddNodesResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 489}, .memSize = sizeof(UA_AddNodesResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ADDNODESRESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_ADDNODESRESULT, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_AddNodesResponse, resultsSize) - (offsetof(UA_AddNodesResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_AddNodesResponse, results) - sizeof(UA_Int32) - offsetof(UA_AddNodesResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_AddNodesResponse, diagnosticInfosSize) - (offsetof(UA_AddNodesResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_AddNodesResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_AddNodesResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_AddReferencesItem */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 379}, .memSize = sizeof(UA_AddReferencesItem), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ADDREFERENCESITEM, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddReferencesItem, sourceNodeId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddReferencesItem, referenceTypeId) - (offsetof(UA_AddReferencesItem, sourceNodeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddReferencesItem, isForward) - (offsetof(UA_AddReferencesItem, referenceTypeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddReferencesItem, targetServerUri) - (offsetof(UA_AddReferencesItem, isForward) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddReferencesItem, targetNodeId) - (offsetof(UA_AddReferencesItem, targetServerUri) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODECLASS, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddReferencesItem, targetNodeClass) - (offsetof(UA_AddReferencesItem, targetNodeId) + sizeof(UA_ExpandedNodeId)), .isArray = UA_FALSE }, }},

/* UA_AddReferencesRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 492}, .memSize = sizeof(UA_AddReferencesRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ADDREFERENCESREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddReferencesRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_ADDREFERENCESITEM, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_AddReferencesRequest, referencesToAddSize) - (offsetof(UA_AddReferencesRequest, requestHeader) + sizeof(UA_RequestHeader))) << 3) + (offsetof(UA_AddReferencesRequest, referencesToAdd) - sizeof(UA_Int32) - offsetof(UA_AddReferencesRequest, referencesToAddSize)), .isArray = UA_TRUE }, }},

/* UA_AddReferencesResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 495}, .memSize = sizeof(UA_AddReferencesResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ADDREFERENCESRESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddReferencesResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_AddReferencesResponse, resultsSize) - (offsetof(UA_AddReferencesResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_AddReferencesResponse, results) - sizeof(UA_Int32) - offsetof(UA_AddReferencesResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_AddReferencesResponse, diagnosticInfosSize) - (offsetof(UA_AddReferencesResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_AddReferencesResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_AddReferencesResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_DeleteNodesItem */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 382}, .memSize = sizeof(UA_DeleteNodesItem), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_DELETENODESITEM, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteNodesItem, nodeId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteNodesItem, deleteTargetReferences) - (offsetof(UA_DeleteNodesItem, nodeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, }},

/* UA_DeleteNodesRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 498}, .memSize = sizeof(UA_DeleteNodesRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_DELETENODESREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteNodesRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DELETENODESITEM, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_DeleteNodesRequest, nodesToDeleteSize) - (offsetof(UA_DeleteNodesRequest, requestHeader) + sizeof(UA_RequestHeader))) << 3) + (offsetof(UA_DeleteNodesRequest, nodesToDelete) - sizeof(UA_Int32) - offsetof(UA_DeleteNodesRequest, nodesToDeleteSize)), .isArray = UA_TRUE }, }},

/* UA_DeleteNodesResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 501}, .memSize = sizeof(UA_DeleteNodesResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_DELETENODESRESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteNodesResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_DeleteNodesResponse, resultsSize) - (offsetof(UA_DeleteNodesResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_DeleteNodesResponse, results) - sizeof(UA_Int32) - offsetof(UA_DeleteNodesResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_DeleteNodesResponse, diagnosticInfosSize) - (offsetof(UA_DeleteNodesResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_DeleteNodesResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_DeleteNodesResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_DeleteReferencesItem */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 385}, .memSize = sizeof(UA_DeleteReferencesItem), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_DELETEREFERENCESITEM, .membersSize = 5,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteReferencesItem, sourceNodeId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteReferencesItem, referenceTypeId) - (offsetof(UA_DeleteReferencesItem, sourceNodeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteReferencesItem, isForward) - (offsetof(UA_DeleteReferencesItem, referenceTypeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteReferencesItem, targetNodeId) - (offsetof(UA_DeleteReferencesItem, isForward) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteReferencesItem, deleteBidirectional) - (offsetof(UA_DeleteReferencesItem, targetNodeId) + sizeof(UA_ExpandedNodeId)), .isArray = UA_FALSE }, }},

/* UA_DeleteReferencesRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 504}, .memSize = sizeof(UA_DeleteReferencesRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_DELETEREFERENCESREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteReferencesRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DELETEREFERENCESITEM, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_DeleteReferencesRequest, referencesToDeleteSize) - (offsetof(UA_DeleteReferencesRequest, requestHeader) + sizeof(UA_RequestHeader))) << 3) + (offsetof(UA_DeleteReferencesRequest, referencesToDelete) - sizeof(UA_Int32) - offsetof(UA_DeleteReferencesRequest, referencesToDeleteSize)), .isArray = UA_TRUE }, }},

/* UA_DeleteReferencesResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 507}, .memSize = sizeof(UA_DeleteReferencesResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_DELETEREFERENCESRESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteReferencesResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_DeleteReferencesResponse, resultsSize) - (offsetof(UA_DeleteReferencesResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_DeleteReferencesResponse, results) - sizeof(UA_Int32) - offsetof(UA_DeleteReferencesResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_DeleteReferencesResponse, diagnosticInfosSize) - (offsetof(UA_DeleteReferencesResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_DeleteReferencesResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_DeleteReferencesResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_BrowseDirection */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 510}, .memSize = sizeof(UA_BrowseDirection), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_ViewDescription */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 511}, .memSize = sizeof(UA_ViewDescription), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_VIEWDESCRIPTION, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ViewDescription, viewId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DATETIME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ViewDescription, timestamp) - (offsetof(UA_ViewDescription, viewId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ViewDescription, viewVersion) - (offsetof(UA_ViewDescription, timestamp) + sizeof(UA_DateTime)), .isArray = UA_FALSE }, }},

/* UA_BrowseDescription */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 514}, .memSize = sizeof(UA_BrowseDescription), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_BROWSEDESCRIPTION, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseDescription, nodeId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BROWSEDIRECTION, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseDescription, browseDirection) - (offsetof(UA_BrowseDescription, nodeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseDescription, referenceTypeId) - (offsetof(UA_BrowseDescription, browseDirection) + sizeof(UA_BrowseDirection)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseDescription, includeSubtypes) - (offsetof(UA_BrowseDescription, referenceTypeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseDescription, nodeClassMask) - (offsetof(UA_BrowseDescription, includeSubtypes) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseDescription, resultMask) - (offsetof(UA_BrowseDescription, nodeClassMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_BrowseResultMask */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 517}, .memSize = sizeof(UA_BrowseResultMask), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_ReferenceDescription */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 518}, .memSize = sizeof(UA_ReferenceDescription), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_REFERENCEDESCRIPTION, .membersSize = 7,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceDescription, referenceTypeId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceDescription, isForward) - (offsetof(UA_ReferenceDescription, referenceTypeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceDescription, nodeId) - (offsetof(UA_ReferenceDescription, isForward) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_QUALIFIEDNAME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceDescription, browseName) - (offsetof(UA_ReferenceDescription, nodeId) + sizeof(UA_ExpandedNodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceDescription, displayName) - (offsetof(UA_ReferenceDescription, browseName) + sizeof(UA_QualifiedName)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODECLASS, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceDescription, nodeClass) - (offsetof(UA_ReferenceDescription, displayName) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceDescription, typeDefinition) - (offsetof(UA_ReferenceDescription, nodeClass) + sizeof(UA_NodeClass)), .isArray = UA_FALSE }, }},

/* UA_BrowseResult */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 522}, .memSize = sizeof(UA_BrowseResult), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_BROWSERESULT, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseResult, statusCode) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseResult, continuationPoint) - (offsetof(UA_BrowseResult, statusCode) + sizeof(UA_StatusCode)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_REFERENCEDESCRIPTION, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_BrowseResult, referencesSize) - (offsetof(UA_BrowseResult, continuationPoint) + sizeof(UA_ByteString))) << 3) + (offsetof(UA_BrowseResult, references) - sizeof(UA_Int32) - offsetof(UA_BrowseResult, referencesSize)), .isArray = UA_TRUE }, }},

/* UA_BrowseRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 525}, .memSize = sizeof(UA_BrowseRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_BROWSEREQUEST, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_VIEWDESCRIPTION, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseRequest, view) - (offsetof(UA_BrowseRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseRequest, requestedMaxReferencesPerNode) - (offsetof(UA_BrowseRequest, view) + sizeof(UA_ViewDescription)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BROWSEDESCRIPTION, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_BrowseRequest, nodesToBrowseSize) - (offsetof(UA_BrowseRequest, requestedMaxReferencesPerNode) + sizeof(UA_UInt32))) << 3) + (offsetof(UA_BrowseRequest, nodesToBrowse) - sizeof(UA_Int32) - offsetof(UA_BrowseRequest, nodesToBrowseSize)), .isArray = UA_TRUE }, }},

/* UA_BrowseResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 528}, .memSize = sizeof(UA_BrowseResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_BROWSERESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BROWSERESULT, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_BrowseResponse, resultsSize) - (offsetof(UA_BrowseResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_BrowseResponse, results) - sizeof(UA_Int32) - offsetof(UA_BrowseResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_BrowseResponse, diagnosticInfosSize) - (offsetof(UA_BrowseResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_BrowseResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_BrowseResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_RelativePathElement */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 537}, .memSize = sizeof(UA_RelativePathElement), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_RELATIVEPATHELEMENT, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RelativePathElement, referenceTypeId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RelativePathElement, isInverse) - (offsetof(UA_RelativePathElement, referenceTypeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RelativePathElement, includeSubtypes) - (offsetof(UA_RelativePathElement, isInverse) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_QUALIFIEDNAME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RelativePathElement, targetName) - (offsetof(UA_RelativePathElement, includeSubtypes) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, }},

/* UA_RelativePath */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 540}, .memSize = sizeof(UA_RelativePath), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_RELATIVEPATH, .membersSize = 1,
	.members={
	{.memberTypeIndex = UA_TYPES_RELATIVEPATHELEMENT, .namespaceZero = UA_TRUE, .padding = (offsetof(UA_RelativePath, elements) - sizeof(UA_Int32) - offsetof(UA_RelativePath, elementsSize)), .isArray = UA_TRUE }, }},

/* UA_BrowsePath */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 543}, .memSize = sizeof(UA_BrowsePath), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_BROWSEPATH, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowsePath, startingNode) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_RELATIVEPATH, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowsePath, relativePath) - (offsetof(UA_BrowsePath, startingNode) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, }},

/* UA_BrowsePathTarget */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 546}, .memSize = sizeof(UA_BrowsePathTarget), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_BROWSEPATHTARGET, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowsePathTarget, targetId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowsePathTarget, remainingPathIndex) - (offsetof(UA_BrowsePathTarget, targetId) + sizeof(UA_ExpandedNodeId)), .isArray = UA_FALSE }, }},

/* UA_BrowsePathResult */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 549}, .memSize = sizeof(UA_BrowsePathResult), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_BROWSEPATHRESULT, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowsePathResult, statusCode) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BROWSEPATHTARGET, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_BrowsePathResult, targetsSize) - (offsetof(UA_BrowsePathResult, statusCode) + sizeof(UA_StatusCode))) << 3) + (offsetof(UA_BrowsePathResult, targets) - sizeof(UA_Int32) - offsetof(UA_BrowsePathResult, targetsSize)), .isArray = UA_TRUE }, }},

/* UA_TranslateBrowsePathsToNodeIdsRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 552}, .memSize = sizeof(UA_TranslateBrowsePathsToNodeIdsRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_TRANSLATEBROWSEPATHSTONODEIDSREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TranslateBrowsePathsToNodeIdsRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BROWSEPATH, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_TranslateBrowsePathsToNodeIdsRequest, browsePathsSize) - (offsetof(UA_TranslateBrowsePathsToNodeIdsRequest, requestHeader) + sizeof(UA_RequestHeader))) << 3) + (offsetof(UA_TranslateBrowsePathsToNodeIdsRequest, browsePaths) - sizeof(UA_Int32) - offsetof(UA_TranslateBrowsePathsToNodeIdsRequest, browsePathsSize)), .isArray = UA_TRUE }, }},

/* UA_TranslateBrowsePathsToNodeIdsResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 555}, .memSize = sizeof(UA_TranslateBrowsePathsToNodeIdsResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_TRANSLATEBROWSEPATHSTONODEIDSRESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TranslateBrowsePathsToNodeIdsResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BROWSEPATHRESULT, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_TranslateBrowsePathsToNodeIdsResponse, resultsSize) - (offsetof(UA_TranslateBrowsePathsToNodeIdsResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_TranslateBrowsePathsToNodeIdsResponse, results) - sizeof(UA_Int32) - offsetof(UA_TranslateBrowsePathsToNodeIdsResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_TranslateBrowsePathsToNodeIdsResponse, diagnosticInfosSize) - (offsetof(UA_TranslateBrowsePathsToNodeIdsResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_TranslateBrowsePathsToNodeIdsResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_TranslateBrowsePathsToNodeIdsResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_RegisterNodesRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 558}, .memSize = sizeof(UA_RegisterNodesRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_REGISTERNODESREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RegisterNodesRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_RegisterNodesRequest, nodesToRegisterSize) - (offsetof(UA_RegisterNodesRequest, requestHeader) + sizeof(UA_RequestHeader))) << 3) + (offsetof(UA_RegisterNodesRequest, nodesToRegister) - sizeof(UA_Int32) - offsetof(UA_RegisterNodesRequest, nodesToRegisterSize)), .isArray = UA_TRUE }, }},

/* UA_RegisterNodesResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 561}, .memSize = sizeof(UA_RegisterNodesResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_REGISTERNODESRESPONSE, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RegisterNodesResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_RegisterNodesResponse, registeredNodeIdsSize) - (offsetof(UA_RegisterNodesResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_RegisterNodesResponse, registeredNodeIds) - sizeof(UA_Int32) - offsetof(UA_RegisterNodesResponse, registeredNodeIdsSize)), .isArray = UA_TRUE }, }},

/* UA_UnregisterNodesRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 564}, .memSize = sizeof(UA_UnregisterNodesRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_UNREGISTERNODESREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_UnregisterNodesRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_UnregisterNodesRequest, nodesToUnregisterSize) - (offsetof(UA_UnregisterNodesRequest, requestHeader) + sizeof(UA_RequestHeader))) << 3) + (offsetof(UA_UnregisterNodesRequest, nodesToUnregister) - sizeof(UA_Int32) - offsetof(UA_UnregisterNodesRequest, nodesToUnregisterSize)), .isArray = UA_TRUE }, }},

/* UA_UnregisterNodesResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 567}, .memSize = sizeof(UA_UnregisterNodesResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_UNREGISTERNODESRESPONSE, .membersSize = 1,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_UnregisterNodesResponse, responseHeader) - 0, .isArray = UA_FALSE }, }},

/* UA_TimestampsToReturn */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 625}, .memSize = sizeof(UA_TimestampsToReturn), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_ReadValueId */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 626}, .memSize = sizeof(UA_ReadValueId), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_READVALUEID, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReadValueId, nodeId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReadValueId, attributeId) - (offsetof(UA_ReadValueId, nodeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReadValueId, indexRange) - (offsetof(UA_ReadValueId, attributeId) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_QUALIFIEDNAME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReadValueId, dataEncoding) - (offsetof(UA_ReadValueId, indexRange) + sizeof(UA_String)), .isArray = UA_FALSE }, }},

/* UA_ReadRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 629}, .memSize = sizeof(UA_ReadRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_READREQUEST, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReadRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DOUBLE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReadRequest, maxAge) - (offsetof(UA_ReadRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_TIMESTAMPSTORETURN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReadRequest, timestampsToReturn) - (offsetof(UA_ReadRequest, maxAge) + sizeof(UA_Double)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_READVALUEID, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ReadRequest, nodesToReadSize) - (offsetof(UA_ReadRequest, timestampsToReturn) + sizeof(UA_TimestampsToReturn))) << 3) + (offsetof(UA_ReadRequest, nodesToRead) - sizeof(UA_Int32) - offsetof(UA_ReadRequest, nodesToReadSize)), .isArray = UA_TRUE }, }},

/* UA_ReadResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 632}, .memSize = sizeof(UA_ReadResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_READRESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReadResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DATAVALUE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ReadResponse, resultsSize) - (offsetof(UA_ReadResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_ReadResponse, results) - sizeof(UA_Int32) - offsetof(UA_ReadResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ReadResponse, diagnosticInfosSize) - (offsetof(UA_ReadResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_ReadResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_ReadResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_WriteValue */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 668}, .memSize = sizeof(UA_WriteValue), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_WRITEVALUE, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_WriteValue, nodeId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_WriteValue, attributeId) - (offsetof(UA_WriteValue, nodeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_WriteValue, indexRange) - (offsetof(UA_WriteValue, attributeId) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DATAVALUE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_WriteValue, value) - (offsetof(UA_WriteValue, indexRange) + sizeof(UA_String)), .isArray = UA_FALSE }, }},

/* UA_WriteRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 671}, .memSize = sizeof(UA_WriteRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_WRITEREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_WriteRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_WRITEVALUE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_WriteRequest, nodesToWriteSize) - (offsetof(UA_WriteRequest, requestHeader) + sizeof(UA_RequestHeader))) << 3) + (offsetof(UA_WriteRequest, nodesToWrite) - sizeof(UA_Int32) - offsetof(UA_WriteRequest, nodesToWriteSize)), .isArray = UA_TRUE }, }},

/* UA_WriteResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 674}, .memSize = sizeof(UA_WriteResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_WRITERESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_WriteResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_WriteResponse, resultsSize) - (offsetof(UA_WriteResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_WriteResponse, results) - sizeof(UA_Int32) - offsetof(UA_WriteResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_WriteResponse, diagnosticInfosSize) - (offsetof(UA_WriteResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_WriteResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_WriteResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_MonitoringMode */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 716}, .memSize = sizeof(UA_MonitoringMode), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_MonitoringParameters */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 740}, .memSize = sizeof(UA_MonitoringParameters), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_MONITORINGPARAMETERS, .membersSize = 5,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoringParameters, clientHandle) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DOUBLE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoringParameters, samplingInterval) - (offsetof(UA_MonitoringParameters, clientHandle) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXTENSIONOBJECT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoringParameters, filter) - (offsetof(UA_MonitoringParameters, samplingInterval) + sizeof(UA_Double)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoringParameters, queueSize) - (offsetof(UA_MonitoringParameters, filter) + sizeof(UA_ExtensionObject)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoringParameters, discardOldest) - (offsetof(UA_MonitoringParameters, queueSize) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_MonitoredItemCreateRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 743}, .memSize = sizeof(UA_MonitoredItemCreateRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_MONITOREDITEMCREATEREQUEST, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_READVALUEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoredItemCreateRequest, itemToMonitor) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_MONITORINGMODE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoredItemCreateRequest, monitoringMode) - (offsetof(UA_MonitoredItemCreateRequest, itemToMonitor) + sizeof(UA_ReadValueId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_MONITORINGPARAMETERS, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoredItemCreateRequest, requestedParameters) - (offsetof(UA_MonitoredItemCreateRequest, monitoringMode) + sizeof(UA_MonitoringMode)), .isArray = UA_FALSE }, }},

/* UA_MonitoredItemCreateResult */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 746}, .memSize = sizeof(UA_MonitoredItemCreateResult), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_MONITOREDITEMCREATERESULT, .membersSize = 5,
	.members={
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoredItemCreateResult, statusCode) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoredItemCreateResult, monitoredItemId) - (offsetof(UA_MonitoredItemCreateResult, statusCode) + sizeof(UA_StatusCode)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DOUBLE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoredItemCreateResult, revisedSamplingInterval) - (offsetof(UA_MonitoredItemCreateResult, monitoredItemId) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoredItemCreateResult, revisedQueueSize) - (offsetof(UA_MonitoredItemCreateResult, revisedSamplingInterval) + sizeof(UA_Double)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXTENSIONOBJECT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoredItemCreateResult, filterResult) - (offsetof(UA_MonitoredItemCreateResult, revisedQueueSize) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_CreateMonitoredItemsRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 749}, .memSize = sizeof(UA_CreateMonitoredItemsRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CREATEMONITOREDITEMSREQUEST, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateMonitoredItemsRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateMonitoredItemsRequest, subscriptionId) - (offsetof(UA_CreateMonitoredItemsRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_TIMESTAMPSTORETURN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateMonitoredItemsRequest, timestampsToReturn) - (offsetof(UA_CreateMonitoredItemsRequest, subscriptionId) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_MONITOREDITEMCREATEREQUEST, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_CreateMonitoredItemsRequest, itemsToCreateSize) - (offsetof(UA_CreateMonitoredItemsRequest, timestampsToReturn) + sizeof(UA_TimestampsToReturn))) << 3) + (offsetof(UA_CreateMonitoredItemsRequest, itemsToCreate) - sizeof(UA_Int32) - offsetof(UA_CreateMonitoredItemsRequest, itemsToCreateSize)), .isArray = UA_TRUE }, }},

/* UA_CreateMonitoredItemsResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 752}, .memSize = sizeof(UA_CreateMonitoredItemsResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CREATEMONITOREDITEMSRESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateMonitoredItemsResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_MONITOREDITEMCREATERESULT, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_CreateMonitoredItemsResponse, resultsSize) - (offsetof(UA_CreateMonitoredItemsResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_CreateMonitoredItemsResponse, results) - sizeof(UA_Int32) - offsetof(UA_CreateMonitoredItemsResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_CreateMonitoredItemsResponse, diagnosticInfosSize) - (offsetof(UA_CreateMonitoredItemsResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_CreateMonitoredItemsResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_CreateMonitoredItemsResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_CreateSubscriptionRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 785}, .memSize = sizeof(UA_CreateSubscriptionRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CREATESUBSCRIPTIONREQUEST, .membersSize = 7,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DOUBLE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionRequest, requestedPublishingInterval) - (offsetof(UA_CreateSubscriptionRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionRequest, requestedLifetimeCount) - (offsetof(UA_CreateSubscriptionRequest, requestedPublishingInterval) + sizeof(UA_Double)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionRequest, requestedMaxKeepAliveCount) - (offsetof(UA_CreateSubscriptionRequest, requestedLifetimeCount) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionRequest, maxNotificationsPerPublish) - (offsetof(UA_CreateSubscriptionRequest, requestedMaxKeepAliveCount) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionRequest, publishingEnabled) - (offsetof(UA_CreateSubscriptionRequest, maxNotificationsPerPublish) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionRequest, priority) - (offsetof(UA_CreateSubscriptionRequest, publishingEnabled) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, }},

/* UA_CreateSubscriptionResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 788}, .memSize = sizeof(UA_CreateSubscriptionResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CREATESUBSCRIPTIONRESPONSE, .membersSize = 5,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionResponse, subscriptionId) - (offsetof(UA_CreateSubscriptionResponse, responseHeader) + sizeof(UA_ResponseHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DOUBLE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionResponse, revisedPublishingInterval) - (offsetof(UA_CreateSubscriptionResponse, subscriptionId) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionResponse, revisedLifetimeCount) - (offsetof(UA_CreateSubscriptionResponse, revisedPublishingInterval) + sizeof(UA_Double)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionResponse, revisedMaxKeepAliveCount) - (offsetof(UA_CreateSubscriptionResponse, revisedLifetimeCount) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_SetPublishingModeRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 797}, .memSize = sizeof(UA_SetPublishingModeRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_SETPUBLISHINGMODEREQUEST, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SetPublishingModeRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SetPublishingModeRequest, publishingEnabled) - (offsetof(UA_SetPublishingModeRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_SetPublishingModeRequest, subscriptionIdsSize) - (offsetof(UA_SetPublishingModeRequest, publishingEnabled) + sizeof(UA_Boolean))) << 3) + (offsetof(UA_SetPublishingModeRequest, subscriptionIds) - sizeof(UA_Int32) - offsetof(UA_SetPublishingModeRequest, subscriptionIdsSize)), .isArray = UA_TRUE }, }},

/* UA_SetPublishingModeResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 800}, .memSize = sizeof(UA_SetPublishingModeResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_SETPUBLISHINGMODERESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SetPublishingModeResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_SetPublishingModeResponse, resultsSize) - (offsetof(UA_SetPublishingModeResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_SetPublishingModeResponse, results) - sizeof(UA_Int32) - offsetof(UA_SetPublishingModeResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_SetPublishingModeResponse, diagnosticInfosSize) - (offsetof(UA_SetPublishingModeResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_SetPublishingModeResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_SetPublishingModeResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_NotificationMessage */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 803}, .memSize = sizeof(UA_NotificationMessage), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_NOTIFICATIONMESSAGE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_NotificationMessage, sequenceNumber) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DATETIME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_NotificationMessage, publishTime) - (offsetof(UA_NotificationMessage, sequenceNumber) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXTENSIONOBJECT, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_NotificationMessage, notificationDataSize) - (offsetof(UA_NotificationMessage, publishTime) + sizeof(UA_DateTime))) << 3) + (offsetof(UA_NotificationMessage, notificationData) - sizeof(UA_Int32) - offsetof(UA_NotificationMessage, notificationDataSize)), .isArray = UA_TRUE }, }},

/* UA_SubscriptionAcknowledgement */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 821}, .memSize = sizeof(UA_SubscriptionAcknowledgement), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = sizeof(UA_SubscriptionAcknowledgement) == 8, .typeIndex = UA_TYPES_SUBSCRIPTIONACKNOWLEDGEMENT, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SubscriptionAcknowledgement, subscriptionId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SubscriptionAcknowledgement, sequenceNumber) - (offsetof(UA_SubscriptionAcknowledgement, subscriptionId) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_PublishRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 824}, .memSize = sizeof(UA_PublishRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_PUBLISHREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_PublishRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_SUBSCRIPTIONACKNOWLEDGEMENT, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_PublishRequest, subscriptionAcknowledgementsSize) - (offsetof(UA_PublishRequest, requestHeader) + sizeof(UA_RequestHeader))) << 3) + (offsetof(UA_PublishRequest, subscriptionAcknowledgements) - sizeof(UA_Int32) - offsetof(UA_PublishRequest, subscriptionAcknowledgementsSize)), .isArray = UA_TRUE }, }},

/* UA_PublishResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 827}, .memSize = sizeof(UA_PublishResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_PUBLISHRESPONSE, .membersSize = 7,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_PublishResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_PublishResponse, subscriptionId) - (offsetof(UA_PublishResponse, responseHeader) + sizeof(UA_ResponseHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_PublishResponse, availableSequenceNumbersSize) - (offsetof(UA_PublishResponse, subscriptionId) + sizeof(UA_UInt32))) << 3) + (offsetof(UA_PublishResponse, availableSequenceNumbers) - sizeof(UA_Int32) - offsetof(UA_PublishResponse, availableSequenceNumbersSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_PublishResponse, moreNotifications) - (offsetof(UA_PublishResponse, availableSequenceNumbers) + sizeof(void*)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NOTIFICATIONMESSAGE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_PublishResponse, notificationMessage) - (offsetof(UA_PublishResponse, moreNotifications) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_PublishResponse, resultsSize) - (offsetof(UA_PublishResponse, notificationMessage) + sizeof(UA_NotificationMessage))) << 3) + (offsetof(UA_PublishResponse, results) - sizeof(UA_Int32) - offsetof(UA_PublishResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_PublishResponse, diagnosticInfosSize) - (offsetof(UA_PublishResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_PublishResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_PublishResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_BuildInfo */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 338}, .memSize = sizeof(UA_BuildInfo), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_BUILDINFO, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BuildInfo, productUri) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BuildInfo, manufacturerName) - (offsetof(UA_BuildInfo, productUri) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BuildInfo, productName) - (offsetof(UA_BuildInfo, manufacturerName) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BuildInfo, softwareVersion) - (offsetof(UA_BuildInfo, productName) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BuildInfo, buildNumber) - (offsetof(UA_BuildInfo, softwareVersion) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DATETIME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BuildInfo, buildDate) - (offsetof(UA_BuildInfo, buildNumber) + sizeof(UA_String)), .isArray = UA_FALSE }, }},

/* UA_ServerState */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 852}, .memSize = sizeof(UA_ServerState), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_ServerStatusDataType */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 862}, .memSize = sizeof(UA_ServerStatusDataType), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_SERVERSTATUSDATATYPE, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_DATETIME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ServerStatusDataType, startTime) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DATETIME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ServerStatusDataType, currentTime) - (offsetof(UA_ServerStatusDataType, startTime) + sizeof(UA_DateTime)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_SERVERSTATE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ServerStatusDataType, state) - (offsetof(UA_ServerStatusDataType, currentTime) + sizeof(UA_DateTime)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BUILDINFO, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ServerStatusDataType, buildInfo) - (offsetof(UA_ServerStatusDataType, state) + sizeof(UA_ServerState)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ServerStatusDataType, secondsTillShutdown) - (offsetof(UA_ServerStatusDataType, buildInfo) + sizeof(UA_BuildInfo)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ServerStatusDataType, shutdownReason) - (offsetof(UA_ServerStatusDataType, secondsTillShutdown) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},
};

/**
* @file ua_transport_generated.c
*
* @brief Autogenerated data types
*
* Generated from Custom.Opc.Ua.Transport.bsd with script C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/tools/generate_datatypes.py
* on host SPECTRE by user lars at 2015-04-17 10:13:36
*/


const UA_DataType *UA_TRANSPORT = (UA_DataType[]){

/* UA_MessageTypeAndFinal */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0}, .memSize = sizeof(UA_MessageTypeAndFinal), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_TcpMessageHeader */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0}, .memSize = sizeof(UA_TcpMessageHeader), .namespaceZero = UA_FALSE, .fixedSize = UA_TRUE, .zeroCopyable = sizeof(UA_TcpMessageHeader) == 8, .typeIndex = UA_TRANSPORT_TCPMESSAGEHEADER, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpMessageHeader, messageTypeAndFinal) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpMessageHeader, messageSize) - (offsetof(UA_TcpMessageHeader, messageTypeAndFinal) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_TcpHelloMessage */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0}, .memSize = sizeof(UA_TcpHelloMessage), .namespaceZero = UA_FALSE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TRANSPORT_TCPHELLOMESSAGE, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpHelloMessage, protocolVersion) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpHelloMessage, receiveBufferSize) - (offsetof(UA_TcpHelloMessage, protocolVersion) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpHelloMessage, sendBufferSize) - (offsetof(UA_TcpHelloMessage, receiveBufferSize) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpHelloMessage, maxMessageSize) - (offsetof(UA_TcpHelloMessage, sendBufferSize) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpHelloMessage, maxChunkCount) - (offsetof(UA_TcpHelloMessage, maxMessageSize) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpHelloMessage, endpointUrl) - (offsetof(UA_TcpHelloMessage, maxChunkCount) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_TcpAcknowledgeMessage */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0}, .memSize = sizeof(UA_TcpAcknowledgeMessage), .namespaceZero = UA_FALSE, .fixedSize = UA_TRUE, .zeroCopyable = sizeof(UA_TcpAcknowledgeMessage) == 20, .typeIndex = UA_TRANSPORT_TCPACKNOWLEDGEMESSAGE, .membersSize = 5,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpAcknowledgeMessage, protocolVersion) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpAcknowledgeMessage, receiveBufferSize) - (offsetof(UA_TcpAcknowledgeMessage, protocolVersion) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpAcknowledgeMessage, sendBufferSize) - (offsetof(UA_TcpAcknowledgeMessage, receiveBufferSize) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpAcknowledgeMessage, maxMessageSize) - (offsetof(UA_TcpAcknowledgeMessage, sendBufferSize) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpAcknowledgeMessage, maxChunkCount) - (offsetof(UA_TcpAcknowledgeMessage, maxMessageSize) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_SecureConversationMessageHeader */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0}, .memSize = sizeof(UA_SecureConversationMessageHeader), .namespaceZero = UA_FALSE, .fixedSize = UA_TRUE, .zeroCopyable = sizeof(UA_SecureConversationMessageHeader) == 12, .typeIndex = UA_TRANSPORT_SECURECONVERSATIONMESSAGEHEADER, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TRANSPORT_TCPMESSAGEHEADER, .namespaceZero = UA_FALSE, .padding = offsetof(UA_SecureConversationMessageHeader, messageHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SecureConversationMessageHeader, secureChannelId) - (offsetof(UA_SecureConversationMessageHeader, messageHeader) + sizeof(UA_TcpMessageHeader)), .isArray = UA_FALSE }, }},

/* UA_AsymmetricAlgorithmSecurityHeader */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0}, .memSize = sizeof(UA_AsymmetricAlgorithmSecurityHeader), .namespaceZero = UA_FALSE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TRANSPORT_ASYMMETRICALGORITHMSECURITYHEADER, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AsymmetricAlgorithmSecurityHeader, securityPolicyUri) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AsymmetricAlgorithmSecurityHeader, senderCertificate) - (offsetof(UA_AsymmetricAlgorithmSecurityHeader, securityPolicyUri) + sizeof(UA_ByteString)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AsymmetricAlgorithmSecurityHeader, receiverCertificateThumbprint) - (offsetof(UA_AsymmetricAlgorithmSecurityHeader, senderCertificate) + sizeof(UA_ByteString)), .isArray = UA_FALSE }, }},

/* UA_SymmetricAlgorithmSecurityHeader */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0}, .memSize = sizeof(UA_SymmetricAlgorithmSecurityHeader), .namespaceZero = UA_FALSE, .fixedSize = UA_TRUE, .zeroCopyable = sizeof(UA_SymmetricAlgorithmSecurityHeader) == 4, .typeIndex = UA_TRANSPORT_SYMMETRICALGORITHMSECURITYHEADER, .membersSize = 1,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SymmetricAlgorithmSecurityHeader, tokenId) - 0, .isArray = UA_FALSE }, }},

/* UA_SequenceHeader */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0}, .memSize = sizeof(UA_SequenceHeader), .namespaceZero = UA_FALSE, .fixedSize = UA_TRUE, .zeroCopyable = sizeof(UA_SequenceHeader) == 8, .typeIndex = UA_TRANSPORT_SEQUENCEHEADER, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SequenceHeader, sequenceNumber) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SequenceHeader, requestId) - (offsetof(UA_SequenceHeader, sequenceNumber) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_SecureConversationMessageFooter */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0}, .memSize = sizeof(UA_SecureConversationMessageFooter), .namespaceZero = UA_FALSE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TRANSPORT_SECURECONVERSATIONMESSAGEFOOTER, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = (offsetof(UA_SecureConversationMessageFooter, padding) - sizeof(UA_Int32) - offsetof(UA_SecureConversationMessageFooter, paddingSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SecureConversationMessageFooter, signature) - (offsetof(UA_SecureConversationMessageFooter, padding) + sizeof(void*)), .isArray = UA_FALSE }, }},

/* UA_SecureConversationMessageAbortBody */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0}, .memSize = sizeof(UA_SecureConversationMessageAbortBody), .namespaceZero = UA_FALSE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TRANSPORT_SECURECONVERSATIONMESSAGEABORTBODY, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SecureConversationMessageAbortBody, error) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SecureConversationMessageAbortBody, reason) - (offsetof(UA_SecureConversationMessageAbortBody, error) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},
};


// max message size is 64k
const UA_ConnectionConfig UA_ConnectionConfig_standard =
    {.protocolVersion = 0, .sendBufferSize = 65536, .recvBufferSize  = 65536,
     .maxMessageSize = 65536, .maxChunkCount   = 1};

UA_ByteString UA_Connection_completeMessages(UA_Connection *connection, UA_ByteString received)
{
    if(received.length == -1)
        return received;

    /* concat the existing incomplete message with the new message */
    UA_ByteString current;
    if(connection->incompleteMessage.length < 0)
        current = received;
    else {
        current.data = UA_realloc(connection->incompleteMessage.data,
                                  connection->incompleteMessage.length + received.length);
        if(!current.data) {
            /* not enough memory */
            UA_ByteString_deleteMembers(&connection->incompleteMessage);
            connection->incompleteMessage.length = -1;
            UA_ByteString_deleteMembers(&received);
            received.length = -1;
            return received;
        }
        UA_memcpy(current.data + connection->incompleteMessage.length, received.data, received.length);
        current.length = connection->incompleteMessage.length + received.length;
        UA_ByteString_deleteMembers(&received);
        UA_ByteString_init(&connection->incompleteMessage);
    }

    /* find the first non-complete message */
    size_t end_pos = 0; // the end of the last complete message
    while(current.length - end_pos >= 32) {
        if(!(current.data[0] == 'M' && current.data[1] == 'S' && current.data[2] == 'G') &&
           !(current.data[0] == 'O' && current.data[1] == 'P' && current.data[2] == 'N') &&
           !(current.data[0] == 'H' && current.data[1] == 'E' && current.data[2] == 'L') &&
           !(current.data[0] == 'A' && current.data[1] == 'C' && current.data[2] == 'K') &&
           !(current.data[0] == 'C' && current.data[1] == 'L' && current.data[2] == 'O')) {
            current.length = end_pos; // throw the remaining bytestring away
            break;
        }
        UA_Int32 length = 0;
        size_t pos = end_pos + 4;
        UA_StatusCode retval = UA_Int32_decodeBinary(&current, &pos, &length);
        if(retval != UA_STATUSCODE_GOOD || length < 32 ||
           length > (UA_Int32)connection->localConf.maxMessageSize) {
            current.length = end_pos; // throw the remaining bytestring away
            break;
        }
        if(length + (UA_Int32)end_pos > current.length)
            break; // the message is incomplete
        end_pos += length;
    }

    if(current.length == 0) {
        /* throw everything away */
        UA_ByteString_deleteMembers(&current);
        current.length = -1;
        return current;
    }

    if(end_pos == 0) {
        /* no complete message in current */
        connection->incompleteMessage = current;
        UA_ByteString_init(&current);
    } else if(current.length != (UA_Int32)end_pos) {
        /* there is an incomplete message at the end of current */
        connection->incompleteMessage.data = UA_malloc(current.length - end_pos);
        if(connection->incompleteMessage.data) {
            UA_memcpy(connection->incompleteMessage.data, &current.data[end_pos], current.length - end_pos);
            connection->incompleteMessage.length = current.length - end_pos;
        }
        current.length = end_pos;
    }
    return current;
}

void UA_SecureChannel_init(UA_SecureChannel *channel) {
    UA_MessageSecurityMode_init(&channel->securityMode);
    UA_ChannelSecurityToken_init(&channel->securityToken);
    UA_AsymmetricAlgorithmSecurityHeader_init(&channel->clientAsymAlgSettings);
    UA_AsymmetricAlgorithmSecurityHeader_init(&channel->serverAsymAlgSettings);
    UA_ByteString_init(&channel->clientNonce);
    UA_ByteString_init(&channel->serverNonce);
    channel->requestId = 0;
    channel->sequenceNumber = 0;
    channel->connection = UA_NULL;
    channel->session    = UA_NULL;
}

void UA_SecureChannel_deleteMembers(UA_SecureChannel *channel) {
    UA_AsymmetricAlgorithmSecurityHeader_deleteMembers(&channel->serverAsymAlgSettings);
    UA_ByteString_deleteMembers(&channel->serverNonce);
    UA_AsymmetricAlgorithmSecurityHeader_deleteMembers(&channel->clientAsymAlgSettings);
    UA_ByteString_deleteMembers(&channel->clientNonce);
    UA_ChannelSecurityToken_deleteMembers(&channel->securityToken);
}

void UA_SecureChannel_delete(UA_SecureChannel *channel) {
    UA_SecureChannel_deleteMembers(channel);
    UA_free(channel);
}

UA_Boolean UA_SecureChannel_compare(UA_SecureChannel *sc1, UA_SecureChannel *sc2) {
    return (sc1->securityToken.channelId == sc2->securityToken.channelId);
}

//TODO implement real nonce generator - DUMMY function
UA_StatusCode UA_SecureChannel_generateNonce(UA_ByteString *nonce) {
    if(!(nonce->data = UA_malloc(1)))
        return UA_STATUSCODE_BADOUTOFMEMORY;
    nonce->length  = 1;
    nonce->data[0] = 'a';
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_SecureChannel_updateRequestId(UA_SecureChannel *channel, UA_UInt32 requestId) {
    //TODO review checking of request id
    if(channel->requestId+1 != requestId)
        return UA_STATUSCODE_BADINTERNALERROR;
    channel->requestId++;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_SecureChannel_updateSequenceNumber(UA_SecureChannel *channel, UA_UInt32 sequenceNumber) {
    //TODO review checking of sequence
    if(channel->sequenceNumber+1  != sequenceNumber)
        return UA_STATUSCODE_BADINTERNALERROR;
    channel->sequenceNumber++;
    return UA_STATUSCODE_GOOD;
}

void UA_Connection_detachSecureChannel(UA_Connection *connection) {
#ifdef UA_MULTITHREADING
    UA_SecureChannel *channel = connection->channel;
    if(channel)
        uatomic_cmpxchg(&channel->connection, connection, UA_NULL);
    uatomic_set(&connection->channel, UA_NULL);
#else
    if(connection->channel)
        connection->channel->connection = UA_NULL;
    connection->channel = UA_NULL;
#endif
}

void UA_Connection_attachSecureChannel(UA_Connection *connection, UA_SecureChannel *channel) {
#ifdef UA_MULTITHREADING
    if(uatomic_cmpxchg(&channel->connection, UA_NULL, connection) == UA_NULL)
        uatomic_set(&connection->channel, channel);
#else
    if(channel->connection != UA_NULL)
        return;
    channel->connection = connection;
    connection->channel = channel;
#endif
}

UA_Session anonymousSession = {
    .clientDescription =  {.applicationUri = {-1, UA_NULL},
                           .productUri = {-1, UA_NULL},
                           .applicationName = {.locale = {-1, UA_NULL}, .text = {-1, UA_NULL}},
                           .applicationType = UA_APPLICATIONTYPE_CLIENT,
                           .gatewayServerUri = {-1, UA_NULL},
                           .discoveryProfileUri = {-1, UA_NULL},
                           .discoveryUrlsSize = -1,
                           .discoveryUrls = UA_NULL},
    .sessionName = {sizeof("Anonymous Session")-1, (UA_Byte*)"Anonymous Session"},
    .authenticationToken = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0}, // is never used, as this session is not stored in the sessionmanager
    .sessionId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0},
    .maxRequestMessageSize = UA_UINT32_MAX,
    .maxResponseMessageSize = UA_UINT32_MAX,
    .timeout = UA_INT64_MAX,
    .validTill = UA_INT64_MAX,
    .channel = UA_NULL};

UA_Session adminSession = {
    .clientDescription =  {.applicationUri = {-1, UA_NULL},
                           .productUri = {-1, UA_NULL},
                           .applicationName = {.locale = {-1, UA_NULL}, .text = {-1, UA_NULL}},
                           .applicationType = UA_APPLICATIONTYPE_CLIENT,
                           .gatewayServerUri = {-1, UA_NULL},
                           .discoveryProfileUri = {-1, UA_NULL},
                           .discoveryUrlsSize = -1,
                           .discoveryUrls = UA_NULL},
    .sessionName = {sizeof("Administrator Session")-1, (UA_Byte*)"Administrator Session"},
    .authenticationToken = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 1}, // is never used, as this session is not stored in the sessionmanager
    .sessionId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 1},
    .maxRequestMessageSize = UA_UINT32_MAX,
    .maxResponseMessageSize = UA_UINT32_MAX,
    .timeout = UA_INT64_MAX,
    .validTill = UA_INT64_MAX,
    .channel = UA_NULL};

UA_Session * UA_Session_new(void) {
    UA_Session *s = UA_malloc(sizeof(UA_Session));
    if(s) UA_Session_init(s);
    return s;
}

/* TODO: Nobody seems to call this function right now */
static UA_StatusCode UA_Session_generateToken(UA_NodeId *newToken, UA_UInt32 *seed) {
    newToken->namespaceIndex = 0; // where else?
    newToken->identifierType = UA_NODEIDTYPE_GUID;
    newToken->identifier.guid = UA_Guid_random(seed);
    return UA_STATUSCODE_GOOD;
}

void UA_Session_init(UA_Session *session) {
    if(!session) return;
    UA_ApplicationDescription_init(&session->clientDescription);
    UA_NodeId_init(&session->authenticationToken);
    UA_NodeId_init(&session->sessionId);
    UA_String_init(&session->sessionName);
    session->maxRequestMessageSize  = 0;
    session->maxResponseMessageSize = 0;
    session->timeout = 0;
    UA_DateTime_init(&session->validTill);
    session->channel = UA_NULL;
}

void UA_Session_deleteMembers(UA_Session *session) {
    UA_ApplicationDescription_deleteMembers(&session->clientDescription);
    UA_NodeId_deleteMembers(&session->authenticationToken);
    UA_NodeId_deleteMembers(&session->sessionId);
    UA_String_deleteMembers(&session->sessionName);
    session->channel = UA_NULL;
}

void UA_Session_delete(UA_Session *session) {
    UA_Session_deleteMembers(session);
    UA_free(session);
}

UA_Boolean UA_Session_compare(UA_Session *session1, UA_Session *session2) {
    if(session1 && session2 && UA_NodeId_equal(&session1->sessionId, &session2->sessionId))
        return UA_TRUE;
    return UA_FALSE;
}

UA_StatusCode UA_Session_setExpirationDate(UA_Session *session) {
    if(!session)
        return UA_STATUSCODE_BADINTERNALERROR;

    session->validTill = UA_DateTime_now() + session->timeout * 100000; //timeout in ms
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_Session_getPendingLifetime(UA_Session *session, UA_Double *pendingLifetime_ms) {
    if(!session)
        return UA_STATUSCODE_BADINTERNALERROR;

    *pendingLifetime_ms = (session->validTill - UA_DateTime_now())/10000000; //difference in ms
    return UA_STATUSCODE_GOOD;
}

void UA_SecureChannel_detachSession(UA_SecureChannel *channel) {
#ifdef UA_MULTITHREADING
    UA_Session *session = channel->session;
    if(session)
        uatomic_cmpxchg(&session->channel, channel, UA_NULL);
    uatomic_set(&channel->session, UA_NULL);
#else
    if(channel->session)
        channel->session->channel = UA_NULL;
    channel->session = UA_NULL;
#endif
}

void UA_SecureChannel_attachSession(UA_SecureChannel *channel, UA_Session *session) {
#ifdef UA_MULTITHREADING
    if(uatomic_cmpxchg(&session->channel, UA_NULL, channel) == UA_NULL)
        uatomic_set(&channel->session, session);
#else
    if(session->channel != UA_NULL)
        return;
    session->channel = channel;
    channel->session = session;
#endif
}

void UA_Session_detachSecureChannel(UA_Session *session) {
#ifdef UA_MULTITHREADING
    UA_SecureChannel *channel = session->channel;
    if(channel)
        uatomic_cmpxchg(&channel->session, session, UA_NULL);
    uatomic_set(&session->channel, UA_NULL);
#else
    if(session->channel)
        session->channel->session = UA_NULL;
    session->channel = UA_NULL;
#endif
}

const char *UA_LoggerCategoryNames[3] = {"communication", "server", "userland"};

/**********************/
/* Namespace Handling */
/**********************/

static void UA_ExternalNamespace_init(UA_ExternalNamespace *ens) {
	ens->index = 0;
	UA_String_init(&ens->url);
}

static void UA_ExternalNamespace_deleteMembers(UA_ExternalNamespace *ens) {
	UA_String_deleteMembers(&ens->url);
    ens->externalNodeStore.destroy(ens->externalNodeStore.ensHandle);
}

/*****************/
/* Configuration */
/*****************/

void UA_Server_addNetworkLayer(UA_Server *server, UA_ServerNetworkLayer networkLayer) {
    UA_ServerNetworkLayer *newlayers =
        UA_realloc(server->nls, sizeof(UA_ServerNetworkLayer)*(server->nlsSize+1));
    if(!newlayers) {
        UA_LOG_ERROR(server->logger, UA_LOGGERCATEGORY_SERVER, "Networklayer added");
        return;
    }
    server->nls = newlayers;
    server->nls[server->nlsSize] = networkLayer;
    server->nlsSize++;

    if(networkLayer.discoveryUrl){
        if(server->description.discoveryUrlsSize < 0)
            server->description.discoveryUrlsSize = 0;
		UA_String* newUrls = UA_realloc(server->description.discoveryUrls,
                                        sizeof(UA_String)*(server->description.discoveryUrlsSize+1));
		if(!newUrls) {
			UA_LOG_ERROR(server->logger, UA_LOGGERCATEGORY_SERVER, "Adding discoveryUrl");
			return;
		}
		server->description.discoveryUrls = newUrls;
		UA_String_copy(networkLayer.discoveryUrl,
                       &server->description.discoveryUrls[server->description.discoveryUrlsSize]);
        server->description.discoveryUrlsSize++;
    }
}

void UA_Server_setServerCertificate(UA_Server *server, UA_ByteString certificate) {
    for(UA_Int32 i = 0; i < server->endpointDescriptionsSize; i++)
        UA_ByteString_copy(&certificate, &server->endpointDescriptions[i].serverCertificate);
}

void UA_Server_setLogger(UA_Server *server, UA_Logger logger) {
    server->logger = logger;
}

UA_UInt16 UA_Server_addNamespace(UA_Server *server, const char* name) {
    server->namespaces = UA_realloc(server->namespaces, sizeof(UA_String) * (server->namespacesSize+1));
    server->namespaces[server->namespacesSize] = UA_STRING_ALLOC(name);
    server->namespacesSize++;
    return server->namespacesSize-1;
}

/**********/
/* Server */
/**********/

/* The server needs to be stopped before it can be deleted */
void UA_Server_delete(UA_Server *server) {
	// Delete the timed work
	UA_Server_deleteTimedWork(server);

	// Delete all internal data
	UA_ApplicationDescription_deleteMembers(&server->description);
	UA_SecureChannelManager_deleteMembers(&server->secureChannelManager);
	UA_SessionManager_deleteMembers(&server->sessionManager);
	UA_NodeStore_delete(server->nodestore);
	UA_ByteString_deleteMembers(&server->serverCertificate);
    UA_Array_delete(server->namespaces, &UA_TYPES[UA_TYPES_STRING], server->namespacesSize);
	UA_Array_delete(server->endpointDescriptions, &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION],
                    server->endpointDescriptionsSize);

	// Delete the network layers
	for(UA_Int32 i = 0; i < server->nlsSize; i++) {
		server->nls[i].free(server->nls[i].nlHandle);
	}
	UA_free(server->nls);

#ifdef UA_MULTITHREADING
	pthread_cond_destroy(&server->dispatchQueue_condition); // so the workers don't spin if the queue is empty
	rcu_barrier(); // wait for all scheduled call_rcu work to complete
#endif
	UA_free(server);
}

static UA_StatusCode readStatus(void *handle, UA_Boolean sourceTimeStamp, UA_DataValue *value) {
    UA_ServerStatusDataType *status = UA_ServerStatusDataType_new();
    status->startTime   = ((const UA_Server*)handle)->startTime;
    status->currentTime = UA_DateTime_now();
    status->state       = UA_SERVERSTATE_RUNNING;
    status->buildInfo.productUri = UA_STRING_ALLOC("http://www.open62541.org");
    status->buildInfo.manufacturerName = UA_STRING_ALLOC("open62541");
    status->buildInfo.productName = UA_STRING_ALLOC("open62541 OPC UA Server");
#define STRINGIFY(x) #x //some magic
#define TOSTRING(x) STRINGIFY(x) //some magic
    status->buildInfo.softwareVersion = UA_STRING_ALLOC(TOSTRING(OPEN62541_VERSION_MAJOR) "." TOSTRING(OPEN62541_VERSION_MINOR) "." TOSTRING(OPEN62541_VERSION_PATCH));
    status->buildInfo.buildNumber = UA_STRING_ALLOC("0");
    status->buildInfo.buildDate = ((const UA_Server*)handle)->buildDate;
    status->secondsTillShutdown = 0;
    value->value.type = &UA_TYPES[UA_TYPES_SERVERSTATUSDATATYPE];
	value->value.arrayLength = -1;
    value->value.data = status;
    value->value.arrayDimensionsSize = -1;
    value->value.arrayDimensions = UA_NULL;
    value->hasValue = UA_TRUE;
    if(sourceTimeStamp) {
        value->hasSourceTimestamp = UA_TRUE;
        value->sourceTimestamp = UA_DateTime_now();
    }
    return UA_STATUSCODE_GOOD;
}

static void releaseStatus(void *handle, UA_DataValue *value) {
    UA_DataValue_deleteMembers(value);
}

static UA_StatusCode readNamespaces(void *handle, UA_Boolean sourceTimestamp, UA_DataValue *value) {
    UA_Server *server = (UA_Server*)handle;
    value->hasValue = UA_TRUE;
    value->value.storageType = UA_VARIANT_DATA_NODELETE;
    value->value.type = &UA_TYPES[UA_TYPES_STRING];
    value->value.arrayLength = server->namespacesSize;
    value->value.data = server->namespaces;
    if(sourceTimestamp) {
        value->hasSourceTimestamp = UA_TRUE;
        value->sourceTimestamp = UA_DateTime_now();
    }
    return UA_STATUSCODE_GOOD;
}

static void releaseNamespaces(void *handle, UA_DataValue *value) {
}

static UA_StatusCode readCurrentTime(void *handle, UA_Boolean sourceTimeStamp, UA_DataValue *value) {
	UA_DateTime *currentTime = UA_DateTime_new();
	if(!currentTime)
		return UA_STATUSCODE_BADOUTOFMEMORY;
	*currentTime = UA_DateTime_now();
	value->value.type = &UA_TYPES[UA_TYPES_DATETIME];
	value->value.arrayLength = -1;
	value->value.data = currentTime;
	value->value.arrayDimensionsSize = -1;
	value->value.arrayDimensions = NULL;
	value->hasValue = UA_TRUE;
	if(sourceTimeStamp) {
		value->hasSourceTimestamp = UA_TRUE;
		value->sourceTimestamp = *currentTime;
	}
	return UA_STATUSCODE_GOOD;
}

static void releaseCurrentTime(void *handle, UA_DataValue *value) {
	UA_DateTime_delete((UA_DateTime*)value->value.data);
}

static void copyNames(UA_Node *node, char *name) {
    node->browseName = UA_QUALIFIEDNAME_ALLOC(0, name);
    node->displayName = UA_LOCALIZEDTEXT_ALLOC("", name);
    node->description = UA_LOCALIZEDTEXT_ALLOC("", name);
}

static void addDataTypeNode(UA_Server *server, char* name, UA_UInt32 datatypeid, UA_Int32 parent) {
    UA_DataTypeNode *datatype = UA_DataTypeNode_new();
    copyNames((UA_Node*)datatype, name);
    datatype->nodeId.identifier.numeric = datatypeid;
    UA_Server_addNode(server, (UA_Node*)datatype,
                      &UA_EXPANDEDNODEID_NUMERIC(0, parent),
                      &UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES));
}

static UA_VariableTypeNode*
createVariableTypeNode(UA_Server *server, char* name, UA_UInt32 variabletypeid,
                       UA_Int32 parent, UA_Boolean abstract)
{
    UA_VariableTypeNode *variabletype = UA_VariableTypeNode_new();
    copyNames((UA_Node*)variabletype, name);
    variabletype->nodeId.identifier.numeric = variabletypeid;
    variabletype->isAbstract = abstract;
    variabletype->value.variant.type = &UA_TYPES[UA_TYPES_VARIANT];
    return variabletype;
}

static void addVariableTypeNode_organized(UA_Server *server, char* name, UA_UInt32 variabletypeid,
                                          UA_Int32 parent, UA_Boolean abstract) {
	UA_VariableTypeNode *variabletype = createVariableTypeNode(server, name, variabletypeid, parent, abstract);

    UA_Server_addNode(server, (UA_Node*)variabletype,
                      &UA_EXPANDEDNODEID_NUMERIC(0, parent),
                      &UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES));
}

static void addVariableTypeNode_subtype(UA_Server *server, char* name, UA_UInt32 variabletypeid,
                                        UA_Int32 parent, UA_Boolean abstract) {
	UA_VariableTypeNode *variabletype = createVariableTypeNode(server, name, variabletypeid, parent, abstract);

    UA_Server_addNode(server, (UA_Node*)variabletype,
                      &UA_EXPANDEDNODEID_NUMERIC(0, parent),
                      &UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));
}

UA_Server * UA_Server_new(void) {
    UA_Server *server = UA_malloc(sizeof(UA_Server));
    if(!server)
        return UA_NULL;

    LIST_INIT(&server->timedWork);
#ifdef UA_MULTITHREADING
    rcu_init();
    cds_wfcq_init(&server->dispatchQueue_head, &server->dispatchQueue_tail);
    server->delayedWork = UA_NULL;
#endif

    // logger
    server->logger = (UA_Logger){ UA_NULL, UA_NULL, UA_NULL, UA_NULL, UA_NULL, UA_NULL };

    // random seed
    server->random_seed = (UA_UInt32)UA_DateTime_now();

    // networklayers
    server->nls = UA_NULL;
    server->nlsSize = 0;

    UA_ByteString_init(&server->serverCertificate);

#define PRODUCT_URI "http://open62541.org"
#define APPLICATION_URI "urn:unconfigured:open62541:open62541Server"
    // mockup application description
    UA_ApplicationDescription_init(&server->description);
    server->description.productUri = UA_STRING_ALLOC(PRODUCT_URI);
    server->description.applicationUri = UA_STRING_ALLOC(APPLICATION_URI);
    server->description.discoveryUrlsSize = 0;

    server->description.applicationName = UA_LOCALIZEDTEXT_ALLOC("", "Unconfigured open62541 application");
    server->description.applicationType = UA_APPLICATIONTYPE_SERVER;
    server->externalNamespacesSize = 0;
    server->externalNamespaces = UA_NULL;

    server->namespaces = UA_String_new();
    *server->namespaces = UA_STRING_ALLOC("http://opcfoundation.org/UA/");
    server->namespacesSize = 1;

    server->endpointDescriptions = UA_NULL;
    server->endpointDescriptionsSize = 0;

    UA_EndpointDescription *endpoint = UA_EndpointDescription_new(); // todo: check return code
    if(endpoint) {
        endpoint->securityMode = UA_MESSAGESECURITYMODE_NONE;
        endpoint->securityPolicyUri = UA_STRING_ALLOC("http://opcfoundation.org/UA/SecurityPolicy#None");
        endpoint->transportProfileUri = UA_STRING_ALLOC("http://opcfoundation.org/UA-Profile/Transport/uatcp-uasc-uabinary");

        endpoint->userIdentityTokensSize = 2;
        endpoint->userIdentityTokens = UA_Array_new(&UA_TYPES[UA_TYPES_USERTOKENPOLICY], 2);

        UA_UserTokenPolicy_init(&endpoint->userIdentityTokens[0]);
        endpoint->userIdentityTokens[0].tokenType = UA_USERTOKENTYPE_ANONYMOUS;
        endpoint->userIdentityTokens[0].policyId = UA_STRING_ALLOC("my-anonymous-policy"); // defined per server

        UA_UserTokenPolicy_init(&endpoint->userIdentityTokens[1]);
        endpoint->userIdentityTokens[1].tokenType = UA_USERTOKENTYPE_USERNAME;
        endpoint->userIdentityTokens[1].policyId = UA_STRING_ALLOC("my-username-policy"); // defined per server

        /* UA_String_copy(endpointUrl, &endpoint->endpointUrl); */
        /* /\* The standard says "the HostName specified in the Server Certificate is the */
        /*    same as the HostName contained in the endpointUrl provided in the */
        /*    EndpointDescription *\/ */
        /* UA_String_copy(&server->serverCertificate, &endpoint->serverCertificate); */
        UA_ApplicationDescription_copy(&server->description, &endpoint->server);

        server->endpointDescriptions = endpoint;
        server->endpointDescriptionsSize = 1;
    }

#define MAXCHANNELCOUNT 100
#define STARTCHANNELID 1
#define TOKENLIFETIME 600000
#define STARTTOKENID 1
    UA_SecureChannelManager_init(&server->secureChannelManager, MAXCHANNELCOUNT,
                                 TOKENLIFETIME, STARTCHANNELID, STARTTOKENID);

#define MAXSESSIONCOUNT 1000
#define MAXSESSIONLIFETIME 10000
#define STARTSESSIONID 1
    UA_SessionManager_init(&server->sessionManager, MAXSESSIONCOUNT, MAXSESSIONLIFETIME, STARTSESSIONID);

    server->nodestore = UA_NodeStore_new();

    /**************/
    /* References */
    /**************/
    
    /* bootstrap by manually inserting "references" and "hassubtype" */
    UA_ReferenceTypeNode *references = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)references, "References");
    references->nodeId.identifier.numeric = UA_NS0ID_REFERENCES;
    references->isAbstract = UA_TRUE;
    references->symmetric  = UA_TRUE;
    // this node has no parent??
    UA_NodeStore_insert(server->nodestore, (UA_Node*)references, UA_NULL);

    UA_ReferenceTypeNode *hassubtype = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hassubtype, "HasSubtype");
    hassubtype->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "HasSupertype");
    hassubtype->nodeId.identifier.numeric = UA_NS0ID_HASSUBTYPE;
    hassubtype->isAbstract = UA_FALSE;
    hassubtype->symmetric  = UA_FALSE;
    UA_NodeStore_insert(server->nodestore, (UA_Node*)hassubtype, UA_NULL);

    /* continue adding reference types with normal "addnode" */
    UA_ReferenceTypeNode *hierarchicalreferences = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hierarchicalreferences, "Hierarchicalreferences");
    hierarchicalreferences->nodeId.identifier.numeric = UA_NS0ID_HIERARCHICALREFERENCES;
    hierarchicalreferences->isAbstract = UA_TRUE;
    hierarchicalreferences->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)hierarchicalreferences,
                      &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_REFERENCES),
                      &UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *nonhierarchicalreferences = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)nonhierarchicalreferences, "NonHierarchicalReferences");
    nonhierarchicalreferences->nodeId.identifier.numeric = UA_NS0ID_NONHIERARCHICALREFERENCES;
    nonhierarchicalreferences->isAbstract = UA_TRUE;
    nonhierarchicalreferences->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)nonhierarchicalreferences,
                      &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_REFERENCES),
                      &UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *haschild = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)haschild, "HasChild");
    haschild->nodeId.identifier.numeric = UA_NS0ID_HASCHILD;
    haschild->isAbstract = UA_TRUE;
    haschild->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)haschild,
                      &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_HIERARCHICALREFERENCES),
                      &UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *organizes = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)organizes, "Organizes");
    organizes->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "OrganizedBy");
    organizes->nodeId.identifier.numeric = UA_NS0ID_ORGANIZES;
    organizes->isAbstract = UA_FALSE;
    organizes->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)organizes,
                      &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_HIERARCHICALREFERENCES),
                      &UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *haseventsource = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)haseventsource, "HasEventSource");
    haseventsource->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "EventSourceOf");
    haseventsource->nodeId.identifier.numeric = UA_NS0ID_HASEVENTSOURCE;
    haseventsource->isAbstract = UA_FALSE;
    haseventsource->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)haseventsource,
                      &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_HIERARCHICALREFERENCES),
                      &UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *hasmodellingrule = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hasmodellingrule, "HasModellingRule");
    hasmodellingrule->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "ModellingRuleOf");
    hasmodellingrule->nodeId.identifier.numeric = UA_NS0ID_HASMODELLINGRULE;
    hasmodellingrule->isAbstract = UA_FALSE;
    hasmodellingrule->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)hasmodellingrule,
                      &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_NONHIERARCHICALREFERENCES),
                      &UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *hasencoding = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hasencoding, "HasEncoding");
    hasencoding->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "EncodingOf");
    hasencoding->nodeId.identifier.numeric = UA_NS0ID_HASENCODING;
    hasencoding->isAbstract = UA_FALSE;
    hasencoding->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)hasencoding,
                      &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_NONHIERARCHICALREFERENCES),
                      &UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *hasdescription = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hasdescription, "HasDescription");
    hasdescription->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "DescriptionOf");
    hasdescription->nodeId.identifier.numeric = UA_NS0ID_HASDESCRIPTION;
    hasdescription->isAbstract = UA_FALSE;
    hasdescription->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)hasdescription,
                      &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_NONHIERARCHICALREFERENCES),
                      &UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *hastypedefinition = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hastypedefinition, "HasTypeDefinition");
    hastypedefinition->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "TypeDefinitionOf");
    hastypedefinition->nodeId.identifier.numeric = UA_NS0ID_HASTYPEDEFINITION;
    hastypedefinition->isAbstract = UA_FALSE;
    hastypedefinition->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)hastypedefinition,
                      &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_NONHIERARCHICALREFERENCES),
                      &UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *generatesevent = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)generatesevent, "GeneratesEvent");
    generatesevent->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "GeneratedBy");
    generatesevent->nodeId.identifier.numeric = UA_NS0ID_GENERATESEVENT;
    generatesevent->isAbstract = UA_FALSE;
    generatesevent->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)generatesevent,
                      &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_NONHIERARCHICALREFERENCES),
                      &UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *aggregates = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)aggregates, "Aggregates");
    // Todo: Is there an inverse name?
    aggregates->nodeId.identifier.numeric = UA_NS0ID_AGGREGATES;
    aggregates->isAbstract = UA_TRUE;
    aggregates->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)aggregates,
                      &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_HASCHILD),
                      &UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    // complete bootstrap of hassubtype
    ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_HASCHILD), UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
                 UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *hasproperty = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hasproperty, "HasProperty");
    hasproperty->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "PropertyOf");
    hasproperty->nodeId.identifier.numeric = UA_NS0ID_HASPROPERTY;
    hasproperty->isAbstract = UA_FALSE;
    hasproperty->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)hasproperty,
                      &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_AGGREGATES),
                      &UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *hascomponent = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hascomponent, "HasComponent");
    hascomponent->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "ComponentOf");
    hascomponent->nodeId.identifier.numeric = UA_NS0ID_HASCOMPONENT;
    hascomponent->isAbstract = UA_FALSE;
    hascomponent->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)hascomponent,
                      &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_AGGREGATES),
                      &UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *hasnotifier = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hasnotifier, "HasNotifier");
    hasnotifier->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "NotifierOf");
    hasnotifier->nodeId.identifier.numeric = UA_NS0ID_HASNOTIFIER;
    hasnotifier->isAbstract = UA_FALSE;
    hasnotifier->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)hasnotifier,
                      &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_HASEVENTSOURCE),
                      &UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *hasorderedcomponent = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hasorderedcomponent, "HasOrderedComponent");
    hasorderedcomponent->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "OrderedComponentOf");
    hasorderedcomponent->nodeId.identifier.numeric = UA_NS0ID_HASORDEREDCOMPONENT;
    hasorderedcomponent->isAbstract = UA_FALSE;
    hasorderedcomponent->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)hasorderedcomponent,
                      &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                      &UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *hasmodelparent = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hasmodelparent, "HasModelParent");
    hasmodelparent->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "ModelParentOf");
    hasmodelparent->nodeId.identifier.numeric = UA_NS0ID_HASMODELPARENT;
    hasmodelparent->isAbstract = UA_FALSE;
    hasmodelparent->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)hasmodelparent,
                      &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_NONHIERARCHICALREFERENCES),
                      &UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *fromstate = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)fromstate, "FromState");
    fromstate->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "ToTransition");
    fromstate->nodeId.identifier.numeric = UA_NS0ID_FROMSTATE;
    fromstate->isAbstract = UA_FALSE;
    fromstate->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)fromstate,
                      &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_NONHIERARCHICALREFERENCES),
                      &UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *tostate = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)tostate, "ToState");
    tostate->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "FromTransition");
    tostate->nodeId.identifier.numeric = UA_NS0ID_TOSTATE;
    tostate->isAbstract = UA_FALSE;
    tostate->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)tostate,
                      &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_NONHIERARCHICALREFERENCES),
                      &UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *hascause = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hascause, "HasCause");
    hascause->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "MayBeCausedBy");
    hascause->nodeId.identifier.numeric = UA_NS0ID_HASCAUSE;
    hascause->isAbstract = UA_FALSE;
    hascause->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)hascause,
                      &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_NONHIERARCHICALREFERENCES),
                      &UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));
    
    UA_ReferenceTypeNode *haseffect = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)haseffect, "HasEffect");
    haseffect->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "MayBeEffectedBy");
    haseffect->nodeId.identifier.numeric = UA_NS0ID_HASEFFECT;
    haseffect->isAbstract = UA_FALSE;
    haseffect->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)haseffect,
                      &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_NONHIERARCHICALREFERENCES),
                      &UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *hashistoricalconfiguration = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hashistoricalconfiguration, "HasHistoricalConfiguration");
    hashistoricalconfiguration->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "HistoricalConfigurationOf");
    hashistoricalconfiguration->nodeId.identifier.numeric = UA_NS0ID_HASHISTORICALCONFIGURATION;
    hashistoricalconfiguration->isAbstract = UA_FALSE;
    hashistoricalconfiguration->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)hashistoricalconfiguration,
                      &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_AGGREGATES),
                      &UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    /**********************/
    /* Basic Object Types */
    /**********************/

    UA_ObjectTypeNode *baseObjectType = UA_ObjectTypeNode_new();
    baseObjectType->nodeId.identifier.numeric = UA_NS0ID_BASEOBJECTTYPE;
    copyNames((UA_Node*)baseObjectType, "BaseObjectType");
    UA_NodeStore_insert(server->nodestore, (UA_Node*)baseObjectType, UA_NULL);

    UA_ObjectTypeNode *baseDataVarialbeType = UA_ObjectTypeNode_new();
    baseDataVarialbeType->nodeId.identifier.numeric = UA_NS0ID_BASEDATAVARIABLETYPE;
    copyNames((UA_Node*)baseDataVarialbeType, "BaseDataVariableType");
    UA_NodeStore_insert(server->nodestore, (UA_Node*)baseDataVarialbeType, UA_NULL);

    UA_ObjectTypeNode *folderType = UA_ObjectTypeNode_new();
    folderType->nodeId.identifier.numeric = UA_NS0ID_FOLDERTYPE;
    copyNames((UA_Node*)folderType, "FolderType");
    UA_NodeStore_insert(server->nodestore, (UA_Node*)folderType, UA_NULL);
    ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE), UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
 		   UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE));

    /*****************/
    /* Basic Folders */
    /*****************/

    UA_ObjectNode *root = UA_ObjectNode_new();
    copyNames((UA_Node*)root, "Root");
    root->nodeId.identifier.numeric = UA_NS0ID_ROOTFOLDER;
    UA_NodeStore_insert(server->nodestore, (UA_Node*)root, UA_NULL);
    ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
 		   UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE));

    UA_ObjectNode *objects = UA_ObjectNode_new();
    copyNames((UA_Node*)objects, "Objects");
    objects->nodeId.identifier.numeric = UA_NS0ID_OBJECTSFOLDER;
    UA_Server_addNode(server, (UA_Node*)objects,
 		   &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER),
 		   &UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES));
    ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
 		   UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE));

    UA_ObjectNode *types = UA_ObjectNode_new();
    copyNames((UA_Node*)types, "Types");
    types->nodeId.identifier.numeric = UA_NS0ID_TYPESFOLDER;
    UA_Server_addNode(server, (UA_Node*)types,
 		   &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER),
 		   &UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES));
    ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_TYPESFOLDER), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
 		   UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE));

    UA_ObjectNode *views = UA_ObjectNode_new();
    copyNames((UA_Node*)views, "Views");
    views->nodeId.identifier.numeric = UA_NS0ID_VIEWSFOLDER;
    UA_Server_addNode(server, (UA_Node*)views,
 		   &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER),
 		   &UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES));
    ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_VIEWSFOLDER), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
                 UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE));

    /**********************/
    /* Further Data Types */
    /**********************/

    UA_ObjectNode *datatypes = UA_ObjectNode_new();
    copyNames((UA_Node*)datatypes, "DataTypes");
    datatypes->nodeId.identifier.numeric = UA_NS0ID_DATATYPESFOLDER;
    UA_Server_addNode(server, (UA_Node*)datatypes,
                      &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_TYPESFOLDER),
                      &UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES));
    ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_DATATYPESFOLDER), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
                 UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE));

    addDataTypeNode(server, "BaseDataType", UA_NS0ID_BASEDATATYPE, UA_NS0ID_DATATYPESFOLDER);
    addDataTypeNode(server, "Boolean", UA_NS0ID_BOOLEAN, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "Number", UA_NS0ID_NUMBER, UA_NS0ID_BASEDATATYPE);
    	addDataTypeNode(server, "Float", UA_NS0ID_FLOAT, UA_NS0ID_NUMBER);
    	addDataTypeNode(server, "Double", UA_NS0ID_DOUBLE, UA_NS0ID_NUMBER);
    	addDataTypeNode(server, "Integer", UA_NS0ID_INTEGER, UA_NS0ID_NUMBER);
    		addDataTypeNode(server, "SByte", UA_NS0ID_SBYTE, UA_NS0ID_INTEGER);
    		addDataTypeNode(server, "Int16", UA_NS0ID_INT16, UA_NS0ID_INTEGER);
    		addDataTypeNode(server, "Int32", UA_NS0ID_INT32, UA_NS0ID_INTEGER);
    		addDataTypeNode(server, "Int64", UA_NS0ID_INT64, UA_NS0ID_INTEGER);
    		addDataTypeNode(server, "UInteger", UA_NS0ID_UINTEGER, UA_NS0ID_INTEGER);
				addDataTypeNode(server, "Byte", UA_NS0ID_BYTE, UA_NS0ID_UINTEGER);
				addDataTypeNode(server, "UInt16", UA_NS0ID_UINT16, UA_NS0ID_UINTEGER);
				addDataTypeNode(server, "UInt32", UA_NS0ID_UINT32, UA_NS0ID_UINTEGER);
				addDataTypeNode(server, "UInt64", UA_NS0ID_UINT64, UA_NS0ID_UINTEGER);
    addDataTypeNode(server, "String", UA_NS0ID_STRING, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "DateTime", UA_NS0ID_DATETIME, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "Guid", UA_NS0ID_GUID, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "ByteString", UA_NS0ID_BYTESTRING, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "XmlElement", UA_NS0ID_XMLELEMENT, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "NodeId", UA_NS0ID_NODEID, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "ExpandedNodeId", UA_NS0ID_EXPANDEDNODEID, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "StatusCode", UA_NS0ID_STATUSCODE, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "QualifiedName", UA_NS0ID_QUALIFIEDNAME, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "LocalizedText", UA_NS0ID_LOCALIZEDTEXT, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "Structure", UA_NS0ID_STRUCTURE, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "DataValue", UA_NS0ID_DATAVALUE, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "DiagnosticInfo", UA_NS0ID_DIAGNOSTICINFO, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "Enumeration", UA_NS0ID_ENUMERATION, UA_NS0ID_BASEDATATYPE);
    	addDataTypeNode(server, "ServerState", UA_NS0ID_SERVERSTATE, UA_NS0ID_ENUMERATION);

   UA_ObjectNode *variabletypes = UA_ObjectNode_new();
   copyNames((UA_Node*)variabletypes, "VariableTypes");
   variabletypes->nodeId.identifier.numeric = UA_NS0ID_VARIABLETYPESFOLDER;
   UA_Server_addNode(server, (UA_Node*)variabletypes,
                     &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_TYPESFOLDER),
                     &UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES));
   ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_VARIABLETYPESFOLDER), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
                UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE));
   addVariableTypeNode_organized(server, "BaseVariableType", UA_NS0ID_BASEVARIABLETYPE, UA_NS0ID_VARIABLETYPESFOLDER, UA_TRUE);
   addVariableTypeNode_subtype(server, "PropertyType", UA_NS0ID_PROPERTYTYPE, UA_NS0ID_BASEVARIABLETYPE, UA_FALSE);

   /*******************/
   /* Further Objects */
   /*******************/

   UA_ObjectNode *servernode = UA_ObjectNode_new();
   copyNames((UA_Node*)servernode, "Server");
   servernode->nodeId.identifier.numeric = UA_NS0ID_SERVER;
   UA_Server_addNode(server, (UA_Node*)servernode,
		   &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
		   &UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES));

   UA_VariableNode *namespaceArray = UA_VariableNode_new();
   copyNames((UA_Node*)namespaceArray, "NamespaceArray");
   namespaceArray->nodeId.identifier.numeric = UA_NS0ID_SERVER_NAMESPACEARRAY;
   namespaceArray->valueSource = UA_VALUESOURCE_DATASOURCE;
   namespaceArray->value.dataSource = (UA_DataSource) {.handle = server, .read = readNamespaces,
	   .release = releaseNamespaces, .write = UA_NULL};
   namespaceArray->valueRank = 1;
   namespaceArray->minimumSamplingInterval = 1.0;
   namespaceArray->historizing = UA_FALSE;
   UA_Server_addNode(server, (UA_Node*)namespaceArray,
		   &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVER),
		   &UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY));
   ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_NAMESPACEARRAY),
                UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
                UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_PROPERTYTYPE));

   UA_VariableNode *serverArray = UA_VariableNode_new();
   copyNames((UA_Node*)serverArray, "ServerArray");
   serverArray->nodeId.identifier.numeric = UA_NS0ID_SERVER_SERVERARRAY;
   serverArray->value.variant.data = UA_Array_new(&UA_TYPES[UA_TYPES_STRING], 1);
   serverArray->value.variant.arrayLength = 1;
   serverArray->value.variant.type = &UA_TYPES[UA_TYPES_STRING];
   *(UA_String *)serverArray->value.variant.data = UA_STRING_ALLOC(APPLICATION_URI);
   serverArray->valueRank = 1;
   serverArray->minimumSamplingInterval = 1.0;
   serverArray->historizing = UA_FALSE;
   UA_Server_addNode(server, (UA_Node*)serverArray,
 		   &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVER),
 		   &UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY));
   ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERARRAY),
                UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
                UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_PROPERTYTYPE));

   UA_ObjectNode *servercapablities = UA_ObjectNode_new();
   copyNames((UA_Node*)servercapablities, "ServerCapabilities");
   servercapablities->nodeId.identifier.numeric = UA_NS0ID_SERVER_SERVERCAPABILITIES;
   UA_Server_addNode(server, (UA_Node*)servercapablities,
		   &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVER),
		   &UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT));

   UA_VariableNode *localeIdArray = UA_VariableNode_new();
   copyNames((UA_Node*)localeIdArray, "LocaleIdArray");
   localeIdArray->nodeId.identifier.numeric = UA_NS0ID_SERVER_SERVERCAPABILITIES_LOCALEIDARRAY;
   localeIdArray->value.variant.data = UA_Array_new(&UA_TYPES[UA_TYPES_STRING], 2);
   localeIdArray->value.variant.arrayLength = 2;
   localeIdArray->value.variant.type = &UA_TYPES[UA_TYPES_STRING];
   *(UA_String *)localeIdArray->value.variant.data = UA_STRING_ALLOC("en");
   localeIdArray->valueRank = 1;
   localeIdArray->minimumSamplingInterval = 1.0;
   localeIdArray->historizing = UA_FALSE;
   UA_Server_addNode(server, (UA_Node*)localeIdArray,
		   &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERCAPABILITIES),
		   &UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY));
   ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERCAPABILITIES_LOCALEIDARRAY), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
		   UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_PROPERTYTYPE));

   UA_VariableNode *serverstatus = UA_VariableNode_new();
   copyNames((UA_Node*)serverstatus, "ServerStatus");
   serverstatus->nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS);
   serverstatus->valueSource = UA_VALUESOURCE_DATASOURCE;
   serverstatus->value.dataSource = (UA_DataSource) {.handle = server, .read = readStatus,
	   .release = releaseStatus, .write = UA_NULL};
   UA_Server_addNode(server, (UA_Node*)serverstatus, &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVER),
		   &UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT));

   UA_VariableNode *state = UA_VariableNode_new();
   UA_ServerState *stateEnum = UA_ServerState_new();
   *stateEnum = UA_SERVERSTATE_RUNNING;
   copyNames((UA_Node*)state, "State");
   state->nodeId.identifier.numeric = UA_NS0ID_SERVER_SERVERSTATUS_STATE;
   state->value.variant.type = &UA_TYPES[UA_TYPES_SERVERSTATE];
   state->value.variant.arrayLength = -1;
   state->value.variant.data = stateEnum; // points into the other object.
   UA_NodeStore_insert(server->nodestore, (UA_Node*)state, UA_NULL);
   ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS), UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
		   UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_STATE));

   UA_VariableNode *currenttime = UA_VariableNode_new();
   copyNames((UA_Node*)currenttime, "CurrentTime");
   currenttime->nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_CURRENTTIME);
   currenttime->valueSource = UA_VALUESOURCE_DATASOURCE;
   currenttime->value.dataSource = (UA_DataSource) {.handle = NULL, .read = readCurrentTime,
	   .release = releaseCurrentTime, .write = UA_NULL};
   UA_Server_addNode(server, (UA_Node*)currenttime, &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS),
		   &UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT));

#ifdef DEMO_NODESET
   /**************/
   /* Demo Nodes */
   /**************/

#define DEMOID 990
   UA_ObjectNode *demo = UA_ObjectNode_new();
   copyNames((UA_Node*)demo, "Demo");
   demo->nodeId = UA_NODEID_NUMERIC(1, DEMOID);
   UA_Server_addNode(server, (UA_Node*)demo,
		   &UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
		   &UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES));
   ADDREFERENCE(UA_NODEID_NUMERIC(1, DEMOID), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
		   UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE));

#define SCALARID 991
   UA_ObjectNode *scalar = UA_ObjectNode_new();
   copyNames((UA_Node*)scalar, "Scalar");
   scalar->nodeId = UA_NODEID_NUMERIC(1, SCALARID);
   UA_Server_addNode(server, (UA_Node*)scalar,
		   &UA_EXPANDEDNODEID_NUMERIC(1, DEMOID),
		   &UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES));
   ADDREFERENCE(UA_NODEID_NUMERIC(1, SCALARID), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
		   UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE));

#define ARRAYID 992
   UA_ObjectNode *array = UA_ObjectNode_new();
   copyNames((UA_Node*)array, "Arrays");
   array->nodeId = UA_NODEID_NUMERIC(1, ARRAYID);
   UA_Server_addNode(server, (UA_Node*)array,
		   &UA_EXPANDEDNODEID_NUMERIC(1, DEMOID),
		   &UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES));
   ADDREFERENCE(UA_NODEID_NUMERIC(1, ARRAYID), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
		   UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE));

   UA_UInt32 id = 1000; //running id in namespace 1
   for(UA_UInt32 type = 0; UA_IS_BUILTIN(type); type++) {
       if(type == UA_TYPES_VARIANT || type == UA_TYPES_DIAGNOSTICINFO)
           continue;
	   //add a scalar node for every built-in type
	    void *value = UA_new(&UA_TYPES[type]);
	    UA_Variant *variant = UA_Variant_new();
	    UA_Variant_setScalar(variant, value, &UA_TYPES[type]);
	    char name[15];
	    sprintf(name, "%02d", type);
	    UA_QualifiedName myIntegerName = UA_QUALIFIEDNAME(1, name);
	    UA_Server_addVariableNode(server, variant, myIntegerName, UA_NODEID_NUMERIC(1, ++id),
	                              UA_NODEID_NUMERIC(1, SCALARID), UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES));

        //add an array node for every built-in type
        UA_Variant *arrayvar = UA_Variant_new();
        UA_Variant_setArray(arrayvar, UA_Array_new(&UA_TYPES[type], 10), 10, &UA_TYPES[type]);
        UA_Server_addVariableNode(server, arrayvar, myIntegerName, UA_NODEID_NUMERIC(1, ++id),
                                  UA_NODEID_NUMERIC(1, ARRAYID), UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES));
   }
#endif

   return server;
}

UA_StatusCode
UA_Server_addVariableNode(UA_Server *server, UA_Variant *value, const UA_QualifiedName browseName, 
                          UA_NodeId nodeId, const UA_NodeId parentNodeId, const UA_NodeId referenceTypeId)
{
    UA_VariableNode *node = UA_VariableNode_new();
    node->value.variant = *value; // copy content
    UA_NodeId_copy(&nodeId, &node->nodeId);
    UA_QualifiedName_copy(&browseName, &node->browseName);
    UA_String_copy(&browseName.name, &node->displayName.text);
    UA_ExpandedNodeId parentId; // we need an expandednodeid
    UA_ExpandedNodeId_init(&parentId);
    UA_NodeId_copy(&parentNodeId, &parentId.nodeId);
    UA_AddNodesResult res =
        UA_Server_addNodeWithSession(server, &adminSession, (UA_Node*)node, &parentId, &referenceTypeId);
    ADDREFERENCE(res.addedNodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
                 UA_EXPANDEDNODEID_NUMERIC(0, value->type->typeId.identifier.numeric));
    if(res.statusCode != UA_STATUSCODE_GOOD) {
        UA_Variant_init(&node->value.variant);
        UA_VariableNode_delete(node);
    } else
        UA_free(value);
    UA_AddNodesResult_deleteMembers(&res);
    return res.statusCode;
}

UA_StatusCode
UA_Server_addDataSourceVariableNode(UA_Server *server, UA_DataSource dataSource,
                                    const UA_QualifiedName browseName, UA_NodeId nodeId,
                                    const UA_NodeId parentNodeId, const UA_NodeId referenceTypeId)
{
    UA_VariableNode *node = UA_VariableNode_new();
    node->valueSource = UA_VALUESOURCE_DATASOURCE;
    node->value.dataSource = dataSource;
    UA_NodeId_copy(&nodeId, &node->nodeId);
    UA_QualifiedName_copy(&browseName, &node->browseName);
    UA_String_copy(&browseName.name, &node->displayName.text);
    UA_ExpandedNodeId parentId; // dummy exapndednodeid
    UA_ExpandedNodeId_init(&parentId);
    UA_NodeId_copy(&parentNodeId, &parentId.nodeId);
    UA_AddNodesResult res =
        UA_Server_addNodeWithSession(server, &adminSession, (UA_Node*)node, &parentId, &referenceTypeId);
    ADDREFERENCE(res.addedNodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
                 UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE));
    if(res.statusCode != UA_STATUSCODE_GOOD)
        UA_VariableNode_delete(node);
    UA_AddNodesResult_deleteMembers(&res);
    return res.statusCode;
}

/* Adds a one-way reference to the local nodestore */
static UA_StatusCode
addOneWayReferenceWithSession(UA_Server *server, UA_Session *session, const UA_AddReferencesItem *item)
{
    const UA_Node *node = UA_NodeStore_get(server->nodestore, &item->sourceNodeId);
    if(!node)
        return UA_STATUSCODE_BADINTERNALERROR;

    UA_Node *newNode = UA_NULL;
    void (*deleteNode)(UA_Node*) = UA_NULL;
    switch(node->nodeClass) {
    case UA_NODECLASS_OBJECT:
        newNode = (UA_Node*)UA_ObjectNode_new();
        UA_ObjectNode_copy((const UA_ObjectNode*)node, (UA_ObjectNode*)newNode);
        deleteNode = (void (*)(UA_Node*))UA_ObjectNode_delete;
        break;
    case UA_NODECLASS_VARIABLE:
        newNode = (UA_Node*)UA_VariableNode_new();
        UA_VariableNode_copy((const UA_VariableNode*)node, (UA_VariableNode*)newNode);
        deleteNode = (void (*)(UA_Node*))UA_VariableNode_delete;
        break;
    case UA_NODECLASS_METHOD:
        newNode = (UA_Node*)UA_MethodNode_new();
        UA_MethodNode_copy((const UA_MethodNode*)node, (UA_MethodNode*)newNode);
        deleteNode = (void (*)(UA_Node*))UA_MethodNode_delete;
        break;
    case UA_NODECLASS_OBJECTTYPE:
        newNode = (UA_Node*)UA_ObjectTypeNode_new();
        UA_ObjectTypeNode_copy((const UA_ObjectTypeNode*)node, (UA_ObjectTypeNode*)newNode);
        deleteNode = (void (*)(UA_Node*))UA_ObjectTypeNode_delete;
        break;
    case UA_NODECLASS_VARIABLETYPE:
        newNode = (UA_Node*)UA_VariableTypeNode_new();
        UA_VariableTypeNode_copy((const UA_VariableTypeNode*)node, (UA_VariableTypeNode*)newNode);
        deleteNode = (void (*)(UA_Node*))UA_VariableTypeNode_delete;
        break;
    case UA_NODECLASS_REFERENCETYPE:
        newNode = (UA_Node*)UA_ReferenceTypeNode_new();
        UA_ReferenceTypeNode_copy((const UA_ReferenceTypeNode*)node, (UA_ReferenceTypeNode*)newNode);
        deleteNode = (void (*)(UA_Node*))UA_ReferenceTypeNode_delete;
        break;
    case UA_NODECLASS_DATATYPE:
        newNode = (UA_Node*)UA_DataTypeNode_new();
        UA_DataTypeNode_copy((const UA_DataTypeNode*)node, (UA_DataTypeNode*)newNode);
        deleteNode = (void (*)(UA_Node*))UA_DataTypeNode_delete;
        break;
    case UA_NODECLASS_VIEW:
        newNode = (UA_Node*)UA_ViewNode_new();
        UA_ViewNode_copy((const UA_ViewNode*)node, (UA_ViewNode*)newNode);
        deleteNode = (void (*)(UA_Node*))UA_ViewNode_delete;
        break;
    default:
        return UA_STATUSCODE_BADINTERNALERROR;
    }

    UA_Int32 count = node->referencesSize;
    if(count < 0)
        count = 0;
    UA_ReferenceNode *old_refs = newNode->references;
    UA_ReferenceNode *new_refs = UA_malloc(sizeof(UA_ReferenceNode)*(count+1));
    if(!new_refs) {
        deleteNode(newNode);
        UA_NodeStore_release(node);
        return UA_STATUSCODE_BADOUTOFMEMORY;
    }

    // insert the new reference
    UA_memcpy(new_refs, old_refs, sizeof(UA_ReferenceNode)*count);
    UA_ReferenceNode_init(&new_refs[count]);
    UA_StatusCode retval = UA_NodeId_copy(&item->referenceTypeId, &new_refs[count].referenceTypeId);
    new_refs[count].isInverse = !item->isForward;
    retval |= UA_ExpandedNodeId_copy(&item->targetNodeId, &new_refs[count].targetId);
    if(retval != UA_STATUSCODE_GOOD) {
        UA_Array_delete(new_refs, &UA_TYPES[UA_TYPES_REFERENCENODE], ++count);
        newNode->references = UA_NULL;
        newNode->referencesSize = 0;
        deleteNode(newNode);
        UA_NodeStore_release(node);
        return UA_STATUSCODE_BADOUTOFMEMORY;
    }

    UA_free(old_refs);
    newNode->references = new_refs;
    newNode->referencesSize = ++count;
    retval = UA_NodeStore_replace(server->nodestore, node, newNode, UA_NULL);
    UA_NodeStore_release(node);
    if(retval != UA_STATUSCODE_BADINTERNALERROR)
        return retval;
    
    // error presumably because the node was replaced and an old version was updated just try again
    deleteNode(newNode);
    return addOneWayReferenceWithSession(server, session, item);
}

/* userland version of addReferenceWithSession */
UA_StatusCode
UA_Server_addReference(UA_Server *server, const UA_AddReferencesItem *item)
{
    return UA_Server_addReferenceWithSession(server, &adminSession, item);
}

UA_StatusCode
UA_Server_addReferenceWithSession(UA_Server *server, UA_Session *session, const UA_AddReferencesItem *item)
{
    if(item->targetServerUri.length > 0)
        return UA_STATUSCODE_BADNOTIMPLEMENTED; // currently no expandednodeids are allowed
    
    // Is this for an external nodestore?
    UA_ExternalNodeStore *ensFirst = UA_NULL;
    UA_ExternalNodeStore *ensSecond = UA_NULL;
    for(UA_Int32 j = 0;j<server->externalNamespacesSize && (!ensFirst || !ensSecond);j++) {
        if(item->sourceNodeId.namespaceIndex == server->externalNamespaces[j].index)
            ensFirst = &server->externalNamespaces[j].externalNodeStore;
        if(item->targetNodeId.nodeId.namespaceIndex == server->externalNamespaces[j].index)
            ensSecond = &server->externalNamespaces[j].externalNodeStore;
    }

    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    if(ensFirst) {
        // todo: use external nodestore
    } else
        retval = addOneWayReferenceWithSession(server, session, item);

    if(retval) return retval;

    UA_AddReferencesItem secondItem;
    secondItem = *item;
    secondItem.targetNodeId.nodeId = item->sourceNodeId;
    secondItem.sourceNodeId = item->targetNodeId.nodeId;
    secondItem.isForward = !item->isForward;
    if(ensSecond) {
        // todo: use external nodestore
    } else
        retval = addOneWayReferenceWithSession (server, session, &secondItem);
    // todo: remove reference if the second direction failed

    return retval;
} 

/* userland version of addNodeWithSession */
UA_AddNodesResult
UA_Server_addNode(UA_Server *server, UA_Node *node, const UA_ExpandedNodeId *parentNodeId,
                  const UA_NodeId *referenceTypeId)
{
    return UA_Server_addNodeWithSession(server, &adminSession, node, parentNodeId, referenceTypeId);
}

UA_AddNodesResult
UA_Server_addNodeWithSession(UA_Server *server, UA_Session *session, UA_Node *node,
                             const UA_ExpandedNodeId *parentNodeId, const UA_NodeId *referenceTypeId)
{
    UA_AddNodesResult result;
    UA_AddNodesResult_init(&result);

    if(node->nodeId.namespaceIndex >= server->namespacesSize) {
        result.statusCode = UA_STATUSCODE_BADNODEIDINVALID;
        return result;
    }

    const UA_Node *parent = UA_NodeStore_get(server->nodestore, &parentNodeId->nodeId);
    if(!parent) {
        result.statusCode = UA_STATUSCODE_BADPARENTNODEIDINVALID;
        return result;
    }

    const UA_ReferenceTypeNode *referenceType =
        (const UA_ReferenceTypeNode *)UA_NodeStore_get(server->nodestore, referenceTypeId);
    if(!referenceType) {
        result.statusCode = UA_STATUSCODE_BADREFERENCETYPEIDINVALID;
        goto ret;
    }

    if(referenceType->nodeClass != UA_NODECLASS_REFERENCETYPE) {
        result.statusCode = UA_STATUSCODE_BADREFERENCETYPEIDINVALID;
        goto ret2;
    }

    if(referenceType->isAbstract == UA_TRUE) {
        result.statusCode = UA_STATUSCODE_BADREFERENCENOTALLOWED;
        goto ret2;
    }

    // todo: test if the referencetype is hierarchical
    const UA_Node *managed = UA_NULL;
    if(UA_NodeId_isNull(&node->nodeId)) {
        if(UA_NodeStore_insert(server->nodestore, node, &managed) != UA_STATUSCODE_GOOD) {
            result.statusCode = UA_STATUSCODE_BADOUTOFMEMORY;
            goto ret2;
        }
        result.addedNodeId = managed->nodeId; // cannot fail as unique nodeids are numeric
    } else {
        if(UA_NodeId_copy(&node->nodeId, &result.addedNodeId) != UA_STATUSCODE_GOOD) {
            result.statusCode = UA_STATUSCODE_BADOUTOFMEMORY;
            goto ret2;
        }

        if(UA_NodeStore_insert(server->nodestore, node, &managed) != UA_STATUSCODE_GOOD) {
            result.statusCode = UA_STATUSCODE_BADNODEIDEXISTS;  // todo: differentiate out of memory
            UA_NodeId_deleteMembers(&result.addedNodeId);
            goto ret2;
        }
    }
    
    // reference back to the parent
    UA_AddReferencesItem item;
    UA_AddReferencesItem_init(&item);
    item.sourceNodeId = managed->nodeId;
    item.referenceTypeId = referenceType->nodeId;
    item.isForward = UA_FALSE;
    item.targetNodeId.nodeId = parent->nodeId;
    UA_Server_addReference(server, &item);

    // todo: error handling. remove new node from nodestore

    UA_NodeStore_release(managed);
    
 ret2:
    UA_NodeStore_release((const UA_Node*)referenceType);
 ret:
    UA_NodeStore_release(parent);

    return result;
}

/** Max size of messages that are allocated on the stack */
#define MAX_STACK_MESSAGE 65536

static UA_StatusCode UA_ByteStringArray_deleteMembers(UA_ByteStringArray *stringarray) {
    if(!stringarray)
        return UA_STATUSCODE_BADINTERNALERROR;
    for(UA_UInt32 i = 0; i < stringarray->stringsSize; i++)
        UA_String_deleteMembers(&stringarray->strings[i]);
    return UA_STATUSCODE_GOOD;
}

static void processHEL(UA_Connection *connection, const UA_ByteString *msg, size_t *pos) {
    UA_TcpHelloMessage helloMessage;
    if(UA_TcpHelloMessage_decodeBinary(msg, pos, &helloMessage) != UA_STATUSCODE_GOOD) {
        connection->close(connection);
        return;
    }

    connection->remoteConf.maxChunkCount = helloMessage.maxChunkCount;
    connection->remoteConf.maxMessageSize = helloMessage.maxMessageSize;
    connection->remoteConf.protocolVersion = helloMessage.protocolVersion;
    connection->remoteConf.recvBufferSize = helloMessage.receiveBufferSize;
    connection->remoteConf.sendBufferSize = helloMessage.sendBufferSize;
    connection->state = UA_CONNECTION_ESTABLISHED;

    // build acknowledge response
    UA_TcpAcknowledgeMessage ackMessage;
    ackMessage.protocolVersion = connection->localConf.protocolVersion;
    ackMessage.receiveBufferSize = connection->localConf.recvBufferSize;
    ackMessage.sendBufferSize = connection->localConf.sendBufferSize;
    ackMessage.maxMessageSize = connection->localConf.maxMessageSize;
    ackMessage.maxChunkCount = connection->localConf.maxChunkCount;

    UA_TcpMessageHeader ackHeader;
    ackHeader.messageTypeAndFinal = UA_MESSAGETYPEANDFINAL_ACKF;
    ackHeader.messageSize = UA_TcpAcknowledgeMessage_calcSizeBinary(&ackMessage) +
        UA_TcpMessageHeader_calcSizeBinary(&ackHeader);

    // The message is on the stack. That's ok since ack is very small.
    UA_ByteString ack_msg = (UA_ByteString){ .length = ackHeader.messageSize,
                                             .data = UA_alloca(ackHeader.messageSize) };
    size_t tmpPos = 0;
    UA_TcpMessageHeader_encodeBinary(&ackHeader, &ack_msg, &tmpPos);
    UA_TcpAcknowledgeMessage_encodeBinary(&ackMessage, &ack_msg, &tmpPos);
    UA_ByteStringArray answer_buf = { .stringsSize = 1, .strings = &ack_msg };
    connection->write(connection, answer_buf);
    UA_TcpHelloMessage_deleteMembers(&helloMessage);
}

static void processOPN(UA_Connection *connection, UA_Server *server, const UA_ByteString *msg,
                       size_t *pos) {
    if(connection->state != UA_CONNECTION_ESTABLISHED) {
        connection->close(connection);
        return;
    }

    UA_UInt32 secureChannelId;
    UA_StatusCode retval = UA_UInt32_decodeBinary(msg, pos, &secureChannelId);

    UA_AsymmetricAlgorithmSecurityHeader asymHeader;
    retval |= UA_AsymmetricAlgorithmSecurityHeader_decodeBinary(msg, pos, &asymHeader);

    UA_SequenceHeader seqHeader;
    retval |= UA_SequenceHeader_decodeBinary(msg, pos, &seqHeader);

    UA_NodeId requestType;
    retval |= UA_NodeId_decodeBinary(msg, pos, &requestType);

    UA_OpenSecureChannelRequest r;
    retval |= UA_OpenSecureChannelRequest_decodeBinary(msg, pos, &r);

    if(retval != UA_STATUSCODE_GOOD || requestType.identifier.numeric != 446) {
        UA_AsymmetricAlgorithmSecurityHeader_deleteMembers(&asymHeader);
        UA_SequenceHeader_deleteMembers(&seqHeader);
        UA_NodeId_deleteMembers(&requestType);
        UA_OpenSecureChannelRequest_deleteMembers(&r);
        connection->close(connection);
        return;
    }

    UA_OpenSecureChannelResponse p;
    UA_OpenSecureChannelResponse_init(&p);
    Service_OpenSecureChannel(server, connection, &r, &p);

    UA_SecureConversationMessageHeader respHeader;
    respHeader.messageHeader.messageTypeAndFinal = UA_MESSAGETYPEANDFINAL_OPNF;
    respHeader.messageHeader.messageSize = 0;
    respHeader.secureChannelId = p.securityToken.channelId;

    UA_NodeId responseType = UA_NODEID_NUMERIC(0, UA_NS0ID_OPENSECURECHANNELRESPONSE +
                                               UA_ENCODINGOFFSET_BINARY);

    respHeader.messageHeader.messageSize =
        UA_SecureConversationMessageHeader_calcSizeBinary(&respHeader)
        + UA_AsymmetricAlgorithmSecurityHeader_calcSizeBinary(&asymHeader)
        + UA_SequenceHeader_calcSizeBinary(&seqHeader)
        + UA_NodeId_calcSizeBinary(&responseType)
        + UA_OpenSecureChannelResponse_calcSizeBinary(&p);

    UA_ByteString resp_msg = (UA_ByteString){
        .length = respHeader.messageHeader.messageSize,
        .data = UA_alloca(respHeader.messageHeader.messageSize)
    };

    size_t tmpPos = 0;
    UA_SecureConversationMessageHeader_encodeBinary(&respHeader, &resp_msg, &tmpPos);
    UA_AsymmetricAlgorithmSecurityHeader_encodeBinary(&asymHeader, &resp_msg, &tmpPos); // just mirror back
    UA_SequenceHeader_encodeBinary(&seqHeader, &resp_msg, &tmpPos); // just mirror back
    UA_NodeId_encodeBinary(&responseType, &resp_msg, &tmpPos);
    UA_OpenSecureChannelResponse_encodeBinary(&p, &resp_msg, &tmpPos);

    UA_OpenSecureChannelRequest_deleteMembers(&r);
    UA_OpenSecureChannelResponse_deleteMembers(&p);
    UA_AsymmetricAlgorithmSecurityHeader_deleteMembers(&asymHeader);
    connection->write(connection, (UA_ByteStringArray){ .stringsSize = 1, .strings = &resp_msg });
}

static void init_response_header(const UA_RequestHeader *p, UA_ResponseHeader *r) {
    r->requestHandle = p->requestHandle;
    r->serviceResult = UA_STATUSCODE_GOOD;
    r->stringTableSize = 0;
    r->timestamp = UA_DateTime_now();
}

// if the message is small enough, we allocate it on the stack and save a malloc
#define ALLOC_MESSAGE(MESSAGE, SIZE) do {                               \
        UA_UInt32 messageSize = SIZE;                                   \
        if(messageSize <= MAX_STACK_MESSAGE) {                          \
            messageOnStack = UA_TRUE;                                   \
            *MESSAGE = (UA_ByteString){.length = messageSize,           \
                                       .data = UA_alloca(messageSize)}; \
                        } else                                          \
            UA_ByteString_newMembers(MESSAGE, messageSize);             \
    } while(0)

#define INVOKE_SERVICE(TYPE) do {                                       \
        UA_##TYPE##Request p;                                           \
        UA_##TYPE##Response r;                                          \
        if(UA_##TYPE##Request_decodeBinary(msg, pos, &p))               \
            return;                                                     \
        UA_##TYPE##Response_init(&r);                                   \
        init_response_header(&p.requestHeader, &r.responseHeader);      \
        Service_##TYPE(server, clientSession, &p, &r);                  \
        ALLOC_MESSAGE(message, UA_##TYPE##Response_calcSizeBinary(&r)); \
        UA_##TYPE##Response_encodeBinary(&r, message, &sendOffset);     \
        UA_##TYPE##Request_deleteMembers(&p);                           \
        UA_##TYPE##Response_deleteMembers(&r);                          \
        responseType = requestType.identifier.numeric + 3;              \
    } while(0)

static void processMSG(UA_Connection *connection, UA_Server *server, const UA_ByteString *msg, size_t *pos) {
    // 1) Read in the securechannel
    UA_UInt32 secureChannelId;
    UA_StatusCode retval = UA_UInt32_decodeBinary(msg, pos, &secureChannelId);
    if(retval != UA_STATUSCODE_GOOD)
        return;

    UA_SecureChannel *clientChannel = connection->channel;
    UA_SecureChannel anonymousChannel;
    if(!clientChannel) {
        UA_SecureChannel_init(&anonymousChannel);
        clientChannel = &anonymousChannel;
    }

    UA_Session *clientSession = clientChannel->session;
#ifdef EXTENSION_STATELESS
    if(secureChannelId == 0)
        clientSession = &anonymousSession;
#endif

    // 2) Read the security header
    UA_UInt32 tokenId;
    UA_SequenceHeader sequenceHeader;
    retval = UA_UInt32_decodeBinary(msg, pos, &tokenId);
    retval |= UA_SequenceHeader_decodeBinary(msg, pos, &sequenceHeader);
    if(retval != UA_STATUSCODE_GOOD)
        return;

    //UA_SecureChannel_checkSequenceNumber(channel,sequenceHeader.sequenceNumber);
    //UA_SecureChannel_checkRequestId(channel,sequenceHeader.requestId);
    clientChannel->sequenceNumber = sequenceHeader.sequenceNumber;
    clientChannel->requestId = sequenceHeader.requestId;

    // 3) Read the nodeid of the request
    UA_NodeId requestType;
    if(UA_NodeId_decodeBinary(msg, pos, &requestType))
        return;
    if(requestType.identifierType != UA_NODEIDTYPE_NUMERIC) {
        // That must not happen. The requestType does not have to be deleted at the end.
        UA_NodeId_deleteMembers(&requestType);
        return;
    }

    // 4) process the request
    UA_ByteString responseBufs[2]; // 0->header, 1->response payload
    UA_UInt32 responseType;
    UA_ByteString *header = &responseBufs[0];
    UA_ByteString *message = &responseBufs[1];
    UA_Boolean messageOnStack = UA_FALSE;
    size_t sendOffset = 0;

#ifdef EXTENSION_STATELESS
    switch(requestType.identifier.numeric - UA_ENCODINGOFFSET_BINARY) {
    case UA_NS0ID_READREQUEST:
    case UA_NS0ID_WRITEREQUEST:
    case UA_NS0ID_BROWSEREQUEST:
        break;
    default:
        if(clientSession != &anonymousSession)
            retval = UA_STATUSCODE_BADNOTCONNECTED;
    }
#endif

    //subtract UA_ENCODINGOFFSET_BINARY for binary encoding, if retval is set, this forces the default path
    switch(requestType.identifier.numeric - UA_ENCODINGOFFSET_BINARY + retval) {
    case UA_NS0ID_GETENDPOINTSREQUEST: {
        UA_GetEndpointsRequest  p;
        UA_GetEndpointsResponse r;
        if(UA_GetEndpointsRequest_decodeBinary(msg, pos, &p))
            return;
        UA_GetEndpointsResponse_init(&r);
        init_response_header(&p.requestHeader, &r.responseHeader);
        Service_GetEndpoints(server, &p, &r);
        ALLOC_MESSAGE(message, UA_GetEndpointsResponse_calcSizeBinary(&r));
        UA_GetEndpointsResponse_encodeBinary(&r, message, &sendOffset);
        UA_GetEndpointsRequest_deleteMembers(&p);
        UA_GetEndpointsResponse_deleteMembers(&r);
        responseType = requestType.identifier.numeric + 3;
        break;
    }

    case UA_NS0ID_FINDSERVERSREQUEST: {
        UA_FindServersRequest  p;
        UA_FindServersResponse r;
        if(UA_FindServersRequest_decodeBinary(msg, pos, &p))
            return;
        UA_FindServersResponse_init(&r);
        init_response_header(&p.requestHeader, &r.responseHeader);
        Service_FindServers(server, &p, &r);
        ALLOC_MESSAGE(message, UA_FindServersResponse_calcSizeBinary(&r));
        UA_FindServersResponse_encodeBinary(&r, message, &sendOffset);
        UA_FindServersRequest_deleteMembers(&p);
        UA_FindServersResponse_deleteMembers(&r);
        responseType = requestType.identifier.numeric + 3;
        break;
    }

    case UA_NS0ID_CREATESESSIONREQUEST: {
        UA_CreateSessionRequest  p;
        UA_CreateSessionResponse r;
        if(UA_CreateSessionRequest_decodeBinary(msg, pos, &p))
            return;
        UA_CreateSessionResponse_init(&r);
        init_response_header(&p.requestHeader, &r.responseHeader);
        Service_CreateSession(server, clientChannel, &p, &r);
        ALLOC_MESSAGE(message, UA_CreateSessionResponse_calcSizeBinary(&r));
        UA_CreateSessionResponse_encodeBinary(&r, message, &sendOffset);
        UA_CreateSessionRequest_deleteMembers(&p);
        UA_CreateSessionResponse_deleteMembers(&r);
        responseType = requestType.identifier.numeric + 3;
        break;
    }

    case UA_NS0ID_ACTIVATESESSIONREQUEST: {
        UA_ActivateSessionRequest  p;
        UA_ActivateSessionResponse r;
        if(UA_ActivateSessionRequest_decodeBinary(msg, pos, &p))
            return;
        UA_ActivateSessionResponse_init(&r);
        init_response_header(&p.requestHeader, &r.responseHeader);
        Service_ActivateSession(server, clientChannel, &p, &r);
        ALLOC_MESSAGE(message, UA_ActivateSessionResponse_calcSizeBinary(&r));
        UA_ActivateSessionResponse_encodeBinary(&r, message, &sendOffset);
        UA_ActivateSessionRequest_deleteMembers(&p);
        UA_ActivateSessionResponse_deleteMembers(&r);
        responseType = requestType.identifier.numeric + 3;
        break;
    }

    case UA_NS0ID_CLOSESESSIONREQUEST: {
        UA_CloseSessionRequest  p;
        UA_CloseSessionResponse r;
        if(UA_CloseSessionRequest_decodeBinary(msg, pos, &p))
            return;
        UA_CloseSessionResponse_init(&r);
        init_response_header(&p.requestHeader, &r.responseHeader);
        Service_CloseSession(server, &p, &r);
        ALLOC_MESSAGE(message, UA_CloseSessionResponse_calcSizeBinary(&r));
        UA_CloseSessionResponse_encodeBinary(&r, message, &sendOffset);
        UA_CloseSessionRequest_deleteMembers(&p);
        UA_CloseSessionResponse_deleteMembers(&r);
        responseType = requestType.identifier.numeric + 3;
        break;
    }

    case UA_NS0ID_READREQUEST:
        INVOKE_SERVICE(Read);
        break;

    case UA_NS0ID_WRITEREQUEST:
        INVOKE_SERVICE(Write);
        break;

    case UA_NS0ID_BROWSEREQUEST:
        INVOKE_SERVICE(Browse);
        break;

    case UA_NS0ID_ADDREFERENCESREQUEST:
        INVOKE_SERVICE(AddReferences);
        break;

    case UA_NS0ID_REGISTERNODESREQUEST:
        INVOKE_SERVICE(RegisterNodes);
        break;

    case UA_NS0ID_UNREGISTERNODESREQUEST:
        INVOKE_SERVICE(UnregisterNodes);
        break;

    case UA_NS0ID_TRANSLATEBROWSEPATHSTONODEIDSREQUEST:
        INVOKE_SERVICE(TranslateBrowsePathsToNodeIds);
        break;

    default: {
        UA_LOG_INFO(server->logger, UA_LOGGERCATEGORY_COMMUNICATION, "Unknown request: NodeId(ns=%d, i=%d)",
                    requestType.namespaceIndex, requestType.identifier.numeric);

        UA_RequestHeader  p;
        UA_ResponseHeader r;
        if(UA_RequestHeader_decodeBinary(msg, pos, &p))
            return;
        UA_ResponseHeader_init(&r);
        init_response_header(&p, &r);
        r.serviceResult = UA_STATUSCODE_BADSERVICEUNSUPPORTED;
#ifdef EXTENSION_STATELESS
        if(retval != UA_STATUSCODE_GOOD)
            r.serviceResult = retval;
#endif
        ALLOC_MESSAGE(message, UA_ResponseHeader_calcSizeBinary(&r));
        UA_ResponseHeader_encodeBinary(&r, message, &sendOffset);
        UA_RequestHeader_deleteMembers(&p);
        UA_ResponseHeader_deleteMembers(&r);
        responseType = UA_NS0ID_RESPONSEHEADER + UA_ENCODINGOFFSET_BINARY;
        break;
    }
    }

    // 5) Build the header
    UA_SecureConversationMessageHeader respHeader;
    respHeader.messageHeader.messageTypeAndFinal = UA_MESSAGETYPEANDFINAL_MSGF;
    respHeader.messageHeader.messageSize = 0;
    respHeader.secureChannelId = clientChannel->securityToken.channelId;

    UA_SymmetricAlgorithmSecurityHeader symSecHeader;
    symSecHeader.tokenId = clientChannel->securityToken.tokenId;

    UA_SequenceHeader seqHeader;
    seqHeader.sequenceNumber = clientChannel->sequenceNumber;
    seqHeader.requestId = clientChannel->requestId;

    UA_NodeId response_nodeid = { .namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC,
        .identifier.numeric = responseType };

    UA_UInt32 headerSize =
        UA_SecureConversationMessageHeader_calcSizeBinary(&respHeader)
        + UA_SymmetricAlgorithmSecurityHeader_calcSizeBinary(&symSecHeader)
        + UA_SequenceHeader_calcSizeBinary(&seqHeader)
        + UA_NodeId_calcSizeBinary(&response_nodeid);

    *header = (UA_ByteString){ .length = headerSize, .data = UA_alloca(headerSize) };
    respHeader.messageHeader.messageSize = header->length + message->length;

    size_t rpos = 0;
    UA_SecureConversationMessageHeader_encodeBinary(&respHeader, header, &rpos);
    UA_SymmetricAlgorithmSecurityHeader_encodeBinary(&symSecHeader, header, &rpos);
    UA_SequenceHeader_encodeBinary(&seqHeader, header, &rpos);
    UA_NodeId_encodeBinary(&response_nodeid, header, &rpos);

    // todo: sign & encrypt

    // 6) Send it over the wire.
    UA_ByteStringArray responseBufArray;
    responseBufArray.strings = responseBufs;
    responseBufArray.stringsSize = 2;
    connection->write(connection, responseBufArray);

    if(!messageOnStack)
        UA_free(message->data);
}

static void processCLO(UA_Connection *connection, UA_Server *server, const UA_ByteString *msg,
                       size_t *pos) {
    UA_UInt32 secureChannelId;
    UA_StatusCode retval = UA_UInt32_decodeBinary(msg, pos, &secureChannelId);

    if(retval != UA_STATUSCODE_GOOD || !connection->channel ||
       connection->channel->securityToken.channelId != secureChannelId)
        return;

    Service_CloseSecureChannel(server, secureChannelId);
}

void UA_Server_processBinaryMessage(UA_Server *server, UA_Connection *connection, UA_ByteString *msg) {
    *msg = UA_Connection_completeMessages(connection, *msg);
    if(msg->length <= 0)
        return;

    size_t pos = 0;
    UA_TcpMessageHeader tcpMessageHeader;
    do {
        if(UA_TcpMessageHeader_decodeBinary(msg, &pos, &tcpMessageHeader) != UA_STATUSCODE_GOOD) {
            UA_LOG_INFO(server->logger, UA_LOGGERCATEGORY_COMMUNICATION, "Decoding of message header failed");
            connection->close(connection);
            break;
        }

        if(tcpMessageHeader.messageSize < 32)
            break; // there is no usefull message of that size

        size_t targetpos = pos - 8 + tcpMessageHeader.messageSize;

        switch(tcpMessageHeader.messageTypeAndFinal & 0xffffff) {
        case UA_MESSAGETYPEANDFINAL_HELF & 0xffffff:
            processHEL(connection, msg, &pos);
            break;
        case UA_MESSAGETYPEANDFINAL_OPNF & 0xffffff:
            processOPN(connection, server, msg, &pos);
            break;
        case UA_MESSAGETYPEANDFINAL_MSGF & 0xffffff:
#ifdef EXTENSION_STATELESS
            processMSG(connection, server, msg, &pos);
            break;
#endif
            if(connection->state != UA_CONNECTION_ESTABLISHED) {
                connection->close(connection);
                break;
            }
            processMSG(connection, server, msg, &pos);
            break;
        case UA_MESSAGETYPEANDFINAL_CLOF & 0xffffff:
            processCLO(connection, server, msg, &pos);
            connection->close(connection);
            return;
        }

        UA_TcpMessageHeader_deleteMembers(&tcpMessageHeader);
        if(pos != targetpos) {
            UA_LOG_INFO(server->logger, UA_LOGGERCATEGORY_COMMUNICATION,
                        "The message was not entirely processed, skipping to the end");
            pos = targetpos;
        }
    } while(msg->length > (UA_Int32)pos);
}

/* UA_Node */
static void UA_Node_init(UA_Node *p) {
	UA_NodeId_init(&p->nodeId);
	UA_NodeClass_init(&p->nodeClass);
	UA_QualifiedName_init(&p->browseName);
	UA_LocalizedText_init(&p->displayName);
	UA_LocalizedText_init(&p->description);
	UA_UInt32_init(&p->writeMask);
	UA_UInt32_init(&p->userWriteMask);
	p->referencesSize = -1;
	p->references = UA_NULL;
}

static void UA_Node_deleteMembers(UA_Node *p) {
	UA_NodeId_deleteMembers(&p->nodeId);
	UA_QualifiedName_deleteMembers(&p->browseName);
	UA_LocalizedText_deleteMembers(&p->displayName);
	UA_LocalizedText_deleteMembers(&p->description);
	UA_Array_delete(p->references, &UA_TYPES[UA_TYPES_REFERENCENODE], p->referencesSize);
}

static UA_StatusCode UA_Node_copy(const UA_Node *src, UA_Node *dst) {
	UA_StatusCode retval = UA_STATUSCODE_GOOD;
	UA_Node_init(dst);
	retval |= UA_NodeId_copy(&src->nodeId, &dst->nodeId);
	dst->nodeClass = src->nodeClass;
	retval |= UA_QualifiedName_copy(&src->browseName, &dst->browseName);
	retval |= UA_LocalizedText_copy(&src->displayName, &dst->displayName);
	retval |= UA_LocalizedText_copy(&src->description, &dst->description);
	dst->writeMask = src->writeMask;
	dst->userWriteMask = src->userWriteMask;
	dst->referencesSize = src->referencesSize;
	retval |= UA_Array_copy(src->references, (void**)&dst->references, &UA_TYPES[UA_TYPES_REFERENCENODE],
                            src->referencesSize);
	if(retval)
    	UA_Node_deleteMembers(dst);
	return retval;
}

/* UA_ObjectNode */
void UA_ObjectNode_init(UA_ObjectNode *p) {
	UA_Node_init((UA_Node*)p);
    p->nodeClass = UA_NODECLASS_OBJECT;
    p->eventNotifier = 0;
}

UA_ObjectNode * UA_ObjectNode_new(void) {
    UA_ObjectNode *p = (UA_ObjectNode*)UA_malloc(sizeof(UA_ObjectNode));
    if(p)
        UA_ObjectNode_init(p);
    return p;
}

void UA_ObjectNode_deleteMembers(UA_ObjectNode *p) {
    UA_Node_deleteMembers((UA_Node*)p);
}

void UA_ObjectNode_delete(UA_ObjectNode *p) {
    UA_ObjectNode_deleteMembers(p);
    UA_free(p);
}

UA_StatusCode UA_ObjectNode_copy(const UA_ObjectNode *src, UA_ObjectNode *dst) {
    dst->eventNotifier = src->eventNotifier;
	return UA_Node_copy((const UA_Node*)src, (UA_Node*)dst);
}

/* UA_ObjectTypeNode */
void UA_ObjectTypeNode_init(UA_ObjectTypeNode *p) {
	UA_Node_init((UA_Node*)p);
    p->nodeClass = UA_NODECLASS_OBJECTTYPE;
    p->isAbstract = UA_FALSE;
}

UA_ObjectTypeNode * UA_ObjectTypeNode_new(void) {
    UA_ObjectTypeNode *p = (UA_ObjectTypeNode*)UA_malloc(sizeof(UA_ObjectTypeNode));
    if(p)
        UA_ObjectTypeNode_init(p);
    return p;
}

void UA_ObjectTypeNode_deleteMembers(UA_ObjectTypeNode *p) {
    UA_Node_deleteMembers((UA_Node*)p);
}

void UA_ObjectTypeNode_delete(UA_ObjectTypeNode *p) {
    UA_ObjectTypeNode_deleteMembers(p);
    UA_free(p);
}

UA_StatusCode UA_ObjectTypeNode_copy(const UA_ObjectTypeNode *src, UA_ObjectTypeNode *dst) {
    dst->isAbstract = src->isAbstract;
	return UA_Node_copy((const UA_Node*)src, (UA_Node*)dst);
}

/* UA_VariableNode */
void UA_VariableNode_init(UA_VariableNode *p) {
	UA_Node_init((UA_Node*)p);
    p->nodeClass = UA_NODECLASS_VARIABLE;
    p->valueSource = UA_VALUESOURCE_VARIANT;
    UA_Variant_init(&p->value.variant);
    p->valueRank = -2; // scalar or array of any dimension
    p->accessLevel = 0;
    p->userAccessLevel = 0;
    p->minimumSamplingInterval = 0.0;
    p->historizing = UA_FALSE;
}

UA_VariableNode * UA_VariableNode_new(void) {
    UA_VariableNode *p = (UA_VariableNode*)UA_malloc(sizeof(UA_VariableNode));
    if(p)
        UA_VariableNode_init(p);
    return p;
}

void UA_VariableNode_deleteMembers(UA_VariableNode *p) {
    UA_Node_deleteMembers((UA_Node*)p);
    if(p->valueSource == UA_VALUESOURCE_VARIANT)
        UA_Variant_deleteMembers(&p->value.variant);
}

void UA_VariableNode_delete(UA_VariableNode *p) {
    UA_VariableNode_deleteMembers(p);
    UA_free(p);
}

UA_StatusCode UA_VariableNode_copy(const UA_VariableNode *src, UA_VariableNode *dst) {
    UA_VariableNode_init(dst);
	UA_StatusCode retval = UA_Node_copy((const UA_Node*)src, (UA_Node*)dst);
    dst->valueRank = src->valueRank;
    dst->valueSource = src->valueSource;
    if(src->valueSource == UA_VALUESOURCE_VARIANT)
        retval = UA_Variant_copy(&src->value.variant, &dst->value.variant);
    else
        dst->value.dataSource = src->value.dataSource;
    if(retval) {
        UA_VariableNode_deleteMembers(dst);
        return retval;
    }
    dst->accessLevel = src->accessLevel;
    dst->userAccessLevel = src->accessLevel;
    dst->minimumSamplingInterval = src->minimumSamplingInterval;
    dst->historizing = src->historizing;
    return UA_STATUSCODE_GOOD;
}

/* UA_VariableTypeNode */
void UA_VariableTypeNode_init(UA_VariableTypeNode *p) {
	UA_Node_init((UA_Node*)p);
    p->nodeClass = UA_NODECLASS_VARIABLETYPE;
    p->valueSource = UA_VALUESOURCE_VARIANT;
    UA_Variant_init(&p->value.variant);
    p->valueRank = -2; // scalar or array of any dimension
    p->isAbstract = UA_FALSE;
}

UA_VariableTypeNode * UA_VariableTypeNode_new(void) {
    UA_VariableTypeNode *p = (UA_VariableTypeNode*)UA_malloc(sizeof(UA_VariableTypeNode));
    if(p)
        UA_VariableTypeNode_init(p);
    return p;
}

void UA_VariableTypeNode_deleteMembers(UA_VariableTypeNode *p) {
    UA_Node_deleteMembers((UA_Node*)p);
    if(p->valueSource == UA_VALUESOURCE_VARIANT)
        UA_Variant_deleteMembers(&p->value.variant);
}

void UA_VariableTypeNode_delete(UA_VariableTypeNode *p) {
    UA_VariableTypeNode_deleteMembers(p);
    UA_free(p);
}

UA_StatusCode UA_VariableTypeNode_copy(const UA_VariableTypeNode *src, UA_VariableTypeNode *dst) {
    UA_VariableTypeNode_init(dst);
	UA_StatusCode retval = UA_Node_copy((const UA_Node*)src, (UA_Node*)dst);
    dst->valueRank = src->valueRank;
    dst->valueSource = src->valueSource;
    if(src->valueSource == UA_VALUESOURCE_VARIANT)
        UA_Variant_copy(&src->value.variant, &dst->value.variant);
    else
        dst->value.dataSource = src->value.dataSource;
    if(retval) {
        UA_VariableTypeNode_deleteMembers(dst);
        return retval;
    }
    dst->isAbstract = src->isAbstract;
    return UA_STATUSCODE_GOOD;
}

/* UA_ReferenceTypeNode */
void UA_ReferenceTypeNode_init(UA_ReferenceTypeNode *p) {
	UA_Node_init((UA_Node*)p);
    p->nodeClass = UA_NODECLASS_REFERENCETYPE;
    p->isAbstract = UA_FALSE;
    p->symmetric = UA_FALSE;
    UA_LocalizedText_init(&p->inverseName);
}

UA_ReferenceTypeNode * UA_ReferenceTypeNode_new(void) {
    UA_ReferenceTypeNode *p = (UA_ReferenceTypeNode*)UA_malloc(sizeof(UA_ReferenceTypeNode));
    if(p)
        UA_ReferenceTypeNode_init(p);
    return p;
}

void UA_ReferenceTypeNode_deleteMembers(UA_ReferenceTypeNode *p) {
    UA_Node_deleteMembers((UA_Node*)p);
    UA_LocalizedText_deleteMembers(&p->inverseName);
}

void UA_ReferenceTypeNode_delete(UA_ReferenceTypeNode *p) {
    UA_ReferenceTypeNode_deleteMembers(p);
    UA_free(p);
}

UA_StatusCode UA_ReferenceTypeNode_copy(const UA_ReferenceTypeNode *src, UA_ReferenceTypeNode *dst) {
	UA_StatusCode retval = UA_Node_copy((const UA_Node*)src, (UA_Node*)dst);
    if(retval)
        return retval;
    retval = UA_LocalizedText_copy(&src->inverseName, &dst->inverseName);
    if(retval) {
        UA_ReferenceTypeNode_deleteMembers(dst);
        return retval;
    }
    dst->isAbstract = src->isAbstract;
    dst->symmetric = src->symmetric;
    return UA_STATUSCODE_GOOD;
}

/* UA_MethodNode */
void UA_MethodNode_init(UA_MethodNode *p) {
	UA_Node_init((UA_Node*)p);
    p->nodeClass = UA_NODECLASS_METHOD;
    p->executable = UA_FALSE;
    p->userExecutable = UA_FALSE;
}

UA_MethodNode * UA_MethodNode_new(void) {
    UA_MethodNode *p = (UA_MethodNode*)UA_malloc(sizeof(UA_MethodNode));
    if(p)
        UA_MethodNode_init(p);
    return p;
}

void UA_MethodNode_deleteMembers(UA_MethodNode *p) {
    UA_Node_deleteMembers((UA_Node*)p);
}

void UA_MethodNode_delete(UA_MethodNode *p) {
    UA_MethodNode_deleteMembers(p);
    UA_free(p);
}

UA_StatusCode UA_MethodNode_copy(const UA_MethodNode *src, UA_MethodNode *dst) {
    dst->executable = src->executable;
    dst->userExecutable = src->userExecutable;
	return UA_Node_copy((const UA_Node*)src, (UA_Node*)dst);
}

/* UA_ViewNode */
void UA_ViewNode_init(UA_ViewNode *p) {
	UA_Node_init((UA_Node*)p);
    p->nodeClass = UA_NODECLASS_VIEW;
    p->containsNoLoops = UA_FALSE;
    p->eventNotifier = 0;
}

UA_ViewNode * UA_ViewNode_new(void) {
    UA_ViewNode *p = UA_malloc(sizeof(UA_ViewNode));
    if(p)
        UA_ViewNode_init(p);
    return p;
}

void UA_ViewNode_deleteMembers(UA_ViewNode *p) {
    UA_Node_deleteMembers((UA_Node*)p);
}

void UA_ViewNode_delete(UA_ViewNode *p) {
    UA_ViewNode_deleteMembers(p);
    UA_free(p);
}

UA_StatusCode UA_ViewNode_copy(const UA_ViewNode *src, UA_ViewNode *dst) {
    dst->containsNoLoops = src->containsNoLoops;
    dst->eventNotifier = src->eventNotifier;
	return UA_Node_copy((const UA_Node*)src, (UA_Node*)dst);
}

/* UA_DataTypeNode */
void UA_DataTypeNode_init(UA_DataTypeNode *p) {
	UA_Node_init((UA_Node*)p);
    p->nodeClass = UA_NODECLASS_DATATYPE;
    p->isAbstract = UA_FALSE;
}

UA_DataTypeNode * UA_DataTypeNode_new(void) {
    UA_DataTypeNode *p = UA_malloc(sizeof(UA_DataTypeNode));
    if(p)
        UA_DataTypeNode_init(p);
    return p;
}

void UA_DataTypeNode_deleteMembers(UA_DataTypeNode *p) {
    UA_Node_deleteMembers((UA_Node*)p);
}

void UA_DataTypeNode_delete(UA_DataTypeNode *p) {
    UA_DataTypeNode_deleteMembers(p);
    UA_free(p);
}

UA_StatusCode UA_DataTypeNode_copy(const UA_DataTypeNode *src, UA_DataTypeNode *dst) {
    dst->isAbstract = src->isAbstract;
	return UA_Node_copy((const UA_Node*)src, (UA_Node*)dst);
}

/**
 * There are three types of work:
 *
 * 1. Ordinary WorkItems (that are dispatched to worker threads if
 *    multithreading is activated)
 *
 * 2. Timed work that is executed at a precise date (with an optional repetition
 *    interval)
 *
 * 3. Delayed work that is executed at a later time when it is guaranteed that
 *    all previous work has actually finished (only for multithreading)
 */

#define MAXTIMEOUT 50000 // max timeout in usec until the next main loop iteration
#define BATCHSIZE 20 // max size of worklists that are dispatched to workers

static void processWork(UA_Server *server, UA_WorkItem *work, UA_Int32 workSize) {
    for(UA_Int32 i = 0;i<workSize;i++) {
        UA_WorkItem *item = &work[i];
        switch(item->type) {
        case UA_WORKITEMTYPE_BINARYNETWORKMESSAGE:
            UA_Server_processBinaryMessage(server, item->work.binaryNetworkMessage.connection,
                                           &item->work.binaryNetworkMessage.message);
            UA_free(item->work.binaryNetworkMessage.message.data);
            break;

        case UA_WORKITEMTYPE_METHODCALL:
        case UA_WORKITEMTYPE_DELAYEDMETHODCALL:
            item->work.methodCall.method(server, item->work.methodCall.data);
            break;

        default:
            break;
        }
    }
}

/*******************************/
/* Worker Threads and Dispatch */
/*******************************/

#ifdef UA_MULTITHREADING

/** Entry in the dipatch queue */
struct workListNode {
    struct cds_wfcq_node node; // node for the queue
    UA_UInt32 workSize;
    UA_WorkItem *work;
};

/** Dispatch work to workers. Slices the work up if it contains more than
    BATCHSIZE items. The work array is freed by the worker threads. */
static void dispatchWork(UA_Server *server, UA_Int32 workSize, UA_WorkItem *work) {
    UA_Int32 startIndex = workSize; // start at the end
    while(workSize > 0) {
        UA_Int32 size = BATCHSIZE;
        if(size > workSize)
            size = workSize;
        startIndex = startIndex - size;
        struct workListNode *wln = UA_malloc(sizeof(struct workListNode));
        if(startIndex > 0) {
            UA_WorkItem *workSlice = UA_malloc(size * sizeof(UA_WorkItem));
            UA_memcpy(workSlice, &work[startIndex], size * sizeof(UA_WorkItem));
            *wln = (struct workListNode){.workSize = size, .work = workSlice};
        }
        else {
            // do not alloc, but forward the original array
            *wln = (struct workListNode){.workSize = size, .work = work};
        }
        cds_wfcq_node_init(&wln->node);
        cds_wfcq_enqueue(&server->dispatchQueue_head, &server->dispatchQueue_tail, &wln->node);
        workSize -= size;
    } 
}

// throwaway struct to bring data into the worker threads
struct workerStartData {
    UA_Server *server;
    UA_UInt32 **workerCounter;
};

/** Waits until work arrives in the dispatch queue (restart after 10ms) and
    processes it. */
static void * workerLoop(struct workerStartData *startInfo) {
   	rcu_register_thread();
    UA_UInt32 *c = UA_malloc(sizeof(UA_UInt32));
    uatomic_set(c, 0);

    *startInfo->workerCounter = c;
    UA_Server *server = startInfo->server;
    UA_free(startInfo);
    
    pthread_mutex_t mutex; // required for the condition variable
    pthread_mutex_init(&mutex,0);
    pthread_mutex_lock(&mutex);
    struct timespec to;

    while(*server->running) {
        struct workListNode *wln = (struct workListNode*)
            cds_wfcq_dequeue_blocking(&server->dispatchQueue_head, &server->dispatchQueue_tail);
        if(wln) {
            processWork(server, wln->work, wln->workSize);
            UA_free(wln->work);
            UA_free(wln);
        } else {
            clock_gettime(CLOCK_REALTIME, &to);
            to.tv_sec += 2;
            pthread_cond_timedwait(&server->dispatchQueue_condition, &mutex, &to);
        }
        uatomic_inc(c); // increase the workerCounter;
    }
    pthread_mutex_unlock(&mutex);
    pthread_mutex_destroy(&mutex);
   	rcu_unregister_thread();
    return UA_NULL;
}

static void emptyDispatchQueue(UA_Server *server) {
    while(!cds_wfcq_empty(&server->dispatchQueue_head, &server->dispatchQueue_tail)) {
        struct workListNode *wln = (struct workListNode*)
            cds_wfcq_dequeue_blocking(&server->dispatchQueue_head, &server->dispatchQueue_tail);
        processWork(server, wln->work, wln->workSize);
        UA_free(wln->work);
        UA_free(wln);
    }
}

#endif

/**************/
/* Timed Work */
/**************/

struct UA_TimedWork {
    LIST_ENTRY(UA_TimedWork) pointers;
    UA_UInt16 workSize;
    UA_WorkItem *work;
    UA_Guid *workIds;
    UA_DateTime time;
    UA_UInt32 repetitionInterval; // in 100ns resolution, 0 means no repetition
};

/* The item is copied and not freed by this function. */
static UA_StatusCode addTimedWork(UA_Server *server, const UA_WorkItem *item, UA_DateTime firstTime,
                                  UA_UInt32 repetitionInterval, UA_Guid *resultWorkGuid) {
    UA_TimedWork *tw, *lastTw = UA_NULL;

    // search for matching entry
    LIST_FOREACH(tw, &server->timedWork, pointers) {
        if(tw->repetitionInterval == repetitionInterval &&
           (repetitionInterval > 0 || tw->time == firstTime))
            break; // found a matching entry
        if(tw->time > firstTime) {
            tw = UA_NULL; // not matchin entry exists
            lastTw = tw;
            break;
        }
    }
    
    if(tw) {
        // append to matching entry
        tw->workSize++;
        UA_WorkItem *biggerWorkArray = UA_realloc(tw->work, sizeof(UA_WorkItem)*tw->workSize);
        if(!biggerWorkArray)
            return UA_STATUSCODE_BADOUTOFMEMORY;
        tw->work = biggerWorkArray;
        UA_Guid *biggerWorkIds = UA_realloc(tw->workIds, sizeof(UA_Guid)*tw->workSize);
        if(!biggerWorkIds)
            return UA_STATUSCODE_BADOUTOFMEMORY;
        tw->workIds = biggerWorkIds;
        tw->work[tw->workSize-1] = *item;
        tw->workIds[tw->workSize-1] = UA_Guid_random(&server->random_seed);
        if(resultWorkGuid)
            *resultWorkGuid = tw->workIds[tw->workSize-1];
        return UA_STATUSCODE_GOOD;
    }

    // create a new entry
    if(!(tw = UA_malloc(sizeof(UA_TimedWork))))
        return UA_STATUSCODE_BADOUTOFMEMORY;
    if(!(tw->work = UA_malloc(sizeof(UA_WorkItem)))) {
        UA_free(tw);
        return UA_STATUSCODE_BADOUTOFMEMORY;
    }
    if(!(tw->workIds = UA_malloc(sizeof(UA_Guid)))) {
        UA_free(tw->work);
        UA_free(tw);
        return UA_STATUSCODE_BADOUTOFMEMORY;
    }

    tw->workSize = 1;
    tw->time = firstTime;
    tw->repetitionInterval = repetitionInterval;
    tw->work[0] = *item;
    tw->workIds[0] = UA_Guid_random(&server->random_seed);
    if(lastTw)
        LIST_INSERT_AFTER(lastTw, tw, pointers);
    else
        LIST_INSERT_HEAD(&server->timedWork, tw, pointers);

    if(resultWorkGuid)
        *resultWorkGuid = tw->workIds[0];

    return UA_STATUSCODE_GOOD;
}

// Currently, these functions need to get the server mutex, but should be sufficiently fast
UA_StatusCode UA_Server_addTimedWorkItem(UA_Server *server, const UA_WorkItem *work, UA_DateTime executionTime,
                                         UA_Guid *resultWorkGuid) {
    return addTimedWork(server, work, executionTime, 0, resultWorkGuid);
}

UA_StatusCode UA_Server_addRepeatedWorkItem(UA_Server *server, const UA_WorkItem *work, UA_UInt32 interval,
                                            UA_Guid *resultWorkGuid) {
    return addTimedWork(server, work, UA_DateTime_now() + interval, interval, resultWorkGuid);
}

/** Dispatches timed work, returns the timeout until the next timed work in ms */
static UA_UInt16 processTimedWork(UA_Server *server) {
    UA_DateTime current = UA_DateTime_now();
    UA_TimedWork *next = LIST_FIRST(&server->timedWork);
    UA_TimedWork *tw = UA_NULL;

    while(next) {
        tw = next;
        if(tw->time > current)
            break;
        next = LIST_NEXT(tw, pointers);

#ifdef UA_MULTITHREADING
        if(tw->repetitionInterval > 0) {
            // copy the entry and insert at the new location
            UA_WorkItem *workCopy = UA_malloc(sizeof(UA_WorkItem) * tw->workSize);
            UA_memcpy(workCopy, tw->work, sizeof(UA_WorkItem) * tw->workSize);
            dispatchWork(server, tw->workSize, workCopy); // frees the work pointer
            tw->time += tw->repetitionInterval;

            UA_TimedWork *prevTw = tw; // after which tw do we insert?
            while(UA_TRUE) {
                UA_TimedWork *n = LIST_NEXT(prevTw, pointers);
                if(!n || n->time > tw->time)
                    break;
                prevTw = n;
            }
            if(prevTw != tw) {
                LIST_REMOVE(tw, pointers);
                LIST_INSERT_AFTER(prevTw, tw, pointers);
            }
        } else {
            dispatchWork(server, tw->workSize, tw->work); // frees the work pointer
            LIST_REMOVE(tw, pointers);
            UA_free(tw->workIds);
            UA_free(tw);
        }
#else
        // 1) Process the work since it is past its due date
        processWork(server, tw->work, tw->workSize); // does not free the work

        // 2) If the work is repeated, add it back into the list. Otherwise remove it.
        if(tw->repetitionInterval > 0) {
            tw->time += tw->repetitionInterval;
            UA_TimedWork *prevTw = tw;
            while(UA_TRUE) {
                UA_TimedWork *n = LIST_NEXT(prevTw, pointers);
                if(!n || n->time > tw->time)
                    break;
                prevTw = n;
            }
            if(prevTw != tw) {
                LIST_REMOVE(tw, pointers);
                LIST_INSERT_AFTER(prevTw, tw, pointers);
            }
        } else {
            LIST_REMOVE(tw, pointers);
            UA_free(tw->work);
            UA_free(tw->workIds);
            UA_free(tw);
        }
#endif
    }

    // check if the next timed work is sooner than the usual timeout
    UA_TimedWork *first = LIST_FIRST(&server->timedWork);
    UA_UInt16 timeout = MAXTIMEOUT;
    if(first) {
        timeout = (first->time - current)/10;
        if(timeout > MAXTIMEOUT)
            timeout = MAXTIMEOUT;
    }
    return timeout;
}

void UA_Server_deleteTimedWork(UA_Server *server) {
    UA_TimedWork *current;
    UA_TimedWork *next = LIST_FIRST(&server->timedWork);
    while(next) {
        current = next;
        next = LIST_NEXT(current, pointers);
        LIST_REMOVE(current, pointers);
        UA_free(current->work);
        UA_free(current->workIds);
        UA_free(current);
    }
}

/****************/
/* Delayed Work */
/****************/

#ifdef UA_MULTITHREADING

#define DELAYEDWORKSIZE 100 // Collect delayed work until we have DELAYEDWORKSIZE items

struct UA_DelayedWork {
    UA_DelayedWork *next;
    UA_UInt32 *workerCounters; // initially UA_NULL until a workitem gets the counters
    UA_UInt32 workItemsCount; // the size of the array is DELAYEDWORKSIZE, the count may be less
    UA_WorkItem *workItems; // when it runs full, a new delayedWork entry is created
};

// Dispatched as a methodcall-WorkItem when the delayedwork is added
static void getCounters(UA_Server *server, UA_DelayedWork *delayed) {
    UA_UInt32 *counters = UA_malloc(server->nThreads * sizeof(UA_UInt32));
    for(UA_UInt16 i = 0;i<server->nThreads;i++)
        counters[i] = *server->workerCounters[i];
    delayed->workerCounters = counters;
}

// Call from the main thread only. This is the only function that modifies
// server->delayedWork. processDelayedWorkQueue modifies the "next" (after the
// head).
static void addDelayedWork(UA_Server *server, UA_WorkItem work) {
    UA_DelayedWork *dw = server->delayedWork;
    if(!dw || dw->workItemsCount >= DELAYEDWORKSIZE) {
        UA_DelayedWork *newwork = UA_malloc(sizeof(UA_DelayedWork));
        newwork->workItems = UA_malloc(sizeof(UA_WorkItem)*DELAYEDWORKSIZE);
        newwork->workItemsCount = 0;
        newwork->workerCounters = UA_NULL;
        newwork->next = server->delayedWork;

        // dispatch a method that sets the counter
        if(dw && dw->workItemsCount >= DELAYEDWORKSIZE) {
            UA_WorkItem *setCounter = UA_malloc(sizeof(UA_WorkItem));
            *setCounter = (UA_WorkItem)
                {.type = UA_WORKITEMTYPE_METHODCALL,
                 .work.methodCall = {.method = (void (*)(UA_Server*, void*))getCounters, .data = dw}};
            dispatchWork(server, 1, setCounter);
        }

        server->delayedWork = newwork;
        dw = newwork;
    }
    dw->workItems[dw->workItemsCount] = work;
    dw->workItemsCount++;
}

static void processDelayedWork(UA_Server *server) {
    UA_DelayedWork *dw = server->delayedWork;
    while(dw) {
        processWork(server, dw->workItems, dw->workItemsCount);
        UA_DelayedWork *next = dw->next;
        UA_free(dw->workerCounters);
        UA_free(dw->workItems);
        UA_free(dw);
        dw = next;
    }
}

// Execute this every N seconds (repeated work) to execute delayed work that is ready
static void dispatchDelayedWork(UA_Server *server, void *data /* not used, but needed for the signature*/) {
    UA_DelayedWork *dw = UA_NULL;
    UA_DelayedWork *readydw = UA_NULL;
    UA_DelayedWork *beforedw = server->delayedWork;

    // start at the second...
    if(beforedw)
        dw = beforedw->next;

    // find the first delayedwork where the counters are set and have been moved
    while(dw) {
        if(!dw->workerCounters) {
            beforedw = dw;
            dw = dw->next;
            continue;
        }

        UA_Boolean countersMoved = UA_TRUE;
        for(UA_UInt16 i=0;i<server->nThreads;i++) {
            if(*server->workerCounters[i] == dw->workerCounters[i])
                countersMoved = UA_FALSE;
                break;
        }
        
        if(countersMoved) {
            readydw = uatomic_xchg(&beforedw->next, UA_NULL);
            break;
        } else {
            beforedw = dw;
            dw = dw->next;
        }
    }

    // we have a ready entry. all afterwards are also ready
    while(readydw) {
        dispatchWork(server, readydw->workItemsCount, readydw->workItems);
        beforedw = readydw;
        readydw = readydw->next;
        UA_free(beforedw->workerCounters);
        UA_free(beforedw);
    }
}

#endif

/********************/
/* Main Server Loop */
/********************/

UA_StatusCode UA_Server_run(UA_Server *server, UA_UInt16 nThreads, UA_Boolean *running) {
#ifdef UA_MULTITHREADING
    // 1) Prepare the threads
    server->running = running; // the threads need to access the variable
    server->nThreads = nThreads;
    pthread_cond_init(&server->dispatchQueue_condition, 0);
    pthread_t *thr = UA_malloc(nThreads * sizeof(pthread_t));
    server->workerCounters = UA_malloc(nThreads * sizeof(UA_UInt32 *));
    for(UA_UInt32 i=0;i<nThreads;i++) {
        struct workerStartData *startData = UA_malloc(sizeof(struct workerStartData));
        startData->server = server;
        startData->workerCounter = &server->workerCounters[i];
        pthread_create(&thr[i], UA_NULL, (void* (*)(void*))workerLoop, startData);
    }

    UA_WorkItem processDelayed = {.type = UA_WORKITEMTYPE_METHODCALL,
                                  .work.methodCall = {.method = dispatchDelayedWork,
                                                      .data = UA_NULL} };
    UA_Server_addRepeatedWorkItem(server, &processDelayed, 10000000, UA_NULL);
#endif

    // 2a) Start the networklayers
    for(UA_Int32 i=0;i<server->nlsSize;i++)
        server->nls[i].start(server->nls[i].nlHandle, &server->logger);

    // 2b) Init server's meta-information
    //fill startTime
    server->startTime = UA_DateTime_now();

    //fill build date
    {
		static struct tm ct;

		ct.tm_year = (__DATE__[7] - '0') * 1000 +  (__DATE__[8] - '0') * 100 + (__DATE__[9] - '0') * 10 + (__DATE__[10] - '0')- 1900;

		if (0) ;
		else if ((__DATE__[0]=='J') && (__DATE__[1]=='a') && (__DATE__[2]=='n')) ct.tm_mon = 1-1;
		else if ((__DATE__[0]=='F') && (__DATE__[1]=='e') && (__DATE__[2]=='b')) ct.tm_mon = 2-1;
		else if ((__DATE__[0]=='M') && (__DATE__[1]=='a') && (__DATE__[2]=='r')) ct.tm_mon = 3-1;
		else if ((__DATE__[0]=='A') && (__DATE__[1]=='p') && (__DATE__[2]=='r')) ct.tm_mon = 4-1;
		else if ((__DATE__[0]=='M') && (__DATE__[1]=='a') && (__DATE__[2]=='y')) ct.tm_mon = 5-1;
		else if ((__DATE__[0]=='J') && (__DATE__[1]=='u') && (__DATE__[2]=='n')) ct.tm_mon = 6-1;
		else if ((__DATE__[0]=='J') && (__DATE__[1]=='u') && (__DATE__[2]=='l')) ct.tm_mon = 7-1;
		else if ((__DATE__[0]=='A') && (__DATE__[1]=='u') && (__DATE__[2]=='g')) ct.tm_mon = 8-1;
		else if ((__DATE__[0]=='S') && (__DATE__[1]=='e') && (__DATE__[2]=='p')) ct.tm_mon = 9-1;
		else if ((__DATE__[0]=='O') && (__DATE__[1]=='c') && (__DATE__[2]=='t')) ct.tm_mon = 10-1;
		else if ((__DATE__[0]=='N') && (__DATE__[1]=='o') && (__DATE__[2]=='v')) ct.tm_mon = 11-1;
		else if ((__DATE__[0]=='D') && (__DATE__[1]=='e') && (__DATE__[2]=='c')) ct.tm_mon = 12-1;

		// special case to handle __DATE__ not inserting leading zero on day of month
		// if Day of month is less than 10 - it inserts a blank character
		// this results in a negative number for tm_mday

		if(__DATE__[4] == ' ')
		{
			ct.tm_mday =  __DATE__[5]-'0';
		}
		else
		{
			ct.tm_mday = (__DATE__[4]-'0')*10 + (__DATE__[5]-'0');
		}

		ct.tm_hour = ((__TIME__[0] - '0') * 10 + __TIME__[1] - '0');
		ct.tm_min = ((__TIME__[3] - '0') * 10 + __TIME__[4] - '0');
		ct.tm_sec = ((__TIME__[6] - '0') * 10 + __TIME__[7] - '0');

		ct.tm_isdst = -1;  // information is not available.

		//FIXME: next 3 lines are copy-pasted from ua_types.c
		#define UNIX_EPOCH_BIAS_SEC 11644473600LL // Number of seconds from 1 Jan. 1601 00:00 to 1 Jan 1970 00:00 UTC
		#define HUNDRED_NANOSEC_PER_USEC 10LL
		#define HUNDRED_NANOSEC_PER_SEC (HUNDRED_NANOSEC_PER_USEC * 1000000LL)
		server->buildDate = (mktime(&ct) + UNIX_EPOCH_BIAS_SEC) * HUNDRED_NANOSEC_PER_SEC;
    }

    //3) The loop
    while(1) {
        // 3.1) Process timed work
        UA_UInt16 timeout = processTimedWork(server);

        // 3.2) Get work from the networklayer and dispatch it
        for(UA_Int32 i=0;i<server->nlsSize;i++) {
            UA_ServerNetworkLayer *nl = &server->nls[i];
            UA_WorkItem *work;
            UA_Int32 workSize;
            if(*running) {
            	if(i == server->nlsSize-1)
            		workSize = nl->getWork(nl->nlHandle, &work, timeout);
            	else
            		workSize = nl->getWork(nl->nlHandle, &work, 0);
            } else {
                workSize = server->nls[i].stop(nl->nlHandle, &work);
            }

#ifdef UA_MULTITHREADING
            // Filter out delayed work
            for(UA_Int32 k=0;k<workSize;k++) {
                if(work[k].type != UA_WORKITEMTYPE_DELAYEDMETHODCALL)
                    continue;
                addDelayedWork(server, work[k]);
                work[k].type = UA_WORKITEMTYPE_NOTHING;
            }
            dispatchWork(server, workSize, work);
            if(workSize > 0)
                pthread_cond_broadcast(&server->dispatchQueue_condition); 
#else
            processWork(server, work, workSize);
            if(workSize > 0)
                UA_free(work);
#endif
        }

        // 3.3) Exit?
        if(!*running)
            break;
    }

#ifdef UA_MULTITHREADING
    // 4) Clean up: Wait until all worker threads finish, then empty the
    // dispatch queue, then process the remaining delayed work
    for(UA_UInt32 i=0;i<nThreads;i++) {
        pthread_join(thr[i], UA_NULL);
        UA_free(server->workerCounters[i]);
    }
    UA_free(server->workerCounters);
    UA_free(thr);
    emptyDispatchQueue(server);
    processDelayedWork(server);
#endif

    return UA_STATUSCODE_GOOD;
}

typedef struct channel_list_entry {
    UA_SecureChannel channel;
    LIST_ENTRY(channel_list_entry) pointers;
} channel_list_entry;

UA_StatusCode UA_SecureChannelManager_init(UA_SecureChannelManager *cm, UA_UInt32 maxChannelCount,
                                           UA_UInt32 tokenLifetime, UA_UInt32 startChannelId,
                                           UA_UInt32 startTokenId) {
    LIST_INIT(&cm->channels);
    cm->lastChannelId      = startChannelId;
    cm->lastTokenId        = startTokenId;
    cm->maxChannelLifetime = tokenLifetime;
    cm->maxChannelCount    = maxChannelCount;
    return UA_STATUSCODE_GOOD;
}

void UA_SecureChannelManager_deleteMembers(UA_SecureChannelManager *cm) {
    channel_list_entry *current, *next = LIST_FIRST(&cm->channels);
    while(next) {
        current = next;
        next = LIST_NEXT(current, pointers);
        LIST_REMOVE(current, pointers);
        if(current->channel.session)
            current->channel.session->channel = UA_NULL;
        if(current->channel.connection)
            current->channel.connection->channel = UA_NULL;
        UA_SecureChannel_deleteMembers(&current->channel);
        UA_free(current);
    }
}

UA_StatusCode UA_SecureChannelManager_open(UA_SecureChannelManager *cm, UA_Connection *conn,
                                           const UA_OpenSecureChannelRequest *request,
                                           UA_OpenSecureChannelResponse *response)
{
    switch(request->securityMode) {
    case UA_MESSAGESECURITYMODE_INVALID:
        response->responseHeader.serviceResult = UA_STATUSCODE_BADSECURITYMODEREJECTED;
        return response->responseHeader.serviceResult;

        // fall through and handle afterwards
    /* case UA_MESSAGESECURITYMODE_NONE: */
    /*     UA_ByteString_copy(&request->clientNonce, &entry->channel.clientNonce); */
    /*     break; */

    case UA_MESSAGESECURITYMODE_SIGN:
    case UA_MESSAGESECURITYMODE_SIGNANDENCRYPT:
        response->responseHeader.serviceResult = UA_STATUSCODE_BADSECURITYMODEREJECTED;
        return response->responseHeader.serviceResult;

    default:
        // do nothing
        break;
    }

    channel_list_entry *entry = UA_malloc(sizeof(channel_list_entry));
    if(!entry) return UA_STATUSCODE_BADOUTOFMEMORY;
    UA_SecureChannel_init(&entry->channel);

    response->responseHeader.stringTableSize = 0;
    response->responseHeader.timestamp       = UA_DateTime_now();

    entry->channel.connection = conn;
    conn->channel = &entry->channel;
    entry->channel.securityToken.channelId       = cm->lastChannelId++;
    entry->channel.securityToken.tokenId         = cm->lastTokenId++;
    entry->channel.securityToken.createdAt       = UA_DateTime_now();
    entry->channel.securityToken.revisedLifetime = (request->requestedLifetime > cm->maxChannelLifetime) ?
                                                   cm->maxChannelLifetime : request->requestedLifetime;

    UA_ByteString_copy(&request->clientNonce, &entry->channel.clientNonce);
    entry->channel.serverAsymAlgSettings.securityPolicyUri =
        UA_STRING_ALLOC("http://opcfoundation.org/UA/SecurityPolicy#None");
    LIST_INSERT_HEAD(&cm->channels, entry, pointers);

    response->serverProtocolVersion = 0;
    UA_SecureChannel_generateNonce(&entry->channel.serverNonce);
    UA_ByteString_copy(&entry->channel.serverNonce, &response->serverNonce);
    UA_ChannelSecurityToken_copy(&entry->channel.securityToken, &response->securityToken);
    conn->channel = &entry->channel;

    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_SecureChannelManager_renew(UA_SecureChannelManager *cm, UA_Connection *conn,
                                            const UA_OpenSecureChannelRequest *request,
                                            UA_OpenSecureChannelResponse *response)
{
    UA_SecureChannel *channel = conn->channel;
    if(channel == UA_NULL)
        return UA_STATUSCODE_BADINTERNALERROR;

    channel->securityToken.tokenId         = cm->lastTokenId++;
    channel->securityToken.createdAt       = UA_DateTime_now(); // todo: is wanted?
    channel->securityToken.revisedLifetime = (request->requestedLifetime > cm->maxChannelLifetime) ?
                                             cm->maxChannelLifetime : request->requestedLifetime;

    if(channel->serverNonce.data != UA_NULL)
        UA_ByteString_deleteMembers(&channel->serverNonce);
    UA_SecureChannel_generateNonce(&channel->serverNonce);
    UA_ByteString_copy(&channel->serverNonce, &response->serverNonce);
    UA_ChannelSecurityToken_copy(&channel->securityToken, &response->securityToken);

    return UA_STATUSCODE_GOOD;
}

UA_SecureChannel * UA_SecureChannelManager_get(UA_SecureChannelManager *cm, UA_UInt32 channelId) {
    channel_list_entry *entry;
    LIST_FOREACH(entry, &cm->channels, pointers) {
        if(entry->channel.securityToken.channelId == channelId)
            return &entry->channel;
    }
    return UA_NULL;
}

UA_StatusCode UA_SecureChannelManager_close(UA_SecureChannelManager *cm, UA_UInt32 channelId) {
    // TODO lock access
    // TODO: close the binaryconnection if it is still open. So we do not have stray pointers..
    channel_list_entry *entry;
    LIST_FOREACH(entry, &cm->channels, pointers) {
        if(entry->channel.securityToken.channelId == channelId) {
            if(entry->channel.connection)
                entry->channel.connection->channel = UA_NULL; // remove pointer back to the channel
            if(entry->channel.session)
                entry->channel.session->channel = UA_NULL; // remove ponter back to the channel
            UA_SecureChannel_deleteMembers(&entry->channel);
            LIST_REMOVE(entry, pointers);
            UA_free(entry);
            return UA_STATUSCODE_GOOD;
        }
    }
    //TODO notify server application that secureChannel has been closed part 6 - §7.1.4
    return UA_STATUSCODE_BADINTERNALERROR;
}

/**
 The functions in this file are not thread-safe. For multi-threaded access, a
 second implementation should be provided. See for example, how a nodestore
 implementation is choosen based on whether multithreading is enabled or not.
 */

typedef struct session_list_entry {
    UA_Session session;
    LIST_ENTRY(session_list_entry) pointers;
} session_list_entry;

UA_StatusCode UA_SessionManager_init(UA_SessionManager *sessionManager, UA_UInt32 maxSessionCount,
                                    UA_UInt32 maxSessionLifeTime, UA_UInt32 startSessionId) {
    LIST_INIT(&sessionManager->sessions);
    sessionManager->maxSessionCount = maxSessionCount;
    sessionManager->lastSessionId   = startSessionId;
    sessionManager->maxSessionLifeTime  = maxSessionLifeTime;
    sessionManager->currentSessionCount = 0;
    return UA_STATUSCODE_GOOD;
}

void UA_SessionManager_deleteMembers(UA_SessionManager *sessionManager) {
    session_list_entry *current, *next = LIST_FIRST(&sessionManager->sessions);
    while(next) {
        current = next;
        next = LIST_NEXT(current, pointers);
        LIST_REMOVE(current, pointers);
        if(current->session.channel)
            current->session.channel->session = UA_NULL; // the channel is no longer attached to a session
        UA_Session_deleteMembers(&current->session);
        UA_free(current);
    }
}

UA_StatusCode
UA_SessionManager_getSessionById(UA_SessionManager *sessionManager, const UA_NodeId *sessionId,
                                 UA_Session **session)
{
    if(sessionManager == UA_NULL) {
        *session = UA_NULL;
        return UA_STATUSCODE_BADINTERNALERROR;
    }

    session_list_entry *current = UA_NULL;
    LIST_FOREACH(current, &sessionManager->sessions, pointers) {
        if(UA_NodeId_equal(&current->session.sessionId, sessionId))
            break;
    }

    if(!current) {
        *session = UA_NULL;
        return UA_STATUSCODE_BADINTERNALERROR;
    }

    // Lifetime handling is not done here, but in a regular cleanup by the
    // server. If the session still exists, then it is valid.
    *session = &current->session;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode
UA_SessionManager_getSessionByToken(UA_SessionManager *sessionManager, const UA_NodeId *token,
                                    UA_Session **session)
{
    if(sessionManager == UA_NULL) {
        *session = UA_NULL;
        return UA_STATUSCODE_BADINTERNALERROR;
    }

    session_list_entry *current = UA_NULL;
    LIST_FOREACH(current, &sessionManager->sessions, pointers) {
        if(UA_NodeId_equal(&current->session.authenticationToken, token))
            break;
    }

    if(!current) {
        *session = UA_NULL;
        return UA_STATUSCODE_BADINTERNALERROR;
    }

    // Lifetime handling is not done here, but in a regular cleanup by the
    // server. If the session still exists, then it is valid.
    *session = &current->session;
    return UA_STATUSCODE_GOOD;
}

/** Creates and adds a session. */
UA_StatusCode
UA_SessionManager_createSession(UA_SessionManager *sessionManager, UA_SecureChannel *channel,
                                const UA_CreateSessionRequest *request, UA_Session **session)
{
    if(sessionManager->currentSessionCount >= sessionManager->maxSessionCount)
        return UA_STATUSCODE_BADTOOMANYSESSIONS;

    session_list_entry *newentry = UA_malloc(sizeof(session_list_entry));
    if(!newentry)
        return UA_STATUSCODE_BADOUTOFMEMORY;

    UA_Session_init(&newentry->session);
    newentry->session.sessionId = UA_NODEID_NUMERIC(1, sessionManager->lastSessionId++);
    newentry->session.authenticationToken = UA_NODEID_NUMERIC(1, sessionManager->lastSessionId);
    newentry->session.channel = channel;
    newentry->session.timeout =
        (request->requestedSessionTimeout <= sessionManager->maxSessionLifeTime &&
         request->requestedSessionTimeout>0) ?
        request->requestedSessionTimeout : sessionManager->maxSessionLifeTime;
    UA_Session_setExpirationDate(&newentry->session);

    sessionManager->currentSessionCount++;
    LIST_INSERT_HEAD(&sessionManager->sessions, newentry, pointers);
    *session = &newentry->session;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode
UA_SessionManager_removeSession(UA_SessionManager *sessionManager, const UA_NodeId *sessionId)
{
    session_list_entry *current = UA_NULL;
    LIST_FOREACH(current, &sessionManager->sessions, pointers) {
        if(UA_NodeId_equal(&current->session.sessionId, sessionId))
            break;
    }

    if(!current)
        return UA_STATUSCODE_BADINTERNALERROR;

    LIST_REMOVE(current, pointers);
    if(current->session.channel)
        current->session.channel->session = UA_NULL; // the channel is no longer attached to a session
    UA_Session_deleteMembers(&current->session);
    UA_free(current);
    return UA_STATUSCODE_GOOD;
}

void Service_FindServers(UA_Server *server, const UA_FindServersRequest *request, UA_FindServersResponse *response) {
    response->servers = UA_malloc(sizeof(UA_ApplicationDescription));
    if(!response->servers) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;
        return;
    }
    if(UA_ApplicationDescription_copy(&server->description, response->servers) != UA_STATUSCODE_GOOD) {
        UA_free(response->servers);
        response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;
        return;
    }
    UA_String_deleteMembers(response->servers->discoveryUrls);
    UA_String_copy(&request->endpointUrl, response->servers->discoveryUrls);
	response->serversSize = 1;
}

void Service_GetEndpoints(UA_Server *server, const UA_GetEndpointsRequest *request, UA_GetEndpointsResponse *response) {
    /* test if the supported binary profile shall be returned */
    UA_Boolean relevant_endpoints[server->endpointDescriptionsSize];
    size_t relevant_count = 0;
    for(UA_Int32 j = 0; j < server->endpointDescriptionsSize; j++) {
        relevant_endpoints[j] = UA_FALSE;
        if(request->profileUrisSize <= 0) {
            relevant_endpoints[j] = UA_TRUE;
            relevant_count++;
            continue;
        }
        for(UA_Int32 i = 0; i < request->profileUrisSize; i++) {
            if(UA_String_equal(&request->profileUris[i], &server->endpointDescriptions->transportProfileUri)) {
                relevant_endpoints[j] = UA_TRUE;
                relevant_count++;
                break;
            }
        }
    }

    if(relevant_count == 0) {
        response->endpointsSize = 0;
        return;
    }

    response->endpoints = UA_malloc(sizeof(UA_EndpointDescription) * relevant_count);
    if(!response->endpoints) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;
        return;
    }

    size_t k = 0;
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    for(UA_Int32 j = 0; j < server->endpointDescriptionsSize && retval == UA_STATUSCODE_GOOD; j++) {
        if(relevant_endpoints[j] != UA_TRUE)
            continue;
        retval = UA_copy(&server->endpointDescriptions[j], &response->endpoints[j],
                         &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION]);
        UA_String_deleteMembers(&response->endpoints[j].endpointUrl);
        UA_String_copy(&request->endpointUrl, &response->endpoints[j].endpointUrl);
        k++;
    }

    if(retval != UA_STATUSCODE_GOOD) {
        response->responseHeader.serviceResult = retval;
        UA_Array_delete(response->endpoints, &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION], --k);
        return;
    }
    response->endpointsSize = relevant_count;
}


void Service_OpenSecureChannel(UA_Server *server, UA_Connection *connection,
                               const UA_OpenSecureChannelRequest *request,
                               UA_OpenSecureChannelResponse *response) {
    // todo: if(request->clientProtocolVersion != protocolVersion)
    if(request->requestType == UA_SECURITYTOKENREQUESTTYPE_ISSUE)
        UA_SecureChannelManager_open(&server->secureChannelManager, connection, request, response);
    else
        UA_SecureChannelManager_renew(&server->secureChannelManager, connection, request, response);
}

void Service_CloseSecureChannel(UA_Server *server, UA_Int32 channelId) {
	//Sten: this service is a bit assymmetric to OpenSecureChannel since CloseSecureChannelRequest does not contain any indormation
    UA_SecureChannelManager_close(&server->secureChannelManager, channelId);
    // 62451 Part 6 Chapter 7.1.4 - The server does not send a CloseSecureChannel response
}

void Service_CreateSession(UA_Server *server, UA_SecureChannel *channel,
                           const UA_CreateSessionRequest *request,
                           UA_CreateSessionResponse *response) {

    response->serverEndpoints = UA_malloc(sizeof(UA_EndpointDescription));
    if(!response->serverEndpoints || (response->responseHeader.serviceResult =
        UA_EndpointDescription_copy(server->endpointDescriptions, response->serverEndpoints)) !=
       UA_STATUSCODE_GOOD)
        return;
    response->serverEndpointsSize = 1;

    // creates a session and adds a pointer to the channel. Only when the
    // session is activated will the channel point to the session as well
	UA_Session *newSession;
    response->responseHeader.serviceResult = UA_SessionManager_createSession(&server->sessionManager,
                                                                             channel, request, &newSession);
	if(response->responseHeader.serviceResult != UA_STATUSCODE_GOOD)
		return;

    //TODO get maxResponseMessageSize internally
    newSession->maxResponseMessageSize = request->maxResponseMessageSize;
    response->sessionId = newSession->sessionId;
    response->revisedSessionTimeout = newSession->timeout;
    response->authenticationToken = newSession->authenticationToken;
    response->responseHeader.serviceResult = UA_String_copy(&request->sessionName, &newSession->sessionName);
    if(server->endpointDescriptions)
        response->responseHeader.serviceResult |=
            UA_ByteString_copy(&server->endpointDescriptions->serverCertificate, &response->serverCertificate);
    if(response->responseHeader.serviceResult != UA_STATUSCODE_GOOD) {
        UA_SessionManager_removeSession(&server->sessionManager, &newSession->sessionId);
         return;
    }
}

void Service_ActivateSession(UA_Server *server,UA_SecureChannel *channel,
                             const UA_ActivateSessionRequest *request,
                             UA_ActivateSessionResponse *response) {
    // make the channel know about the session
	UA_Session *foundSession;
	UA_SessionManager_getSessionByToken(&server->sessionManager,
                                        (const UA_NodeId*)&request->requestHeader.authenticationToken,
                                        &foundSession);

	if(foundSession == UA_NULL)
        response->responseHeader.serviceResult = UA_STATUSCODE_BADIDENTITYTOKENINVALID;
    else
        channel->session = foundSession;
}

void Service_CloseSession(UA_Server *server, const UA_CloseSessionRequest *request,
                              UA_CloseSessionResponse *response) {
	UA_Session *foundSession;
	UA_SessionManager_getSessionByToken(&server->sessionManager,
			(const UA_NodeId*)&request->requestHeader.authenticationToken, &foundSession);

	if(foundSession == UA_NULL){
		response->responseHeader.serviceResult = UA_STATUSCODE_BADIDENTITYTOKENINVALID;
		return;
	}

	if(UA_SessionManager_removeSession(&server->sessionManager, &foundSession->sessionId) == UA_STATUSCODE_GOOD)
		response->responseHeader.serviceResult = UA_STATUSCODE_GOOD;
	else
		response->responseHeader.serviceResult = UA_STATUSCODE_BADSECURECHANNELIDINVALID;
}

static UA_StatusCode
parse_numericrange(const UA_String str, UA_NumericRange *range)
{
    if(str.length < 0 || str.length >= 1023)
        return UA_STATUSCODE_BADINTERNALERROR;
    char *cstring = UA_alloca(str.length+1);
    UA_memcpy(cstring, str.data, str.length);
    cstring[str.length] = 0;
    UA_Int32 index = 0;
    size_t dimensionsIndex = 0;
    size_t dimensionsMax = 3; // more should be uncommon
    struct UA_NumericRangeDimension *dimensions = UA_malloc(sizeof(struct UA_NumericRangeDimension) * 3);
    if(!dimensions)
        return UA_STATUSCODE_BADOUTOFMEMORY;
    
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    do {
        UA_Int32 min, max;
        UA_Int32 progress;
        UA_Int32 res = sscanf(&cstring[index], "%" SCNu32 "%n", &min, &progress);
        if(res <= 0 || min < 0) {
            retval = UA_STATUSCODE_BADINDEXRANGEINVALID;
            break;
        }
        index += progress;
        if(index >= str.length || cstring[index] == ',')
            max = min;
        else {
            res = sscanf(&cstring[index], ":%" SCNu32 "%n", &max, &progress);
            if(res <= 0 || max < 0 || min >= max) {
                retval = UA_STATUSCODE_BADINDEXRANGEINVALID;
                break;
            }
            index += progress;
        }
        
        if(dimensionsIndex >= dimensionsMax) {
            struct UA_NumericRangeDimension *newDimensions =
                UA_realloc(dimensions, sizeof(struct UA_NumericRangeDimension) * 2 * dimensionsMax);
            if(!newDimensions) {
                UA_free(dimensions);
                return UA_STATUSCODE_BADOUTOFMEMORY;
            }
            dimensions = newDimensions;
            dimensionsMax *= 2;
        }

        dimensions[dimensionsIndex].min = min;
        dimensions[dimensionsIndex].max = max;
        dimensionsIndex++;
    } while(retval == UA_STATUSCODE_GOOD && index + 1 < str.length && cstring[index] == ',' && ++index);

    if(retval != UA_STATUSCODE_GOOD) {
        UA_free(dimensions);
        return retval;
    }
        
    range->dimensions = dimensions;
    range->dimensionsSize = dimensionsIndex;
    return retval;
}

#define CHECK_NODECLASS(CLASS)                                  \
    if(!(node->nodeClass & (CLASS))) {                          \
        v->hasStatus = UA_TRUE;                                 \
        v->status = UA_STATUSCODE_BADATTRIBUTEIDINVALID;        \
        break;                                                  \
    }

static void handleServerTimestamps(UA_TimestampsToReturn timestamps, UA_DataValue* v) {
	if (v && (timestamps == UA_TIMESTAMPSTORETURN_SERVER
			|| timestamps == UA_TIMESTAMPSTORETURN_BOTH)) {
		v->hasServerTimestamp = UA_TRUE;
		v->serverTimestamp = UA_DateTime_now();
	}
}

static void handleSourceTimestamps(UA_TimestampsToReturn timestamps, UA_DataValue* v) {
	if(timestamps == UA_TIMESTAMPSTORETURN_SOURCE || timestamps == UA_TIMESTAMPSTORETURN_BOTH) {
		v->hasSourceTimestamp = UA_TRUE;
		v->sourceTimestamp = UA_DateTime_now();
	}
}

/** Reads a single attribute from a node in the nodestore. */
static void readValue(UA_Server *server, UA_TimestampsToReturn timestamps,
                      const UA_ReadValueId *id, UA_DataValue *v) {

	if(id->dataEncoding.name.length >= 0){
		if(memcmp(id->dataEncoding.name.data, "DefaultBinary", 13) != 0 &&
           memcmp(id->dataEncoding.name.data, "DefaultXml", 10) != 0) {
			v->hasStatus = UA_TRUE;
			v->status = UA_STATUSCODE_BADDATAENCODINGINVALID;
			return;
		}
	}

	//index range for a non-value
	if(id->indexRange.length >= 0 && id->attributeId != UA_ATTRIBUTEID_VALUE){
		v->hasStatus = UA_TRUE;
		v->status = UA_STATUSCODE_BADINDEXRANGENODATA;
		return;
	}

    UA_Node const *node = UA_NodeStore_get(server->nodestore, &(id->nodeId));
    if(!node) {
        v->hasStatus = UA_TRUE;
        v->status = UA_STATUSCODE_BADNODEIDUNKNOWN;
        return;
    }

    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    switch(id->attributeId) {
    case UA_ATTRIBUTEID_NODEID:
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &node->nodeId, &UA_TYPES[UA_TYPES_NODEID]);
        break;
    case UA_ATTRIBUTEID_NODECLASS:
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &node->nodeClass, &UA_TYPES[UA_TYPES_INT32]);
        break;
    case UA_ATTRIBUTEID_BROWSENAME:
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &node->browseName, &UA_TYPES[UA_TYPES_QUALIFIEDNAME]);
        break;
    case UA_ATTRIBUTEID_DISPLAYNAME:
        retval |= UA_Variant_setScalarCopy(&v->value, &node->displayName, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
        if(retval == UA_STATUSCODE_GOOD)
            v->hasValue = UA_TRUE;
        break;
    case UA_ATTRIBUTEID_DESCRIPTION:
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &node->description, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
        break;
    case UA_ATTRIBUTEID_WRITEMASK:
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &node->writeMask, &UA_TYPES[UA_TYPES_UINT32]);
        break;
    case UA_ATTRIBUTEID_USERWRITEMASK:
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &node->userWriteMask, &UA_TYPES[UA_TYPES_UINT32]);
        break;
    case UA_ATTRIBUTEID_ISABSTRACT:
        CHECK_NODECLASS(UA_NODECLASS_REFERENCETYPE | UA_NODECLASS_OBJECTTYPE | UA_NODECLASS_VARIABLETYPE |
                        UA_NODECLASS_DATATYPE);
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &((const UA_ReferenceTypeNode *)node)->isAbstract,
                                          &UA_TYPES[UA_TYPES_BOOLEAN]);
        break;
    case UA_ATTRIBUTEID_SYMMETRIC:
        CHECK_NODECLASS(UA_NODECLASS_REFERENCETYPE);
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &((const UA_ReferenceTypeNode *)node)->symmetric,
                                          &UA_TYPES[UA_TYPES_BOOLEAN]);
        break;
    case UA_ATTRIBUTEID_INVERSENAME:
        CHECK_NODECLASS(UA_NODECLASS_REFERENCETYPE);
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &((const UA_ReferenceTypeNode *)node)->inverseName,
                                          &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
        break;
    case UA_ATTRIBUTEID_CONTAINSNOLOOPS:
        CHECK_NODECLASS(UA_NODECLASS_VIEW);
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &((const UA_ViewNode *)node)->containsNoLoops,
                                          &UA_TYPES[UA_TYPES_BOOLEAN]);
        break;
    case UA_ATTRIBUTEID_EVENTNOTIFIER:
        CHECK_NODECLASS(UA_NODECLASS_VIEW | UA_NODECLASS_OBJECT);
        v->hasValue = UA_TRUE;
        if(node->nodeClass == UA_NODECLASS_VIEW){
        	retval |= UA_Variant_setScalarCopy(&v->value, &((const UA_ViewNode *)node)->eventNotifier,
                                          	  &UA_TYPES[UA_TYPES_BYTE]);
        } else {
        	retval |= UA_Variant_setScalarCopy(&v->value, &((const UA_ObjectNode *)node)->eventNotifier,
                                              &UA_TYPES[UA_TYPES_BYTE]);
        }
        break;

    case UA_ATTRIBUTEID_VALUE:
        CHECK_NODECLASS(UA_NODECLASS_VARIABLE | UA_NODECLASS_VARIABLETYPE);
        {
        	if(node->nodeClass != UA_NODECLASS_VARIABLE) {
    			v->hasValue = UA_FALSE;
    			handleSourceTimestamps(timestamps, v);
            }

            UA_Boolean hasRange = UA_FALSE;
            UA_NumericRange range;
            if(id->indexRange.length > 0) {
                retval = parse_numericrange(id->indexRange, &range);
                if(retval != UA_STATUSCODE_GOOD)
                    break;
                hasRange = UA_TRUE;
            }

            const UA_VariableNode *vn = (const UA_VariableNode*)node;
            if(vn->valueSource == UA_VALUESOURCE_VARIANT) {
                if(hasRange)
                    retval |= UA_Variant_copyRange(&vn->value.variant, &v->value, range);
                else
                    retval |= UA_Variant_copy(&vn->value.variant, &v->value);
                if(retval == UA_STATUSCODE_GOOD) {
                    v->hasValue = UA_TRUE;
                    handleSourceTimestamps(timestamps, v);
                }
            } else {
                UA_DataValue val;
                UA_DataValue_init(&val);
                UA_Boolean sourceTimeStamp = (timestamps == UA_TIMESTAMPSTORETURN_SOURCE ||
                                              timestamps == UA_TIMESTAMPSTORETURN_BOTH);
                retval |= vn->value.dataSource.read(vn->value.dataSource.handle, sourceTimeStamp, &val);
                if(retval == UA_STATUSCODE_GOOD) {
                    retval |= UA_DataValue_copy(&val, v); // todo: selection of indexranges
                }
                vn->value.dataSource.release(vn->value.dataSource.handle, &val);
            }

            if(hasRange)
                UA_free(range.dimensions);
        }
        break;

    case UA_ATTRIBUTEID_DATATYPE: {
		CHECK_NODECLASS(UA_NODECLASS_VARIABLE | UA_NODECLASS_VARIABLETYPE);
        const UA_VariableNode *vn = (const UA_VariableNode*)node;
        if(vn->valueSource == UA_VALUESOURCE_VARIANT)
            retval = UA_Variant_setScalarCopy(&v->value, &vn->value.variant.type->typeId,
                                              &UA_TYPES[UA_TYPES_NODEID]);
        else {
            UA_DataValue val;
            UA_DataValue_init(&val);
            retval = vn->value.dataSource.read(vn->value.dataSource.handle, UA_FALSE, &val);
            if(retval != UA_STATUSCODE_GOOD)
                break;
            retval = UA_Variant_setScalarCopy(&v->value, &val.value.type->typeId, &UA_TYPES[UA_TYPES_NODEID]);
            vn->value.dataSource.release(vn->value.dataSource.handle, &val);
        }
        if(retval == UA_STATUSCODE_GOOD)
            v->hasValue = UA_TRUE;
        }
        break;

    case UA_ATTRIBUTEID_VALUERANK:
        CHECK_NODECLASS(UA_NODECLASS_VARIABLE | UA_NODECLASS_VARIABLETYPE);
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &((const UA_VariableTypeNode *)node)->valueRank,
                                          &UA_TYPES[UA_TYPES_INT32]);
        break;

    case UA_ATTRIBUTEID_ARRAYDIMENSIONS:
        CHECK_NODECLASS(UA_NODECLASS_VARIABLE | UA_NODECLASS_VARIABLETYPE);
        {
            const UA_VariableNode *vn = (const UA_VariableNode *)node;
            if(vn->valueSource == UA_VALUESOURCE_VARIANT) {
                retval = UA_Variant_setArrayCopy(&v->value, vn->value.variant.arrayDimensions,
                                                 vn->value.variant.arrayDimensionsSize,
                                                 &UA_TYPES[UA_TYPES_INT32]);
                if(retval == UA_STATUSCODE_GOOD)
                    v->hasValue = UA_TRUE;
            } else {
                UA_DataValue val;
                UA_DataValue_init(&val);
                retval |= vn->value.dataSource.read(vn->value.dataSource.handle, UA_FALSE, &val);
                if(retval != UA_STATUSCODE_GOOD)
                    break;
                if(!val.hasValue)
                    retval = UA_STATUSCODE_BADNOTREADABLE;
                else
                    retval = UA_Variant_setArrayCopy(&v->value, val.value.arrayDimensions,
                                                     val.value.arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);
                vn->value.dataSource.release(vn->value.dataSource.handle, &val);
            }
        }
        break;

    case UA_ATTRIBUTEID_ACCESSLEVEL:
        CHECK_NODECLASS(UA_NODECLASS_VARIABLE);
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &((const UA_VariableNode *)node)->accessLevel,
                                          &UA_TYPES[UA_TYPES_BYTE]);
        break;

    case UA_ATTRIBUTEID_USERACCESSLEVEL:
        CHECK_NODECLASS(UA_NODECLASS_VARIABLE);
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &((const UA_VariableNode *)node)->userAccessLevel,
                                          &UA_TYPES[UA_TYPES_BYTE]);
        break;

    case UA_ATTRIBUTEID_MINIMUMSAMPLINGINTERVAL:
        CHECK_NODECLASS(UA_NODECLASS_VARIABLE);
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &((const UA_VariableNode *)node)->minimumSamplingInterval,
                                          &UA_TYPES[UA_TYPES_DOUBLE]);
        break;

    case UA_ATTRIBUTEID_HISTORIZING:
        CHECK_NODECLASS(UA_NODECLASS_VARIABLE);
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &((const UA_VariableNode *)node)->historizing,
                                          &UA_TYPES[UA_TYPES_BOOLEAN]);
        break;

    case UA_ATTRIBUTEID_EXECUTABLE:
        CHECK_NODECLASS(UA_NODECLASS_METHOD);
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &((const UA_MethodNode *)node)->executable,
                                          &UA_TYPES[UA_TYPES_BOOLEAN]);
        break;

    case UA_ATTRIBUTEID_USEREXECUTABLE:
        CHECK_NODECLASS(UA_NODECLASS_METHOD);
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &((const UA_MethodNode *)node)->userExecutable,
                                          &UA_TYPES[UA_TYPES_BOOLEAN]);
        break;

    default:
        v->hasStatus = UA_TRUE;
        v->status = UA_STATUSCODE_BADATTRIBUTEIDINVALID;
        break;
    }

    UA_NodeStore_release(node);

    if(retval != UA_STATUSCODE_GOOD) {
        v->hasStatus = UA_TRUE;
        v->status = retval;
    }

    handleServerTimestamps(timestamps, v);
}

void Service_Read(UA_Server *server, UA_Session *session, const UA_ReadRequest *request,
                  UA_ReadResponse *response) {
    if(request->nodesToReadSize <= 0) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADNOTHINGTODO;
        return;
    }

    if(request->timestampsToReturn > 3){
    	response->responseHeader.serviceResult = UA_STATUSCODE_BADTIMESTAMPSTORETURNINVALID;
    	return;
    }

    size_t size = request->nodesToReadSize;

    response->results = UA_Array_new(&UA_TYPES[UA_TYPES_DATAVALUE], size);
    if(!response->results) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;
        return;
    }

    response->resultsSize = size;

    if(request->maxAge < 0) {
    	response->responseHeader.serviceResult = UA_STATUSCODE_BADMAXAGEINVALID;
        return;
    }

    /* ### Begin External Namespaces */
    UA_Boolean *isExternal = UA_alloca(sizeof(UA_Boolean) * size);
    UA_memset(isExternal, UA_FALSE, sizeof(UA_Boolean) * size);
    UA_UInt32 *indices = UA_alloca(sizeof(UA_UInt32) * size);
    for(UA_Int32 j = 0;j<server->externalNamespacesSize;j++) {
        size_t indexSize = 0;
        for(size_t i = 0;i < size;i++) {
            if(request->nodesToRead[i].nodeId.namespaceIndex != server->externalNamespaces[j].index)
                continue;
            isExternal[i] = UA_TRUE;
            indices[indexSize] = i;
            indexSize++;
        }
        if(indexSize == 0)
            continue;
        UA_ExternalNodeStore *ens = &server->externalNamespaces[j].externalNodeStore;
        ens->readNodes(ens->ensHandle, &request->requestHeader, request->nodesToRead,
                       indices, indexSize, response->results, UA_FALSE, response->diagnosticInfos);
    }
    /* ### End External Namespaces */

    for(size_t i = 0;i < size;i++) {
        if(!isExternal[i])
            readValue(server, request->timestampsToReturn, &request->nodesToRead[i], &response->results[i]);
    }

#ifdef EXTENSION_STATELESS
    if(session==&anonymousSession){
		/* expiry header */
		UA_ExtensionObject additionalHeader;
		UA_ExtensionObject_init(&additionalHeader);
		additionalHeader.encoding = UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISBYTESTRING;

		UA_Variant variant;
		UA_Variant_init(&variant);
		variant.type = &UA_TYPES[UA_TYPES_DATETIME];
		variant.arrayLength = request->nodesToReadSize;

		UA_DateTime* expireArray = UA_NULL;
		expireArray = UA_Array_new(&UA_TYPES[UA_TYPES_DATETIME], request->nodesToReadSize);
		variant.data = expireArray;

		UA_ByteString str;
		UA_ByteString_init(&str);

		/*expires in 20 seconds*/
		for(UA_Int32 i = 0;i < response->resultsSize;i++) {
			expireArray[i] = UA_DateTime_now() + 20 * 100 * 1000 * 1000;
		}
		size_t offset = 0;
		str.data = UA_malloc(UA_Variant_calcSizeBinary(&variant));
		str.length = UA_Variant_calcSizeBinary(&variant);
		UA_Variant_encodeBinary(&variant, &str, &offset);
		additionalHeader.body = str;
		response->responseHeader.additionalHeader = additionalHeader;
    }
#endif
}

static UA_StatusCode writeValue(UA_Server *server, UA_WriteValue *wvalue) {
    UA_StatusCode retval = UA_STATUSCODE_GOOD;

    /* is there a value at all */
    if(!wvalue->value.hasValue)
        return UA_STATUSCODE_BADTYPEMISMATCH;

    // we might repeat writing, e.g. when the node got replaced mid-work
    UA_Boolean done = UA_FALSE;
    while(!done) {
        const UA_Node *node = UA_NodeStore_get(server->nodestore, &wvalue->nodeId);
        if(!node)
            return UA_STATUSCODE_BADNODEIDUNKNOWN;

        switch(wvalue->attributeId) {
        case UA_ATTRIBUTEID_NODEID:
        case UA_ATTRIBUTEID_NODECLASS:
        case UA_ATTRIBUTEID_BROWSENAME:
        case UA_ATTRIBUTEID_DISPLAYNAME:
        case UA_ATTRIBUTEID_DESCRIPTION:
        case UA_ATTRIBUTEID_WRITEMASK:
        case UA_ATTRIBUTEID_USERWRITEMASK:
        case UA_ATTRIBUTEID_ISABSTRACT:
        case UA_ATTRIBUTEID_SYMMETRIC:
        case UA_ATTRIBUTEID_INVERSENAME:
        case UA_ATTRIBUTEID_CONTAINSNOLOOPS:
        case UA_ATTRIBUTEID_EVENTNOTIFIER:
            retval = UA_STATUSCODE_BADWRITENOTSUPPORTED;
            break;
        case UA_ATTRIBUTEID_VALUE: {
            if(node->nodeClass != UA_NODECLASS_VARIABLE &&
               node->nodeClass != UA_NODECLASS_VARIABLETYPE) {
                retval = UA_STATUSCODE_BADTYPEMISMATCH;
                break;
            }

            /* parse the range */
            UA_Boolean hasRange = UA_FALSE;
            UA_NumericRange range;
            if(wvalue->indexRange.length > 0) {
                retval = parse_numericrange(wvalue->indexRange, &range);
                if(retval != UA_STATUSCODE_GOOD)
                    break;
                hasRange = UA_TRUE;
            }

            /* the relevant members are similar for variables and variabletypes */
            const UA_VariableNode *vn = (const UA_VariableNode*)node;
            if(vn->valueSource == UA_VALUESOURCE_DATASOURCE) {
                if(!vn->value.dataSource.write) {
                    retval = UA_STATUSCODE_BADWRITENOTSUPPORTED;
                    break;
                }
                // todo: writing ranges
                retval = vn->value.dataSource.write(vn->value.dataSource.handle, &wvalue->value.value);
                done = UA_TRUE;
                break;
            }
            const UA_Variant *oldV = &vn->value.variant;

            /* the nodeid on the wire may be != the nodeid in the node: opaque types, enums and bytestrings */
            if(!UA_NodeId_equal(&oldV->type->typeId, &wvalue->value.value.type->typeId)) {
                if(oldV->type->namespaceZero && wvalue->value.value.type->namespaceZero &&
                   oldV->type->typeIndex == wvalue->value.value.type->typeIndex)
                    /* An enum was sent as an int32, or an opaque type as a bytestring. This is
                       detected with the typeIndex indicated the "true" datatype. */
                    wvalue->value.value.type = oldV->type;
                else if(oldV->type == &UA_TYPES[UA_TYPES_BYTE] &&
                        (!oldV->data || vn->value.variant.arrayLength > -1) /* isArray */ &&
                        wvalue->value.value.type == &UA_TYPES[UA_TYPES_BYTESTRING] &&
                        wvalue->value.value.data && wvalue->value.value.arrayLength == -1 /* isScalar */) {
                    /* a string is written to a byte array */
                    UA_ByteString *str = (UA_ByteString*) wvalue->value.value.data;
                    wvalue->value.value.arrayLength = str->length;
                    wvalue->value.value.data = str->data;
                    wvalue->value.value.type = &UA_TYPES[UA_TYPES_BYTE];
                    UA_free(str);
                } else {
                    retval = UA_STATUSCODE_BADTYPEMISMATCH;
                    break;
                }
            }

            /* copy the node */
            UA_VariableNode *newVn = (node->nodeClass == UA_NODECLASS_VARIABLE) ?
                UA_VariableNode_new() : (UA_VariableNode*)UA_VariableTypeNode_new();
            if(!newVn) {
                retval = UA_STATUSCODE_BADOUTOFMEMORY;
                break;
            }
            retval = (node->nodeClass == UA_NODECLASS_VARIABLE) ? UA_VariableNode_copy(vn, newVn) : 
                UA_VariableTypeNode_copy((const UA_VariableTypeNode*)vn, (UA_VariableTypeNode*)newVn);
            if(retval != UA_STATUSCODE_GOOD)
                goto clean_up;
                
            /* insert the new value*/
            if(hasRange)
                retval = UA_Variant_setRange(&newVn->value.variant, wvalue->value.value.data, range);
            else {
                UA_Variant_deleteMembers(&newVn->value.variant);
                retval = UA_Variant_copy(&wvalue->value.value, &newVn->value.variant);
            }
            if(retval != UA_STATUSCODE_GOOD ||
               UA_NodeStore_replace(server->nodestore, node, (UA_Node*)newVn,
                                    UA_NULL) != UA_STATUSCODE_GOOD)
                goto clean_up;
            if(hasRange)
                UA_free(range.dimensions);
            done = UA_TRUE;
            break;

            clean_up:
            if(node->nodeClass == UA_NODECLASS_VARIABLE)
                UA_VariableNode_delete(newVn);
            else
                UA_VariableTypeNode_delete((UA_VariableTypeNode*)newVn);
            if(hasRange)
                UA_free(range.dimensions);
            }
            break;
        case UA_ATTRIBUTEID_DATATYPE:
        case UA_ATTRIBUTEID_VALUERANK:
        case UA_ATTRIBUTEID_ARRAYDIMENSIONS:
        case UA_ATTRIBUTEID_ACCESSLEVEL:
        case UA_ATTRIBUTEID_USERACCESSLEVEL:
        case UA_ATTRIBUTEID_MINIMUMSAMPLINGINTERVAL:
        case UA_ATTRIBUTEID_HISTORIZING:
        case UA_ATTRIBUTEID_EXECUTABLE:
        case UA_ATTRIBUTEID_USEREXECUTABLE:
            retval = UA_STATUSCODE_BADWRITENOTSUPPORTED;
            break;
        default:
            retval = UA_STATUSCODE_BADATTRIBUTEIDINVALID;
            break;
        }

        UA_NodeStore_release(node);
        if(retval != UA_STATUSCODE_GOOD)
            break;
    }

    return retval;
}

void Service_Write(UA_Server *server, UA_Session *session,
                   const UA_WriteRequest *request, UA_WriteResponse *response) {
    UA_assert(server != UA_NULL && session != UA_NULL && request != UA_NULL && response != UA_NULL);

    if(request->nodesToWriteSize <= 0){
        response->responseHeader.serviceResult = UA_STATUSCODE_BADNOTHINGTODO;
        return;
    }

    response->results = UA_Array_new(&UA_TYPES[UA_TYPES_STATUSCODE], request->nodesToWriteSize);
    if(!response->results) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;
        return;
    }

    /* ### Begin External Namespaces */
    UA_Boolean *isExternal = UA_alloca(sizeof(UA_Boolean) * request->nodesToWriteSize);
    UA_memset(isExternal, UA_FALSE, sizeof(UA_Boolean)*request->nodesToWriteSize);
    UA_UInt32 *indices = UA_alloca(sizeof(UA_UInt32) * request->nodesToWriteSize);
    for(UA_Int32 j = 0;j<server->externalNamespacesSize;j++) {
        UA_UInt32 indexSize = 0;
        for(UA_Int32 i = 0;i < request->nodesToWriteSize;i++) {
            if(request->nodesToWrite[i].nodeId.namespaceIndex !=
               server->externalNamespaces[j].index)
                continue;
            isExternal[i] = UA_TRUE;
            indices[indexSize] = i;
            indexSize++;
        }
        if(indexSize == 0)
            continue;
        UA_ExternalNodeStore *ens = &server->externalNamespaces[j].externalNodeStore;
        ens->writeNodes(ens->ensHandle, &request->requestHeader, request->nodesToWrite,
                        indices, indexSize, response->results, response->diagnosticInfos);
    }
    /* ### End External Namespaces */
    
    response->resultsSize = request->nodesToWriteSize;
    for(UA_Int32 i = 0;i < request->nodesToWriteSize;i++) {
        if(!isExternal[i])
            response->results[i] = writeValue(server, &request->nodesToWrite[i]);
    }
}

#define COPY_STANDARDATTRIBUTES do {                                    \
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_DISPLAYNAME) {  \
        vnode->displayName = attr.displayName;                          \
        UA_LocalizedText_init(&attr.displayName);                       \
    }                                                                   \
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_DESCRIPTION) {  \
        vnode->description = attr.description;                          \
        UA_LocalizedText_init(&attr.description);                       \
    }                                                                   \
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_WRITEMASK)      \
        vnode->writeMask = attr.writeMask;                              \
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_USERWRITEMASK)  \
        vnode->userWriteMask = attr.userWriteMask;                      \
    } while(0)

static UA_StatusCode parseVariableNode(UA_ExtensionObject *attributes, UA_Node **new_node) {
    if(attributes->typeId.identifier.numeric !=
       UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES].typeId.identifier.numeric + UA_ENCODINGOFFSET_BINARY)
        return UA_STATUSCODE_BADNODEATTRIBUTESINVALID;

    UA_VariableAttributes attr;
    size_t pos = 0;
    // todo return more informative error codes from decodeBinary
    if(UA_VariableAttributes_decodeBinary(&attributes->body, &pos, &attr) != UA_STATUSCODE_GOOD)
        return UA_STATUSCODE_BADNODEATTRIBUTESINVALID;

    UA_VariableNode *vnode = UA_VariableNode_new();
    if(!vnode) {
        UA_VariableAttributes_deleteMembers(&attr);
        return UA_STATUSCODE_BADOUTOFMEMORY;
    }

    // now copy all the attributes. This potentially removes them from the decoded attributes.
    COPY_STANDARDATTRIBUTES;
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_ACCESSLEVEL)
        vnode->accessLevel = attr.accessLevel;
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_USERACCESSLEVEL)
        vnode->userAccessLevel = attr.userAccessLevel;
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_HISTORIZING)
        vnode->historizing = attr.historizing;
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_MINIMUMSAMPLINGINTERVAL)
        vnode->minimumSamplingInterval = attr.minimumSamplingInterval;
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_VALUERANK)
        vnode->valueRank = attr.valueRank;

    // don't use extra dimension spec. This comes from the value.
    /* if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_ARRAYDIMENSIONS) { */
    /*     vnode->arrayDimensionsSize = attr.arrayDimensionsSize; */
    /*     vnode->arrayDimensions = attr.arrayDimensions; */
    /*     attr.arrayDimensionsSize = -1; */
    /*     attr.arrayDimensions = UA_NULL; */
    /* } */

    // don't use the extra type id. This comes from the value.
    /* if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_DATATYPE || */
    /*    attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_OBJECTTYPEORDATATYPE) { */
    /*     vnode->dataType = attr.dataType; */
    /*     UA_NodeId_init(&attr.dataType); */
    /* } */

    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_VALUE) {
        vnode->value.variant = attr.value;
        UA_Variant_init(&attr.value);
    }

    UA_VariableAttributes_deleteMembers(&attr);

    *new_node = (UA_Node*)vnode;
    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode parseObjectNode(UA_ExtensionObject *attributes, UA_Node **new_node) {
    if(attributes->typeId.identifier.numeric !=
       UA_TYPES[UA_TYPES_OBJECTATTRIBUTES].typeId.identifier.numeric + UA_ENCODINGOFFSET_BINARY)
        return UA_STATUSCODE_BADNODEATTRIBUTESINVALID;

    UA_ObjectAttributes attr;
    size_t pos = 0;
    // todo return more informative error codes from decodeBinary
    if (UA_ObjectAttributes_decodeBinary(&attributes->body, &pos, &attr) != UA_STATUSCODE_GOOD)
        return UA_STATUSCODE_BADNODEATTRIBUTESINVALID;
    UA_ObjectNode *vnode = UA_ObjectNode_new();
    if(!vnode) {
        UA_ObjectAttributes_deleteMembers(&attr);
        return UA_STATUSCODE_BADOUTOFMEMORY;
    }

    // now copy all the attributes. This potentially removes them from the decoded attributes.
    COPY_STANDARDATTRIBUTES;
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_EVENTNOTIFIER)
        vnode->eventNotifier = attr.eventNotifier;
    UA_ObjectAttributes_deleteMembers(&attr);
    *new_node = (UA_Node*) vnode;
    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode parseReferenceTypeNode(UA_ExtensionObject *attributes, UA_Node **new_node) {
    UA_ReferenceTypeAttributes attr;
    size_t pos = 0;
    // todo return more informative error codes from decodeBinary
    if(UA_ReferenceTypeAttributes_decodeBinary(&attributes->body, &pos, &attr) != UA_STATUSCODE_GOOD)
        return UA_STATUSCODE_BADNODEATTRIBUTESINVALID;
    UA_ReferenceTypeNode *vnode = UA_ReferenceTypeNode_new();
    if(!vnode) {
        UA_ReferenceTypeAttributes_deleteMembers(&attr);
        return UA_STATUSCODE_BADOUTOFMEMORY;
    }

    // now copy all the attributes. This potentially removes them from the decoded attributes.
    COPY_STANDARDATTRIBUTES;
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_ISABSTRACT)
        vnode->isAbstract = attr.isAbstract;
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_SYMMETRIC)
        vnode->symmetric = attr.symmetric;
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_INVERSENAME) {
        vnode->inverseName = attr.inverseName;
        attr.inverseName.text.length = -1;
        attr.inverseName.text.data = UA_NULL;
        attr.inverseName.locale.length = -1;
        attr.inverseName.locale.data = UA_NULL;
    }
    UA_ReferenceTypeAttributes_deleteMembers(&attr);
    *new_node = (UA_Node*) vnode;
    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode parseObjectTypeNode(UA_ExtensionObject *attributes, UA_Node **new_node) {
    UA_ObjectTypeAttributes attr;
    size_t pos = 0;
    // todo return more informative error codes from decodeBinary
    if(UA_ObjectTypeAttributes_decodeBinary(&attributes->body, &pos, &attr) != UA_STATUSCODE_GOOD)
        return UA_STATUSCODE_BADNODEATTRIBUTESINVALID;
    UA_ObjectTypeNode *vnode = UA_ObjectTypeNode_new();
    if(!vnode) {
        UA_ObjectTypeAttributes_deleteMembers(&attr);
        return UA_STATUSCODE_BADOUTOFMEMORY;
    }
    
    // now copy all the attributes. This potentially removes them from the decoded attributes.
    COPY_STANDARDATTRIBUTES;
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_ISABSTRACT) {
        vnode->isAbstract = attr.isAbstract;
    }
    UA_ObjectTypeAttributes_deleteMembers(&attr);
    *new_node = (UA_Node*) vnode;
    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode parseViewNode(UA_ExtensionObject *attributes, UA_Node **new_node) {
    UA_ViewAttributes attr;
    size_t pos = 0;
    // todo return more informative error codes from decodeBinary
    if(UA_ViewAttributes_decodeBinary(&attributes->body, &pos, &attr) != UA_STATUSCODE_GOOD)
        return UA_STATUSCODE_BADNODEATTRIBUTESINVALID;
    UA_ViewNode *vnode = UA_ViewNode_new();
    if(!vnode) {
        UA_ViewAttributes_deleteMembers(&attr);
        return UA_STATUSCODE_BADOUTOFMEMORY;
    }
    // now copy all the attributes. This potentially removes them from the decoded attributes.
    COPY_STANDARDATTRIBUTES;
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_CONTAINSNOLOOPS)
        vnode->containsNoLoops = attr.containsNoLoops;
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_EVENTNOTIFIER)
        vnode->eventNotifier = attr.eventNotifier;
    UA_ViewAttributes_deleteMembers(&attr);
    *new_node = (UA_Node*) vnode;
    return UA_STATUSCODE_GOOD;
}

static void addNodeFromAttributes(UA_Server *server, UA_Session *session, UA_AddNodesItem *item,
                                  UA_AddNodesResult *result) {
    // adding nodes to ns0 is not allowed over the wire
    if(item->requestedNewNodeId.nodeId.namespaceIndex == 0) {
        result->statusCode = UA_STATUSCODE_BADNODEIDREJECTED;
        return;
    }

    // parse the node
    UA_Node *node = UA_NULL;

    switch (item->nodeClass) {
    case UA_NODECLASS_OBJECT:
        result->statusCode = parseObjectNode(&item->nodeAttributes, &node);
        break;
    case UA_NODECLASS_OBJECTTYPE:
        result->statusCode = parseObjectTypeNode(&item->nodeAttributes, &node);
        break;
    case UA_NODECLASS_REFERENCETYPE:
        result->statusCode = parseReferenceTypeNode(&item->nodeAttributes, &node);
        break;
    case UA_NODECLASS_VARIABLE:
        result->statusCode = parseVariableNode(&item->nodeAttributes, &node);
        break;
    default:
        result->statusCode = UA_STATUSCODE_BADNOTIMPLEMENTED;
    }

    if(result->statusCode != UA_STATUSCODE_GOOD)
        return;

    // add the node
    *result = UA_Server_addNodeWithSession(server, session, node, &item->parentNodeId,
                                           &item->referenceTypeId);
    if(result->statusCode != UA_STATUSCODE_GOOD) {
        switch (node->nodeClass) {
        case UA_NODECLASS_OBJECT:
            UA_ObjectNode_delete((UA_ObjectNode*)node);
            break;
        case UA_NODECLASS_OBJECTTYPE:
            UA_ObjectTypeNode_delete((UA_ObjectTypeNode*)node);
            break;
        case UA_NODECLASS_REFERENCETYPE:
            UA_ReferenceTypeNode_delete((UA_ReferenceTypeNode*)node);
            break;
        case UA_NODECLASS_VARIABLE:
            UA_VariableNode_delete((UA_VariableNode*)node);
            break;
        default:
            UA_assert(UA_FALSE);
        }
    }
}

void Service_AddNodes(UA_Server *server, UA_Session *session, const UA_AddNodesRequest *request,
                      UA_AddNodesResponse *response) {
    if(request->nodesToAddSize <= 0) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADNOTHINGTODO;
        return;
    }
    size_t size = request->nodesToAddSize;

    response->results = UA_Array_new(&UA_TYPES[UA_TYPES_ADDNODESRESULT], size);
    if(!response->results) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;
        return;
    }

    /* ### Begin External Namespaces */
    UA_Boolean *isExternal = UA_alloca(sizeof(UA_Boolean) * size);
    UA_memset(isExternal, UA_FALSE, sizeof(UA_Boolean) * size);
    UA_UInt32 *indices = UA_alloca(sizeof(UA_UInt32) * size);
    for(UA_Int32 j = 0;j<server->externalNamespacesSize;j++) {
        size_t indexSize = 0;
        for(size_t i = 0;i < size;i++) {
            if(request->nodesToAdd[i].requestedNewNodeId.nodeId.namespaceIndex !=
               server->externalNamespaces[j].index)
                continue;
            isExternal[i] = UA_TRUE;
            indices[indexSize] = i;
            indexSize++;
        }
        if(indexSize == 0)
            continue;
        UA_ExternalNodeStore *ens = &server->externalNamespaces[j].externalNodeStore;
        ens->addNodes(ens->ensHandle, &request->requestHeader, request->nodesToAdd,
                      indices, indexSize, response->results, response->diagnosticInfos);
    }
    /* ### End External Namespaces */
    
    response->resultsSize = size;
    for(size_t i = 0;i < size;i++) {
        if(!isExternal[i])
            addNodeFromAttributes(server, session, &request->nodesToAdd[i], &response->results[i]);
    }
}

void Service_AddReferences(UA_Server *server, UA_Session *session, const UA_AddReferencesRequest *request,
                           UA_AddReferencesResponse *response) {
	if(request->referencesToAddSize <= 0) {
		response->responseHeader.serviceResult = UA_STATUSCODE_BADNOTHINGTODO;
		return;
	}
    size_t size = request->referencesToAddSize;
	
    if(!(response->results = UA_malloc(sizeof(UA_StatusCode) * size))) {
		response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;
		return;
	}
	response->resultsSize = size;
	UA_memset(response->results, UA_STATUSCODE_GOOD, sizeof(UA_StatusCode) * size);

	/* ### Begin External Namespaces */
	UA_Boolean *isExternal = UA_alloca(sizeof(UA_Boolean) * size);
	UA_memset(isExternal, UA_FALSE, sizeof(UA_Boolean) * size);
	UA_UInt32 *indices = UA_alloca(sizeof(UA_UInt32) * size);
	for(UA_Int32 j = 0; j < server->externalNamespacesSize; j++) {
		size_t indexSize = 0;
		for(size_t i = 0;i < size;i++) {
			if(request->referencesToAdd[i].sourceNodeId.namespaceIndex
					!= server->externalNamespaces[j].index)
				continue;
			isExternal[i] = UA_TRUE;
			indices[indexSize] = i;
			indexSize++;
		}
		if (indexSize == 0)
			continue;
		UA_ExternalNodeStore *ens = &server->externalNamespaces[j].externalNodeStore;
		ens->addReferences(ens->ensHandle, &request->requestHeader, request->referencesToAdd,
                           indices, indexSize, response->results, response->diagnosticInfos);
	}
	/* ### End External Namespaces */

	response->resultsSize = size;
	for(UA_Int32 i = 0; i < response->resultsSize; i++) {
		if(!isExternal[i])
			UA_Server_addReference(server, &request->referencesToAdd[i]);
	}
}

void Service_DeleteNodes(UA_Server *server, UA_Session *session, const UA_DeleteNodesRequest *request,
                         UA_DeleteNodesResponse *response) {

}

void Service_DeleteReferences(UA_Server *server, UA_Session *session,
                              const UA_DeleteReferencesRequest *request,
                              UA_DeleteReferencesResponse *response) {

}

static UA_StatusCode
fillrefdescr(UA_NodeStore *ns, const UA_Node *curr, UA_ReferenceNode *ref, UA_UInt32 mask,
             UA_ReferenceDescription *descr)
{
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    UA_ReferenceDescription_init(descr);
    retval |= UA_NodeId_copy(&curr->nodeId, &descr->nodeId.nodeId);
    //TODO: ExpandedNodeId is mocked up
    descr->nodeId.serverIndex = 0;
    descr->nodeId.namespaceUri.length = -1;

    if(mask & UA_BROWSERESULTMASK_REFERENCETYPEID)
        retval |= UA_NodeId_copy(&ref->referenceTypeId, &descr->referenceTypeId);
    if(mask & UA_BROWSERESULTMASK_ISFORWARD)
        descr->isForward = !ref->isInverse;
    if(mask & UA_BROWSERESULTMASK_NODECLASS)
        retval |= UA_NodeClass_copy(&curr->nodeClass, &descr->nodeClass);
    if(mask & UA_BROWSERESULTMASK_BROWSENAME)
        retval |= UA_QualifiedName_copy(&curr->browseName, &descr->browseName);
    if(mask & UA_BROWSERESULTMASK_DISPLAYNAME)
        retval |= UA_LocalizedText_copy(&curr->displayName, &descr->displayName);
    if(mask & UA_BROWSERESULTMASK_TYPEDEFINITION ) {
        for(UA_Int32 i = 0;i < curr->referencesSize;i++) {
            UA_ReferenceNode *refnode = &curr->references[i];
            if(refnode->referenceTypeId.identifier.numeric == UA_NS0ID_HASTYPEDEFINITION) {
                retval |= UA_ExpandedNodeId_copy(&refnode->targetId, &descr->typeDefinition);
                break;
            }
        }
    }

    if(retval)
        UA_ReferenceDescription_deleteMembers(descr);
    return retval;
}

/* Tests if the node is relevant to the browse request and shall be returned. If
   so, it is retrieved from the Nodestore. If not, null is returned. */
static const UA_Node
*relevant_node(UA_NodeStore *ns, const UA_BrowseDescription *descr, UA_Boolean return_all,
               UA_ReferenceNode *reference, UA_NodeId *relevant, size_t relevant_count)
{
    if(reference->isInverse == UA_TRUE && descr->browseDirection == UA_BROWSEDIRECTION_FORWARD)
        return UA_NULL;
    else if(reference->isInverse == UA_FALSE && descr->browseDirection == UA_BROWSEDIRECTION_INVERSE)
        return UA_NULL;

    if(!return_all) {
        for(size_t i = 0; i < relevant_count; i++) {
            if(UA_NodeId_equal(&reference->referenceTypeId, &relevant[i]))
                goto is_relevant;
        }
        return UA_NULL;
    }
is_relevant: ;
    const UA_Node *node = UA_NodeStore_get(ns, &reference->targetId.nodeId);
    if(node && descr->nodeClassMask != 0 && (node->nodeClass & descr->nodeClassMask) == 0) {
        UA_NodeStore_release(node);
        return UA_NULL;
    }
    return node;
}

/**
 * We find all subtypes by a single iteration over the array. We start with an array with a single
 * root nodeid at the beginning. When we find relevant references, we add the nodeids to the back of
 * the array and increase the size. Since the hierarchy is not cyclic, we can safely progress in the
 * array to process the newly found referencetype nodeids (emulated recursion).
 */
static UA_StatusCode
findsubtypes(UA_NodeStore *ns, const UA_NodeId *root, UA_NodeId **reftypes, size_t *reftypes_count)
{
    size_t results_size = 20; // probably too big, but saves mallocs
    UA_NodeId *results = UA_malloc(sizeof(UA_NodeId) * results_size);
    if(!results)
        return UA_STATUSCODE_BADOUTOFMEMORY;

    UA_StatusCode retval = UA_NodeId_copy(root, &results[0]);
    if(retval != UA_STATUSCODE_GOOD) {
        UA_free(results);
        return retval;
    }
        
    size_t index = 0; // where are we currently in the array?
    size_t last = 0; // where is the last element in the array?
    do {
        const UA_Node *node = UA_NodeStore_get(ns, &results[index]);
        if(!node || node->nodeClass != UA_NODECLASS_REFERENCETYPE)
            continue;
        for(UA_Int32 i = 0; i < node->referencesSize; i++) {
            if(node->references[i].referenceTypeId.identifier.numeric != UA_NS0ID_HASSUBTYPE ||
               node->references[i].isInverse == UA_TRUE)
                continue;

            if(++last >= results_size) { // is the array big enough?
                UA_NodeId *new_results = UA_realloc(results, sizeof(UA_NodeId) * results_size * 2);
                if(!new_results) {
                    retval = UA_STATUSCODE_BADOUTOFMEMORY;
                    break;
                }
                results = new_results;
                results_size *= 2;
            }

            retval = UA_NodeId_copy(&node->references[i].targetId.nodeId, &results[last]);
            if(retval != UA_STATUSCODE_GOOD) {
                last--; // for array_delete
                break;
            }
        }
        UA_NodeStore_release(node);
    } while(++index <= last && retval == UA_STATUSCODE_GOOD);

    if(retval) {
        UA_Array_delete(results, &UA_TYPES[UA_TYPES_NODEID], last);
        return retval;
    }

    *reftypes = results;
    *reftypes_count = last + 1;
    return UA_STATUSCODE_GOOD;
}

/* Results for a single browsedescription. */
static void
browse(UA_NodeStore *ns, const UA_BrowseDescription *descr, UA_UInt32 maxrefs, UA_BrowseResult *result)
{
    size_t relevant_refs_size = 0;
    UA_NodeId *relevant_refs = UA_NULL;

    // what are the relevant references?
    UA_Boolean all_refs = UA_NodeId_isNull(&descr->referenceTypeId);
    if(!all_refs) {
        if(descr->includeSubtypes) {
            result->statusCode = findsubtypes(ns, &descr->referenceTypeId,
                                              &relevant_refs, &relevant_refs_size);
            if(result->statusCode != UA_STATUSCODE_GOOD)
                return;
        } else {
            relevant_refs = (UA_NodeId*)(uintptr_t)&descr->referenceTypeId;
            relevant_refs_size = 1;
        }
    }

    // get the node
    const UA_Node *node = UA_NodeStore_get(ns, &descr->nodeId);
    if(!node) {
        result->statusCode = UA_STATUSCODE_BADNODEIDUNKNOWN;
        if(!all_refs && descr->includeSubtypes)
            UA_Array_delete(relevant_refs, &UA_TYPES[UA_TYPES_NODEID], relevant_refs_size);
        return;
    }

    if(node->referencesSize <= 0) {
        result->referencesSize = 0;
        goto cleanup;
    }

    // allocate memory for the results
    maxrefs = node->referencesSize; // allocate enough space for all of them
    result->references = UA_malloc(sizeof(UA_ReferenceDescription) * maxrefs);
    if(!result->references) {
        result->statusCode = UA_STATUSCODE_BADOUTOFMEMORY;
        goto cleanup;
    }

    size_t count = 0;
    for(UA_Int32 i = 0; i < node->referencesSize && count < maxrefs; i++) {
        const UA_Node *current;
        current = relevant_node(ns, descr, all_refs, &node->references[i], relevant_refs, relevant_refs_size);
        if(!current)
            continue;

        UA_StatusCode retval = fillrefdescr(ns, current, &node->references[i],
                                            descr->resultMask, &result->references[count]);
        UA_NodeStore_release(current);

        if(retval) {
            UA_Array_delete(result->references, &UA_TYPES[UA_TYPES_REFERENCEDESCRIPTION], count);
            count = 0;
            result->references = UA_NULL;
            result->statusCode = UA_STATUSCODE_UNCERTAINNOTALLNODESAVAILABLE;
            break;
        }
        count++;
    }
    if(count != 0)
        result->referencesSize = count;
    else {
        UA_free(result->references);
        result->references = UA_NULL;
    }

cleanup:
    UA_NodeStore_release(node);
    if(!all_refs && descr->includeSubtypes)
        UA_Array_delete(relevant_refs, &UA_TYPES[UA_TYPES_NODEID], relevant_refs_size);
}

void Service_Browse(UA_Server *server, UA_Session *session, const UA_BrowseRequest *request,
                    UA_BrowseResponse *response) {
   if(request->nodesToBrowseSize <= 0) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADNOTHINGTODO;
        return;
    }
   size_t size = request->nodesToBrowseSize;

   response->results = UA_Array_new(&UA_TYPES[UA_TYPES_BROWSERESULT], size);
    if(!response->results) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;
        return;
    }

    /* ### Begin External Namespaces */
    UA_Boolean *isExternal = UA_alloca(sizeof(UA_Boolean) * size);
    UA_memset(isExternal, UA_FALSE, sizeof(UA_Boolean) * size);
    UA_UInt32 *indices = UA_alloca(sizeof(UA_UInt32) * size);
    for(UA_Int32 j = 0;j<server->externalNamespacesSize;j++) {
        size_t indexSize = 0;
        for(size_t i = 0;i < size;i++) {
            if(request->nodesToBrowse[i].nodeId.namespaceIndex != server->externalNamespaces[j].index)
                continue;
            isExternal[i] = UA_TRUE;
            indices[indexSize] = i;
            indexSize++;
        }
        if(indexSize == 0)
            continue;

        UA_ExternalNodeStore *ens = &server->externalNamespaces[j].externalNodeStore;
        ens->browseNodes(ens->ensHandle, &request->requestHeader, request->nodesToBrowse, indices, indexSize,
                         request->requestedMaxReferencesPerNode, response->results, response->diagnosticInfos);
    }
    /* ### End External Namespaces */

    response->resultsSize = size;
    for(size_t i = 0;i < size;i++){
        if(!isExternal[i])
            browse(server->nodestore, &request->nodesToBrowse[i],
                   request->requestedMaxReferencesPerNode, &response->results[i]);
    }
}

/***********************/
/* TranslateBrowsePath */
/***********************/

static UA_StatusCode
walkBrowsePath(UA_Server *server, UA_Session *session, const UA_Node *node, const UA_RelativePath *path,
               UA_Int32 pathindex, UA_BrowsePathTarget **targets, UA_Int32 *targets_size,
               UA_Int32 *target_count)
{
    const UA_RelativePathElement *elem = &path->elements[pathindex];
    if(elem->targetName.name.length == -1)
        return UA_STATUSCODE_BADBROWSENAMEINVALID;

    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    UA_NodeId *reftypes;
    size_t reftypes_count = 1; // all_refs or no subtypes => 1
    UA_Boolean all_refs = UA_FALSE;
    if(UA_NodeId_isNull(&elem->referenceTypeId))
        all_refs = UA_TRUE;
    else if(!elem->includeSubtypes)
        reftypes = (UA_NodeId*)(uintptr_t)&elem->referenceTypeId; // ptr magic due to const cast
    else
        retval = findsubtypes(server->nodestore, &elem->referenceTypeId, &reftypes, &reftypes_count);

    for(UA_Int32 i = 0; i < node->referencesSize && retval == UA_STATUSCODE_GOOD; i++) {
        UA_Boolean match = all_refs;
        for(size_t j = 0; j < reftypes_count && !match; j++) {
            if(node->references[i].isInverse == elem->isInverse &&
               UA_NodeId_equal(&node->references[i].referenceTypeId, &reftypes[j]))
                match = UA_TRUE;
        }
        if(!match)
            continue;

        // get the node, todo: expandednodeid
        const UA_Node *next = UA_NodeStore_get(server->nodestore, &node->references[i].targetId.nodeId);
        if(!next)
            continue;

        // test the browsename
        if(elem->targetName.namespaceIndex != next->browseName.namespaceIndex ||
           !UA_String_equal(&elem->targetName.name, &next->browseName.name)) {
            UA_NodeStore_release(next);
            continue;
        }

        if(pathindex + 1 < path->elementsSize) {
            // recursion if the path is longer
            retval = walkBrowsePath(server, session, next, path, pathindex + 1,
                                    targets, targets_size, target_count);
            UA_NodeStore_release(next);
        } else {
            // add the browsetarget
            if(*target_count >= *targets_size) {
                UA_BrowsePathTarget *newtargets;
                newtargets = UA_realloc(targets, sizeof(UA_BrowsePathTarget) * (*targets_size) * 2);
                if(!newtargets) {
                    retval = UA_STATUSCODE_BADOUTOFMEMORY;
                    UA_NodeStore_release(next);
                    break;
                }
                *targets = newtargets;
                *targets_size *= 2;
            }

            UA_BrowsePathTarget *res = *targets;
            UA_ExpandedNodeId_init(&res[*target_count].targetId);
            retval = UA_NodeId_copy(&next->nodeId, &res[*target_count].targetId.nodeId);
            UA_NodeStore_release(next);
            if(retval != UA_STATUSCODE_GOOD)
                break;
            res[*target_count].remainingPathIndex = UA_UINT32_MAX;
            *target_count += 1;
        }
    }

    if(!all_refs && elem->includeSubtypes)
        UA_Array_delete(reftypes, &UA_TYPES[UA_TYPES_NODEID], (UA_Int32)reftypes_count);
    return retval;
}

static void translateBrowsePath(UA_Server *server, UA_Session *session, const UA_BrowsePath *path,
                                UA_BrowsePathResult *result) {
    if(path->relativePath.elementsSize <= 0) {
        result->statusCode = UA_STATUSCODE_BADNOTHINGTODO;
        return;
    }
        
    UA_Int32 arraySize = 10;
    result->targets = UA_malloc(sizeof(UA_BrowsePathTarget) * arraySize);
    if(!result->targets) {
        result->statusCode = UA_STATUSCODE_BADOUTOFMEMORY;
        return;
    }
    result->targetsSize = 0;
    const UA_Node *firstNode = UA_NodeStore_get(server->nodestore, &path->startingNode);
    if(!firstNode) {
        result->statusCode = UA_STATUSCODE_BADNODEIDUNKNOWN;
        UA_free(result->targets);
        result->targets = UA_NULL;
        return;
    }
    result->statusCode = walkBrowsePath(server, session, firstNode, &path->relativePath, 0,
                                        &result->targets, &arraySize, &result->targetsSize);
    UA_NodeStore_release(firstNode);
    if(result->targetsSize == 0 && result->statusCode == UA_STATUSCODE_GOOD)
        result->statusCode = UA_STATUSCODE_BADNOMATCH;
    if(result->statusCode != UA_STATUSCODE_GOOD) {
        UA_Array_delete(result->targets, &UA_TYPES[UA_TYPES_BROWSEPATHTARGET], result->targetsSize);
        result->targets = UA_NULL;
        result->targetsSize = -1;
    }
}

void Service_TranslateBrowsePathsToNodeIds(UA_Server *server, UA_Session *session,
                                           const UA_TranslateBrowsePathsToNodeIdsRequest *request,
                                           UA_TranslateBrowsePathsToNodeIdsResponse *response) {
    size_t size = request->browsePathsSize;
	if(request->browsePathsSize <= 0) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADNOTHINGTODO;
        return;
    }

    response->results = UA_Array_new(&UA_TYPES[UA_TYPES_BROWSEPATHRESULT], size);
    if(!response->results) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;
        return;
    }
    /* ### Begin External Namespaces */
    UA_Boolean *isExternal = UA_alloca(sizeof(UA_Boolean) * size);
    UA_memset(isExternal, UA_FALSE, sizeof(UA_Boolean) * size);
    UA_UInt32 *indices = UA_alloca(sizeof(UA_UInt32) * size);
    for(UA_Int32 j = 0;j<server->externalNamespacesSize;j++) {
    	size_t indexSize = 0;
    	for(size_t i = 0;i < size;i++) {
    		if(request->browsePaths[i].startingNode.namespaceIndex != server->externalNamespaces[j].index)
    			continue;
    		isExternal[i] = UA_TRUE;
    		indices[indexSize] = i;
    		indexSize++;
    	}
    	if(indexSize == 0)
    		continue;

    	UA_ExternalNodeStore *ens = &server->externalNamespaces[j].externalNodeStore;
    	ens->translateBrowsePathsToNodeIds(ens->ensHandle, &request->requestHeader, request->browsePaths,
    			indices, indexSize, response->results, response->diagnosticInfos);
    }
    /* ### End External Namespaces */
    response->resultsSize = size;
    for(size_t i = 0;i < size;i++){
    	if(!isExternal[i])
    		translateBrowsePath(server, session, &request->browsePaths[i], &response->results[i]);
    }
}

void Service_RegisterNodes(UA_Server *server, UA_Session *session,
                                           const UA_RegisterNodesRequest *request,
                                           UA_RegisterNodesResponse *response) {

	//TODO: hang the nodeids to the session if really needed
	response->responseHeader.timestamp = UA_DateTime_now();
	response->registeredNodeIdsSize = request->nodesToRegisterSize;
	response->registeredNodeIds = request->nodesToRegister;
	if(request->nodesToRegisterSize==0){
		response->responseHeader.serviceResult = UA_STATUSCODE_BADNOTHINGTODO;
	}
	if(UA_NodeId_equal(&request->requestHeader.authenticationToken, &UA_NODEID_NULL) || !UA_NodeId_equal(&request->requestHeader.authenticationToken, &session->authenticationToken) ){
		response->responseHeader.serviceResult = UA_STATUSCODE_BADSESSIONIDINVALID;
	}

}

void Service_UnregisterNodes(UA_Server *server, UA_Session *session,
                                           const UA_UnregisterNodesRequest *request,
                                           UA_UnregisterNodesResponse *response) {

	//TODO: remove the nodeids from the session if really needed
	response->responseHeader.timestamp = UA_DateTime_now();
	if(request->nodesToUnregisterSize==0){
		response->responseHeader.serviceResult = UA_STATUSCODE_BADNOTHINGTODO;
	}
	if(UA_NodeId_equal(&request->requestHeader.authenticationToken, &UA_NODEID_NULL) || !UA_NodeId_equal(&request->requestHeader.authenticationToken, &session->authenticationToken) ){
		response->responseHeader.serviceResult = UA_STATUSCODE_BADSESSIONIDINVALID;
	}
}

struct UA_Client {
    /* Connection */
    UA_ClientNetworkLayer networkLayer;
    UA_String endpointUrl;
    UA_Connection connection;

    UA_UInt32 sequenceNumber;
    UA_UInt32 requestId;

    /* Secure Channel */
    UA_ChannelSecurityToken securityToken;
    UA_ByteString clientNonce;
    UA_ByteString serverNonce;
    /* UA_SequenceHeader sequenceHdr; */
    /* UA_NodeId authenticationToken; */

    /* Session */
    UA_NodeId sessionId;
    UA_NodeId authenticationToken;

    /* Config */
    UA_ClientConfig config;
};

const UA_ClientConfig UA_ClientConfig_standard = { 500 };

UA_Client * UA_Client_new(UA_ClientConfig config) {
    UA_Client *client = UA_malloc(sizeof(UA_Client));
    if(!client)
        return UA_NULL;
    client->config = config;
    UA_String_init(&client->endpointUrl);
    client->connection.state = UA_CONNECTION_OPENING;
    UA_ByteString_init(&client->connection.incompleteMessage);

    client->sequenceNumber = 0;
    client->requestId = 0;

    /* Secure Channel */
    UA_ChannelSecurityToken_deleteMembers(&client->securityToken);
    UA_ByteString_init(&client->clientNonce);
    UA_ByteString_init(&client->serverNonce);
    
    UA_NodeId_init(&client->authenticationToken);

    return client;
}

void UA_Client_delete(UA_Client* client){
    client->networkLayer.destroy(client->networkLayer.nlHandle);
    UA_String_deleteMembers(&client->endpointUrl);
    // client->connection

    /* Secure Channel */
    UA_ByteString_deleteMembers(&client->clientNonce);
    UA_ByteString_deleteMembers(&client->serverNonce);

    free(client);
}

// The tcp connection is established. Now do the handshake
static UA_StatusCode HelAckHandshake(UA_Client *c) {
    UA_TcpMessageHeader messageHeader;
    messageHeader.messageTypeAndFinal = UA_MESSAGETYPEANDFINAL_HELF;

    UA_TcpHelloMessage hello;
    UA_String_copy(&c->endpointUrl, &hello.endpointUrl);

    UA_Connection *conn = &c->connection;
    hello.maxChunkCount = conn->localConf.maxChunkCount;
    hello.maxMessageSize = conn->localConf.maxMessageSize;
    hello.protocolVersion = conn->localConf.protocolVersion;
    hello.receiveBufferSize = conn->localConf.recvBufferSize;
    hello.sendBufferSize = conn->localConf.sendBufferSize;

    messageHeader.messageSize = UA_TcpHelloMessage_calcSizeBinary((UA_TcpHelloMessage const*)&hello) +
                                UA_TcpMessageHeader_calcSizeBinary((UA_TcpMessageHeader const*)&messageHeader);
    UA_ByteString message;
    message.data = UA_alloca(messageHeader.messageSize);
    message.length = messageHeader.messageSize;

    size_t offset = 0;
    UA_TcpMessageHeader_encodeBinary(&messageHeader, &message, &offset);
    UA_TcpHelloMessage_encodeBinary(&hello, &message, &offset);
    UA_TcpHelloMessage_deleteMembers(&hello);

    UA_ByteStringArray buf = {.stringsSize = 1, .strings = &message};
    UA_StatusCode retval = c->networkLayer.send(c->networkLayer.nlHandle, buf);
    if(retval)
        return retval;

    UA_Byte replybuf[1024];
    UA_ByteString reply = {.data = replybuf, .length = 1024};
    retval = c->networkLayer.awaitResponse(c->networkLayer.nlHandle, &reply, c->config.timeout * 1000);
    if (retval)
        return retval;

    offset = 0;
    UA_TcpMessageHeader_decodeBinary(&reply, &offset, &messageHeader);
    UA_TcpAcknowledgeMessage ackMessage;
    retval = UA_TcpAcknowledgeMessage_decodeBinary(&reply, &offset, &ackMessage);
    if(retval != UA_STATUSCODE_GOOD) {
        UA_TcpAcknowledgeMessage_deleteMembers(&ackMessage);
        return retval;
    }
    conn->remoteConf.maxChunkCount = ackMessage.maxChunkCount;
    conn->remoteConf.maxMessageSize = ackMessage.maxMessageSize;
    conn->remoteConf.protocolVersion = ackMessage.protocolVersion;
    conn->remoteConf.recvBufferSize = ackMessage.receiveBufferSize;
    conn->remoteConf.sendBufferSize = ackMessage.sendBufferSize;
    conn->state = UA_CONNECTION_ESTABLISHED;

    UA_TcpAcknowledgeMessage_deleteMembers(&ackMessage);
    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode SecureChannelHandshake(UA_Client *client) {
    UA_ByteString_deleteMembers(&client->clientNonce); // if the handshake is repeated
    UA_ByteString_newMembers(&client->clientNonce, 1);
    client->clientNonce.data[0] = 0;

    UA_SecureConversationMessageHeader messageHeader;
    messageHeader.messageHeader.messageTypeAndFinal = UA_MESSAGETYPEANDFINAL_OPNF;
    messageHeader.secureChannelId = 0;

    UA_SequenceHeader seqHeader;
    seqHeader.sequenceNumber = ++client->sequenceNumber;
    seqHeader.requestId = ++client->requestId;

    UA_AsymmetricAlgorithmSecurityHeader asymHeader;
    UA_AsymmetricAlgorithmSecurityHeader_init(&asymHeader);
    asymHeader.securityPolicyUri = UA_STRING_ALLOC("http://opcfoundation.org/UA/SecurityPolicy#None");

    /* id of opensecurechannelrequest */
    UA_NodeId requestType = UA_NODEID_NUMERIC(0, UA_NS0ID_OPENSECURECHANNELREQUEST + UA_ENCODINGOFFSET_BINARY);

    UA_OpenSecureChannelRequest opnSecRq;
    UA_OpenSecureChannelRequest_init(&opnSecRq);
    opnSecRq.requestHeader.timestamp = UA_DateTime_now();
    UA_ByteString_copy(&client->clientNonce, &opnSecRq.clientNonce);
    opnSecRq.requestedLifetime = 30000;
    opnSecRq.securityMode = UA_MESSAGESECURITYMODE_NONE;
    opnSecRq.requestType = UA_SECURITYTOKENREQUESTTYPE_ISSUE;
    opnSecRq.requestHeader.authenticationToken.identifier.numeric = 10;
    opnSecRq.requestHeader.authenticationToken.identifierType = UA_NODEIDTYPE_NUMERIC;
    opnSecRq.requestHeader.authenticationToken.namespaceIndex = 10;

    messageHeader.messageHeader.messageSize =
        UA_SecureConversationMessageHeader_calcSizeBinary(&messageHeader) +
        UA_AsymmetricAlgorithmSecurityHeader_calcSizeBinary(&asymHeader) +
        UA_SequenceHeader_calcSizeBinary(&seqHeader) +
        UA_NodeId_calcSizeBinary(&requestType) +
        UA_OpenSecureChannelRequest_calcSizeBinary(&opnSecRq);

    UA_ByteString message;
    message.data = UA_alloca(messageHeader.messageHeader.messageSize);
    message.length = messageHeader.messageHeader.messageSize;

    size_t offset = 0;
    UA_SecureConversationMessageHeader_encodeBinary(&messageHeader, &message, &offset);
    UA_AsymmetricAlgorithmSecurityHeader_encodeBinary(&asymHeader, &message, &offset);
    UA_SequenceHeader_encodeBinary(&seqHeader, &message, &offset);
    UA_NodeId_encodeBinary(&requestType, &message, &offset);
    UA_OpenSecureChannelRequest_encodeBinary(&opnSecRq, &message, &offset);

    UA_AsymmetricAlgorithmSecurityHeader_deleteMembers(&asymHeader);
    UA_OpenSecureChannelRequest_deleteMembers(&opnSecRq);

    UA_ByteStringArray buf = {.stringsSize = 1, .strings = &message};
    UA_StatusCode retval = client->networkLayer.send(client->networkLayer.nlHandle, buf);
    if(retval)
        return retval;

    // parse the response
    UA_ByteString reply;
    UA_ByteString_newMembers(&reply, client->connection.localConf.recvBufferSize);
    do {
        retval = client->networkLayer.awaitResponse(client->networkLayer.nlHandle, &reply, client->config.timeout * 1000);
        if(retval != UA_STATUSCODE_GOOD) {
            UA_ByteString_deleteMembers(&reply);
            return retval;
        }
        reply = UA_Connection_completeMessages(&client->connection, reply);
    } while(reply.length < 0);

    offset = 0;
    UA_SecureConversationMessageHeader_decodeBinary(&reply, &offset, &messageHeader);
    UA_AsymmetricAlgorithmSecurityHeader_decodeBinary(&reply, &offset, &asymHeader);
    UA_SequenceHeader_decodeBinary(&reply, &offset, &seqHeader);
    UA_NodeId_decodeBinary(&reply, &offset, &requestType);

    if(!UA_NodeId_equal(&requestType, &UA_NODEID_NUMERIC(0, UA_NS0ID_OPENSECURECHANNELRESPONSE +
                                                        UA_ENCODINGOFFSET_BINARY))) {
        UA_ByteString_deleteMembers(&reply);
        UA_AsymmetricAlgorithmSecurityHeader_deleteMembers(&asymHeader);
        return UA_STATUSCODE_BADINTERNALERROR;
    }

    UA_OpenSecureChannelResponse response;
    UA_OpenSecureChannelResponse_decodeBinary(&reply, &offset, &response);
    UA_ByteString_deleteMembers(&reply);
    retval = response.responseHeader.serviceResult;

    if(retval == UA_STATUSCODE_GOOD) {
        UA_ChannelSecurityToken_copy(&response.securityToken, &client->securityToken);
        UA_ByteString_deleteMembers(&client->serverNonce); // if the handshake is repeated
        UA_ByteString_copy(&response.serverNonce, &client->serverNonce);
    }

    UA_OpenSecureChannelResponse_deleteMembers(&response);
    UA_AsymmetricAlgorithmSecurityHeader_deleteMembers(&asymHeader);
    return retval;

}

/** If the request fails, then the response is cast to UA_ResponseHeader (at the beginning of every
    response) and filled with the appropriate error code */
static void sendReceiveRequest(UA_RequestHeader *request, const UA_DataType *requestType,
                               void *response, const UA_DataType *responseType, UA_Client *client,
                               UA_Boolean sendOnly)
{
    if(response)
        UA_init(response, responseType);

    UA_NodeId_copy(&client->authenticationToken, &request->authenticationToken);

    UA_SecureConversationMessageHeader msgHeader;
    if(sendOnly)
        msgHeader.messageHeader.messageTypeAndFinal = UA_MESSAGETYPEANDFINAL_CLOF;
    else
        msgHeader.messageHeader.messageTypeAndFinal = UA_MESSAGETYPEANDFINAL_MSGF;
    msgHeader.secureChannelId = client->securityToken.channelId;

    UA_SymmetricAlgorithmSecurityHeader symHeader;
    symHeader.tokenId = client->securityToken.tokenId;
    
    UA_SequenceHeader seqHeader;
    seqHeader.sequenceNumber = ++client->sequenceNumber;
    seqHeader.requestId = ++client->requestId;

    UA_NodeId requestId = UA_NODEID_NUMERIC(0, requestType->typeId.identifier.numeric +
                                           UA_ENCODINGOFFSET_BINARY);

    msgHeader.messageHeader.messageSize =
        UA_SecureConversationMessageHeader_calcSizeBinary(&msgHeader) +
        UA_SymmetricAlgorithmSecurityHeader_calcSizeBinary(&symHeader) +
        UA_SequenceHeader_calcSizeBinary(&seqHeader) +
        UA_NodeId_calcSizeBinary(&requestId) +
        UA_calcSizeBinary(request, requestType);

    UA_ByteString message;
    UA_StatusCode retval = UA_ByteString_newMembers(&message, msgHeader.messageHeader.messageSize);
    if(retval != UA_STATUSCODE_GOOD) {
        // todo
    }

    size_t offset = 0;
    retval |= UA_SecureConversationMessageHeader_encodeBinary(&msgHeader, &message, &offset);
    retval |= UA_SymmetricAlgorithmSecurityHeader_encodeBinary(&symHeader, &message, &offset);
    retval |= UA_SequenceHeader_encodeBinary(&seqHeader, &message, &offset);

    retval |= UA_NodeId_encodeBinary(&requestId, &message, &offset);
    retval |= UA_encodeBinary(request, requestType, &message, &offset);

    UA_ByteStringArray buf = {.stringsSize = 1, .strings = &message};
    retval = client->networkLayer.send(client->networkLayer.nlHandle, buf);
    UA_ByteString_deleteMembers(&message);

    //TODO: rework to get return value
    if(sendOnly)
        return;

    UA_ResponseHeader *respHeader = (UA_ResponseHeader*)response;
    if(retval != UA_STATUSCODE_GOOD) {
        respHeader->serviceResult = retval;
        return;
    }

    /* Response */
    UA_ByteString reply;
    do {
        UA_ByteString_newMembers(&reply, client->connection.localConf.recvBufferSize);
        retval = client->networkLayer.awaitResponse(client->networkLayer.nlHandle, &reply, client->config.timeout * 1000);
        if(retval != UA_STATUSCODE_GOOD) {
            UA_ByteString_deleteMembers(&reply);
            respHeader->serviceResult = retval;
            return;
        }
        reply = UA_Connection_completeMessages(&client->connection, reply);
    } while(reply.length < 0);

    offset = 0;
    retval |= UA_SecureConversationMessageHeader_decodeBinary(&reply, &offset, &msgHeader);
    retval |= UA_SymmetricAlgorithmSecurityHeader_decodeBinary(&reply, &offset, &symHeader);
    retval |= UA_SequenceHeader_decodeBinary(&reply, &offset, &seqHeader);
    UA_NodeId responseId;
    retval |= UA_NodeId_decodeBinary(&reply, &offset, &responseId);

    if(!UA_NodeId_equal(&responseId, &UA_NODEID_NUMERIC(0, responseType->typeId.identifier.numeric +
                                                       UA_ENCODINGOFFSET_BINARY))) {
        UA_ByteString_deleteMembers(&reply);
        UA_SymmetricAlgorithmSecurityHeader_deleteMembers(&symHeader);
        respHeader->serviceResult = retval;
        return;
    }

    retval = UA_decodeBinary(&reply, &offset, response, responseType);
    UA_ByteString_deleteMembers(&reply);
    if(retval != UA_STATUSCODE_GOOD)
        respHeader->serviceResult = retval;
}

static void synchronousRequest(void *request, const UA_DataType *requestType,
                               void *response, const UA_DataType *responseType, UA_Client *client){
    sendReceiveRequest(request, requestType, response, responseType, client, UA_FALSE);
}

static UA_StatusCode ActivateSession(UA_Client *client) {
    UA_ActivateSessionRequest request;
    UA_ActivateSessionRequest_init(&request);

    request.requestHeader.requestHandle = 2; //TODO: is it a magic number?
    request.requestHeader.authenticationToken = client->authenticationToken;
    request.requestHeader.timestamp = UA_DateTime_now();
    request.requestHeader.timeoutHint = 10000;

    UA_ActivateSessionResponse response;
    synchronousRequest(&request, &UA_TYPES[UA_TYPES_ACTIVATESESSIONREQUEST],
                       &response, &UA_TYPES[UA_TYPES_ACTIVATESESSIONRESPONSE],
                       client);

    UA_ActivateSessionRequest_deleteMembers(&request);
    UA_ActivateSessionResponse_deleteMembers(&response);
    return response.responseHeader.serviceResult; // not deleted
}

static UA_StatusCode SessionHandshake(UA_Client *client) {

    UA_CreateSessionRequest request;
    UA_CreateSessionRequest_init(&request);

    // todo: is this needed for all requests?
    UA_NodeId_copy(&client->authenticationToken, &request.requestHeader.authenticationToken);

    request.requestHeader.timestamp = UA_DateTime_now();
    request.requestHeader.timeoutHint = 10000;
    UA_ByteString_copy(&client->clientNonce, &request.clientNonce);
    request.requestedSessionTimeout = 1200000;
    request.maxResponseMessageSize = UA_INT32_MAX;

    /* UA_String_copy(endpointUrl, &rq.endpointUrl); */
    /* UA_String_copycstring("mysession", &rq.sessionName); */
    /* UA_String_copycstring("abcd", &rq.clientCertificate); */

    UA_CreateSessionResponse response;
    UA_CreateSessionResponse_init(&response);
    synchronousRequest(&request, &UA_TYPES[UA_TYPES_CREATESESSIONREQUEST],
                       &response, &UA_TYPES[UA_TYPES_CREATESESSIONRESPONSE],
                       client);

    UA_NodeId_copy(&response.authenticationToken, &client->authenticationToken);

    UA_CreateSessionRequest_deleteMembers(&request);
    UA_CreateSessionResponse_deleteMembers(&response);
    return response.responseHeader.serviceResult; // not deleted
}

static UA_StatusCode CloseSession(UA_Client *client) {
    UA_CloseSessionRequest request;
    UA_CloseSessionRequest_init(&request);

    request.requestHeader.timestamp = UA_DateTime_now();
    request.requestHeader.timeoutHint = 10000;
    request.deleteSubscriptions = UA_TRUE;
    UA_NodeId_copy(&client->authenticationToken, &request.requestHeader.authenticationToken);
    UA_CreateSessionResponse response;
    synchronousRequest(&request, &UA_TYPES[UA_TYPES_CLOSESESSIONREQUEST],
                       &response, &UA_TYPES[UA_TYPES_CLOSESESSIONRESPONSE],
                       client);

    UA_CloseSessionRequest_deleteMembers(&request);
    UA_CloseSessionResponse_deleteMembers(&response);
    return response.responseHeader.serviceResult; // not deleted
}

static UA_StatusCode CloseSecureChannel(UA_Client *client) {
    UA_CloseSecureChannelRequest request;
    UA_CloseSecureChannelRequest_init(&request);

    request.requestHeader.requestHandle = 1; //TODO: magic number?
    request.requestHeader.timestamp = UA_DateTime_now();
    request.requestHeader.timeoutHint = 10000;
    request.requestHeader.authenticationToken = client->authenticationToken;
    sendReceiveRequest(&request.requestHeader, &UA_TYPES[UA_TYPES_CLOSESECURECHANNELREQUEST], UA_NULL, UA_NULL,
                       client, UA_TRUE);

    return UA_STATUSCODE_GOOD;

}

/*************************/
/* User-Facing Functions */
/*************************/


UA_StatusCode UA_Client_connect(UA_Client *client, UA_ConnectionConfig conf,
                                UA_ClientNetworkLayer networkLayer, char *endpointUrl)
{
    client->endpointUrl = UA_STRING_ALLOC(endpointUrl);
    if(client->endpointUrl.length < 0)
        return UA_STATUSCODE_BADOUTOFMEMORY;

    client->networkLayer = networkLayer;
    client->connection.localConf = conf;
    UA_StatusCode retval = networkLayer.connect(client->endpointUrl, client->networkLayer.nlHandle);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;

    retval = HelAckHandshake(client);
    if(retval == UA_STATUSCODE_GOOD)
        retval = SecureChannelHandshake(client);
    if(retval == UA_STATUSCODE_GOOD)
        retval = SessionHandshake(client);
    if(retval == UA_STATUSCODE_GOOD)
        retval = ActivateSession(client);
    return retval;
}


UA_StatusCode UA_EXPORT UA_Client_disconnect(UA_Client *client) {
    UA_StatusCode retval;
    retval = CloseSession(client);
    if(retval == UA_STATUSCODE_GOOD)
        retval = CloseSecureChannel(client);
    return retval;
}

UA_ReadResponse UA_Client_read(UA_Client *client, UA_ReadRequest *request) {
    UA_ReadResponse response;
    synchronousRequest(request, &UA_TYPES[UA_TYPES_READREQUEST], &response,
                       &UA_TYPES[UA_TYPES_READRESPONSE], client);
    return response;
}

UA_WriteResponse UA_Client_write(UA_Client *client, UA_WriteRequest *request) {
    UA_WriteResponse response;
    synchronousRequest(request, &UA_TYPES[UA_TYPES_WRITEREQUEST], &response,
                       &UA_TYPES[UA_TYPES_WRITERESPONSE], client);
    return response;
}

UA_BrowseResponse UA_Client_browse(UA_Client *client, UA_BrowseRequest *request) {
    UA_BrowseResponse response;
    synchronousRequest(request, &UA_TYPES[UA_TYPES_BROWSEREQUEST], &response,
                       &UA_TYPES[UA_TYPES_BROWSERESPONSE], client);
    return response;
}

UA_TranslateBrowsePathsToNodeIdsResponse
    UA_Client_translateTranslateBrowsePathsToNodeIds(UA_Client *client,
                                                     UA_TranslateBrowsePathsToNodeIdsRequest *request) {
    UA_TranslateBrowsePathsToNodeIdsResponse response;
    synchronousRequest(request, &UA_TYPES[UA_TYPES_BROWSEREQUEST], &response,
                       &UA_TYPES[UA_TYPES_BROWSERESPONSE], client);
    return response;
}

UA_AddNodesResponse UA_Client_addNodes(UA_Client *client, UA_AddNodesRequest *request) {
    UA_AddNodesResponse response;
    synchronousRequest(request, &UA_TYPES[UA_TYPES_BROWSEREQUEST], &response,
                       &UA_TYPES[UA_TYPES_BROWSERESPONSE], client);
    return response;
}

UA_AddReferencesResponse UA_Client_addReferences(UA_Client *client, UA_AddReferencesRequest *request) {
    UA_AddReferencesResponse response;
    synchronousRequest(request, &UA_TYPES[UA_TYPES_BROWSEREQUEST], &response,
                       &UA_TYPES[UA_TYPES_BROWSERESPONSE], client);
    return response;
}

UA_DeleteNodesResponse UA_Client_deleteNodes(UA_Client *client, UA_DeleteNodesRequest *request) {
    UA_DeleteNodesResponse response;
    synchronousRequest(request, &UA_TYPES[UA_TYPES_BROWSEREQUEST], &response,
                       &UA_TYPES[UA_TYPES_BROWSERESPONSE], client);
    return response;
}

UA_DeleteReferencesResponse UA_EXPORT
UA_Client_deleteReferences(UA_Client *client, UA_DeleteReferencesRequest *request) {
    UA_DeleteReferencesResponse response;
    synchronousRequest(request, &UA_TYPES[UA_TYPES_BROWSEREQUEST], &response,
                       &UA_TYPES[UA_TYPES_BROWSERESPONSE], client);
    return response;;
}

/* It could happen that we want to delete a node even though a function higher
   in the call-chain still has a reference. So we count references and delete
   once the count falls to zero. That means we copy every node to a new place
   where it is right behind the refcount integer.

   Since we copy nodes on the heap, we make the alloc for the nodeEntry bigger
   to accommodate for the different nodeclasses (the nodeEntry may have an
   overlength "tail"). */
#define ALIVE_BIT (1 << 15) /* Alive bit in the refcount */
struct nodeEntry {
    UA_UInt16 refcount;
    UA_Node node; // could be const, but then we cannot free it without compilers warnings
};

struct UA_NodeStore {
    struct nodeEntry **entries;
    UA_UInt32          size;
    UA_UInt32          count;
    UA_UInt32          sizePrimeIndex;
};


/* The size of the hash-map is always a prime number. They are chosen to be
   close to the next power of 2. So the size ca. doubles with each prime. */
static hash_t const primes[] = {
    7,         13,         31,         61,         127,         251,
    509,       1021,       2039,       4093,       8191,        16381,
    32749,     65521,      131071,     262139,     524287,      1048573,
    2097143,   4194301,    8388593,    16777213,   33554393,    67108859,
    134217689, 268435399,  536870909,  1073741789, 2147483647,  4294967291
};

static UA_Int16 higher_prime_index(hash_t n) {
    UA_UInt16 low  = 0;
    UA_UInt16 high = sizeof(primes) / sizeof(hash_t);
    while(low != high) {
        UA_UInt16 mid = low + (high - low) / 2;
        if(n > primes[mid])
            low = mid + 1;
        else
            high = mid;
    }
    return low;
}

/* Returns UA_TRUE if an entry was found under the nodeid. Otherwise, returns
   false and sets slot to a pointer to the next free slot. */
static UA_Boolean containsNodeId(const UA_NodeStore *ns, const UA_NodeId *nodeid, struct nodeEntry ***entry) {
    hash_t         h     = hash(nodeid);
    UA_UInt32      size  = ns->size;
    hash_t         index = mod(h, size);
    struct nodeEntry **e = &ns->entries[index];

    if(*e == UA_NULL) {
        *entry = e;
        return UA_FALSE;
    }

    if(UA_NodeId_equal(&(*e)->node.nodeId, nodeid)) {
        *entry = e;
        return UA_TRUE;
    }

    hash_t hash2 = mod2(h, size);
    for(;;) {
        index += hash2;
        if(index >= size)
            index -= size;

        e = &ns->entries[index];

        if(*e == UA_NULL) {
            *entry = e;
            return UA_FALSE;
        }

        if(UA_NodeId_equal(&(*e)->node.nodeId, nodeid)) {
            *entry = e;
            return UA_TRUE;
        }
    }

    /* NOTREACHED */
    return UA_TRUE;
}

/* The following function changes size of memory allocated for the entries and
   repeatedly inserts the table elements. The occupancy of the table after the
   call will be about 50%. */
static UA_StatusCode expand(UA_NodeStore *ns) {
    UA_Int32 osize = ns->size;
    UA_Int32 count = ns->count;
    /* Resize only when table after removal of unused elements is either too full or too empty.  */
    if(count * 2 < osize && (count * 8 > osize || osize <= 32))
        return UA_STATUSCODE_GOOD;


    UA_UInt32 nindex = higher_prime_index(count * 2);
    UA_Int32 nsize = primes[nindex];
    struct nodeEntry **nentries;
    if(!(nentries = UA_malloc(sizeof(struct nodeEntry *) * nsize)))
        return UA_STATUSCODE_BADOUTOFMEMORY;

    UA_memset(nentries, 0, nsize * sizeof(struct nodeEntry *));
    struct nodeEntry **oentries = ns->entries;
    ns->entries = nentries;
    ns->size    = nsize;
    ns->sizePrimeIndex = nindex;

    // recompute the position of every entry and insert the pointer
    for(UA_Int32 i=0, j=0;i<osize && j<count;i++) {
        if(!oentries[i])
            continue;
        struct nodeEntry **e;
        containsNodeId(ns, &(*oentries[i]).node.nodeId, &e);  /* We know this returns an empty entry here */
        *e = oentries[i];
        j++;
    }

    UA_free(oentries);
    return UA_STATUSCODE_GOOD;
}

/* Marks the entry dead and deletes if necessary. */
static void deleteEntry(struct nodeEntry *entry) {
    if(entry->refcount > 0)
        return;

    switch(entry->node.nodeClass) {
    case UA_NODECLASS_OBJECT:
        UA_ObjectNode_deleteMembers((UA_ObjectNode*)&entry->node);
        break;
    case UA_NODECLASS_VARIABLE:
        UA_VariableNode_deleteMembers((UA_VariableNode*)&entry->node);
        break;
    case UA_NODECLASS_METHOD:
        UA_MethodNode_deleteMembers((UA_MethodNode *)&entry->node);
        break;
    case UA_NODECLASS_OBJECTTYPE:
        UA_ObjectTypeNode_deleteMembers((UA_ObjectTypeNode*)&entry->node);
        break;
    case UA_NODECLASS_VARIABLETYPE:
        UA_VariableTypeNode_deleteMembers((UA_VariableTypeNode*)&entry->node);
        break;
    case UA_NODECLASS_REFERENCETYPE:
        UA_ReferenceTypeNode_deleteMembers((UA_ReferenceTypeNode*)&entry->node);
        break;
    case UA_NODECLASS_DATATYPE:
        UA_DataTypeNode_deleteMembers((UA_DataTypeNode*)&entry->node);
        break;
    case UA_NODECLASS_VIEW:
        UA_ViewNode_deleteMembers((UA_ViewNode*)&entry->node);
        break;
    default:
        UA_assert(UA_FALSE);
        break;
    }
    UA_free(entry);
}

/** Copies the node into the entry. Then free the original node (but not its content). */
static struct nodeEntry * nodeEntryFromNode(UA_Node *node) {
    size_t nodesize = 0;
    switch(node->nodeClass) {
    case UA_NODECLASS_OBJECT:
        nodesize = sizeof(UA_ObjectNode);
        break;
    case UA_NODECLASS_VARIABLE:
        nodesize = sizeof(UA_VariableNode);
        break;
    case UA_NODECLASS_METHOD:
        nodesize = sizeof(UA_MethodNode);
        break;
    case UA_NODECLASS_OBJECTTYPE:
        nodesize = sizeof(UA_ObjectTypeNode);
        break;
    case UA_NODECLASS_VARIABLETYPE:
        nodesize = sizeof(UA_VariableTypeNode);
        break;
    case UA_NODECLASS_REFERENCETYPE:
        nodesize = sizeof(UA_ReferenceTypeNode);
        break;
    case UA_NODECLASS_DATATYPE:
        nodesize = sizeof(UA_DataTypeNode);
        break;
    case UA_NODECLASS_VIEW:
        nodesize = sizeof(UA_ViewNode);
        break;
    default:
        UA_assert(UA_FALSE);
    }

    struct nodeEntry *newEntry;
    if(!(newEntry = UA_malloc(sizeof(struct nodeEntry) - sizeof(UA_Node) + nodesize)))
        return UA_NULL;

    UA_memcpy(&newEntry->node, node, nodesize);
    UA_free(node);
    return newEntry;
}

/**********************/
/* Exported functions */
/**********************/

UA_NodeStore * UA_NodeStore_new(void) {
    UA_NodeStore *ns;
    if(!(ns = UA_malloc(sizeof(UA_NodeStore))))
        return UA_NULL;

    ns->sizePrimeIndex = higher_prime_index(32);
    ns->size = primes[ns->sizePrimeIndex];
    ns->count = 0;
    if(!(ns->entries = UA_malloc(sizeof(struct nodeEntry *) * ns->size))) {
        UA_free(ns);
        return UA_NULL;
    }
    UA_memset(ns->entries, 0, ns->size * sizeof(struct nodeEntry *));
    return ns;
}

void UA_NodeStore_delete(UA_NodeStore *ns) {
    UA_UInt32 size = ns->size;
    struct nodeEntry **entries = ns->entries;
    for(UA_UInt32 i = 0;i < size;i++) {
        if(entries[i] != UA_NULL) {
            entries[i]->refcount &= ~ALIVE_BIT; // mark dead
            deleteEntry(entries[i]);
            entries[i] = UA_NULL;
            ns->count--;
        }
    }

    UA_free(ns->entries);
    UA_free(ns);
}

UA_StatusCode UA_NodeStore_insert(UA_NodeStore *ns, UA_Node *node, const UA_Node **inserted) {
    if(ns->size * 3 <= ns->count * 4) {
        if(expand(ns) != UA_STATUSCODE_GOOD)
            return UA_STATUSCODE_BADINTERNALERROR;
    }
    
    // get a free slot
    struct nodeEntry **slot;
    if(UA_NodeId_isNull(&node->nodeId)) {
        // find a unique nodeid that is not taken
        node->nodeId.identifierType = UA_NODEIDTYPE_NUMERIC;
        node->nodeId.namespaceIndex = 1; // namespace 1 is always in the local nodestore
        if(node->nodeClass==UA_NODECLASS_VARIABLE){ //set namespaceIndex in browseName in case id is generated
        	((UA_VariableNode*)node)->browseName.namespaceIndex=node->nodeId.namespaceIndex;
        }
        UA_Int32 identifier = ns->count+1; // start value
        UA_Int32 size = ns->size;
        hash_t increase = mod2(identifier, size);
        while(UA_TRUE) {
            node->nodeId.identifier.numeric = identifier;
            if(!containsNodeId(ns, &node->nodeId, &slot))
                break;
            identifier += increase;
            if(identifier >= size)
                identifier -= size;
        }
    } else {
        if(containsNodeId(ns, &node->nodeId, &slot))
            return UA_STATUSCODE_BADNODEIDEXISTS;
    }

    struct nodeEntry *entry = nodeEntryFromNode(node);
    if(!entry)
        return UA_STATUSCODE_BADOUTOFMEMORY;

    *slot = entry;
    ns->count++;

    if(inserted) {
        entry->refcount = ALIVE_BIT + 1;
        *inserted = &entry->node;
    } else {
        entry->refcount = ALIVE_BIT;
    }

    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_NodeStore_replace(UA_NodeStore *ns, const UA_Node *oldNode, UA_Node *node,
                                   const UA_Node **inserted) {
    struct nodeEntry **slot;
    const UA_NodeId *nodeId = &node->nodeId;
    if(!containsNodeId(ns, nodeId, &slot))
        return UA_STATUSCODE_BADNODEIDUNKNOWN;

    // you try to replace an obsolete node (without threading this can't happen
    // if the user doesn't do it deliberately in his code)
    if(&(*slot)->node != oldNode)
        return UA_STATUSCODE_BADINTERNALERROR;

    struct nodeEntry *entry = nodeEntryFromNode(node);
    if(!entry)
        return UA_STATUSCODE_BADOUTOFMEMORY;

    (*slot)->refcount &= ~ALIVE_BIT; // mark dead
    *slot = entry;

    if(inserted) {
        entry->refcount = ALIVE_BIT + 1;
        *inserted = &entry->node;
    } else {
        entry->refcount = ALIVE_BIT;
    }
    return UA_STATUSCODE_GOOD;
}

const UA_Node * UA_NodeStore_get(const UA_NodeStore *ns, const UA_NodeId *nodeid) {
    struct nodeEntry **slot;
    if(!containsNodeId(ns, nodeid, &slot))
        return UA_NULL;
    (*slot)->refcount++;
    return &(*slot)->node;
}

UA_StatusCode UA_NodeStore_remove(UA_NodeStore *ns, const UA_NodeId *nodeid) {
    struct nodeEntry **slot;
    if(!containsNodeId(ns, nodeid, &slot))
        return UA_STATUSCODE_BADNODEIDUNKNOWN;

    // Check before if deleting the node makes the UA_NodeStore inconsistent.
    (*slot)->refcount &= ~ALIVE_BIT; // mark dead
    deleteEntry(*slot);
    *slot = UA_NULL;
    ns->count--;

    /* Downsize the hashmap if it is very empty */
    if(ns->count * 8 < ns->size && ns->size > 32)
        expand(ns); // this can fail. we just continue with the bigger hashmap.

    return UA_STATUSCODE_GOOD;
}

void UA_NodeStore_iterate(const UA_NodeStore *ns, UA_NodeStore_nodeVisitor visitor) {
    for(UA_UInt32 i = 0;i < ns->size;i++) {
        if(ns->entries[i] != UA_NULL)
            visitor(&ns->entries[i]->node);
    }
}

void UA_NodeStore_release(const UA_Node *managed) {
    /* We know what we are doing here and remove a compiler warning. Nobody has
       a reference to the const pointer, so we can free it. */
    struct nodeEntry *entry = (struct nodeEntry *) ((uintptr_t)managed - offsetof(struct nodeEntry, node));
    entry->refcount--;
    deleteEntry(entry);
}
 /*
 * This work is licensed under a Creative Commons CCZero 1.0 Universal License.
 * See http://creativecommons.org/publicdomain/zero/1.0/ for more information.
 */

#ifdef NOT_AMALGATED
# define _XOPEN_SOURCE 500 //some users need this for some reason
# define __USE_BSD
# include <stdlib.h> // malloc, free
# include <stdio.h>
# include <string.h> // memset
#endif

#ifdef _WIN32
# include <malloc.h>
# include <winsock2.h>
# include <sys/types.h>
# include <windows.h>
# include <ws2tcpip.h>
# define CLOSESOCKET(S) closesocket(S)
#else
# include <sys/select.h> 
# include <netinet/in.h>
# include <netinet/tcp.h>
# include <sys/ioctl.h>
# include <netdb.h> //gethostbyname for the client
# include <unistd.h> // read, write, close
# include <arpa/inet.h>
# define CLOSESOCKET(S) close(S)
#endif

#ifdef UA_MULTITHREADING
# include <urcu/uatomic.h>
#endif

/* with a space so amalgamation does not remove the includes */
# include <errno.h> // errno, EINTR
# include <fcntl.h> // fcntl

struct ServerNetworklayer_TCP;

/* Forwarded to the server as a (UA_Connection) and used for callbacks back into
   the networklayer */
typedef struct {
	UA_Connection connection;
	UA_Int32 sockfd;
	void *layer;
} TCPConnection;

/***************************/
/* Server NetworkLayer TCP */
/***************************/

#define MAXBACKLOG 100

/* Internal mapping of sockets to connections */
typedef struct {
    TCPConnection *connection;
#ifdef _WIN32
	UA_UInt32 sockfd;
#else
	UA_Int32 sockfd;
#endif
} ConnectionLink;

typedef struct ServerNetworkLayerTCP {
	UA_ConnectionConfig conf;
	fd_set fdset;
#ifdef _WIN32
	UA_UInt32 serversockfd;
	UA_UInt32 highestfd;
#else
	UA_Int32 serversockfd;
	UA_Int32 highestfd;
#endif
    UA_UInt16 conLinksSize;
    ConnectionLink *conLinks;
    UA_UInt32 port;
    UA_String discoveryUrl;
    /* We remove the connection links only in the main thread. Attach
       to-be-deleted links with atomic operations */
    struct deleteLink {
#ifdef _WIN32
		UA_UInt32 sockfd;
#else
		UA_Int32 sockfd;
#endif
        struct deleteLink *next;

    } *deleteLinkList;
} ServerNetworkLayerTCP;

typedef struct ClientNetworkLayerTCP {
	fd_set read_fds;
#ifdef _WIN32
	UA_UInt32 sockfd;
#else
	UA_Int32 sockfd;
#endif
} ClientNetworkLayerTCP;

static UA_StatusCode setNonBlocking(int sockid) {
#ifdef _WIN32
	u_long iMode = 1;
	if(ioctlsocket(sockid, FIONBIO, &iMode) != NO_ERROR)
		return UA_STATUSCODE_BADINTERNALERROR;
#else
	int opts = fcntl(sockid,F_GETFL);
	if(opts < 0 || fcntl(sockid,F_SETFL,opts|O_NONBLOCK) < 0)
		return UA_STATUSCODE_BADINTERNALERROR;
#endif
	return UA_STATUSCODE_GOOD;
}

static void freeConnectionCallback(UA_Server *server, TCPConnection *connection) {
    UA_ByteString_deleteMembers(&connection->connection.incompleteMessage);
    free(connection);
}

// after every select, reset the set of sockets we want to listen on
static void setFDSet(ServerNetworkLayerTCP *layer) {
	FD_ZERO(&layer->fdset);
	FD_SET(layer->serversockfd, &layer->fdset);
	layer->highestfd = layer->serversockfd;
	for(UA_Int32 i=0;i<layer->conLinksSize;i++) {
		FD_SET(layer->conLinks[i].sockfd, &layer->fdset);
		if(layer->conLinks[i].sockfd > layer->highestfd)
			layer->highestfd = layer->conLinks[i].sockfd;
	}
}

// the callbacks are thread-safe if UA_MULTITHREADING is defined
void closeConnection(TCPConnection *handle);
void writeCallback(TCPConnection *handle, UA_ByteStringArray gather_buf);

static UA_StatusCode ServerNetworkLayerTCP_add(ServerNetworkLayerTCP *layer, UA_Int32 newsockfd) {
    setNonBlocking(newsockfd);
    TCPConnection *c = malloc(sizeof(TCPConnection));
	if(!c)
		return UA_STATUSCODE_BADINTERNALERROR;
	c->sockfd = newsockfd;
    c->layer = layer;
    c->connection.state = UA_CONNECTION_OPENING;
    c->connection.localConf = layer->conf;
    c->connection.channel = (void*)0;
    c->connection.close = (void (*)(void*))closeConnection;
    c->connection.write = (void (*)(void*, UA_ByteStringArray))writeCallback;
    UA_ByteString_init(&c->connection.incompleteMessage);

    layer->conLinks = realloc(layer->conLinks, sizeof(ConnectionLink)*(layer->conLinksSize+1));
	if(!layer->conLinks) {
		free(c);
		return UA_STATUSCODE_BADINTERNALERROR;
	}
    layer->conLinks[layer->conLinksSize].connection = c;
    layer->conLinks[layer->conLinksSize].sockfd = newsockfd;
    layer->conLinksSize++;
	return UA_STATUSCODE_GOOD;
}

/* Removes all connections from the network layer. Returns the work items to close them properly. */
static UA_UInt32 removeAllConnections(ServerNetworkLayerTCP *layer, UA_WorkItem **returnWork) {
    UA_WorkItem *work;
	if (layer->conLinksSize <= 0 || !(work = malloc(sizeof(UA_WorkItem)*layer->conLinksSize))) {
		*returnWork = NULL;
		return 0;
	}
#ifdef UA_MULTITHREADING
    struct deleteLink *d = uatomic_xchg(&layer->deleteLinkList, (void*)0);
#else
    struct deleteLink *d = layer->deleteLinkList;
    layer->deleteLinkList = (void*)0;
#endif
    UA_UInt32 count = 0;
    while(d) {
        UA_Int32 i;
        for(i = 0;i<layer->conLinksSize;i++) {
            if(layer->conLinks[i].sockfd == d->sockfd)
                break;
        }
        if(i < layer->conLinksSize) {
            TCPConnection *c = layer->conLinks[i].connection;
            layer->conLinksSize--;
            layer->conLinks[i] = layer->conLinks[layer->conLinksSize];
            work[count] = (UA_WorkItem)
                {.type = UA_WORKITEMTYPE_DELAYEDMETHODCALL,
                 .work.methodCall = {.data = c,
                                     .method = (void (*)(UA_Server*,void*))freeConnectionCallback} };
        }
        struct deleteLink *oldd = d;
        d = d->next;
        free(oldd);
        count++;
    }
    *returnWork = work;
    return count;
}

#ifdef UA_MULTITHREADING
void closeConnection(TCPConnection *handle) {
    if(uatomic_xchg(&handle->connection.state, UA_CONNECTION_CLOSING) == UA_CONNECTION_CLOSING)
        return;
    
    UA_Connection_detachSecureChannel(&handle->connection);
	shutdown(handle->sockfd,2);
	CLOSESOCKET(handle->sockfd);

    ServerNetworkLayerTCP *layer = (ServerNetworkLayerTCP*)handle->layer;

    // Remove the link later in the main thread
    struct deleteLink *d = malloc(sizeof(struct deleteLink));
    d->sockfd = handle->sockfd;
    while(1) {
        d->next = layer->deleteLinkList;
        if(uatomic_cmpxchg(&layer->deleteLinkList, d->next, d) == d->next)
            break;
    }
}
#else
void closeConnection(TCPConnection *handle) {
    if(handle->connection.state == UA_CONNECTION_CLOSING)
        return;

	struct deleteLink *d = malloc(sizeof(struct deleteLink));
	if(!d)
		return;
    handle->connection.state = UA_CONNECTION_CLOSING;

    UA_Connection_detachSecureChannel(&handle->connection);
	shutdown(handle->sockfd,2);
	CLOSESOCKET(handle->sockfd);

    // Remove the link later in the main thread
    d->sockfd = handle->sockfd;
    ServerNetworkLayerTCP *layer = (ServerNetworkLayerTCP*)handle->layer;
    d->next = layer->deleteLinkList;
    layer->deleteLinkList = d;
}
#endif

/** Accesses only the sockfd in the handle. Can be run from parallel threads. */
void writeCallback(TCPConnection *handle, UA_ByteStringArray gather_buf) {
	UA_UInt32 total_len = 0, nWritten = 0;
#ifdef _WIN32
	LPWSABUF buf = _alloca(gather_buf.stringsSize * sizeof(WSABUF));
	memset(buf, 0, sizeof(gather_buf.stringsSize * sizeof(WSABUF)));
	int result = 0;
	for(UA_UInt32 i = 0; i<gather_buf.stringsSize; i++) {
		buf[i].buf = (char*)gather_buf.strings[i].data;
		buf[i].len = gather_buf.strings[i].length;
		total_len += gather_buf.strings[i].length;
	}
	while(nWritten < total_len) {
		UA_UInt32 n = 0;
		do {
			result = WSASend(handle->sockfd, buf, gather_buf.stringsSize ,
                             (LPDWORD)&n, 0, NULL, NULL);
			if(result != 0)
				printf("Error WSASend, code: %d \n", WSAGetLastError());
		} while(errno == EINTR);
		nWritten += n;
	}
#else
	struct iovec iov[gather_buf.stringsSize];
	memset(iov, 0, sizeof(struct iovec)*gather_buf.stringsSize);
	for(UA_UInt32 i=0;i<gather_buf.stringsSize;i++) {
		iov[i].iov_base = gather_buf.strings[i].data;
		iov[i].iov_len = gather_buf.strings[i].length;
		total_len += gather_buf.strings[i].length;
	}
	struct msghdr message;
	memset(&message, 0, sizeof(message));
	message.msg_iov = iov;
	message.msg_iovlen = gather_buf.stringsSize;
	while (nWritten < total_len) {
		UA_Int32 n = 0;
		do {
            n = sendmsg(handle->sockfd, &message, 0);
        } while (n == -1L && errno == EINTR);
        nWritten += n;
	}
#endif
}

static UA_StatusCode ServerNetworkLayerTCP_start(ServerNetworkLayerTCP *layer, UA_Logger *logger) {
#ifdef _WIN32
	if((layer->serversockfd = socket(PF_INET, SOCK_STREAM,0)) == INVALID_SOCKET) {
		printf("ERROR opening socket, code: %d\n", WSAGetLastError());
		return UA_STATUSCODE_BADINTERNALERROR;
	}
#else
    if((layer->serversockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("ERROR opening socket");
		return UA_STATUSCODE_BADINTERNALERROR;
	} 
#endif

	const struct sockaddr_in serv_addr = {
        .sin_family = AF_INET, .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(layer->port), .sin_zero = {0}};

	int optval = 1;
	if(setsockopt(layer->serversockfd, SOL_SOCKET,
                  SO_REUSEADDR, (const char *)&optval,
                  sizeof(optval)) == -1) {
		perror("setsockopt");
		CLOSESOCKET(layer->serversockfd);
		return UA_STATUSCODE_BADINTERNALERROR;
	}
		
	if(bind(layer->serversockfd, (const struct sockaddr *)&serv_addr,
            sizeof(serv_addr)) < 0) {
		perror("binding");
		CLOSESOCKET(layer->serversockfd);
		return UA_STATUSCODE_BADINTERNALERROR;
	}

	setNonBlocking(layer->serversockfd);
	listen(layer->serversockfd, MAXBACKLOG);
    UA_LOG_INFO((*logger), UA_LOGGERCATEGORY_SERVER, "Listening on %.*s",
                layer->discoveryUrl.length, layer->discoveryUrl.data);
    return UA_STATUSCODE_GOOD;
}

static UA_Int32 ServerNetworkLayerTCP_getWork(ServerNetworkLayerTCP *layer, UA_WorkItem **workItems,
                                        UA_UInt16 timeout) {
    UA_WorkItem *items = (void*)0;
    UA_Int32 itemsCount = removeAllConnections(layer, &items);
    setFDSet(layer);
    struct timeval tmptv = {0, timeout};
    UA_Int32 resultsize = select(layer->highestfd+1, &layer->fdset, NULL, NULL, &tmptv);

    if(resultsize < 0) {
        *workItems = items;
        return itemsCount;
    }

	// accept new connections (can only be a single one)
	if(FD_ISSET(layer->serversockfd,&layer->fdset)) {
		resultsize--;
		struct sockaddr_in cli_addr;
		socklen_t cli_len = sizeof(cli_addr);
		int newsockfd = accept(layer->serversockfd, (struct sockaddr *) &cli_addr, &cli_len);
		int i = 1;
		setsockopt(newsockfd, IPPROTO_TCP, TCP_NODELAY, (void *)&i, sizeof(i));
		if (newsockfd >= 0)
			ServerNetworkLayerTCP_add(layer, newsockfd);
	}
    
    items = realloc(items, sizeof(UA_WorkItem)*(itemsCount+resultsize));

	// read from established sockets
    UA_Int32 j = itemsCount;
	UA_ByteString buf = { -1, NULL};
	for(UA_Int32 i=0;i<layer->conLinksSize && j<itemsCount+resultsize;i++) {
		if(!(FD_ISSET(layer->conLinks[i].sockfd, &layer->fdset)))
            continue;

		if(!buf.data) {
			buf.data = malloc(sizeof(UA_Byte) * layer->conf.recvBufferSize);
			if(!buf.data)
				break;
		}
        
#ifdef _WIN32
        buf.length = recv(layer->conLinks[i].sockfd, (char *)buf.data,
                          layer->conf.recvBufferSize, 0);
#else
        buf.length = read(layer->conLinks[i].sockfd, buf.data, layer->conf.recvBufferSize);
#endif
        if (buf.length <= 0) {
            closeConnection(layer->conLinks[i].connection); // work is returned in the next iteration
        } else {
            items[j].type = UA_WORKITEMTYPE_BINARYNETWORKMESSAGE;
            items[j].work.binaryNetworkMessage.message = buf;
            items[j].work.binaryNetworkMessage.connection = &layer->conLinks[i].connection->connection;
            buf.data = NULL;
            j++;
        }
    }

    if(buf.data)
        free(buf.data);

    if(j == 0) {
        free(items);
        *workItems = NULL;
    } else
        *workItems = items;
    return j;
}

static UA_Int32 ServerNetworkLayerTCP_stop(ServerNetworkLayerTCP * layer, UA_WorkItem **workItems) {
	for(UA_Int32 index = 0;index < layer->conLinksSize;index++)
        closeConnection(layer->conLinks[index].connection);
#ifdef _WIN32
	WSACleanup();
#endif
    return removeAllConnections(layer, workItems);
}

static void ServerNetworkLayerTCP_delete(ServerNetworkLayerTCP *layer) {
	UA_String_deleteMembers(&layer->discoveryUrl);
	for(UA_Int32 i=0;i<layer->conLinksSize;++i){
		free(layer->conLinks[i].connection);
	}
	free(layer->conLinks);
	free(layer);
}

UA_ServerNetworkLayer ServerNetworkLayerTCP_new(UA_ConnectionConfig conf, UA_UInt32 port) {
#ifdef _WIN32
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	WSAStartup(wVersionRequested, &wsaData);
#endif
    ServerNetworkLayerTCP *tcplayer = malloc(sizeof(ServerNetworkLayerTCP));
	tcplayer->conf = conf;
	tcplayer->conLinksSize = 0;
	tcplayer->conLinks = NULL;
    tcplayer->port = port;
    tcplayer->deleteLinkList = (void*)0;
    char hostname[256];
    gethostname(hostname, 255);
    UA_String_copyprintf("opc.tcp://%s:%d", &tcplayer->discoveryUrl, hostname, port);

    UA_ServerNetworkLayer nl;
    nl.nlHandle = tcplayer;
    nl.start = (UA_StatusCode (*)(void*, UA_Logger *logger))ServerNetworkLayerTCP_start;
    nl.getWork = (UA_Int32 (*)(void*, UA_WorkItem**, UA_UInt16))ServerNetworkLayerTCP_getWork;
    nl.stop = (UA_Int32 (*)(void*, UA_WorkItem**))ServerNetworkLayerTCP_stop;
    nl.free = (void (*)(void*))ServerNetworkLayerTCP_delete;
	nl.discoveryUrl = &tcplayer->discoveryUrl;

    return nl;
}

/***************************/
/* Client NetworkLayer TCP */
/***************************/

static UA_StatusCode ClientNetworkLayerTCP_connect(const UA_String endpointUrl, ClientNetworkLayerTCP *resultHandle) {
	if(endpointUrl.length < 11 || endpointUrl.length >= 512) {
        printf("server url size invalid\n");
        return UA_STATUSCODE_BADINTERNALERROR;
    }

    if(strncmp((char*)endpointUrl.data, "opc.tcp://", 10) != 0) {
        printf("server url does not begin with opc.tcp://\n");
        return UA_STATUSCODE_BADINTERNALERROR;
    }

    //this is somewhat ugly, but atoi needs a c string
    char cstringEndpointUrl[endpointUrl.length+1];
    memset(cstringEndpointUrl, 0, endpointUrl.length+1);
    memcpy(cstringEndpointUrl, endpointUrl.data, endpointUrl.length);
    cstringEndpointUrl[endpointUrl.length+1] = '0';

    UA_UInt16 portpos = 9;
    UA_UInt16 port = 0;
    for(;portpos < endpointUrl.length; portpos++) {
        if(endpointUrl.data[portpos] == ':') {
            port = atoi(&cstringEndpointUrl[portpos+1]);
            break;
        }
    }
    if(port == 0) {
        printf("port invalid");
        return UA_STATUSCODE_BADINTERNALERROR;
    }
    
    char hostname[512];
    for(int i=10; i < portpos; i++)
        hostname[i-10] = endpointUrl.data[i];
    hostname[portpos-10] = 0;

#ifdef _WIN32
    UA_UInt32 sock = 0;
#else
    UA_Int32 sock = 0;
#endif
#ifdef _WIN32
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	WSAStartup(wVersionRequested, &wsaData);
    if((sock = socket(PF_INET, SOCK_STREAM,0)) == INVALID_SOCKET) {
#else
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
#endif
		printf("Could not create socket\n");
        return UA_STATUSCODE_BADINTERNALERROR;
    }

    struct hostent *server;
    server = gethostbyname(hostname);
    if (server == NULL) {
        printf("DNS lookup of %s failed\n", hostname);
        return UA_STATUSCODE_BADINTERNALERROR;
    }

	struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    memcpy((char *)&server_addr.sin_addr.s_addr,
    	 (char *)server->h_addr_list[0],
         server->h_length);

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	if(connect(sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        printf("Connect failed.\n");
        return UA_STATUSCODE_BADINTERNALERROR;
    }
    //if(setNonBlocking(*sock) != UA_STATUSCODE_GOOD) {
    //    printf("Could not switch to nonblocking.\n");
    //	FINALLY
    //    return UA_STATUSCODE_BADINTERNALERROR;
    //}
    resultHandle->sockfd = sock;
    return UA_STATUSCODE_GOOD;
}

static void ClientNetworkLayerTCP_disconnect(ClientNetworkLayerTCP* handle) {
	CLOSESOCKET(handle->sockfd);
#ifdef _WIN32
	WSACleanup();
#endif
}

static UA_StatusCode ClientNetworkLayerTCP_send(ClientNetworkLayerTCP *handle, UA_ByteStringArray gather_buf) {
	UA_UInt32 total_len = 0, nWritten = 0;
#ifdef _WIN32
	LPWSABUF buf = _alloca(gather_buf.stringsSize * sizeof(WSABUF));
	int result = 0;
	for(UA_UInt32 i = 0; i<gather_buf.stringsSize; i++) {
		buf[i].buf = (char*)gather_buf.strings[i].data;
		buf[i].len = gather_buf.strings[i].length;
		total_len += gather_buf.strings[i].length;
	}
	while(nWritten < total_len) {
		UA_UInt32 n = 0;
		do {
			result = WSASend(handle->sockfd, buf, gather_buf.stringsSize ,
                             (LPDWORD)&n, 0, NULL, NULL);
			if(result != 0)
				printf("Error WSASend, code: %d \n", WSAGetLastError());
		} while(errno == EINTR);
		nWritten += n;
	}
#else
	struct iovec iov[gather_buf.stringsSize];
	for(UA_UInt32 i=0;i<gather_buf.stringsSize;i++) {
		iov[i] = (struct iovec) {.iov_base = gather_buf.strings[i].data,
                                 .iov_len = gather_buf.strings[i].length};
		total_len += gather_buf.strings[i].length;
	}
	struct msghdr message = {.msg_name = NULL, .msg_namelen = 0, .msg_iov = iov,
							 .msg_iovlen = gather_buf.stringsSize, .msg_control = NULL,
							 .msg_controllen = 0, .msg_flags = 0};
	while (nWritten < total_len) {
        int n = sendmsg(handle->sockfd, &message, 0);
        if(n <= -1)
            return UA_STATUSCODE_BADINTERNALERROR;
        nWritten += n;
	}
#endif
    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode
ClientNetworkLayerTCP_awaitResponse(ClientNetworkLayerTCP *handle, UA_ByteString *response,
                                    UA_UInt32 timeout) {
    struct timeval tmptv = {0, timeout};
    setsockopt(handle->sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tmptv,sizeof(struct timeval));
    int ret = recv(handle->sockfd, (char*)response->data, response->length, 0);
    if(ret <= -1)
    	return UA_STATUSCODE_BADINTERNALERROR;
    if(ret == 0)
        return UA_STATUSCODE_BADSERVERNOTCONNECTED;
    response->length = ret;
    return UA_STATUSCODE_GOOD;
}

static void ClientNetworkLayerTCP_delete(ClientNetworkLayerTCP *layer) {
	if(layer)
		free(layer);
}

UA_ClientNetworkLayer ClientNetworkLayerTCP_new(UA_ConnectionConfig conf) {
	ClientNetworkLayerTCP *tcplayer = malloc(sizeof(ClientNetworkLayerTCP));
	tcplayer->sockfd = 0;

    UA_ClientNetworkLayer layer;
    layer.nlHandle = tcplayer;
    layer.connect = (UA_StatusCode (*)(const UA_String, void**)) ClientNetworkLayerTCP_connect;
    layer.disconnect = (void (*)(void*)) ClientNetworkLayerTCP_disconnect;
    layer.destroy = (void (*)(void*)) ClientNetworkLayerTCP_delete;
    layer.send = (UA_StatusCode (*)(void*, UA_ByteStringArray)) ClientNetworkLayerTCP_send;
    layer.awaitResponse = (UA_StatusCode (*)(void*, UA_ByteString *, UA_UInt32))ClientNetworkLayerTCP_awaitResponse;
    return layer;
}
/*
 * This work is licensed under a Creative Commons CCZero 1.0 Universal License.
 * See http://creativecommons.org/publicdomain/zero/1.0/ for more information.
 */


static void print_time(void) {
	UA_DateTime now = UA_DateTime_now();
	UA_ByteString str;
	UA_DateTime_toString(now, &str);
	printf("%.27s", str.data); //a bit hacky way not to display nanoseconds
	UA_ByteString_deleteMembers(&str);
}

#define LOG_FUNCTION(LEVEL) \
	static void log_##LEVEL(UA_LoggerCategory category, const char *msg, ...) { \
        printf("[");                                                    \
		print_time();                                                   \
        va_list ap;                                                     \
        va_start(ap, msg);                                              \
        printf("] " #LEVEL "/%s\t", UA_LoggerCategoryNames[category]);  \
        vprintf(msg, ap);                                               \
        printf("\n");                                                   \
        va_end(ap);                                                     \
	}

//#if defined(__GNUC__) || defined(__clang__)
//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wformat-nonliteral"
//#endif
LOG_FUNCTION(trace)
LOG_FUNCTION(debug)
LOG_FUNCTION(info)
LOG_FUNCTION(warning)
LOG_FUNCTION(error)
LOG_FUNCTION(fatal)
//#if defined(__GNUC__) || defined(__clang__)
//#pragma GCC diagnostic pop
//#endif

UA_Logger Logger_Stdout_new(void) {
	return (UA_Logger){
		.log_trace = log_trace,
		.log_debug = log_debug,
		.log_info = log_info,
		.log_warning = log_warning,
		.log_error = log_error,
		.log_fatal = log_fatal
	};
}
