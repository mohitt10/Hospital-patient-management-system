#ifndef USER_H
#define USER_H

typedef enum { ROLE_RECEPTIONIST=1, ROLE_DOCTOR=2, ROLE_PHARMACIST=3 } Role;

typedef struct {
    char username[32];
    char password[32];
    Role role;
} User;

typedef struct {
    User *data;
    int size;
    int cap;
} UserArr;

void userarr_init(UserArr *a);
void load_users(UserArr *users);
void save_users(UserArr *users);
int register_user(UserArr *users);
User *login_user(UserArr *users);
void bootstrap_users_if_empty(UserArr *users);

#endif // USER_H