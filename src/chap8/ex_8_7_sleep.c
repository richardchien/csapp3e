//
// Created by Richard on 2019/7/6.
//

#include "ex_8_7_sleep.h"

#include "../csapp.h"

static unsigned snooze(unsigned secs) {
    unsigned remained = sleep(secs);
    printf("Slept for %u of %u secs", secs - remained, secs);
    return remained;
}

static void sigint_handler(int sig) {
}

void main_ex_8_7(int argc, char **argv) {
    if (argc < 2) {
        printf("arguments too few\n");
        exit(1);
    }

    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        fprintf(stderr, "signal error\n");
    }

    unsigned secs = atol(argv[1]);
    snooze(secs);
}
