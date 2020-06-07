//
// Created by Richard on 2019/7/17.
//

#include "ex_10_5.h"

#include "../csapp.h"

void main_ex_10_5(int argc, char *argv[]) {
    int fd1, fd2;
    char c;

    fd1 = Open("CMakeLists.txt", O_RDONLY, 0);
    fd2 = Open("CMakeLists.txt", O_RDONLY, 0);
    Read(fd2, &c, 1);
    printf("fd2 c = %c\n", c);
    dup2(fd2, fd1);
    Read(fd1, &c, 1);
    printf("fd1 c = %c\n", c);
}
