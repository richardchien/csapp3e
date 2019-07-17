//
// Created by Richard on 2019/7/17.
//

#include "ex_10_3.h"

#include "../csapp.h"

void main_ex_10_3(int argc, char *argv[]) {
    int fd;
    char c;

    fd = Open("CMakeLists.txt", O_RDONLY, 0);

    if (Fork() == 0) { // child
        Read(fd, &c, 1);
        printf("child, c = %c\n", c);
        exit(0);
    }

    Wait(NULL);
    Read(fd, &c, 1);
    printf("father, c = %c\n", c);
}
