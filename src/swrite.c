
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
    char c, d;

    fd = libser_open("/dev/ttyS1", B9600);

    if (fd < 0)
        return 1;

    for (;;)
    {
        libser_flush(fd);
        for (c = 'A'; c < 'Z'; c++) 
        {
            if (libser_write(fd, &c, 1) != 1)
                printf(".");
            else
            {
                tv.tv_sec  = 0;
                tv.tv_usec = 10000;
                d = ' ';

                if (libser_read(fd, &d, 1, &tv) == 1) {
                    if (c==d)
                        printf("%c" , d);
                    else
                        printf("!(%c%c)", c, d);
                }
                else
                    printf(".");
            }
            fflush(stdout);
        }
        usleep(100);
    }

    return 0;
}

