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
    OV_STRING pvsName;
    ExpressionLogicEnum ExpressionLogic;
    ExpressionSemanticEnum ExpressionSemantic;
    OV_ANY value;
    OV_STRING unit;
    IdentificationType ID;
    IdentificationType objectID;
    ViewEnum view;
    OV_INT Visibility;
    int mask;
}PropertyValueStatement;


typedef struct PropertyValueStatementList{
    IdentificationType Carrier;
    IdentificationType CreatingInstance;
    OV_TIME CreationTime;
    OV_STRING pvslName;
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
