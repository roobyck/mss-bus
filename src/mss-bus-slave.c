
#include "mss-bus.h"
#include "packet.h"

mss_addr local_addr;

mss_num packet_counter[ MSS_MAX_ADDR + 1 ];

mss_num packet_count;

void init_slave (mss_addr addr) {
    local_addr = addr;
    memset( packet_counter, 0, (MSS_MAX_ADDR + 1) * sizeof(mss_num) );
    packet_count = 0;
}

int mss_slave_send (mss_addr target_addr, const char* data, size_t data_len) {
    /* Prepare for sending... */
    size_t data_sent = 0;
    MssPacket* packet = (MssPacket*) malloc( sizeof(MssPacket) );
    MssPacket* dat_packet = (MssPacket*) malloc( sizeof(MssPacket) );
    
    dat_packet->dat.packet_type = MSS_DAT;
    dat_packet->dat.src_addr = local_addr;
    dat_packet->dat.dst_addr = target_addr;
    
    /* Keep sending until all data was sent. */
    while( data_send != data_len ) {
        int recv_res = receive_mss_packet( packet, MSS_TIMEOUT );
        
        /* Wait for a bus... */
        if(
            (recv_res == MSS_OK) &&
            (packet->generic.type == MSS_BUS) &&
            (packet->bus.slave_addr == local_addr)
        ) {
            /* Send a packet */
            int copy_bytes = data_len - data_sent;
            if( copy_bytes > 10 )
                copy_bytes = 10;
            ++packet_count;
            dat_packet->dat.number = packet_count;
            dat_packet->dat.data_len = copy_bytes;
            memcpy( dat_packet->dat.data, data + data_sent, copy_bytes );
            CRC_FOR_DAT( dat_packet );
            send_mss_packet( dat_packet );
            
            /* Wait for a response (if SDA) */
            if( target_addr != MSS_BROADCAST_ADDR ) {
                recv_res = receive_mss_packet( packet, MSS_TIMEOUT );
                
                /* Got ACK */
                if( (recv_res == MSS_OK) && (packet->generic.type == MSS_ACK) )
                    data_sent += copy_bytes;
    
                /* No ACK ;( */
                else {
                    --packet_count;
                    /*Note: zakomentowanej tej linii sprawi, iz protokol bedzie
                     *      zapewnial poprawnosc transmisji (retransmitowal da-
                            ne do skutku). */
                    return data_sent;
                }
            
            /* SDN - no waiting needed, just increase data counter. */
            } else data_sent += copy_bytes;
        }
        
    } /* while has data to send */
    
    return data_sent; /* Success - sent all the data. */
}

int mss_slave_recv (mss_addr* sender_addr, const char* buffer, int* is_broadcast) {
    
}
