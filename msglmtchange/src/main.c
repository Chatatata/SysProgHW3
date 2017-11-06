#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "kmessaged.h"

#define KMESSAGED_DEVICE_FILE_NAME "/dev/kmessaged0"

int main(const int argc, const char *argv[])
{
    int fd = 0;

    if (argc != 2) {
        fprintf(stderr, "usage: msglmtchange [msglmt]");

        return 9;
    }

    fd = open(KMESSAGED_DEVICE_FILE_NAME, 0);

    printf("msglmtchange: fd is open %d\n", fd);

    if (fd > 0) {
        return ioctl(fd, KMESSAGED_IOC_S_MSGLMT, atoi(argv[1]));
    } else {
        return -fd;
    }
}

