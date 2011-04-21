#ifndef _MSS_BUS_H_
#define _MSS_BUS_H_

#include "./types.h"
#include "./config.h"

/**
 * Launches a mss-master which takes control over the network, allowing connected slaves to exchange
 * data. Slaves are staticly assigned to the master. Note that mss_run_master is blocking function
 * and will continue to run master until mss_stop_master is called.
 * @param slaves A pointer to array of slaves managed by the master. Each entry contains address of one slave in the network.
 * @param slave_count Amount of slaves in slaves. :P
 */
void mss_run_master( const mss_addr* slaves, int slaves_count );

/**
 * Stops execution of mss-master.
 */
void mss_stop_master();

void mss_slave_send( mss_addr addr, const char* data, size_t data_len );

void mss_slave_recv( char buffer, size_t* data_len );

#endif
