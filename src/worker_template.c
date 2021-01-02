#include "worker_template.h"

void setThreadName(pthread_t t, char* name) {
    char* shortened = name;
    if (strlen(name) >= 15) {
        shortened = malloc(16);
        strncpy(shortened, name, 15);
    }
    pthread_setname_np(t, shortened);
}
