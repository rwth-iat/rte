/*
 * openaas_helpers.h
 *
 *  Created on: 07.02.2017
 *      Author: ubuntu
 */

#ifndef OPENAAS_HELPERS_H_
#define OPENAAS_HELPERS_H_

#include "openaas.h"
#include "jsonparsing.h"

IdentificationType* IdentificationType_new();
void IdentificationType_init(IdentificationType *this);
void IdentificationType_deleteMembers(IdentificationType *this);
void IdentificationType_delete(IdentificationType *this);
OV_BOOL IdentificationTypeEqual(IdentificationType *aasId, IdentificationType *aasId2);

PropertyValueStatementList* PropertyValueStatementList_new();
void PropertyValueStatementList_init(PropertyValueStatementList *this);
void PropertyValueStatementList_deleteMembers(PropertyValueStatementList *this);
void PropertyValueStatementList_delete(PropertyValueStatementList *this);

PropertyValueStatement* PropertyValueStatement_new();
void PropertyValueStatement_init(PropertyValueStatement *this);
void PropertyValueStatement_deleteMembers(PropertyValueStatement *this);
void PropertyValueStatement_delete(PropertyValueStatement *this);

#endif /* OPENAAS_HELPERS_H_ */
