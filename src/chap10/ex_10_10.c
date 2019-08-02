//
// Created by Richard on 2019/8/2.
//

#include "ex_10_10.h"

#include "../csapp.h"

void main_ex_10_10(int argc, char **argv) {
    if (argc > 1) {
        int fd = Open(argv[1], O_RDONLY, 0);
        Dup2(fd, STDIN_FILENO);
    }

    int n;
    rio_t rio;
    char buf[MAXLINE];

    Rio_readinitb(&rio, STDIN_FILENO);
    while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
        Rio_writen(STDOUT_FILENO, buf, n);
    }
}
