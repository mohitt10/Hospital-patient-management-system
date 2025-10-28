// FILE: main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "patient.h"
#include "doctor.h"
#include "heap.h"
#include "fileio.h"

// helper to read a line (trim newline)
void readString(char *buf, int n) {
    if (!fgets(buf, n, stdin)) return;
    size_t len = strlen(buf);
    if (len && buf[len-1] == '\n') buf[len-1] = '\0';
}

int main() {
    Patient* patientHead = NULL;
    Doctor* doctorHead = NULL;
    MinHeap* heap = createHeap(64);

    int choice;
    while (1) {
        printf("\n--- Hospital Management (DSA C) ---\n");
        printf("1. Register Doctor\n");
        printf("2. Display All Doctors\n");
        printf("3. Register Patient\n");
        printf("4. Serve Next Critical Patient\n");
        printf("5. Display All Patients\n");
        printf("6. Display Patients by Doctor\n");
        printf("7. Discharge Patient\n");
        printf("8. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) { printf("Invalid input\n"); break; }
        getchar(); // consume newline

        if (choice == 1) {
            int id; char name[50]; char spec[30];
            printf("Enter Doctor ID: "); scanf("%d", &id); getchar();
            printf("Enter Name: "); readString(name, sizeof(name));
            printf("Enter Specialization: "); readString(spec, sizeof(spec));
            doctorHead = addDoctor(doctorHead, id, name, spec);
            printf("Doctor added.\n");
        }
        else if (choice == 2) {
            displayDoctors(doctorHead);
        }
        else if (choice == 3) {
            int id, age, triage; char name[50], type[10]; char gender;
            printf("Enter Patient ID: "); scanf("%d", &id); getchar();
            printf("Enter Name: "); readString(name, sizeof(name));
            printf("Enter Age: "); scanf("%d", &age); getchar();
            printf("Enter Gender (M/F/O): "); scanf("%c", &gender); getchar();
            printf("Enter Triage (1-critical,2-serious,3-normal): "); scanf("%d", &triage); getchar();
            printf("Enter Type (OPD/IPD): "); readString(type, sizeof(type));

            Patient* p = createPatient(id, name, age, gender, triage, type);
            // assign doctor if available
            int did = assignDoctor(doctorHead);
            p->assignedDoctorId = did;
            patientHead = insertPatient(patientHead, p);
            // add to heap
            HeapNode hn = {p->triage, p->arrival, p->id};
            pushHeap(heap, hn);
            printf("Patient registered (ID:%d) and assigned Doctor ID: %d\n", p->id, p->assignedDoctorId);
        }
        else if (choice == 4) {
            if (isHeapEmpty(heap)) { printf("No patients in queue.\n"); continue; }
            // pop until we find a patient that still exists and is waiting
            while (!isHeapEmpty(heap)) {
                HeapNode hn = popMin(heap);
                Patient* p = findPatientById(patientHead, hn.patient_id);
                if (!p) { continue; } // patient removed/discharged earlier
                if (strcmp(p->status, "Waiting") != 0) { continue; }
                // serve this patient
                strcpy(p->status, "UnderTreatment");
                printf("Serving patient ID:%d Name:%s Triage:%d AssignedDoctor:%d\n",
                       p->id, p->name, p->triage, p->assignedDoctorId);
                break;
            }
        }
        else if (choice == 5) {
            displayPatients(patientHead);
        }
        else if (choice == 6) {
            int did; printf("Enter Doctor ID: "); scanf("%d", &did); getchar();
            Doctor* d = findDoctorById(doctorHead, did);
            if (!d) { printf("Doctor not found\n"); continue; }
            printf("Doctor: %s (Patients: %d)\n", d->name, d->numPatients);
            // list patients assigned
            Patient* cur = patientHead; int found = 0;
            while (cur) {
                if (cur->assignedDoctorId == did) { printf("ID:%d Name:%s Triage:%d Status:%s\n",
                                                         cur->id, cur->name, cur->triage, cur->status); found = 1; }
                cur = cur->next;
            }
            if (!found) printf("No patients currently assigned.\n");
        }
        else if (choice == 7) {
            int id; printf("Enter patient ID to discharge: "); scanf("%d", &id); getchar();
            Patient* removed = NULL;
            patientHead = removePatientById(patientHead, id, &removed);
            if (!removed) { printf("Patient not found.\n"); continue; }
            // mark discharged and append record
            strcpy(removed->status, "Discharged");
            appendDischargeRecord(removed);
            if (removed->assignedDoctorId != -1) decrementDoctorCount(doctorHead, removed->assignedDoctorId);
            printf("Patient ID %d discharged and recorded.\n", removed->id);
            free(removed);
        }
        else if (choice == 8) {
            printf("Exiting...\n"); break;
        }
        else {
            printf("Invalid choice\n");
        }
    }

    destroyHeap(heap);
    // free lists (not strictly necessary at exit, but good hygiene)
    Patient* p = patientHead;
    while (p) { Patient* nx = p->next; free(p); p = nx; }
    Doctor* d = doctorHead;
    while (d) { Doctor* nx = d->next; free(d); d = nx; }

    return 0;
}
