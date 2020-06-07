//
// Created by Richard on 2019/7/11.
//

#include "ex_8_25_tfgets.h"

#include "../csapp.h"

#define TIMEOUT 5
#define MAXINPUT 512

sigjmp_buf buf;

void timeout_handler(int sig) {
    Sio_puts("timeout!");
    siglongjmp(buf, 1);
}

void main_ex_8_25(int argc, char *argv[]) {
    signal(SIGUSR1, timeout_handler);

    int child_pid;
    if ((child_pid = Fork()) == 0) {
        // child
        Sleep(TIMEOUT);
        Kill(getppid(), SIGUSR1);
        exit(0);
    }

    if (!sigsetjmp(buf, 1)) {
        char s[MAXINPUT + 1] = {0};
        fgets(s, MAXINPUT + 1, stdin);
        printf("%s", s);
    }

    Kill(child_pid, SIGTERM);
    Waitpid(child_pid, NULL, 0);
}
