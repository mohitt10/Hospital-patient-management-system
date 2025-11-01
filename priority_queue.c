#include <stdlib.h>
#include <stdio.h>
#include "priority_queue.h"

#define INITIAL_CAP 64

static void swap(CritHeap *h, int i, int j){ int t=h->heap[i]; h->heap[i]=h->heap[j]; h->heap[j]=t; }

void critheap_init(CritHeap *h, PatientArr *patients){
    h->size=0; h->cap=INITIAL_CAP;
    h->heap = malloc(sizeof(int)*h->cap);
    if(!h->heap){ perror("malloc critheap"); exit(1); }
    h->patients = patients;
}

static void sift_up(CritHeap *h, int idx){
    while(idx>0){
        int p=(idx-1)/2;
        if(h->patients->data[h->heap[idx]].criticalness < 1 || h->patients->data[h->heap[p]].criticalness < 1) break;
        // smaller number => higher priority (1 is most critical)
        if(h->patients->data[h->heap[idx]].criticalness < h->patients->data[h->heap[p]].criticalness){
            swap(h, idx, p); idx=p;
        } else break;
    }
}

static void sift_down(CritHeap *h, int idx){
    while(1){
        int l=2*idx+1, r=2*idx+2, best=idx;
        if(l<h->size && h->patients->data[h->heap[l]].criticalness < h->patients->data[h->heap[best]].criticalness) best=l;
        if(r<h->size && h->patients->data[h->heap[r]].criticalness < h->patients->data[h->heap[best]].criticalness) best=r;
        if(best!=idx){ swap(h, idx, best); idx=best; } else break;
    }
}

void critheap_push(CritHeap *h, int pi){
    if(h->size==h->cap){ h->cap*=2; h->heap=realloc(h->heap,sizeof(int)*h->cap); if(!h->heap){ perror("realloc heap"); exit(1);} }
    h->heap[h->size] = pi;
    sift_up(h, h->size);
    h->size++;
}

int critheap_pop(CritHeap *h){
    if(h->size==0) return -1;
    int top = h->heap[0];
    h->heap[0] = h->heap[--h->size];
    sift_down(h, 0);
    return top;
}

void critheap_build_from_patients(CritHeap *h){
    h->size = 0;
    if(!h->patients) return;
    for(int i=0;i<h->patients->size;i++){
        if(h->patients->data[i].admitted && h->patients->data[i].criticalness>=1 && h->patients->data[i].criticalness<=3){
            critheap_push(h, i);
        }
    }
}

void critheap_free(CritHeap *h){
    free(h->heap);
    h->heap = NULL;
    h->size = h->cap = 0;
}