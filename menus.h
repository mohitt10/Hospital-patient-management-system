#ifndef MENUS_H
#define MENUS_H

#include "user.h"
#include "patient.h"
#include "priority_queue.h"

void receptionist_menu(PatientArr *patients, CritHeap *heap);
void doctor_menu(PatientArr *patients, CritHeap *heap);
void pharmacist_menu(PatientArr *patients);

#endif // MENUS_H
