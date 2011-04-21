#include "../include/mss-bus.h"
#include "../include/slaves.h"

int keep_master_running;

void mss_run_master( const mss_addr* slaves, int slaves_count ) {
	/* set up working environment */

	// An array containing packet counters for each slave. Array size is MAX_ADDR+1 - slave address is an index
	// of it's counter.
	int* packet_count = malloc( (MAX_ADDR + 1) * sizeof( int ) );
	
	// init mem
	int i;
	for( i = 0; i < MAX_ADDR; ++i )
		packet_count[i] = 0;
	}
	
	keep_master_running = 1;
	
	/* main loop */
	while( keep_master_running ) {
		//TODO: ReceivePacket( timeout )
		//TODO: realizacja maszyny stanow dot. zarzadzania transmisja
	}
	
	/* clean up */
	FreeSlaves( slaves_array, slaves_count );
}

void mss_stop_master() {
	keep_master_running = 0;
}