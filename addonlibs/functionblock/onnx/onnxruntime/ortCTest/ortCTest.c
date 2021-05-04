#include <stdio.h>

#include "ortClib.h"

static void printArray(const char* prefix, const char* delim, float* array, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        printf("%s%.2f", prefix, array[i]);
        if (i < size - 1)
            printf(delim);
    }
    printf("\n");
}

static int maxValue(float* array, size_t size)
{
    int max = 0;
    for (size_t i = 0; i < size; i++) {
        if (array[i] >= array[max]) {
            max = i;
        }
    }
    return max;
}

static void evaluate(ORTCLIB_HANDLE* handle, float* observations, float* actionMask)
{
    //float observations[28] =
    //{ 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1, 0.5,
    //  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
    //  1, 1 };
    size_t observationSize = 28;
    //printf("Observations:");
    //printArray("\n\t", ",", observations, observationSize);
    //float actionMask[9] = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    //printf("Action Mask:");
    //printArray("", ", ", actionMask, 9);

    size_t actionsSize = 0;
    float* actions;
    if (ORTCLIB_evaluate(handle, observations, observationSize, actionMask, &actions, &actionsSize))
    {
        printf("Actions:\n");
        printArray("", ", ", actions, actionsSize);
        printf("Most likely action: %i\n", maxValue(actions, actionsSize));
        free(actions);
    }
}

int main()
{
    printf("--- init ---\n");
    ORTCLIB_HANDLE* handle = ORTCLIB_init("Test");

    printf("--- load ---\n");
    ORTCLIB_load(handle, "..\\..\\doc\\example.onnx");

    printf("--- printHandleInfo ---\n");
    ORTCLIB_printHandleInfo(handle);

    printf("--- evaluate ---\n");

    printf("Move palette from left to right (action 2 = Right):\n");
    evaluate(handle, (float[])
        { 1, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
          1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          1, 1 }, (float[])
        { 0, 1, 1, 0, 0, 0, 0, 0, 1 });
    evaluate(handle, (float[])
        { 0.5, 1, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
          0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          1, 1 }, (float[])
        { 0, 1, 1, 1, 1, 0, 0, 0, 0 });
    evaluate(handle, (float[])
        { 0.5, 0.5, 1, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
          0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          1, 1 }, (float[])
        { 0, 1, 1, 0, 0, 0, 1, 0, 0 });
    evaluate(handle, (float[])
        { 0.5, 0.5, 0.5, 1, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
          0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          1, 1 }, (float[])
        { 0, 1, 1, 0, 0, 0, 0, 0, 0 });
    evaluate(handle, (float[])
        { 0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
          0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
          1, 1 }, (float[])
        { 0, 1, 1, 0, 0, 0, 0, 0, 0 });


    printf("\nPalette at last conveyor, different action masks:\n");
    evaluate(handle, (float[])
        { 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1, 0.5,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
          1, 1 }, (float[])
        { 0, 1, 1, 1, 1, 0, 0, 0, 0 });
    evaluate(handle, (float[])
        { 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1, 0.5,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
          1, 1 }, (float[])
        { 0, 0, 0, 0, 0, 0, 0, 0, 0 });
    evaluate(handle, (float[])
        { 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1, 0.5,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
          1, 1 }, (float[])
        { 1, 1, 1, 1, 1, 1, 1, 1, 1 });
    evaluate(handle, (float[])
        { 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1, 0.5,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
          1, 1 }, NULL);

    printf("--- close ---\n");
    ORTCLIB_close(handle);
}
