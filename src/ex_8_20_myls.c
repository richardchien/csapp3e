//
// Created by Richard on 2019/7/10.
//

#include "ex_8_20_myls.h"

#include "csapp.h"

#define MAXARGS 128

void dump_argv(char *argv[]) {
    for (char **p = argv; *p; p++) {
        printf("%s ", *p);
    }
    printf("\n");
}

void dump_environ() {
    for (char **p = environ; *p; p++) {
        printf("%s\n", *p);
    }
    printf("\n");
}

void main_ex_8_20(int argc, char *argv[]) {
//    dump_environ();
    char *exec_argv[MAXARGS];
    char *columns_s = getenv("COLUMNS");
    exec_argv[0] = "/bin/ls";
    exec_argv[1] = "-w";
    exec_argv[2] = columns_s ? columns_s : "80";
    for (int i = 1; i < argc + 1; i++) {
        exec_argv[3 + i - 1] = argv[i];
    }
//    dump_argv(exec_argv);
    execve(exec_argv[0], exec_argv, environ);
}
