/*
 * propertyValueStatement_helpers.c
 *
 *  Created on: 07.02.2017
 *      Author: ubuntu
 */

#ifndef OV_COMPILE_LIBRARY_propertyValueStatement
#define OV_COMPILE_LIBRARY_propertyValueStatement
#endif

#include "propertyValueStatement.h"
#include "propertyValueStatement_helpers.h"
#include "libov/ov_database.h"
#include "libov/ov_time.h"
#include "libov/ov_macros.h"

OV_DLLFNCEXPORT PropertyValueStatementList* PropertyValueStatementList_new(){
	PropertyValueStatementList* this = ov_database_malloc(sizeof(PropertyValueStatementList));
	PropertyValueStatementList_init(this);
	return this;
}
OV_DLLFNCEXPORT void PropertyValueStatementList_init(PropertyValueStatementList *this){
	IdentificationType_init(&this->Carrier);
	IdentificationType_init(&this->CreatingInstance);
	ov_time_gettime(&this->CreationTime);
	this->pvs = NULL;
	this->pvslName = NULL;
	this->pvsNumber = 0;
}
OV_DLLFNCEXPORT void PropertyValueStatementList_deleteMembers(PropertyValueStatementList *this){
	IdentificationType_deleteMembers(&this->Carrier);
	IdentificationType_deleteMembers(&this->CreatingInstance);
	for (OV_UINT i = 0; i < this->pvsNumber; i++){
		PropertyValueStatement_deleteMembers(&(this->pvs)[i]);
	}
	ov_database_free(this->pvs);
	ov_database_free(this->pvslName);
	PropertyValueStatementList_init(this);
}
OV_DLLFNCEXPORT void PropertyValueStatementList_delete(PropertyValueStatementList *this){
	PropertyValueStatementList_deleteMembers(this);
	ov_database_free(this);
}

OV_DLLFNCEXPORT PropertyValueStatement* PropertyValueStatement_new(){
	PropertyValueStatement* this = ov_database_malloc(sizeof(PropertyValueStatement));
	PropertyValueStatement_init(this);
	return this;
}
OV_DLLFNCEXPORT void PropertyValueStatement_init(PropertyValueStatement *this){
	this->ExpressionSemantic = 0;
	this->Visibility = 0;
	IdentificationType_init(&this->ID);
	this->pvsName = NULL;
	this->ExpressionLogic = 0;
	this->unit = NULL;
	this->view = 0;
	ov_time_gettime(&this->value.time);
	this->value.value.vartype = OV_VT_VOID;
	this->value.value.valueunion.val_string = NULL;
	this->value.value.valueunion.val_bool_vec.value = NULL;
	this->value.value.valueunion.val_bool_vec.veclen = 0;
	this->value.value.valueunion.val_byte_vec.value = NULL;
	this->value.value.valueunion.val_byte_vec.veclen = 0;
	this->value.value.valueunion.val_double_vec.value = NULL;
	this->value.value.valueunion.val_double_vec.veclen = 0;
	this->value.value.valueunion.val_generic_vec.value = NULL;
	this->value.value.valueunion.val_generic_vec.veclen = 0;
	this->value.value.valueunion.val_int_vec.value = NULL;
	this->value.value.valueunion.val_int_vec.veclen = 0;
	this->value.value.valueunion.val_single_vec.value = NULL;
	this->value.value.valueunion.val_single_vec.veclen = 0;
	this->value.value.valueunion.val_state_vec.value = NULL;
	this->value.value.valueunion.val_state_vec.veclen = 0;
	this->value.value.valueunion.val_string_vec.value = NULL;
	this->value.value.valueunion.val_string_vec.veclen = 0;
	this->value.value.valueunion.val_struct_vec.value = NULL;
	this->value.value.valueunion.val_struct_vec.veclen = 0;
	this->value.value.valueunion.val_time_span_vec.value = NULL;
	this->value.value.valueunion.val_time_span_vec.veclen = 0;
	this->value.value.valueunion.val_time_vec.value = NULL;
	this->value.value.valueunion.val_time_vec.veclen = 0;
	this->value.value.valueunion.val_uint_vec.value = NULL;
	this->value.value.valueunion.val_uint_vec.veclen = 0;
}
OV_DLLFNCEXPORT void PropertyValueStatement_deleteMembers(PropertyValueStatement *this){
	IdentificationType_deleteMembers(&this->ID);
	ov_database_free(this->pvsName);
	ov_database_free(this->unit);
	Ov_SetAnyValue(&this->value, NULL);
	PropertyValueStatement_init(this);
}
OV_DLLFNCEXPORT void PropertyValueStatement_delete(PropertyValueStatement *this){
	PropertyValueStatement_deleteMembers(this);
	free(this);
}
