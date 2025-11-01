#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"

#define INITIAL_CAP 64
#define USERS_FILE "users.dat"

void userarr_init(UserArr *a) {
    a->size = 0;
    a->cap = INITIAL_CAP;
    a->data = malloc(sizeof(User) * a->cap);
    if(!a->data){ perror("malloc userarr"); exit(1); }
}

void load_users(UserArr *users) {
    FILE *f = fopen(USERS_FILE, "rb");
    if(!f) { userarr_init(users); return; }
    userarr_init(users);
    User tmp;
    while(fread(&tmp, sizeof(User), 1, f) == 1) {
        if(users->size == users->cap){
            users->cap *= 2;
            users->data = realloc(users->data, sizeof(User)*users->cap);
            if(!users->data){ perror("realloc users"); exit(1); }
        }
        users->data[users->size++] = tmp;
    }
    fclose(f);
}

void save_users(UserArr *users) {
    FILE *f = fopen(USERS_FILE, "wb");
    if(!f){ perror("save_users"); return; }
    fwrite(users->data, sizeof(User), users->size, f);
    fclose(f);
}

static int username_exists(UserArr *users, const char *username) {
    for(int i=0; i<users->size; i++)
        if(strcmp(users->data[i].username, username) == 0) return 1;
    return 0;
}

int register_user(UserArr *users) {
    User u;
    printf("Enter username: "); scanf("%31s", u.username);
    if(username_exists(users, u.username)) { printf("Username exists.\n"); return 0; }
    printf("Enter password: "); scanf("%31s", u.password);
    printf("Role (1 Receptionist, 2 Doctor, 3 Pharmacist): ");
    int r; scanf("%d", &r);
    if(r<1 || r>3) { printf("Invalid role.\n"); return 0; }
    u.role = (Role)r;
    if(users->size == users->cap) {
        users->cap *= 2;
        users->data = realloc(users->data, sizeof(User)*users->cap);
        if(!users->data){ perror("realloc register"); exit(1); }
    }
    users->data[users->size++] = u;
    save_users(users);
    printf("Registered successfully!\n");
    return 1;
}

User *login_user(UserArr *users) {
    char username[32], password[32];
    printf("Username: "); scanf("%31s", username);
    printf("Password: "); scanf("%31s", password);
    for(int i=0;i<users->size;i++)
        if(strcmp(users->data[i].username, username)==0 && strcmp(users->data[i].password, password)==0)
            return &users->data[i];
    printf("Invalid credentials.\n");
    return NULL;
}

void bootstrap_users_if_empty(UserArr *users) {
    if(users->size == 0) {
        if(users->cap <= 0) userarr_init(users);
        User admin;
        strcpy(admin.username, "admin");
        strcpy(admin.password, "admin");
        admin.role = ROLE_RECEPTIONIST;
        users->data[users->size++] = admin;
        save_users(users);
        printf("Bootstrap admin/admin created (receptionist).\n");
    }
}
