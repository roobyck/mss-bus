
/**
 * mss-bus-concurrent is a library built over a mss-bus, providing an
 * abstraction layer between direct bus management through mss-bus and program.
 * It runs several threads responsible for communicating with other peers over
 * the bus and provides interface for convinient data exchange over it.
 */

#ifndef _MSS_BUS_CONCURRENT_H_
#define _MSS_BUS_CONCURRENT_H_

#define NO_SLAVE -1

#include "types.h"

/**
 * 
 * @param addr Address to be set for underlying slave, or NO_SLAVE if no slave
 *          be run locally.
 * @param slaves pointer to table of slaves, or NULL if master shouldn't be
 *          started. In the first case, ownership of mss_addr array is taken by
 *          mss-bus-concurrent.
 */
void mss_concurrent_run (int addr, const mss_addr* slaves = 0, int slaves_count = 0);

void mss_concurrent_stop ();

#endif // _MSS_BUS_CONCURRENT_H_
