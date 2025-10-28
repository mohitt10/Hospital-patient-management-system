// FILE: doctor.h
#ifndef DOCTOR_H
#define DOCTOR_H

typedef struct Doctor {
    int id;
    char name[50];
    char specialization[30];
    int numPatients;
    struct Doctor *next;
} Doctor;

Doctor* addDoctor(Doctor* head, int id, const char* name, const char* specialization);
Doctor* findDoctorById(Doctor* head, int id);
void displayDoctors(Doctor* head);
int assignDoctor(Doctor* head); // returns doctor id or -1
void decrementDoctorCount(Doctor* head, int id);

#endif // DOCTOR_H