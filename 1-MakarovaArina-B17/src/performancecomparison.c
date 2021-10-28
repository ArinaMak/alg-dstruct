#pragma once
#pragma warning(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "memallocator.h"
#define MAX_ALLOCATING_SIZE 1000
#define MAX_ITERATIONS 100

double MallocAnalysis(LARGE_INTEGER frequency, int size) {
    LARGE_INTEGER start;
    LARGE_INTEGER end;
    char* arr[MAX_ITERATIONS] = { NULL };
    int iteration = 0;
    QueryPerformanceCounter(&start);

    for(int iteration=0; iteration<MAX_ITERATIONS; iteration++)
    {
        arr[iteration] = (char*)malloc(size);
    }
    for (int iteration = 0; iteration < MAX_ITERATIONS; iteration++)
    {
        free(arr[iteration]);
    }

    QueryPerformanceCounter(&end);
    return (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
}

double MemallocAnalysis(LARGE_INTEGER frequency, int size) {
    LARGE_INTEGER start;
    LARGE_INTEGER end;
    char* arr[MAX_ITERATIONS] = { NULL };
    int iteration = 0;
    QueryPerformanceCounter(&start);

    for (int iteration = 0; iteration < MAX_ITERATIONS; iteration++)
    {
        arr[iteration] = (char*)memalloc(size);
    }
    for (int iteration = 0; iteration < MAX_ITERATIONS; iteration++)
    {
        memfree(arr[iteration]);
    }
    QueryPerformanceCounter(&end);
    return (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
}

int main()
{
    LARGE_INTEGER lp_frequency;
    int memory_size = MAX_ITERATIONS * (MAX_ALLOCATING_SIZE + memgetblocksize());
    void* ptr = malloc(memory_size);
    if (ptr == NULL) {
        printf("Error allocating memory\n");
        exit(1);
    }
    int init_mem = meminit(ptr, memory_size);
    if (init_mem == 0) {
        printf("Error initialize\n");
        free(ptr);
        exit(1);
    }
    QueryPerformanceFrequency(&lp_frequency);
    double res_mem = 0;
    double res_mal = 0;

    FILE* f = fopen("compare.csv", "w");
    if (f == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    fprintf(f, "malloc , memalloc\n");
 
    for (int i = 10; i < MAX_ALLOCATING_SIZE; i += 10) 
    {
        res_mal = MallocAnalysis(lp_frequency, i);
        res_mem = MemallocAnalysis(lp_frequency, i);
        fprintf(f, "%lf , %lf\n", res_mal, res_mem);
    }
    memdone();
    free(ptr);
    fclose(f);

	return 0;
}
