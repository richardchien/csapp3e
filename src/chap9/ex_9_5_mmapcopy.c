//
// Created by Richard on 2019/7/16.
//

#include "ex_9_5_mmapcopy.h"

#include "../csapp.h"

void main_ex_9_5(int argc, char *argv[]) {
    const char *filename = "../README.md";
    int fd = open(filename, O_RDONLY);
    struct stat st;
    Stat(filename, &st);
    char *ptr = (char *) Mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE | MAP_FILE, fd, 0);
    write(STDOUT_FILENO, ptr, st.st_size);
}
