/*
 * acplt_simpleMsgLib.h
 *
 *  Created on: 24.09.2013
 *      Author: lars
 */

#ifndef OV_COMPILE_LIBRARY_MessageSys
#define OV_COMPILE_LIBRARY_MessageSys
#endif



typedef struct {
	OV_STRING rcvSysAdr;
	OV_STRING rcvLocAdr;
	OV_STRING msgId;
	OV_STRING refMsgId;
	OV_STRING sndSysAdr;
	OV_STRING sndLocAdr;
	OV_STRING auth;
}	ACPLT_MSGHEADER;

OV_DLLFNCEXPORT OV_RESULT acplt_simpleMsg_xml_findElementBegin(char const* xml, const OV_STRING elemName, OV_STRING* pStart);
OV_DLLFNCEXPORT OV_RESULT acplt_simpleMsg_xml_getElementData(char const* xml, const OV_STRING elemName, OV_STRING* pData);
OV_DLLFNCEXPORT OV_RESULT acplt_simpleMsg_xml_getAttributeData(char const* xml, const OV_STRING elemName, const OV_STRING attributeName, OV_STRING* pData);
OV_DLLFNCEXPORT OV_RESULT acplt_simpleMsg_parseMessageHeader(char const* xml, ACPLT_MSGHEADER* header);
OV_DLLFNCEXPORT OV_RESULT acplt_simpleMsg_parseFlatBody(char const* xml, OV_STRING* svc, OV_STRING* op,
		OV_BOOL expectContainingObject, OV_STRING* containerName,
		OV_STRING_VEC* ids, OV_STRING_VEC* values, OV_STRING_VEC* units, OV_STRING_VEC* types);
OV_DLLFNCEXPORT OV_RESULT acplt_simpleMsg_xml_extractString(char const* xmlString, OV_UINT strLength, OV_STRING* targetString);
OV_DLLFNCEXPORT void acplt_simpleMsg_initHeader(ACPLT_MSGHEADER* header);
OV_DLLFNCEXPORT OV_STRING acplt_simpleMsg_generateMsgHeader(ACPLT_MSGHEADER const * header);
OV_DLLFNCEXPORT OV_STRING acplt_simpleMsg_GenerateFlatBody(const OV_STRING service, const OV_STRING operation,
		OV_BOOL createContainingObject, const OV_STRING containerName,
		OV_STRING_VEC* ids, OV_STRING_VEC* values, OV_STRING_VEC* units, OV_STRING_VEC* types);



