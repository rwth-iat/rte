/*
 *  identification_helpers.c
 *
 *  Created on: 07.02.2017
 *      Author: ubuntu
 */

#ifndef OV_COMPILE_LIBRARY_identification
#define OV_COMPILE_LIBRARY_identification
#endif

#include "identification_helpers.h"
#include "libov/ov_string.h"
#include "libov/ov_database.h"

OV_DLLFNCEXPORT OV_BOOL IdentificationTypeEqual(IdentificationType *Id1, IdentificationType *Id2){
    if (Id1->IdType == Id2->IdType){
        if (ov_string_compare(Id1->IdSpec, Id2->IdSpec) == OV_STRCMP_EQUAL)
            return TRUE;
    }
    return FALSE;
}

OV_DLLFNCEXPORT IdentificationType* IdentificationType_new(){
    IdentificationType* this = ov_database_malloc(sizeof(IdentificationType));
    IdentificationType_init(this);
    return this;
}
OV_DLLFNCEXPORT void IdentificationType_init(IdentificationType *this){
    this->IdSpec = NULL;
    this->IdType = 0;
}
OV_DLLFNCEXPORT void IdentificationType_deleteMembers(IdentificationType *this){
    ov_database_free(this->IdSpec);
    IdentificationType_init(this);
}
OV_DLLFNCEXPORT void IdentificationType_delete(IdentificationType *this){
    IdentificationType_deleteMembers(this);
    ov_database_free(this);
}
