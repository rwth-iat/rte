/*
 * propertyValueStatement_helpers.h
 *
 *  Created on: 07.02.2017
 *      Author: ubuntu
 */

#ifndef PROPERTYVALUESTATEMENT_HELPERS_H_
#define PROPERTYVALUESTATEMENT_HELPERS_H_

#include "propertyValueStatement.h"
#include "identification_helpers.h"


typedef enum ExpressionSemanticEnum{
    ASSURANCE,
    SETTING,
    MEASUREMENT,
    REQUIREMENT
}ExpressionSemanticEnum;

typedef enum ExpressionLogicEnum{
    GREATER_THAN,
    GREATER_EQUAL,
    EQUAL,
    NOT_EQUAL,
    LESS_EQUAL,
    LESS_THAN
}ExpressionLogicEnum;


typedef enum ViewEnum{
    BUSINESS,
    CONSTRUCTION,
    POWER,
    FUNCTIONAL,
    LOCATION,
    SECURITY,
    NETWORK,
    LIFECYCLE,
    HUMAN
}ViewEnum;

typedef enum VisibilityEnum{
    PRIVAT,
    CONTRACT,
    PUBLIC
}VisibilityEnum;

typedef struct PropertyValueStatement{
	IdentificationType ID;
	OV_STRING PvsName;
    IdentificationType CarrierId;
    ExpressionLogicEnum ExpressionLogic;
    ExpressionSemanticEnum ExpressionSemantic;
    IdentificationType PropertyId;
    OV_ANY Value;
    ViewEnum View;
    OV_UINT Visibility;
    OV_UINT Mask;
}PropertyValueStatement;


typedef struct PropertyValueStatementList{
	IdentificationType ID;
	OV_STRING PvslName;
	IdentificationType CarrierId;
	ExpressionLogicEnum ExpressionLogic;
	ExpressionSemanticEnum ExpressionSemantic;
	IdentificationType PropertyId;
	ViewEnum View;
	OV_UINT Visibility;
	OV_UINT Mask;
    PropertyValueStatement *pvs;
    OV_UINT pvsNumber;
}PropertyValueStatementList;


PropertyValueStatementList* PropertyValueStatementList_new();
void PropertyValueStatementList_init(PropertyValueStatementList *this);
void PropertyValueStatementList_deleteMembers(PropertyValueStatementList *this);
void PropertyValueStatementList_delete(PropertyValueStatementList *this);

PropertyValueStatement* PropertyValueStatement_new();
void PropertyValueStatement_init(PropertyValueStatement *this);
void PropertyValueStatement_deleteMembers(PropertyValueStatement *this);
void PropertyValueStatement_delete(PropertyValueStatement *this);

#endif /* PROPERTYVALUESTATEMENT_HELPERS_H_ */
