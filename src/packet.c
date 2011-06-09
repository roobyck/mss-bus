
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include "libser.h"
#include "packet.h"

// "Cometh to me, serial port file descriptor, for thou shall serve me well!"
//       -- Packet Manager of mss-bus lib
extern int mss_fd;

/** Defines undefined behaviour. */
#define MSS_WTF -1256251

/*
 * Receives a packet from the mss network. This function performs a crc16
 * packet validation.
 * @param packet A buffer in which received packet would be stored.
 * @param timeout number of bytes function will wait until a BOF is received or
 *        a timeout is reached. Negative value means infinite wait.
 * @return Zero (MSS_OK) when packet was successfuly read, or negative value
 *        otherwise. Errors might be MSS_BAD_CRC, MSS_TIMEOUT or MSS_BAD_TYPE.
 */
int receive_mss_packet( MssPacket* packet, int timeout ) {
    uint8_t c;
    struct timeval tv;
    int got_packet = FALSE;
    int type_known = FALSE;
    int is_dat = FALSE;
    
    unsigned char mss_nrq_blabla = MSS_NRQ;
    GenericPacket MSS_NRQ_PACKET = { crc16(&mss_nrq_blabla,1,0), MSS_NRQ };

    tv.tv_sec  = 1;
    tv.tv_usec = 0;
    
    uint8_t *pak_ptr = (char*) packet;
    
    for( ; timeout != 0; --timeout ) {
        libser_read( mss_fd, &c, 1, &tv );
#ifdef HEAVY_DEBUG
    printf("receive_mss_packet(): Recv: %c\n", c);
#endif
        if( got_packet ) {
            // store char
            *pak_ptr = c;
            
            // if it's time for packet type recognition...
            if( timeout == 1 && !type_known ) {
                type_known = TRUE;
                if( c == MSS_BUS ) timeout = 2;
        else if( c == MSS_NRQ ) timeout = 1;
        else if( c == MSS_DAT ) { timeout = 5; is_dat = TRUE; }
        else if( c == MSS_ACK ) timeout = 2;
                else return MSS_BAD_TYPE;
            } else
            
        // if receiving dat packet
        if( timeout == 1 && is_dat == TRUE ) {
#ifdef HEAVY_DEBUG
        printf("receive_mss_packet(): data len: %d\n", c);
#endif
            timeout += c;
        is_dat = FALSE;
        }
        
            // make ready to read next byte
            ++pak_ptr;
            
        } else if( c == (uint8_t) MSS_BOF ) {
            got_packet = TRUE;
            // at least three more byte must be received
            timeout = 4;
        }
        
        // if infinite wait
        if( timeout < 0 ) timeout = -1;
    }
    
    mss_crc16 received_crc = packet->generic.crc;

    // we have received something...
    if( got_packet ) {
        switch( packet->generic.packet_type ) {
        
        case MSS_BUS:
            CRC_FOR_BUS( packet );
            if( received_crc == packet->bus.crc )
                return MSS_OK;
            else return MSS_BAD_CRC;
            break;
            
        case MSS_NRQ:
            if( packet->nrq.crc == MSS_NRQ_PACKET.crc )
                return MSS_OK;
            else return MSS_BAD_CRC;
            break;

        case MSS_DAT:
            CRC_FOR_DAT( packet );
            if( packet->dat.crc == received_crc )
                    return MSS_OK;
            else return MSS_BAD_CRC;
            break;

        case MSS_ACK:
            CRC_FOR_ACK( packet );
            if( packet->ack.crc == received_crc )
                return MSS_OK;
            else return MSS_BAD_CRC;
            break;            
        }
    
    // timeout reached...
    } else
        return MSS_TIMEOUT;
        
    return MSS_WTF;
}

/*
 * Sends a packet. This functions blocks until a packet is send, which will
 * happen as soon as local slave will gain access to the bus.
 * @param packet A packet to be send.
 * @return Zero (MSS_OK) or MSS_WTF (-1256251) otherwise.
 */
int send_mss_packet (MssPacket* packet)
{
    int bytes_total;    

    switch (packet->generic.packet_type) {
        case MSS_BUS:
            bytes_total = 4; break;
        case MSS_NRQ:
            bytes_total = 3; break;
        case MSS_DAT:
            bytes_total = 7 + packet->dat.data_len; break;
        case MSS_ACK:
            bytes_total = 4; break;
        default:
            return MSS_WTF;
    }
    
    int sent;
    uint8_t bof = (uint8_t) MSS_BOF;

    /* send BOF */
    libser_flush(mss_fd);
    while (libser_write(mss_fd, &bof, 1) != 1)
        ;
    usleep(100);

    /* send rest of the packet */
    while (bytes_total)
    {
        libser_flush(mss_fd);

        sent = libser_write(mss_fd, packet, bytes_total);
        bytes_total -= sent;
        packet = ((uint8_t*) packet) + sent;

        usleep(100);
    }

    return MSS_OK;
}
