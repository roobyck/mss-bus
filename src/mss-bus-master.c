
#include <stdlib.h>
#include "config.h"
#include "mss-bus.h"
#include "packet.h"
#include "crc.h"

int keep_master_running;

void mss_run_master (const mss_addr* slaves, int slaves_count)
{
    int i, current_slave;
    MssPacket *packet, *bus_packet;

    /* set up working environment */
    keep_master_running = 1;

    mss_init();
    
    packet = (MssPacket*) malloc( sizeof(MssPacket) );
    /* Array of BUSes to pre-calculate crc checksums. */
    bus_packet = (MssPacket*) malloc( slaves_count * sizeof(MssPacket) );
    for( i = 0; i < MSS_MAX_ADDR; ++i ) {
        BusPacket* p = &(bus_packet + i)->bus;
        p->slave_addr = slaves[ i ];
        p->packet_type = MSS_BUS;
        CRC_FOR_BUS( (MssPacket*) p );
    }
    current_slave = 0;
    
    /* main loop */
    while( keep_master_running ) {
        int recv_res;

        /* Send BUS to current slave. */
        send_mss_packet( bus_packet + current_slave );
    
        /* Wait for NRQ, timeout, or data message. */
        recv_res = receive_mss_packet( packet, MSS_RECEIVE_TIMEOUT );
        if(
            (recv_res != MSS_OK) || (
                (recv_res == MSS_OK) &&
                (packet->generic.packet_type == MSS_DAT)
            )    
        ) {
            /* Unless DAT is broadcast, we expect an ACK packet to appear. */
            if( packet->dat.dst_addr != MSS_BROADCAST_ADDR ) {
                receive_mss_packet( packet, MSS_RECEIVE_TIMEOUT );
            }
        } /* else current slave has nothing to transfer. */
        
        /* Shift to next slave. */
        ++current_slave;
        if( current_slave == slaves_count )
            current_slave = 0;
        
    } /* while */

    /* clean up */
    free( packet );
    free( bus_packet );
}

void mss_stop_master (void)
{
    keep_master_running = 0;
}

