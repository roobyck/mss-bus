
#include "packet.h"

const GenericPacket MSS_NRQ_PACKET = { crc('MSS_NRQ',1), MSS_NRQ };

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
    char c;
    struct timeval tv;
    int got_packet = FALSE;
    int type_known = FALSE;
    
    tv.tv_sec  = 1;
    tv.tv_usec = 0;
    
    char *pak_ptr = (char*) packet;
    
    for( ; timeout != 0; --timeout ) {
        libser_read( mss_fd, &c, 1, &tv );
        if( got_packet ) {
            // store char
            *pak_ptr = c;
            
            // if it's time for packet type recognition...
            if( timeout == 1 && !type_known ) {
                type_known = TRUE;
                if( c == MSS_BUS ) timeout = 1;  else 
                if( c == MSS_NRQ ) timeout = 0;  else 
                if( c == MSS_DAT ) timeout = 14; else 
                if( c == MSS_ACK ) timeout = 1;
                else return MSS_BAD_TYPE;
            }
            
            // make ready to read next byte
            ++pak_ptr;
            
        } else if( c == MSS_BOF ) {
            got_packet = TRUE;
            // at least three more byte must be received
            timeout = 3;
        }
        
        // if infinite wait
        if( timeout < 0 ) timeout = -1;
    }
    
    // we have received something...
    if( got_packet ) {
        switch( packet->generic.type ) {
        
        case MSS_BUS:
            if( packet->bus.crc == crc(((char*)(pac))+2, 2) )
                return MSS_OK;
            else return MSS_BAD_CRC;
            break;
            
        case MSS_NRQ:
            if( packet->nrq.crc == MSS_NRQ_PACKET->nrq.crc )
                return MSS_OK;
            else return MSS_BAD_CRC;
            break;

        case MSS_DAT:
            if( packet->dat.crc ==
                crc( ((char*)(pac))+2, 5 + pac->dat.data_len ))
                    return MSS_OK;
            else return MSS_BAD_CRC;
            break;

        case MSS_ACK:
            if( packet->ack.crc == crc(((char*)(pac))+2, 2) )
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
int send_mss_packet( MssPacket* packet ) {
    int bytes_total;    
    switch( packet->generic.type ) { 
    case MSS_BUS: bytes_total = 4; break;
    case MSS_NRQ: bytes_total = 3; break;
    case MSS_DAT: bytes_total = 7 + pac->dat.data_len; break;
    case MSS_ACK: bytes_total = 4; break;
    default: return MSS_WTF;
    }

    // declaration for pointer arithmetic magic purposes
    char* packet_b = (char*) packet;
    
    for(
        int sent = 0;
        bytes_total != sent;
        sent += libser_write( mss_fd, packet+sent, bytes_total-sent )
    ) {
        // Why should we code in traditional manner if we can implement
        // everything inside for declaration?
    }
    
    return MSS_OK;
}

