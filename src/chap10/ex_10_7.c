//
// Created by Richard on 2019/7/30.
//

#include "ex_10_7.h"

#include "../csapp.h"

void main_ex_10_7(int argc, char **argv) {
    int n;
    rio_t rio;
    char buf[MAXBUF];

    Rio_readinitb(&rio, STDIN_FILENO);
    while ((n = Rio_readnb(&rio, buf, MAXBUF)) != 0) {
        Rio_writen(STDOUT_FILENO, buf, n);
    }
}
