// FILE: fileio.c
#include <stdio.h>
#include <stdlib.h>
#include "fileio.h"

void appendDischargeRecord(Patient* p) {
    if (!p) return;
    FILE *fp = fopen("discharged.txt", "a");
    if (!fp) return;
    // CSV: id,name,age,gender,triage,type,assignedDoctorId,arrival,timestamp
    fprintf(fp, "%d,%s,%d,%c,%d,%s,%d,%ld\n",
            p->id, p->name, p->age, p->gender, p->triage, p->type, p->assignedDoctorId, (long)p->arrival);
    fclose(fp);
}
