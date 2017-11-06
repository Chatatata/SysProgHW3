#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>

#include "kmessaged.h"

#define KMESSAGED_DEVICE_FILE_NAME "/dev/kmessaged0"

int main(const int argc, const char *argv[])
{
    int fd = 0;

    if (argc != 2 || (strcmp(argv[1], "EXCLUDE_READ") && strcmp(argv[1], "INCLUDE_READ"))) {
        fprintf(stderr, "usage: modchange [EXCLUDE_READ | INCLUDE_READ]");

        return 9;
    }

    fd = open(KMESSAGED_DEVICE_FILE_NAME, 0);

    printf("modchange: fd is open %d\n", fd);

    if (fd > 0) {
        if (strcmp(argv[1], "EXCLUDE_READ") == 0) {
            return ioctl(fd, KMESSAGED_IOC_S_RDMOD, 0);
        } else {
            return ioctl(fd, KMESSAGED_IOC_S_RDMOD, 1);
        }
    } else {
        return -fd;
    }
}

