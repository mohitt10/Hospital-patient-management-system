#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"
#include "patient.h"
#include "priority_queue.h"
#include "menus.h"

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int main() {
    UserArr users;
    PatientArr patients;
    CritHeap heap;

    userarr_init(&users);
    patientarr_init(&patients);

    load_users(&users);
    bootstrap_users_if_empty(&users);  // creates default accounts if none exist

    load_patients(&patients);
    critheap_init(&heap, &patients);
    critheap_build_from_patients(&heap);

    int choice;
    while (1) {
        clear_screen();
        printf("=========================================\n");
        printf("   HOSPITAL PATIENT MANAGEMENT SYSTEM\n");
        printf("=========================================\n");
        printf("1. Login\n");
        printf("2. Register New User\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 0) {
            printf("Exiting system...\n");
            break;
        } 
        else if (choice == 2) {
            register_user(&users);
            save_users(&users);
            continue;
        } 
        else if (choice == 1) {
            User *logged = login_user(&users);
            if (logged == NULL) {
                printf("Invalid credentials.\n");
                getchar();
                continue;
            }

            printf("\nWelcome, %s!\n", logged->username);
            getchar();

            switch (logged->role) {
                case ROLE_RECEPTIONIST:
                    receptionist_menu(&patients, &heap);
                    break;
                case ROLE_DOCTOR:
                    doctor_menu(&patients, &heap);
                    break;
                case ROLE_PHARMACIST:
                    pharmacist_menu(&patients);
                    break;
                default:
                    printf("Unknown role. Contact admin.\n");
                    break;
            }

            save_patients(&patients);
            save_users(&users);
            critheap_build_from_patients(&heap);
        } 
        else {
            printf("Invalid choice.\n");
            getchar();
        }
    }

    // cleanup
    critheap_free(&heap);
    free(users.data);
    free(patients.data);

    return 0;
}
