/*
 * helper.c
 *
 *  Created on: 10.10.2017
 *      Author: ubuntu
 */

#include "helper.h"
#include "openaas.h"

OV_RESULT checkForSameAAS(OV_INSTPTR_ov_object pobject1, OV_INSTPTR_ov_object pobject2){
	OV_INSTPTR_ov_object ptr = NULL;
	OV_INSTPTR_ov_object ptr2 = NULL;
	OV_INSTPTR_ov_object ptr3 = NULL;
	OV_INSTPTR_ov_object ptr4 = NULL;
	OV_BOOL findAAS1 = FALSE;
	OV_BOOL findAAS2 = FALSE;

	ptr = Ov_StaticPtrCast(ov_object, Ov_GetParent(ov_containment, pobject1));
	if (!ptr){
		ptr = pobject1->v_pouterobject;
	}
	do{
		if (Ov_CanCastTo(openaas_aas, ptr)){
			findAAS1 = TRUE;
			break;
		}
		ptr2 = Ov_StaticPtrCast(ov_object, Ov_GetParent(ov_containment, ptr));
		if (!ptr2){
			ptr2 = ptr->v_pouterobject;
		}
		ptr = ptr2;
	}while (ptr);
	if (findAAS1 == FALSE){
		return OV_ERR_BADPATH;
	}

	ptr3 = Ov_StaticPtrCast(ov_object, Ov_GetParent(ov_containment, pobject2));
	if (!ptr3){
		ptr3 = pobject2->v_pouterobject;
	}
	do{
		if (Ov_CanCastTo(openaas_aas, ptr3)){
			findAAS2 = TRUE;
			break;
		}
		ptr4 = Ov_StaticPtrCast(ov_object, Ov_GetParent(ov_containment, ptr3));
		if (!ptr4){
			ptr4 = ptr3->v_pouterobject;
		}
		ptr3 = ptr4;
	}while (ptr3);
	if (findAAS2 == FALSE){
		return OV_ERR_BADPATH;
	}

	if (ptr == ptr3)
		return OV_ERR_OK;
	else
		return OV_ERR_BADPATH;
}
