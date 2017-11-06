#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

#include "kmessaged.h"

#define KMESSAGED_DEVICE_FILE_NAME "/dev/kmessaged0"

int main(const int argc, const char *argv[])
{
    int result = 0;
    int fd = 0;
    char *buf = malloc(sizeof(char) * BUFSIZ);

    fd = open(KMESSAGED_DEVICE_FILE_NAME, 0);

    printf("kmessaged-helper: fd is open %d\n", fd);

    setpwent();

    struct passwd *cur = getpwent();
    
    while (cur) {
        printf("kmessaged-helper: pwentry found %u:%s\n", cur->pw_uid, cur->pw_name);
        sprintf(buf, "%u:%s", cur->pw_uid, cur->pw_name);

        cur = getpwent();

        result = ioctl(fd, KMESSAGED_IOC_S_REGUID, buf);
        memset(buf, 0, sizeof(char)* BUFSIZ);
    }

    return 0;
}
