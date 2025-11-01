#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "patient.h"

#define PATIENTS_FILE "patients.dat"
#define INITIAL_CAP 64

void patientarr_init(PatientArr *a) {
    a->size = 0;
    a->cap = INITIAL_CAP;
    a->data = malloc(sizeof(Patient)*a->cap);
    if(!a->data){ perror("malloc patientarr"); exit(1); }
}

void load_patients(PatientArr *a) {
    FILE *f = fopen(PATIENTS_FILE, "rb");
    if(!f) { patientarr_init(a); return; }
    patientarr_init(a);
    Patient tmp;
    while(fread(&tmp, sizeof(Patient), 1, f)==1) {
        if(a->size == a->cap){
            a->cap *= 2;
            a->data = realloc(a->data, sizeof(Patient)*a->cap);
            if(!a->data){ perror("realloc patients"); exit(1); }
        }
        a->data[a->size++] = tmp;
    }
    fclose(f);
}

void save_patients(PatientArr *a) {
    FILE *f = fopen(PATIENTS_FILE, "wb");
    if(!f){ perror("save_patients"); return; }
    fwrite(a->data, sizeof(Patient), a->size, f);
    fclose(f);
}

int patient_find_index_by_id(PatientArr *a, int id) {
    for(int i=0; i<a->size; i++) if(a->data[i].id == id) return i;
    return -1;
}

int next_patient_id(PatientArr *a) {
    int max = 0;
    for(int i=0; i<a->size; i++) if(a->data[i].id > max) max = a->data[i].id;
    return max + 1;
}
