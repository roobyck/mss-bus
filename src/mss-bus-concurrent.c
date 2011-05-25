
#include <pthread.h>
#include "mss-bus.h"
#include "mss-bus-concurrent.h"

/** Holds threads' handles. */
pthread_t *listener_thread, *writer_thread, *master_thread;

struct mss_master_params {
    const mss_addr* slaves;
    int slaves_count;
};

void* mss_concurrent_master( void* param ) {
    struct mss_master_params* mmp = (struct mss_master_params*) param;
    mss_run_master( mmp->slaves, mmp->slaves_count );
    free( mmp->slaves );
    free( mmp );
    return 0;
}

void mss_concurrent_run( int addr, const mss_addr* slaves, int slaves_count ) {
    // launches master if needed
    if( slaves != 0 ) {
        struct mss_master_params* mmp =
            (struct mss_master_params*) malloc(
                sizeof(struct mss_master_params)
            );
        int res =
            pthread_create( master_thread, NULL, mss_concurrent_master, mmp );
    }

    if( addr != NO_SLAVE ) {
        mss_init_slave( addr );
    }
    
    

}

void mss_concurrent_stop(  ) {
    mss_stop_master();
}