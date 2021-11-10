#ifndef ORTCLIB_ORTCLIB_H
#define ORTCLIB_ORTCLIB_H

#ifdef ORTCLIB_EXPORTS
#define ORTCLIB_API __declspec(dllexport)
#else
#define ORTCLIB_API __declspec(dllimport)
#endif

#include <stdlib.h>
#include <stdbool.h>

#define ORTCLIB_NODENAME_OBSERVATION "vector_observation"
#define ORTCLIB_NODENAME_ACTIONMASK "action_masks"
// alternative: discrete_actions
#define ORTCLIB_NODENAME_ACTION "action" 

typedef struct ORTCLIB_HANDLE ORTCLIB_HANDLE;
ORTCLIB_API void ORTCLIB_printHandleInfo(ORTCLIB_HANDLE* handle);


ORTCLIB_API ORTCLIB_HANDLE* ORTCLIB_init(char* environmentName);
ORTCLIB_API void ORTCLIB_close(ORTCLIB_HANDLE* handle);

ORTCLIB_API bool ORTCLIB_load(ORTCLIB_HANDLE* handle, char* modelPath);
ORTCLIB_API bool ORTCLIB_evaluate(ORTCLIB_HANDLE* handle,
	float* observation,	size_t observationSize, float* actionMask,
	float** actions, size_t* actionsSize);

ORTCLIB_API void ORTCLIB_freeActions(float* actions);

#endif /* ORTCLIB_ORTCLIB_H */