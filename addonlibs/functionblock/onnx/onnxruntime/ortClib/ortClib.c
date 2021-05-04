#include "ortClib.h"

#include <stdio.h>

#include "onnxruntime_c_api.h"

struct ORTCLIB_HANDLE {
	OrtEnv* env;
	OrtSession* session;
	OrtSessionOptions* sessionOpt;
	int idxAction;
	int idxActionMask;
	int idxObservation;
	int dimAction;
	int dimObservation;
};

const OrtApi* g_ort = NULL;

static bool CheckInitialised(void)
{
	if (g_ort == NULL)
	{
		fprintf(stderr, "ORTCLIB: g_ort == NULL. Probably not initialised (use ORTCLIB_init).\n");
		return false;
	}
	return true;
}

static bool CheckStatus(OrtStatus* status)
{
    if (status != NULL) {
        const char* msg = g_ort->GetErrorMessage(status);
        fprintf(stderr, "ORTCLIB: CheckStatus returned: %s\n", msg);
        g_ort->ReleaseStatus(status);
		return false;
    }
	return true;
}

static bool CheckHandle(ORTCLIB_HANDLE* handle, bool checkSession)
{
	if (handle == NULL)
		return false;
	if (checkSession && handle->session == NULL)
	{
		fprintf(stderr, "ORTCLIB: session == NULL. Probably no or wrong model loaded (ORTCLIB_load).\n");
		return false;
	}
	if (checkSession && (handle->idxAction < 0 || handle->idxActionMask < 0 || handle->idxObservation < 0))
	{
		//probably never reached, because session should be NULL in case of an error during load function.
		fprintf(stderr, "ORTCLIB: index of action, actionMask or observation not valid. Probably wrong model loaded (ORTCLIB_load).\n");
		return false;
	}
	return true;
}

ORTCLIB_HANDLE* ORTCLIB_init(char* environmentName)
{
	// Check ORT API handle
	if(g_ort == NULL)
		g_ort = OrtGetApiBase()->GetApi(ORT_API_VERSION);
	if (!CheckInitialised())
		return NULL;

	// Create one environment per handle
	//TODO write init function for handle
	ORTCLIB_HANDLE* h = calloc(1, sizeof(ORTCLIB_HANDLE));
	if (h == NULL)
		return NULL;
	h->idxAction = -1;
	h->idxActionMask = -1;
	h->idxObservation = -1;
	h->dimAction = -1;
	h->dimObservation = -1;
	if (!CheckStatus(g_ort->CreateEnv(ORT_LOGGING_LEVEL_WARNING, environmentName, &h->env)))
		return NULL;

	// Create one session per handle
	if (!CheckStatus(g_ort->CreateSessionOptions(&h->sessionOpt)))
	{
		g_ort->ReleaseEnv(h->env);
		return NULL;
	}
	// Set one thread per session and basic optimization
	g_ort->SetIntraOpNumThreads(h->sessionOpt, 1);
	g_ort->SetSessionGraphOptimizationLevel(h->sessionOpt, ORT_ENABLE_BASIC);
	return h;
}

void ORTCLIB_printHandleInfo(ORTCLIB_HANDLE* handle)
{
	if (g_ort == NULL) {
		printf("ORTCLIB: g_ort is NULL. ORTCLIB not initialised. Use init function. Internal Error.\n");
	}
	if (handle == NULL) {
		printf("ORTCLIB: Handle is null, use init function.\n");
		return;
	}
	if (handle->session != NULL) {
		printf("ORTCLIB: Model loaded:\n");
		printf("\tObservation: index %2i,dimensions %3i,nodename %s\n", handle->idxObservation, handle->dimObservation, ORTCLIB_NODENAME_OBSERVATION);
		printf("\tActionMask:  index %2i,dimensions %3i,nodename %s\n", handle->idxActionMask, handle->dimAction, ORTCLIB_NODENAME_ACTIONMASK);
		printf("\tActions:     index %2i,dimensions %3i,nodename %s\n", handle->idxAction, handle->dimAction, ORTCLIB_NODENAME_ACTION);
	}
	else {
		printf("ORTCLIB: Handle has no session. Load a model via load function.\n");
	}
}

void ORTCLIB_close(ORTCLIB_HANDLE* handle)
{
	// Check ORT API handle
	if (!CheckInitialised())
		return;

	// Free and NULL handle if set
	if (handle != NULL) {
		if (handle->session != NULL) g_ort->ReleaseSession(handle->session);
		if (handle->sessionOpt != NULL) g_ort->ReleaseSessionOptions(handle->sessionOpt);
		if (handle->env != NULL) g_ort->ReleaseEnv(handle->env);
		free(handle);
		handle = NULL;
	}
	return;
}

static int SearchIndexByNodename(ORTCLIB_HANDLE* handle, const char* nodename, bool isOutput, size_t nodeCount)
{
	OrtAllocator* allocator;
	if (!CheckStatus(g_ort->GetAllocatorWithDefaultOptions(&allocator)))
		return -1;
	for (size_t i = 0; i < nodeCount; i++) {
		char* node;
		if (CheckStatus(isOutput
			? g_ort->SessionGetOutputName(handle->session, i, allocator, &node)
			: g_ort->SessionGetInputName(handle->session, i, allocator, &node)))
		{
			if (strcmp(node, nodename) == 0)
			{
				return i;
			}
		}
	}
	fprintf(stderr, "ORTCLIB: couldn't determine index for node: %s\n", nodename);
	return -1;
}

static int CheckTypeAndGetDimension(ORTCLIB_HANDLE* handle, int index, bool isOutput)
{
	OrtTypeInfo* typeinfo;
	if (!CheckStatus(isOutput
		? g_ort->SessionGetOutputTypeInfo(handle->session, index, &typeinfo)
		: g_ort->SessionGetInputTypeInfo(handle->session, index, &typeinfo)))
	{
		return -1;
	}

	const OrtTensorTypeAndShapeInfo* tensor_info;
	if (!CheckStatus(g_ort->CastTypeInfoToTensorInfo(typeinfo, &tensor_info)))
	{
		g_ort->ReleaseTypeInfo(typeinfo);
		return -1;
	}
	ONNXTensorElementDataType type; 
	if (!CheckStatus(g_ort->GetTensorElementType(tensor_info, &type)))
	{
		g_ort->ReleaseTypeInfo(typeinfo);
		return -1;
	}
	if (type != ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT)
	{
		fprintf(stderr, "ORTCLIB: wrong type (%i) of %s with index %i\n", type, isOutput? "output" : "input", index);
		g_ort->ReleaseTypeInfo(typeinfo);
		return -1;
	}

	size_t num_dims;
	if (!CheckStatus(g_ort->GetDimensionsCount(tensor_info, &num_dims)))
	{
		g_ort->ReleaseTypeInfo(typeinfo);
		return -1;
	}
	if (num_dims != 2)
	{
		fprintf(stderr, "ORTCLIB: wrong dimensions (%i) of %s with index %i\n", num_dims, isOutput ? "output" : "input", index);
		g_ort->ReleaseTypeInfo(typeinfo);
		return -1;
	}

	int64_t input_node_dims[2];
	if (!CheckStatus(g_ort->GetDimensions(tensor_info, input_node_dims, num_dims)))
	{
		g_ort->ReleaseTypeInfo(typeinfo);
		return -1;
	}
	g_ort->ReleaseTypeInfo(typeinfo);
	return(int)input_node_dims[1];
}

bool ORTCLIB_load(ORTCLIB_HANDLE* handle, char* modelPath)
{
	// Check ORT API and environment handle
	if (!(CheckInitialised() && CheckHandle(handle, false)))
		return false;

	// Release old handle if already used
	if (handle->session != NULL)
	{
		g_ort->ReleaseSession(handle->session);
		handle->session = NULL;
		handle->idxAction = -1;
		handle->idxActionMask = -1;
		handle->idxObservation = -1;
		handle->dimAction = -1;
		handle->dimObservation = -1;
	}

	// Convert modelPath to wchar
	size_t requiredSize = mbstowcs(NULL, modelPath, 0);
	wchar_t* pwc = (wchar_t*)malloc((requiredSize + 1) * sizeof(wchar_t));
	if (!pwc)
	{
		fprintf(stderr, "ORTCLIB: not enough memory to allocate model path.\n");
		return false;
	}
	if (mbstowcs(pwc, modelPath, requiredSize + 1) == (size_t)(-1))
	{
		fprintf(stderr, "ORTCLIB: couldn't convert modelPath - invalid multibyte character.\n");
		return false;
	}

	// Create new session
	if (!CheckStatus(g_ort->CreateSession(handle->env, pwc, handle->sessionOpt, &handle->session))) {
		free(pwc);
		return false;
	}
	free(pwc);

	// Check model input / output count
	size_t inputCount;
	size_t outputCount;
	if (!CheckStatus(g_ort->SessionGetInputCount(handle->session, &inputCount)) ||
		!CheckStatus(g_ort->SessionGetOutputCount(handle->session, &outputCount)))
	{
		g_ort->ReleaseSession(handle->session);
		handle->session = NULL;
		return false;
	}
	if (inputCount < 2 || outputCount < 1)
	{
		fprintf(stderr, "ORTCLIB: not enough input (%i) or output (%i), at least two inputs and one output needed.\n", inputCount, outputCount);
		g_ort->ReleaseSession(handle->session);
		handle->session = NULL;
		return false;
	}

	// Search inputs and outputs
	handle->idxAction = SearchIndexByNodename(handle, ORTCLIB_NODENAME_ACTION, true, outputCount);
	handle->idxActionMask = SearchIndexByNodename(handle, ORTCLIB_NODENAME_ACTIONMASK, false, inputCount);
	handle->idxObservation = SearchIndexByNodename(handle, ORTCLIB_NODENAME_OBSERVATION, false, inputCount);
	if ((handle->idxAction < 0 || handle->idxActionMask < 0 || handle->idxObservation < 0))
	{
		g_ort->ReleaseSession(handle->session);
		handle->session = NULL;
		return false;
	}

	// Check types and and get dimension
	handle->dimAction = CheckTypeAndGetDimension(handle, handle->idxAction, true);
	int dimActionMask = CheckTypeAndGetDimension(handle, handle->idxActionMask, false);
	handle->dimObservation = CheckTypeAndGetDimension(handle, handle->idxObservation, false);
	if(handle->dimAction < 0 || dimActionMask < 0 || handle->dimObservation < 0)
	{
		g_ort->ReleaseSession(handle->session);
		handle->session = NULL;
		return false;
	}
	if (handle->dimAction != dimActionMask)
	{
		fprintf(stderr, "ORTCLIB: action dimension (%i) and action mask dimension (%i) not equal.\n", handle->dimAction, dimActionMask);
		g_ort->ReleaseSession(handle->session);
		handle->session = NULL;
		return false;
	}
	return true;
}

bool ORTCLIB_evaluate(ORTCLIB_HANDLE* handle,
	float* observation, size_t observationSize, float* actionMask,
	float** actions, size_t* actionsSize)
{
	// Check ORT API and environment handle
	if (!(CheckInitialised() && CheckHandle(handle, true)))
		return false;
	// Check parameter
	if (observation == NULL || actions == NULL) {
		fprintf(stderr, "ORTCLIB: can't evaluate model, because observation or actions parameter is NULL.\n");
		return false;
	}

	// Check compatibility with loaded model via dimensions
	if (observationSize != handle->dimObservation)
	{
		fprintf(stderr, "ORTCLIB: can't evaluate model. Observation size is wrong:%i. Should be %i\n", observationSize, handle->dimObservation);
		return false;
	}

	// Create input tensors
	int64_t dataShape[2] = {1, observationSize};
	OrtMemoryInfo* memory_info;
	CheckStatus(g_ort->CreateCpuMemoryInfo(OrtArenaAllocator, OrtMemTypeDefault, &memory_info));

	OrtValue* observationTensor = NULL;
	CheckStatus(g_ort->CreateTensorWithDataAsOrtValue(memory_info, observation, observationSize * sizeof(float), dataShape, 2, ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT, &observationTensor));

	dataShape[1] = handle->dimAction;
	bool freeActionMask = false;
	if (actionMask == NULL) {
		actionMask = calloc(handle->dimAction, sizeof(float));
		if (actionMask == NULL) {
			fprintf(stderr, "ORTCLIB: can't allocate actionMask array.\n");
			g_ort->ReleaseValue(observationTensor);
			g_ort->ReleaseMemoryInfo(memory_info);
			return false;
		}
		freeActionMask = true;
		for (int i = 0; i < handle->dimAction; i++)
		{
			actionMask[i] = 1.0f;
		}
	}
	OrtValue* actionMaskTensor = NULL;
	CheckStatus(g_ort->CreateTensorWithDataAsOrtValue(memory_info, actionMask, handle->dimAction * sizeof(float), dataShape, 2, ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT, &actionMaskTensor));

	g_ort->ReleaseMemoryInfo(memory_info);

	// Run inference
	OrtValue* inputTensors[2] = { observationTensor, actionMaskTensor };
	OrtValue* actionsTensor = NULL;
	OrtValue* outputTensors[1] = { actionsTensor };
	const char* inputNames[2] = { ORTCLIB_NODENAME_OBSERVATION, ORTCLIB_NODENAME_ACTIONMASK };
	const char* outputNames[1] = { ORTCLIB_NODENAME_ACTION };
	CheckStatus(g_ort->Run(handle->session, NULL, inputNames, (const OrtValue* const*)inputTensors, 2, outputNames, 1, outputTensors));

	// Copy outputs
	float* actionsArray;
	CheckStatus(g_ort->GetTensorMutableData(outputTensors[0], (void**)&actionsArray));
	*actions = calloc(handle->dimAction, sizeof(float));
	if (*actions == NULL) {
		fprintf(stderr, "ORTCLIB: can't allocate actions array.\n");
		g_ort->ReleaseValue(observationTensor);
		g_ort->ReleaseValue(actionMaskTensor);
		g_ort->ReleaseValue(actionsTensor);
		if (freeActionMask) {
			actionMask = NULL;
			free(actionMask);
		}
		return false;
	}
	for (int i = 0; i < handle->dimAction; i++)
	{
		(*actions)[i] = actionsArray[i];
	}
	*actionsSize = handle->dimAction;

	g_ort->ReleaseValue(observationTensor);
	g_ort->ReleaseValue(actionMaskTensor);
	g_ort->ReleaseValue(actionsTensor);
	if (freeActionMask) {
		actionMask = NULL;
		free(actionMask);
	}
	return true;
}
