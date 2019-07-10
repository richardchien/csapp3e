//
// Created by Richard on 2019/7/8.
//

#include "fig_8_37_sigchld.h"

#include "csapp.h"

static void handler1(int sig) {
    int olderrno = errno;

    if (waitpid(-1, NULL, 0) < 0) {
        sio_error("waitpid error");
    }
    Sio_puts("Handler reaped child\n");
    Sleep(1);

    errno = olderrno;
}

static void handler2(int sig) {
    Sio_puts("handler2 begin...\n");
    int olderrno = errno;

    while (waitpid(-1, NULL, 0) > 0) {
        Sio_puts("Handler reaped child\n");
    }
    if (errno != ECHILD) {
        sio_error("waitpid error");
    }

    Sleep(1);
    errno = olderrno;
    Sio_puts("handler2 end..\n");
}

void main_fig_8_37(int argc, char **argv) {
    if (signal(SIGCHLD, handler2) == SIG_ERR) {
        unix_error("signal error");
    }

    for (int i = 0; i < 3; i++) {
        if (Fork() == 0) {
            Sleep(i + 2);
            printf("Hello from child %d\n", (int) getpid());
            exit(0);
        }
    }

    while (1) {
        printf("WOW!\n");
        Sleep(1);
    }
//    char buf[MAXBUF];
//    read(STDIN_FILENO, buf, sizeof(buf));
//    printf("Parent processing input\n");
//    while (1)
//        ;
}
