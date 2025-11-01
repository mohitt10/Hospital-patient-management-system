#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "patient.h"

typedef struct {
    int *heap;         // stores indices into PatientArr
    int size;
    int cap;
    PatientArr *patients;
} CritHeap;

void critheap_init(CritHeap *h, PatientArr *patients);
void critheap_push(CritHeap *h, int patient_index);
int critheap_pop(CritHeap *h);
void critheap_build_from_patients(CritHeap *h);
void critheap_free(CritHeap *h);

#endif // PRIORITY_QUEUE_H
