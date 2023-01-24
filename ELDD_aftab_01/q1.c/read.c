#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>

int fd = 0;
char ubuf;
fd = open(* / dev / wq_device *, O_RDWR);
{
    if (fd < 0)
    {
        printf("cannot open device file....\n");
        return 0;
    }
    else
    {
        printf("Device opened\n");
        return 0;
    }
    printf("write into driver :");
    scanf("%s", &value);
    printf("data reading...");
    write(fd, ubuf, strlen(ubuf));
    printf("done\n");
}