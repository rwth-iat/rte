/*
 * trafo.h
 *
 *  Created on: 26.02.2019
 *      Author: julian
 */

#ifndef MYLIBRARY_TRAFO_H_
#define MYLIBRARY_TRAFO_H_

#include "myLibrary.h"

UA_Nodestore * myLibrary_trafo_new(OV_INSTPTR_myLibrary_uaInterface context);
void myLibrary_trafo_delete(UA_Nodestore * nodestoreInterface);

#endif /* MYLIBRARY_TRAFO_H_ */
