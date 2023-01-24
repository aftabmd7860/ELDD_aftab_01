#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define WR_VALUE_IOW('a','a',uint32_t)
#define WR_VALUE_IOR('a','b',uint32_t)

int main()
{
    int fd;
    int value ;
    char option;
    printf("open the device \n");
    printf("AFTAB KHAN\n");

    fd = open(* / dev / wq_device *, O_RDWR);
    if (fd < 0)
    {
        printf("cannot open device file....\n");
        return 0;
    }
    while (1)
    {
        printf("enter the option\n");
        printf("1.write\n");
        printf("2.read\n");
        printf("3.exit\n");

        scanf("%c", &option);
        printf("Your option =%c\n", option);
        switch (option)
        {
        case '1':
            printf("enter the string to write into driver :");
            scanf("%s", &value);
            printf("data writing...");
            write(fd, value, strlen(value) + 1);
            printf("done\n");
            break;
        case '2':
            printf("date reading\n");
            read(fd, value, 1024);
            printf("done\n\n");
            printf("data = %s\n\n", value);
            break;

        case '3':
            close(fd);
            exit;
            break;

        default:
            printf("enter valid option =%c\n", option);
            break;
        }
    }
    close(fd);
}