/*
 * identification_helpers.h
 *
 *  Created on: 07.02.2017
 *      Author: ubuntu
 */

#ifndef IDENTIFICATION_HELPERS_H_
#define IDENTIFICATION_HELPERS_H_

#include "identification.h"

typedef struct IdentificationType{
    OV_STRING IdSpec;
    enum IdType{
        URI,
        ISO
    }IdType;
}IdentificationType;

IdentificationType* IdentificationType_new();
void IdentificationType_init(IdentificationType *this);
void IdentificationType_deleteMembers(IdentificationType *this);
void IdentificationType_delete(IdentificationType *this);
OV_BOOL IdentificationTypeEqual(IdentificationType *Id1, IdentificationType *Id2);

#endif /* IDENTIFICATION_HELPERS_H_ */
