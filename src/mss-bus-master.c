#include "../include/mss-bus.h"
#include "../include/slaves.h"

int keep_master_running;

void mss_run_master( const mss_addr* slaves_array, int slaves_count ) {
	// set up working environment
	LoadSlaves();
	keep_master_running = 1;
	
	// main loop
	while( keep_master_running ) {
		//TODO: ReceivePacket( timeout )
		//TODO: realizacja maszyny stanow dot. zarzadzania transmisja
	}
	
	// clean up
	FreeSlaves( slaves_array, slaves_count );
}

void mss_stop_master() {
	keep_master_running = 0;
}