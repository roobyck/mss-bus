
#include <sys/types.h>
#include <sys/time.h>

#define RTS_OFF	    0
#define RTS_ON	    1

int libser_open (const char *device, int speed);
void libser_close (int fd);
void libser_flush (int fd);
int libser_setrts (int fd, int onoff);
int serial_isempty (int fd);
int libser_read (int fd, char *buff, size_t len, struct timeval *tv);
int libser_write (int fd, const void * buff, size_t count);

