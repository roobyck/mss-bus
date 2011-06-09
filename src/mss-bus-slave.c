
#include "mss-bus.h"
#include "packet.h"
#include <string.h>
#include <stdlib.h>

#define DEBUG

#ifdef DEBUG
#include <stdio.h>
#endif

/** Local machine address. */
mss_addr local_addr;

/** Contains counters for incoming data packets. */
mss_num incoming_count[ MSS_MAX_ADDR + 1 ];

/** Contains counters which are used to mark outcoming data packets. */
mss_num outcoming_count[ MSS_MAX_ADDR + 1 ];

void mss_init_slave (mss_addr addr) {
    mss_init();
    local_addr = addr;
    memset( incoming_count, 0, (MSS_MAX_ADDR + 1) * sizeof(mss_num) );
    memset( outcoming_count, 0, (MSS_MAX_ADDR + 1) * sizeof(mss_num) );
}

int mss_slave_send (mss_addr target_addr, const char* data, size_t data_len) {
    /* Prepare for sending... */
    size_t data_sent = 0;
    MssPacket* packet = (MssPacket*) malloc( sizeof(MssPacket) );
    MssPacket* dat_packet = (MssPacket*) malloc( sizeof(MssPacket) );
    
    /* Watch out, MSS_BROADCAST_ADDR produces invalid pointer, thus counter
     * shall never be used in SDN mode... */
    mss_num* packet_count = outcoming_count + target_addr;
    
    dat_packet->dat.packet_type = MSS_DAT;
    dat_packet->dat.src_addr = local_addr;
    dat_packet->dat.dst_addr = target_addr;
    
#ifdef DEBUG
    printf( "mss-bus-slave(): Preparing send operation. Waiting for bus.\n" ); 
#endif    
    
    /* Keep sending until all data was sent. */
    while( data_sent != data_len ) {
        int recv_res = receive_mss_packet( packet, MSS_TIMEOUT );
        
        /* Wait for a bus... */
        if(
            (recv_res == MSS_OK) &&
            (packet->generic.packet_type == MSS_BUS) &&
            (packet->bus.slave_addr == local_addr)
        ) {
#ifdef DEBUG
            printf( "mss-bus-slave(): Got bus packet for %d (local machine)\n",
	            packet->bus.slave_addr );
#endif
	
            /* Prepare packet... */
            int copy_bytes = data_len - data_sent;
            if( copy_bytes > 10 )
                copy_bytes = 10;
            /* SDN's not counted. */
            if( target_addr != MSS_BROADCAST_ADDR ) {
                ++(*packet_count);
                dat_packet->dat.number = (*packet_count);
            } else
                dat_packet->dat.number = 0;
            dat_packet->dat.data_len = copy_bytes;
            memcpy( dat_packet->dat.data, data + data_sent, copy_bytes );
            CRC_FOR_DAT( dat_packet );
            /* Send packet. */
            send_mss_packet( dat_packet );
            
#ifdef DEBUG
            printf( "mss-bus-slave(): calling send_mss_packet().\n" );
#endif
	    
            /* Wait for a response (if SDA) */
            if( target_addr != MSS_BROADCAST_ADDR ) {
                recv_res = receive_mss_packet( packet, MSS_TIMEOUT );
#ifdef DEBUG
                printf("mss-bus-slave(): Recv res is %d; packet type is %d\n",recv_res,packet->generic.packet_type);
#endif
 
                /* Got ACK */
                if( (recv_res == MSS_OK) && (packet->generic.packet_type == MSS_ACK) ) {
                    data_sent += copy_bytes;
#ifdef DEBUG
                    printf("mss-bus-slave(): Got ACK!\n");
#endif	    
                }
		    
                /* No ACK ;( */
                else {
                    --(*packet_count);
                    /*Note: zakomentowanej tej linii sprawi, iz protokol bedzie
                     *      zapewnial poprawnosc transmisji (retransmitowal da-
                            ne do skutku). */
                    free( packet );
                    free( dat_packet );
#ifdef DEBUG
                    printf("mss-bus-slave(): No ACK...\n");
#endif	
                    return data_sent;
                }
            
            /* SDN - no waiting needed, just increase data counter. */
            } else data_sent += copy_bytes;
        }
#ifdef DEBUG
	else {
            printf("mss-bus-slave(): Got bus for %d.\n", packet->bus.slave_addr);
	}
#endif	
        
    } /* while has data to send */
    
    free( packet );
    free( dat_packet );
    return data_sent; /* Success - sent all the data. */
}

int mss_slave_recv (mss_addr* sender_addr, char* buffer, int* is_broadcast) {
    MssPacket* packet = (MssPacket*) malloc( sizeof(MssPacket) );
    
    int loop = 1;
    
    /* Keep receiving until received a packet to local machine. */
    while( loop ) {
        int recv_res = receive_mss_packet( packet, MSS_TIMEOUT );
        if( (recv_res == MSS_OK) && (packet->generic.packet_type == MSS_DAT) ) {
            
            /* Catch incoming data. */
            mss_addr dst_addr = packet->dat.dst_addr;
            if( (dst_addr == local_addr) || (dst_addr == MSS_BROADCAST_ADDR) ) {
                memcpy( buffer, packet->dat.data, packet->dat.data_len );
                *sender_addr = packet->dat.src_addr;
                loop = 0;
            }
            
            /* SDA, sender awaits ACK. */
            if( dst_addr == local_addr ) {
                /* Prepare packet... */
                MssPacket* ack_packet = (MssPacket*) malloc( sizeof(MssPacket) );
                ack_packet->ack.packet_type = MSS_ACK;
                ack_packet->ack.number = packet->dat.number;
                CRC_FOR_ACK( ack_packet );
                /* ...then send it... */
                send_mss_packet( ack_packet );
                /* ...and destroy eventually, mwahaha! */
                free( ack_packet );
                /* Notify user it was SDA... */
                (*is_broadcast) = 0;
                
            /* Otherwise notify user it was SDN... */
            } else
                (*is_broadcast) = 1;
        
        } /* if */
    
    } /* while( loop ) */
    
    /* Return. */
    int bytes_received = packet->dat.data_len;
    free( packet );
    return bytes_received;
}

