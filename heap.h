// FILE: heap.h
#ifndef HEAP_H
#define HEAP_H

#include <time.h>

typedef struct {
    int triage; // key
    time_t arrival; // tiebreaker
    int patient_id;
} HeapNode;

typedef struct {
    HeapNode *arr;
    int size;
    int capacity;
} MinHeap;

MinHeap* createHeap(int cap);
void destroyHeap(MinHeap* h);
void pushHeap(MinHeap* h, HeapNode node);
HeapNode popMin(MinHeap* h);
int isHeapEmpty(MinHeap* h);

// lazy removal: if patient discharged while in heap, we'll skip when popped

#endif // HEAP_H