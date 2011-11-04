#include "msgconfig.h"

OV_DLLVAREXPORT const OV_STRING TECHROOT = "/TechUnits";
OV_DLLVAREXPORT const OV_STRING MSGCOMPONENTROOT = "/TechUnits/MsgSys";
OV_DLLVAREXPORT const OV_STRING MSGCOMPONENTCONTAINER = "MsgSys";
OV_DLLVAREXPORT const OV_STRING MESSAGECONTAINER = "Message";
OV_DLLVAREXPORT const OV_STRING SENDINGINSTANCEPATH = "/TechUnits/MsgSys/sendingInstance";
OV_DLLVAREXPORT const OV_STRING SERVICEROOT = "/TechUnits/MsgSys/Services";

OV_INT LOCALMSGCOUNTER = 0;

const OV_INT MSGNEW = 0;
const OV_INT MSGWAITING = 1;
const OV_INT MSGDONE = 2;
const OV_INT MSGRECEIVERSERVICEERROR = 3;
const OV_INT MSGFATALERROR = 4;

//Cyletime
OV_DLLVAREXPORT const OV_TIME_SPAN MSGCYCTIME = {0,100000}; // 1 second is default

static OV_BOOL MSG_isDelimiter(OV_STRING delimiter,OV_STRING x)
{
  OV_INT i;
  OV_BOOL res = TRUE;

  //ov_logfile_debug("===>>>> isDELIMITER: delimiter='%s' -- x='%s'",delimiter,x);
  for(i = 0; (unsigned) i < ov_string_getlength(delimiter);i++)
    {
      if(x[i] == 0 || x[i] != delimiter[i] )
	res = FALSE;
    }
  return res;
}


OV_BOOL MSG_split(OV_STRING victim, OV_STRING delimiter, OV_INT *position, OV_STRING *token) {
	OV_INT curPosition = 0;
	char buffer[32000] = {0};

	//ctpeer_logfile_debug("splitting... %s with position %d", victim, position);
	if(victim[*position] == 0) return FALSE;
	//ctpeer_logfile_info("0");
	for(curPosition = (*position); (unsigned)curPosition <= ov_string_getlength(victim); curPosition++) {
		//ctpeer_logfile_info("2 %c %d", victim[curPosition], curPosition);
		if(victim[curPosition] == 0 || MSG_isDelimiter(delimiter, victim+curPosition)) {
			//ctpeer_logfile_info("2b %c %d", victim[curPosition], curPosition);
			memcpy(buffer, victim+*position, curPosition-(*position) );
			if(victim[curPosition]==0)
				*position = curPosition;
			else
			  *position = curPosition+ov_string_getlength(delimiter);
			//ctpeer_logfile_info("3");
			//ctpeer_logfile_info("token [%s] buffer [%s]", *token, buffer);
			ov_string_setvalue(token, buffer);
			return TRUE;
		}	
	}
	return FALSE;//never reached
}