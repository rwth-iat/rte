/*
 * acplt_simpleMsgCreator.c
 *
 *  Created on: 24.09.2013
 *      Author: lars
 */
#include "libov/ov_memstack.h"
#include "libov/ov_ov.h"
#include "libov/ov_string.h"
#include "libov/ov_logfile.h"
#include "acplt_simpleMsgHandling.h"

static OV_INT msgID = 0x48ad3548;

	/*	statics for Message Generation	*/

static const char msgHdr [] = "hdr";
static const char msgRcvSysAdr [] = "rcvSysAdr";
static const char msgRcvLocAdr [] = "rcvLocAdr";
static const char msgId [] = "msgId";
static const char msgSndSysAdr [] = "sndSysAdr";
static const char msgSndLocAdr [] = "sndLocAdr";
static const char msgRefId [] = "refMsgId";
static const char msgAuth [] = "auth";

static const char msgBdy [] = "bdy";
static const char msgSvc [] = "Svc=\"";
static const char msgOp [] = "Op=\"";
static const char msgVal [] = "val";
static const char msgValId [] = "id=\"";
static const char msgValType [] = "type=\"";
static const char msgValUnit [] = "unit=\"";
static const char msgSd [] = "sd";



#define GEN_ADD_TAG(tagname)	*tempinside = '<';	\
		tempinside++;	\
		memcpy(tempinside,tagname, sizeof(tagname)-1);	\
		tempinside += sizeof(tagname)-1;	\
		*tempinside = '>';	\
		tempinside++

#define GEN_ADD_TAGBEGIN(tagname)	*tempinside = '<';	\
		tempinside++;	\
		memcpy(tempinside,tagname, sizeof(tagname)-1);	\
		tempinside += sizeof(tagname)-1

#define GEN_ADD_CLOSETAG(tagname)	*tempinside = '<';	\
		tempinside++;	\
		*tempinside = '/';	\
		tempinside++;	\
		memcpy(tempinside,tagname, sizeof(tagname)-1);	\
		tempinside += sizeof(tagname)-1;	\
		*tempinside = '>';	\
		tempinside++

#define GEN_ADD_EMPTYTAG(tagname)	*tempinside = '<';	\
		tempinside++;	\
		memcpy(tempinside,tagname, sizeof(tagname)-1);	\
		tempinside += sizeof(tagname)-1;	\
		*tempinside = '/';	\
		tempinside++;	\
		*tempinside = '>';	\
		tempinside++

/*
 * generates an xml-MessageHeader from the header-structure
 * if msdId is NULL in header structure an internal counter will be used
 */
OV_DLLFNCEXPORT OV_STRING acplt_simpleMsg_generateMsgHeader(ACPLT_MSGHEADER const * header)
{
	OV_STRING temp = NULL;
	OV_STRING tempinside = NULL;
	OV_UINT length = 0;
	OV_UINT rcvSysAdrLength = 0;
	OV_UINT rcvLocAdrLength = 0;
	OV_UINT msgIdLength = 0;
	OV_UINT sndSysAdrLength = 0;
	OV_UINT sndLocAdrLength = 0;
	OV_UINT refMsgIdLength = 0;
	OV_UINT authLength = 0;

	/*	hdr element (<>+</> + 2*$msgHdr)	*/
	length = 2+3+2*sizeof(msgHdr);
	/*	receiver System Address 	*/
	if(header->rcvSysAdr)
	{	/*	<> + </> + 	2*$msgRcvSysAdr + $rvcSysAdrLength	*/
		rcvSysAdrLength = strlen(header->rcvSysAdr);
		length += 2+3+2*(sizeof(msgRcvSysAdr)-1)+rcvSysAdrLength;		/*	no terminating \0	*/
	}
	else
	{	/*	</$msgRcvSysAdr>	*/
		length += 3+sizeof(msgRcvSysAdr)-1;
	}

	/*	receiver local Address 	*/
	if(header->rcvLocAdr)
	{	/*	<> + </> + 	2*$msgLocSysAdr + $rvcLocAdrLength	*/
		rcvLocAdrLength = strlen(header->rcvLocAdr);
		length += 2+3+2*(sizeof(msgRcvLocAdr)-1)+rcvLocAdrLength;
	}
	else
	{	/*	recevier local address cannot be empty	*/
		ov_logfile_error("generateRegMsgHeader: receiver local address empty");
		return NULL;
	}

	/*	message ID	*/
	if(header->msgId)
	{	/*	<> + </> + 	2*$msgId + $msgIdLength	*/
		msgIdLength = strlen(header->msgId);
		length += 2+3+2*(sizeof(msgId)-1)+msgIdLength;
	}
	else
	{	/*	<> + </> + 2*$msgId + 10 (max length od UINT)	*/
		msgIdLength = 10;
		length += 2+3+2*(sizeof(msgId)-1)+10;
	}

	/*	reference message ID	*/
	if(header->refMsgId)
	{	/*	<> + </> + 	2*$msgRefId + $refMsgIdLength	*/
		refMsgIdLength = strlen(header->refMsgId);
		length += 2+3+2*(sizeof(msgRefId)-1)+refMsgIdLength;
	}

	/*	sender system address	*/
	if(header->sndSysAdr)
	{	/*	<> + </> + 	2*$msgSndSysAdr + $sndSysAdrLength 	*/
		sndSysAdrLength = strlen(header->sndSysAdr);
		length += 2+3+2*sizeof(msgSndSysAdr)+sndSysAdrLength;
	}

	/*	sender local address	*/
	if(header->sndLocAdr)
	{	/*	<> + </> + 	2*$msgSndLocAdr + $sndLocAdrLength 	*/
		sndLocAdrLength = strlen(header->sndLocAdr);
		length += 2+3+2*(sizeof(msgSndLocAdr)-1)+sndLocAdrLength;
	}

	/*	auth	*/
	if(header->auth)
	{	/*	<> + </> + 	2*$msgAuth + $authLength 	*/
		authLength = strlen((const char*) header->auth);
		length += 2+3+2*(sizeof(msgAuth)-1)+authLength;
	}

	temp = ov_memstack_alloc((length+1) * sizeof(char));
	if(!temp)
	{
		ov_logfile_error("generateMsgHeader: heap out of memory");
		return NULL;
	}

	tempinside = temp;

	/*	begin header	*/
	GEN_ADD_TAG(msgHdr);

	/*	receiver System Address	*/
	if(header->rcvSysAdr)
	{
		GEN_ADD_TAG(msgRcvSysAdr);
		memcpy(tempinside, header->rcvSysAdr, rcvSysAdrLength);
		tempinside += rcvSysAdrLength;
		GEN_ADD_CLOSETAG(msgRcvSysAdr);
	}
	else
	{
		GEN_ADD_EMPTYTAG(msgRcvSysAdr);
	}

	/*	receiver local Address	*/
	GEN_ADD_TAG(msgRcvLocAdr);
	memcpy(tempinside, header->rcvLocAdr, rcvLocAdrLength);
	tempinside += rcvLocAdrLength;
	GEN_ADD_CLOSETAG(msgRcvLocAdr);

	/*	message ID	*/
	GEN_ADD_TAG(msgId);
	if(header->msgId)
	{
		memcpy(tempinside, header->msgId, msgIdLength);
		tempinside += msgIdLength;
	}
	else
	{
		snprintf(tempinside, 11, "%i", msgID);
		tempinside += strlen(tempinside);
	}
	GEN_ADD_CLOSETAG(msgId);

	/*	reference message id	*/
	if(header->refMsgId)
	{
		GEN_ADD_TAG(msgRefId);
		memcpy(tempinside, header->refMsgId, refMsgIdLength);
		tempinside += refMsgIdLength;
		GEN_ADD_CLOSETAG(msgRefId);
	}

	/*	sender System Address	*/
	if(header->sndSysAdr)
	{
		GEN_ADD_TAG(msgSndSysAdr);
		memcpy(tempinside, header->sndSysAdr, sndSysAdrLength);
		tempinside += sndSysAdrLength;
		GEN_ADD_CLOSETAG(msgSndSysAdr);
	}

	/*	sender local Address	*/
	if(header->sndLocAdr)
	{
		GEN_ADD_TAG(msgSndLocAdr);
		memcpy(tempinside, header->sndLocAdr, sndLocAdrLength);
		tempinside += sndLocAdrLength;
		GEN_ADD_CLOSETAG(msgSndLocAdr);
	}

	/*	auth	*/
	if(header->auth)
	{
		GEN_ADD_TAG(msgAuth);
		memcpy(tempinside, header->auth, authLength);
		tempinside += authLength;
		GEN_ADD_CLOSETAG(msgAuth);
	}

	/*	close header	*/
	GEN_ADD_CLOSETAG(msgHdr);
	/*	terminating '\0'	*/
	*tempinside = '\0';
	return temp;
}


/*
 * generates a message with flat body form from strings service, operation and respective val-definitions
 * uses the memstack. don't forget to call lock/unlock around it.
 * createContainerObject = TRUE tells the function to put a containing sd-element around the val-elements. the name of this element
 * can be specified via containerName
 * ids, values, units and types have to be of same length. each set of entries [i] defines one <val> element. the entries may be empty
 */
OV_DLLFNCEXPORT OV_STRING acplt_simpleMsg_GenerateFlatBody(const OV_STRING service, const OV_STRING operation,
		OV_BOOL createContainingObject, const OV_STRING containerName,
		OV_STRING_VEC* ids, OV_STRING_VEC* values, OV_STRING_VEC* units, OV_STRING_VEC* types)
{
	OV_STRING temp = NULL;
	OV_UINT svcLength = 0;
	OV_UINT opLength = 0;
	OV_UINT containerNameLength = 0;
	OV_UINT itemLength = 0;
	OV_STRING tempinside = NULL;
	OV_UINT length = 0;
	OV_UINT i;

	if(ids->veclen != values->veclen || ids->veclen != units->veclen || ids->veclen != types->veclen)
	{
		ov_logfile_error("GenerateFlatBody: vectors are not of same length");
		return NULL;
	}

	/*	determine message length (memory!)	*/
	/*	bdy element (<> + </> + 2*msgBdy)	*/
	length = 2+3+2*sizeof(msgBdy);

	/*	containing sd if wanted	*/
	if(createContainingObject)
	{	/*	(<> + </> + 2*msgSd)	*/
		length += 2+3+2*(sizeof(msgSd)-1);
		if(containerName)
		{
			containerNameLength = strlen(containerName);
			length += 1+(sizeof(msgValId)-1)+containerNameLength+1;
		}
	}
	/*	service 	*/
	if(service)
	{	/*	' ' +$msgSvc+$svcLength+"	*/
		svcLength = strlen(service);
		length += 1+(sizeof(msgSvc)-1)+svcLength+1;		/*	no terminating \0	*/
	}

	/*	operation	*/
	if(operation)
	{	/*	' '+$msgOp+$opLength+"	*/
		opLength = strlen(operation);
		length += 1+(sizeof(msgOp)-1)+opLength+1;		/*	no terminating \0	*/
	}

	for(i=0; i<ids->veclen; i++)
	{	/*	iterate over all values and reserve space	*/
		/*	generic val element <>+$msgVal	*/
		length += 2+sizeof(msgVal)-1;
		/*	value	*/
		if(values->value[i])
		{
			length += strlen(values->value[i]);
		}
		else
		{	/*	empty value /	*/
			length += 1;
		}
		/*	id ' '+$msgValId+idLength+"	*/
		if(ids->value[i])
		{
			length += 1+(sizeof(msgValId)-1)+strlen(ids->value[i])+1;
		}
		/*	type ' '+$msgValType+typeLength+"	*/
		if(types->value[i])
		{
			length += 1+(sizeof(msgValType)-1)+strlen(types->value[i])+1;
		}
		/*	unit ' '+$msgValUnit+unitLength+"	*/
		if(units->value[i])
		{
			length += 1+(sizeof(msgValUnit)-1)+strlen(units->value[i])+1;
		}
	}

	/*	length determined; allocate memory and concatenate elements and attributes	*/
	temp = ov_memstack_alloc((length+1) * sizeof(char));
	if(!temp)
	{
		ov_logfile_error("generateFlatBody: heap out of memory");
		return NULL;
	}
	tempinside = temp;


	/*	build body	*/
	GEN_ADD_TAGBEGIN(msgBdy);
	/*	service 	*/
	if(service)
	{
		*tempinside = ' ';
		tempinside++;
		memcpy(tempinside, msgSvc, sizeof(msgSvc)-1);
		tempinside += sizeof(msgSvc)-1;
		memcpy(tempinside, service, svcLength);
		tempinside += svcLength;
		*tempinside = '"';
		tempinside++;
	}

	/*	operation	*/
	if(operation)
	{
		*tempinside = ' ';
		tempinside++;
		memcpy(tempinside, msgOp, sizeof(msgOp)-1);
		tempinside += sizeof(msgOp)-1;
		memcpy(tempinside, operation, opLength);
		tempinside += opLength;
		*tempinside = '"';
		tempinside++;
	}

	*tempinside = '>';
	tempinside++;

	/*	containing sd-element if wanted	*/
	if(createContainingObject)
	{
		GEN_ADD_TAGBEGIN(msgSd);
		/*	id 	*/
		if(containerName)
		{
			*tempinside = ' ';
			tempinside++;
			memcpy(tempinside, msgValId, sizeof(msgValId)-1);
			tempinside += sizeof(msgValId)-1;
			memcpy(tempinside, containerName, containerNameLength);
			tempinside += containerNameLength;
			*tempinside = '"';
			tempinside++;
		}
		*tempinside = '>';
		tempinside++;
	}

	/*	values	*/
	for(i=0; i<ids->veclen; i++)
	{	/*	iterate over all values and concatenate	*/

		GEN_ADD_TAGBEGIN(msgVal);
		/*	id 	*/
		if(ids->value[i])
		{
			*tempinside = ' ';
			tempinside++;
			memcpy(tempinside, msgValId, sizeof(msgValId)-1);
			tempinside += sizeof(msgValId)-1;
			itemLength = strlen(ids->value[i]);
			memcpy(tempinside, ids->value[i], itemLength);
			tempinside += itemLength;
			*tempinside = '"';
			tempinside++;
		}
		/*	type	*/
		if(types->value[i])
		{
			*tempinside = ' ';
			tempinside++;
			memcpy(tempinside, msgValType, sizeof(msgValType)-1);
			tempinside += sizeof(msgValType)-1;
			itemLength = strlen(types->value[i]);
			memcpy(tempinside, types->value[i], itemLength);
			tempinside += itemLength;
			*tempinside = '"';
			tempinside++;
		}
		/*	unit	*/
		if(units->value[i])
		{
			*tempinside = ' ';
			tempinside++;
			memcpy(tempinside, msgValUnit, sizeof(msgValUnit)-1);
			tempinside += sizeof(msgValUnit)-1;
			itemLength = strlen(units->value[i]);
			memcpy(tempinside, units->value[i], itemLength);
			tempinside += itemLength;
			*tempinside = '"';
			tempinside++;
		}
		/*	value	*/
		if(values->value[i])
		{
			*tempinside = '>';
			tempinside++;
			itemLength = strlen(values->value[i]);
			memcpy(tempinside, values->value[i], itemLength);
			tempinside += itemLength;
			GEN_ADD_CLOSETAG(msgVal);
		}
		else
		{
			*tempinside = '/';
			tempinside++;
			*tempinside = '>';
			tempinside++;
		}
	}

	/*	containing sd-element if wanted	*/
	if(createContainingObject)
	{
		GEN_ADD_CLOSETAG(msgSd);
	}

	GEN_ADD_CLOSETAG(msgBdy);
	/*	terminating '\0'	*/
	*tempinside = '\0';
	return temp;
}
