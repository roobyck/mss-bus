

#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/time.h>

#include "libser.h"

int main (void) 
{
    int fd;
    struct timeval tv;
    char c;

    fd = libser_open("/dev/ttyS1", B9600);

    for (;;)
    {
        tv.tv_sec  = 2;
        tv.tv_usec = 0;

        if (libser_read(fd, &c, 1, &tv) != 1)
            printf(".");
        else
            printf("%c", c);

        fflush(stdout);
    }

    return 0;
}

