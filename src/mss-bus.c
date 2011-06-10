
/**
 * Contains common functions, required for both client and server.
 */

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <sys/time.h>
 
#include "libser.h"
#include "mss-bus.h"

int mss_fd = -1;

int mss_init (const char *device, int speed) {
    if( mss_fd < 0 ) {
        mss_fd = libser_open( device, speed );
        if( mss_fd < 0 ) return MSS_UNINITIALIZED;
    }
    return MSS_OK;
}

void mss_close () {
    libser_close( mss_fd );
    mss_fd = -1;
}

