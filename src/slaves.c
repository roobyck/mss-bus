#include "../include/slaves.h"
#include "../include/netstruct.h"

// Slaves (hashed) array.
mss_addr *slaves;

int slaves_count;

int *packet_count;

// Performs search through hash table containing slave data. Optimistic search time is O(1), pessimistic
// is O(n), altough no more than sizeof( mss_addr ) / MAX_SLAVES checks would be performed.
/*mss_addr* GetSlave( mss_addr addr ) {
	// fast modulo calculation. Assumes that MAX_SLAVES is power of 2. Let's hope it is.
	int entry_no = addr & ( MAX_SLAVES-1 );
	mss_addr* slave = slaves + entry_no;
	while( slave->addr != addr ) {
		if( (++entry_no) != MAX_SLAVES+1 ) {
			++slave;
		} else {
			entry_no = 0;
			slave = slaves;
		}
	}
	return slave;
}*/

void LoadSlaves( const mss_addr* slaves_list, int slaves_count ) {
	slaves = slave_list;
	packet_count = malloc( (MAX_ADDR + 1) * sizeof( int ) );
	NetStruct* iter = slaves_list;
	int i;
	// init mem
	for( i = 0; i < MAX_ADDR; ++i )
		packet_count[i] = 0;
	}
}

void FreeSlaves() {
	free( packet_count );
}
