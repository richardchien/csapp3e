//
// Created by Richard on 2019/7/11.
//

#include "ex_8_22_mysystem.h"

#include "../csapp.h"

int mysystem(char *command) {
    pid_t pid = Fork();
    if (pid == 0) {
        // child
        char *argv[] = {"/bin/sh", "-c", command, NULL};
        Execve(argv[0], argv, environ);
    }

    printf("pid: %d\n", pid);

    int retpid;
    int status;
    if ((retpid = Waitpid(pid, &status, 0)) > 0) {
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else {
            return status;
        }
    } else {
        return -1;
    }
}

void main_ex_8_22(int argc, char *argv[]) {
    int ret = mysystem("sleep 230");
    printf("\nmysystem return: %d\n", ret);
}
