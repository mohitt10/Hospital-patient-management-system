#ifndef PATIENT_H
#define PATIENT_H

#include <time.h>

typedef struct Patient {
    int id;
    char name[50];
    int age;
    char gender; // 'M'/'F'/'O'
    int triage; // 1=Critical, 2=Serious, 3=Normal
    char type[10]; // "OPD" / "IPD"
    char status[20]; // "Waiting" / "UnderTreatment" / "Discharged"
    int assignedDoctorId; // -1 if none
    time_t arrival;
    struct Patient *next;
} Patient;

// Linked list ops
Patient* insertPatient(Patient* head, Patient* node);
Patient* createPatient(int id, const char* name, int age, char gender, int triage, const char* type);
Patient* findPatientById(Patient* head, int id);
Patient* removePatientById(Patient* head, int id, Patient** removed);
void displayPatients(Patient* head);
void displayPatientsBrief(Patient* head);

#endif // PATIENT_H