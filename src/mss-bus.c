
/**
 * Contains common functions, required for both client and server.
 */

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <sys/time.h>
 
#include "libser.h"
#include "mss-bus.h"

int mss_is_initialized = FALSE;
int mss_fd = -1;

int mss_init () {
    if( !mss_is_initialized ) {
        mss_fd = libser_open( "/dev/ttyS1", B9600 );
        if( mss_fd < 0 ) return -1;
        mss_is_initialized = TRUE;
    }
    return 0;
}

void mss_close () {
    mss_is_initialized = FALSE;
    libser_close( mss_fd );
    mss_fd = -1;
}

