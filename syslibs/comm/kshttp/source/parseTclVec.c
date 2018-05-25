#include "config.h"
#include <ctype.h>
#include "parseTclVec.h"

OV_RESULT parseTclVec(
	OV_STRING** list,
	const OV_STRING str,
	char open,
	char close,
	OV_UINT* len){

	OV_STRING dup = NULL;
	OV_UINT count = 0;
	OV_BOOL inFlag = FALSE;
	OV_UINT i = 0;

	OV_STRING ph = NULL;
	OV_STRING phdup = NULL;
	*list = NULL;

	*len = 0;
	if(!str || !*str)
		return OV_ERR_OK;

	for(i = 0, ph=str; *ph; i++, ph++){
		if(*ph==open && !inFlag){
			inFlag = TRUE;
			count++;
		} else if(*ph==close){
			if(inFlag)
				inFlag = FALSE;
			else
				return OV_ERR_BADVALUE; // closing bracket without opening
		} else if(*ph=='\\' && *(ph+1)){ // skip next character
			ph++;
			i++;
		}
	}

	if(inFlag) // mismatched brackets
		return OV_ERR_BADVALUE;

	if(!count){ // return string if no delimiter
		*len = 1;
		*list = ov_memstack_alloc(sizeof(OV_STRING));
		**list = ov_memstack_alloc(i+1);
		for(ph=str, phdup=**list; *ph; ph++, phdup++){
			if(*ph=='\\'){ //
				if(*(ph+1)){ // prevent escaping zero terminator
					ph++;
					switch(*ph){
					case 'n':
						*phdup = '\n';
						break;
					case 't':
						*phdup = '\t';
						break;
					case 'r':
						*phdup = '\r';
						break;
					case 'v':
						*phdup = '\v';
						break;
					default:
						*phdup = *ph;
					}
				} else {
					*phdup = '\0';
				}
			} else {
				*phdup = *ph;
			}
		}
		*phdup = '\0'; // add zero terminator
		return OV_ERR_OK;
	}

	*len = count;

	*list = ov_memstack_alloc((*len)*sizeof(OV_STRING));
	dup = ov_memstack_alloc(i+1);

	count = 0;
	for(ph=str, phdup=dup; *ph; ph++, phdup++){
		*phdup = *ph;
		if(*ph==open && !inFlag){
			inFlag = TRUE;
				(*list)[count] = phdup+1;
			count++;
		} else if(*ph==close && inFlag){
				inFlag = FALSE;
				*phdup='\0';
		} else if(*ph=='\\'){ // next character is escaped
			if(*(ph+1)){ // prevent escaping zero terminator
				ph++;
				switch(*ph){
				case 'n':
					*phdup = '\n';
					break;
				case 't':
					*phdup = '\t';
					break;
				case 'r':
					*phdup = '\r';
					break;
				case 'v':
					*phdup = '\v';
					break;
				default:
					*phdup = *ph;
				}
			} else {
				*phdup = '\0';
			}
		}
	}
	*phdup = '\0'; // add zero terminator

	return OV_ERR_OK;
}
