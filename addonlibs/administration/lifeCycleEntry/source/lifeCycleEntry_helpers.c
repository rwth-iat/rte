/*
 * lifeCycleEntry_helpers.c
 *
 *  Created on: 07.02.2017
 *      Author: ubuntu
 */

#ifndef OV_COMPILE_LIBRARY_lifeCycleEntry
#define OV_COMPILE_LIBRARY_lifeCycleEntry
#endif

#include "lifeCycleEntry.h"
#include "lifeCycleEntry_helpers.h"
#include "libov/ov_database.h"
#include "libov/ov_time.h"
#include "libov/ov_macros.h"

OV_DLLFNCEXPORT LifeCycleEntry* LifeCycleEntry_new(){
	LifeCycleEntry* this = ov_database_malloc(sizeof(LifeCycleEntry));
	LifeCycleEntry_init(this);
	return this;
}
OV_DLLFNCEXPORT void LifeCycleEntry_init(LifeCycleEntry *this){
	IdentificationType_init(&this->creatingInstance);
	IdentificationType_init(&this->writingInstance);
	this->eventClass = NULL;
	this->lceId = 0;
	this->subject = NULL;
	ov_time_gettime(&this->data.time);
	this->data.value.vartype = OV_VT_VOID;
	this->data.value.valueunion.val_string = NULL;
	this->data.value.valueunion.val_bool_vec.value = NULL;
	this->data.value.valueunion.val_bool_vec.veclen = 0;
	this->data.value.valueunion.val_byte_vec.value = NULL;
	this->data.value.valueunion.val_byte_vec.veclen = 0;
	this->data.value.valueunion.val_double_vec.value = NULL;
	this->data.value.valueunion.val_double_vec.veclen = 0;
	this->data.value.valueunion.val_generic_vec.value = NULL;
	this->data.value.valueunion.val_generic_vec.veclen = 0;
	this->data.value.valueunion.val_int_vec.value = NULL;
	this->data.value.valueunion.val_int_vec.veclen = 0;
	this->data.value.valueunion.val_single_vec.value = NULL;
	this->data.value.valueunion.val_single_vec.veclen = 0;
	this->data.value.valueunion.val_state_vec.value = NULL;
	this->data.value.valueunion.val_state_vec.veclen = 0;
	this->data.value.valueunion.val_string_vec.value = NULL;
	this->data.value.valueunion.val_string_vec.veclen = 0;
	this->data.value.valueunion.val_struct_vec.value = NULL;
	this->data.value.valueunion.val_struct_vec.veclen = 0;
	this->data.value.valueunion.val_time_span_vec.value = NULL;
	this->data.value.valueunion.val_time_span_vec.veclen = 0;
	this->data.value.valueunion.val_time_vec.value = NULL;
	this->data.value.valueunion.val_time_vec.veclen = 0;
	this->data.value.valueunion.val_uint_vec.value = NULL;
	this->data.value.valueunion.val_uint_vec.veclen = 0;
}
OV_DLLFNCEXPORT void LifeCycleEntry_deleteMembers(LifeCycleEntry *this){
	IdentificationType_deleteMembers(&this->creatingInstance);
	IdentificationType_deleteMembers(&this->writingInstance);
	ov_database_free(this->eventClass);
	ov_database_free(this->subject);
	Ov_SetAnyValue(&this->data, NULL);
}
OV_DLLFNCEXPORT void LifeCycleEntry_delete(LifeCycleEntry *this){
	LifeCycleEntry_deleteMembers(this);
	ov_database_free(this);
}
