#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "menus.h"
#include "patient.h"
#include "priority_queue.h"

#define MAX_INPUT 256

/* Helper: print patient brief */
static void print_patient_brief(const Patient *p){
    printf("ID:%d Name:%s Age:%d Room:%d Crit:%d Admitted:%d Bill:%.2f\n",
           p->id, p->name, p->age, p->room, p->criticalness, p->admitted, p->bill);
}

/* ---------- Receptionist operations ---------- */

static void add_patient(PatientArr *a, CritHeap *h){
    Patient p;
    memset(&p, 0, sizeof(p));
    p.id = next_patient_id(a);
    printf("Name: "); getchar(); fgets(p.name, MAX_NAME, stdin); p.name[strcspn(p.name, "\n")] = 0;
    printf("Age: "); scanf("%d", &p.age);
    printf("Criticalness (1-high,2-mid,3-low): "); scanf("%d", &p.criticalness);
    printf("Admit? (1 yes, 0 OPD): "); scanf("%d", &p.admitted);
    if(p.admitted){
        printf("Assign room number: "); scanf("%d", &p.room);
    } else p.room = 0;
    p.bill = 0.0;
    p.has_appointment = 0;
    p.clinical_history[0] = 0;
    p.prescription[0] = 0;

    if(a->size == a->cap){
        a->cap *= 2;
        a->data = realloc(a->data, sizeof(Patient)*a->cap);
        if(!a->data){ perror("realloc add_patient"); exit(1); }
    }
    a->data[a->size++] = p;
    int idx = a->size - 1;
    if(p.admitted && p.criticalness>=1 && p.criticalness<=3) critheap_push(h, idx);
    save_patients(a);
    printf("Added patient ID=%d name=%s\n", p.id, p.name);
}

static void remove_patient(PatientArr *a, CritHeap *h){
    int id; printf("Patient id to remove: "); scanf("%d", &id);
    int idx = patient_find_index_by_id(a, id);
    if(idx==-1){ printf("No patient with id %d\n", id); return; }
    // swap with last and shrink
    a->data[idx] = a->data[a->size-1];
    a->size--;
    save_patients(a);
    critheap_build_from_patients(h);
    printf("Removed patient id %d\n", id);
}

static void discharge_patient(PatientArr *a, CritHeap *h){
    int id; printf("Patient id to discharge: "); scanf("%d", &id);
    int idx = patient_find_index_by_id(a, id);
    if(idx==-1){ printf("No patient with id %d\n", id); return; }
    a->data[idx].admitted = 0;
    a->data[idx].room = 0;
    double amount;
    printf("Enter final payment to add (current bill %.2f): ", a->data[idx].bill);
    scanf("%lf", &amount);
    a->data[idx].bill += amount;
    save_patients(a);
    critheap_build_from_patients(h);
    printf("Patient %d discharged. Total bill %.2f\n", id, a->data[idx].bill);
}

static void view_rooms(PatientArr *a){
    printf("Room -> Patient mapping:\n");
    for(int i=0;i<a->size;i++){
        if(a->data[i].room > 0){
            printf("Room %d : ID=%d Name=%s\n", a->data[i].room, a->data[i].id, a->data[i].name);
        }
    }
}

static void schedule_appointment(PatientArr *a){
    int id; printf("Patient id to schedule appointment: "); scanf("%d", &id);
    int idx = patient_find_index_by_id(a, id);
    if(idx==-1){ printf("No patient with id %d\n", id); return; }
    Patient *p = &a->data[idx];
    printf("Appointment with (doctor username): "); scanf("%31s", p->appointment_with);
    long offset; printf("Enter offset minutes from now for appointment (e.g. 60): "); scanf("%ld", &offset);
    p->appointment_time = time(NULL) + offset*60;
    p->has_appointment = 1;
    save_patients(a);
    printf("Scheduled appointment for %s at %s", p->name, ctime(&p->appointment_time));
}

static void view_appointments(PatientArr *a){
    int found = 0;
    for(int i=0;i<a->size;i++){
        if(a->data[i].has_appointment){
            found = 1;
            printf("ID=%d Name=%s With=%s At=%s", a->data[i].id, a->data[i].name,
                   a->data[i].appointment_with, ctime(&a->data[i].appointment_time));
        }
    }
    if(!found) printf("No appointments scheduled.\n");
}

static void billing_for_patient(PatientArr *a){
    int id; printf("Patient id for billing: "); scanf("%d", &id);
    int idx = patient_find_index_by_id(a, id);
    if(idx==-1){ printf("No patient with id %d\n", id); return; }
    double amt; printf("Enter amount to add to bill: "); scanf("%lf", &amt);
    a->data[idx].bill += amt;
    save_patients(a);
    printf("Added %.2f to bill. Current bill %.2f\n", amt, a->data[idx].bill);
}

/* Patient lookup by id/name/room */
static void lookup_patient(PatientArr *a){
    printf("Lookup by: 1.ID 2.Name 3.Room\nChoice: ");
    int ch; scanf("%d", &ch);
    if(ch==1){
        int id; printf("Enter ID: "); scanf("%d", &id);
        int idx = patient_find_index_by_id(a, id);
        if(idx==-1) printf("Not found.\n");
        else {
            Patient *p = &a->data[idx];
            print_patient_brief(p);
            printf("History: %s\nPrescription: %s\n", p->clinical_history, p->prescription);
        }
    } else if(ch==2){
        char name[MAX_INPUT];
        printf("Enter name: "); getchar(); fgets(name, MAX_INPUT, stdin); name[strcspn(name, "\n")] = 0;
        int found = 0;
        for(int i=0;i<a->size;i++){
            if(strcasecmp(a->data[i].name, name)==0){
                found = 1;
                print_patient_brief(&a->data[i]);
            }
        }
        if(!found) printf("No patient with that name.\n");
    } else if(ch==3){
        int room; printf("Enter room number: "); scanf("%d", &room);
        int found=0;
        for(int i=0;i<a->size;i++){
            if(a->data[i].room == room){
                found=1;
                print_patient_brief(&a->data[i]);
            }
        }
        if(!found) printf("No patient in that room.\n");
    } else printf("Invalid choice.\n");
}

/* ---------- Doctor operations ---------- */

static void view_clinical_history(PatientArr *a){
    int id; printf("Patient id: "); scanf("%d", &id);
    int idx = patient_find_index_by_id(a, id);
    if(idx==-1) { printf("No patient with id %d\n", id); return; }
    printf("Clinical history for %s:\n%s\n", a->data[idx].name, a->data[idx].clinical_history);
}

static void add_clinical_history(PatientArr *a){
    int id; printf("Patient id to add history: "); scanf("%d", &id);
    int idx = patient_find_index_by_id(a, id);
    if(idx==-1) { printf("No patient with id %d\n", id); return; }
    printf("Enter clinical notes (single line): "); getchar();
    char buf[MAX_HISTORY];
    fgets(buf, MAX_HISTORY, stdin); buf[strcspn(buf, "\n")] = 0;
    if(strlen(a->data[idx].clinical_history) + strlen(buf) + 2 < MAX_HISTORY) {
        if(strlen(a->data[idx].clinical_history)>0) strncat(a->data[idx].clinical_history, "\n", MAX_HISTORY - strlen(a->data[idx].clinical_history) - 1);
        strncat(a->data[idx].clinical_history, buf, MAX_HISTORY - strlen(a->data[idx].clinical_history) - 1);
    } else {
        printf("History truncated (not enough space).\n");
    }
    save_patients(a);
    printf("Updated clinical history.\n");
}

static void prescribe(PatientArr *a){
    int id; printf("Patient id to prescribe for: "); scanf("%d", &id);
    int idx = patient_find_index_by_id(a, id);
    if(idx==-1) { printf("No patient with id %d\n", id); return; }
    printf("Enter prescription (single line): "); getchar();
    char presc[MAX_PRESC];
    fgets(presc, MAX_PRESC, stdin); presc[strcspn(presc, "\n")] = 0;
    strncpy(a->data[idx].prescription, presc, MAX_PRESC-1);
    save_patients(a);
    printf("Prescription saved.\n");
}

static void opd_to_admit(PatientArr *a, CritHeap *h){
    int id; printf("Patient id to admit: "); scanf("%d", &id);
    int idx = patient_find_index_by_id(a, id);
    if(idx==-1) { printf("No patient with id %d\n", id); return; }
    a->data[idx].admitted = 1;
    printf("Assign room number: "); scanf("%d", &a->data[idx].room);
    printf("Set criticalness (1-3): "); scanf("%d", &a->data[idx].criticalness);
    save_patients(a);
    critheap_build_from_patients(h);
    printf("Patient admitted.\n");
}

/* ---------- Pharmacist operations ---------- */

static void find_medicine_by_patient(PatientArr *a){
    int id; printf("Patient id (to find medicine location): "); scanf("%d", &id);
    int idx = patient_find_index_by_id(a, id);
    if(idx==-1){ printf("No patient with id %d\n", id); return; }
    Patient *p = &a->data[idx];
    if(strlen(p->prescription)==0){ printf("No prescription set by doctor.\n"); return; }
    if(p->room==0) printf("Patient is OPD. Check pharmacy counters.\n");
    else {
        int shelf = (p->room % 10) + 1; // simple mapping for demo
        printf("Prescription for %s: %s\n", p->name, p->prescription);
        printf("Suggested pharmacy shelf: %d (derived from room %d)\n", shelf, p->room);
    }
}

/* ---------- Menus exposed ---------- */

void receptionist_menu(PatientArr *patients, CritHeap *heap){
    while(1){
        printf("\n--- Receptionist Menu ---\n");
        printf("1 Add patient\n2 Remove patient\n3 Billing\n4 Discharge\n5 View room assignments\n6 View appointments\n7 Schedule appointment\n8 Patient lookup\n9 View next critical patient\n0 Logout\nChoice: ");
        int ch; scanf("%d", &ch);
        if(ch==1) add_patient(patients, heap);
        else if(ch==2) remove_patient(patients, heap);
        else if(ch==3) billing_for_patient(patients);
        else if(ch==4) discharge_patient(patients, heap);
        else if(ch==5) view_rooms(patients);
        else if(ch==6) view_appointments(patients);
        else if(ch==7) schedule_appointment(patients);
        else if(ch==8) lookup_patient(patients);
        else if(ch==9){
            int idx = critheap_pop(heap);
            if(idx==-1) printf("No critical patients\n");
            else {
                Patient *p = &patients->data[idx];
                printf("Next critical patient:\n");
                print_patient_brief(p);
                // push back (keep in queue) for demo purposes
                critheap_push(heap, idx);
            }
        }
        else if(ch==0) break;
        else printf("Invalid choice.\n");
    }
}

void doctor_menu(PatientArr *patients, CritHeap *heap){
    while(1){
        printf("\n--- Doctor Menu ---\n");
        printf("1 View clinical history\n2 Add clinical notes\n3 Prescribe\n4 OPD->Admit\n5 Lookup patient\n6 View next critical patient\n0 Logout\nChoice: ");
        int ch; scanf("%d", &ch);
        if(ch==1) view_clinical_history(patients);
        else if(ch==2) add_clinical_history(patients);
        else if(ch==3) prescribe(patients);
        else if(ch==4) opd_to_admit(patients, heap);
        else if(ch==5) lookup_patient(patients);
        else if(ch==6){
            int idx = critheap_pop(heap);
            if(idx==-1) printf("No critical patients\n");
            else {
                Patient *p = &patients->data[idx];
                printf("Next critical patient:\n");
                print_patient_brief(p);
                critheap_push(heap, idx);
            }
        }
        else if(ch==0) break;
        else printf("Invalid choice.\n");
    }
}

void pharmacist_menu(PatientArr *patients){
    while(1){
        printf("\n--- Pharmacist Menu ---\n");
        printf("1 Find medicine by patient\n2 Lookup patient\n0 Logout\nChoice: ");
        int ch; scanf("%d", &ch);
        if(ch==1) find_medicine_by_patient(patients);
        else if(ch==2) lookup_patient(patients);
        else if(ch==0) break;
        else printf("Invalid choice.\n");
    }
}
