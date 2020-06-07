//
// Created by Richard on 2019/7/11.
//

#include "ex_8_24.h"

#include "../csapp.h"

#define N 2

void main_ex_8_24(int argc, char *argv[]) {
    int status, i;
    pid_t pid;

    for (i = 0; i < N; i++) {
        if ((pid = Fork()) == 0) {
            // child
            *((int *) 0) = 1;
            exit(100 + i);
        }
    }

    while ((pid = waitpid(-1, &status, 0)) > 0) {
        if (WIFEXITED(status)) {
            printf("child %d terminated normally with exit status = %d\n", pid, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            char s[50];
            int sig = WTERMSIG(status);
            sprintf(s, "child %d terminated by signal %d", pid, sig);
            psignal(sig, s);
        } else {
            printf("child %d terminated abnormally\n", pid);
        }
    }
}
