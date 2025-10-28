// FILE: patient.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "patient.h"

Patient* createPatient(int id, const char* name, int age, char gender, int triage, const char* type) {
    Patient* p = (Patient*)malloc(sizeof(Patient));
    if (!p) return NULL;
    p->id = id;
    strncpy(p->name, name, sizeof(p->name)-1); p->name[sizeof(p->name)-1]='\0';
    p->age = age;
    p->gender = gender;
    p->triage = triage;
    strncpy(p->type, type, sizeof(p->type)-1); p->type[sizeof(p->type)-1] = '\0';
    strcpy(p->status, "Waiting");
    p->assignedDoctorId = -1;
    p->arrival = time(NULL);
    p->next = NULL;
    return p;
}

Patient* insertPatient(Patient* head, Patient* node) {
    if (!node) return head;
    if (!head) return node;
    Patient* cur = head;
    while (cur->next) cur = cur->next;
    cur->next = node;
    return head;
}

Patient* findPatientById(Patient* head, int id) {
    Patient* cur = head;
    while (cur) {
        if (cur->id == id) return cur;
        cur = cur->next;
    }
    return NULL;
}

// remove from linked list; if found, *removed points to the node (caller must free)
Patient* removePatientById(Patient* head, int id, Patient** removed) {
    *removed = NULL;
    Patient* cur = head;
    Patient* prev = NULL;
    while (cur) {
        if (cur->id == id) break;
        prev = cur; cur = cur->next;
    }
    if (!cur) return head; // not found
    if (prev) prev->next = cur->next; else head = cur->next;
    cur->next = NULL;
    *removed = cur;
    return head;
}

void displayPatients(Patient* head) {
    printf("\n%-5s %-15s %-4s %-6s %-7s %-10s %-6s %-12s\n",
           "ID","Name","Age","Gen","Triage","Type","Doc","Status");
    printf("--------------------------------------------------------------------------------\n");
    Patient* cur = head;
    while (cur) {
        printf("%-5d %-15s %-4d %-6c %-7d %-10s %-6d %-12s\n",
               cur->id, cur->name, cur->age, cur->gender, cur->triage,
               cur->type, cur->assignedDoctorId, cur->status);
        cur = cur->next;
    }
}

void displayPatientsBrief(Patient* head) {
    Patient* cur = head;
    while (cur) {
        printf("ID:%d Name:%s Triage:%d Status:%s Doc:%d\n",
               cur->id, cur->name, cur->triage, cur->status, cur->assignedDoctorId);
        cur = cur->next;
    }
}