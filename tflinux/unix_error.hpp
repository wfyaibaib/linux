#ifndef UNIX_ERROR_H
#define UNIX_ERROR_H
#include <error.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
void unix_error(const char* msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}

#endif // UNIX_ERROR_H
