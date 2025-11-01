#ifndef PATIENT_H
#define PATIENT_H

#include <time.h>
#include "user.h"

#define MAX_NAME 64
#define MAX_HISTORY 512
#define MAX_PRESC 256

typedef struct {
    int id;
    char name[MAX_NAME];
    int age;
    int room; // 0 if not assigned
    int criticalness; // 1 (high) - 3 (low)
    double bill;
    int admitted; // 1 admitted, 0 OPD
    char clinical_history[MAX_HISTORY];
    char prescription[MAX_PRESC];
    int has_appointment; // 0/1
    char appointment_with[32];
    time_t appointment_time;
} Patient;

typedef struct {
    Patient *data;
    int size;
    int cap;
} PatientArr;

void patientarr_init(PatientArr *a);
void load_patients(PatientArr *a);
void save_patients(PatientArr *a);
int patient_find_index_by_id(PatientArr *a, int id);
int next_patient_id(PatientArr *a);

#endif // PATIENT_H