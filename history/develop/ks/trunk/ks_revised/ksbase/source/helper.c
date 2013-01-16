
#include "helper.h"
#include <string.h>
#include "libov/ov_macros.h"

OV_BOOL ks_isvalidname(OV_STRING name)
{

	OV_UINT i = 0;
	OV_UINT length;
	length = strlen(name);
	if(length <= KS_NAME_MAXLEN)
	{
		while(i<length)
		{
			if(!((name[i] >= 65 && name[i] <= 90)	//A-Z
					|| (name[i] >= 97 && name[i] <= 122)	//a-z
					|| name[i] == 95))		//'_'
					return FALSE;

		}
		return TRUE;
	}
	else
		return FALSE;
}
