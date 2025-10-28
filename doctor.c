#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "doctor.h"

Doctor* addDoctor(Doctor* head, int id, const char* name, const char* specialization) {
    Doctor* d = (Doctor*)malloc(sizeof(Doctor));
    if (!d) return head;
    d->id = id;
    strncpy(d->name, name, sizeof(d->name)-1); d->name[sizeof(d->name)-1] = '\0';
    strncpy(d->specialization, specialization, sizeof(d->specialization)-1);
    d->specialization[sizeof(d->specialization)-1]='\0';
    d->numPatients = 0;
    d->next = NULL;
    if (!head) return d;
    Doctor* cur = head;
    while (cur->next) cur = cur->next;
    cur->next = d;
    return head;
}

Doctor* findDoctorById(Doctor* head, int id) {
    Doctor* cur = head;
    while (cur) {
        if (cur->id == id) return cur;
        cur = cur->next;
    }
    return NULL;
}

void displayDoctors(Doctor* head) {
    printf("\n%-5s %-20s %-20s %-8s\n","ID","Name","Specialization","Patients");
    printf("---------------------------------------------------------------\n");
    Doctor* cur = head;
    while (cur) {
        printf("%-5d %-20s %-20s %-8d\n", cur->id, cur->name, cur->specialization, cur->numPatients);
        cur = cur->next;
    }
}

int assignDoctor(Doctor* head) {
    if (!head) return -1;
    Doctor* cur = head;
    Doctor* min = head;
    while (cur) {
        if (cur->numPatients < min->numPatients) min = cur;
        cur = cur->next;
    }
    min->numPatients++;
    return min->id;
}

void decrementDoctorCount(Doctor* head, int id) {
    Doctor* d = findDoctorById(head, id);
    if (d && d->numPatients > 0) d->numPatients--;
}
