/*
 * Defines data and function used by mss-master to manage it's slaves. For internal use only.
 */
#ifndef _MSS_BUS_SLAVES_H_
#define _MSS_BUS_SLAVES_H_

#include "./types.h"
#include "./config.h"

/** Array describing structure of the network. Size of the array is stored in the slaves_count. Each
 * entry contains address of one slave in the network. */
extern const mss_bus* slaves;

/**  Size of slaves. */
extern int slaves_count;

/** An array containing packet counters for each slave. Array size is MAX_ADDR+1 - slave address is an index
 * of it's counter. */
extern int* packet_count;

/** 
 * Inits data structures contained by slaves.h. Does not take ownership over slaves.
 */
void LoadSlaves( const mss_addr* slaves, int slaves_count );


/**
 * Should be called when data structures will no longer be used in order to free up the memory.
 */
void FreeSlaves();

#endif //_MSS_BUS_SLAVES_H_