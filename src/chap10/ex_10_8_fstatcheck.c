//
// Created by Richard on 2019/7/30.
//

#include "ex_10_8_fstatcheck.h"

#include "../csapp.h"

void main_ex_10_8(int argc, char **argv) {
    struct stat st;
    char *type, *readok;

    Fstat(atoi(argv[1]), &st);

    if (S_ISREG(st.st_mode)) {
        type = "regular";
    } else if (S_ISDIR(st.st_mode)) {
        type = "directory";
    } else {
        type = "other";
    }

    if ((st.st_mode & S_IRUSR)) {
        readok = "yes";
    } else {
        readok = "no";
    }

    printf("type: %s, read: %s\n", type, readok);
    exit(0);
}
