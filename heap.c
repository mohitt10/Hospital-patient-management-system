// FILE: heap.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "heap.h"

MinHeap* createHeap(int cap) {
    MinHeap* h = (MinHeap*)malloc(sizeof(MinHeap));
    if (!h) return NULL;
    h->arr = (HeapNode*)malloc(sizeof(HeapNode)*cap);
    h->size = 0; h->capacity = cap;
    return h;
}

void destroyHeap(MinHeap* h) {
    if (!h) return;
    free(h->arr);
    free(h);
}

int isHeapEmpty(MinHeap* h) { return h->size == 0; }

static void swapNodes(HeapNode* a, HeapNode* b) { HeapNode t = *a; *a = *b; *b = t; }

static int lessThan(const HeapNode* a, const HeapNode* b) {
    if (a->triage != b->triage) return a->triage < b->triage; // smaller triage is more urgent
    return a->arrival < b->arrival;
}

void pushHeap(MinHeap* h, HeapNode node) {
    if (!h) return;
    if (h->size == h->capacity) {
        h->capacity *= 2;
        h->arr = (HeapNode*)realloc(h->arr, sizeof(HeapNode)*h->capacity);
    }
    int idx = h->size++;
    h->arr[idx] = node;
    while (idx > 0) {
        int parent = (idx-1)/2;
        if (lessThan(&h->arr[idx], &h->arr[parent])) { swapNodes(&h->arr[idx], &h->arr[parent]); idx = parent; }
        else break;
    }
}

HeapNode popMin(MinHeap* h) {
    if (!h || h->size == 0) {
        HeapNode emp = {999,0,-1}; return emp;
    }
    HeapNode root = h->arr[0];
    h->arr[0] = h->arr[--h->size];
    int idx = 0;
    while (1) {
        int l = 2*idx+1, r = 2*idx+2, smallest = idx;
        if (l < h->size && lessThan(&h->arr[l], &h->arr[smallest])) smallest = l;
        if (r < h->size && lessThan(&h->arr[r], &h->arr[smallest])) smallest = r;
        if (smallest != idx) { swapNodes(&h->arr[idx], &h->arr[smallest]); idx = smallest; }
        else break;
    }
    return root;
}