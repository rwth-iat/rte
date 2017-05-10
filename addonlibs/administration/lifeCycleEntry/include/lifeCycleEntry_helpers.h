/*
 * lifeCycleEntry_helpers.h
 *
 *  Created on: 07.02.2017
 *      Author: ubuntu
 */

#ifndef LIFECYCLEENTRY_HELPERS_H_
#define LIFECYCLEENTRY_HELPERS_H_

#include "lifeCycleEntry.h"
#include "identification_helpers.h"

typedef struct LifeCycleEntry{
    OV_UINT64 lceId;
    IdentificationType creatingInstance;
    IdentificationType writingInstance;
    OV_STRING eventClass;
    OV_STRING subject;
    OV_ANY data;
}LifeCycleEntry;


LifeCycleEntry* LifeCycleEntry_new();
void LifeCycleEntry_init(LifeCycleEntry *this);
void LifeCycleEntry_deleteMembers(LifeCycleEntry *this);
void LifeCycleEntry_delete(LifeCycleEntry *this);

#endif /* LIFECYCLEENTRY_HELPERS_H_ */
