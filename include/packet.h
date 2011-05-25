
#ifndef _MSS_BUS_PACKET_H_
#define _MSS_BUS_PACKET_H_

#include "types.h"
#include "config.h"
#include "crc.h"

/** Begin of Frame tag. */
#define MSS_BOF 0xBF

/** Bus offering, sent by master to one of his minions. */
#define MSS_BUS 0x1

/** Give the bus away. */
#define MSS_NRQ 0x2

/** Data packet. */
#define MSS_DAT 0x4

/** Sent when data was received and it's verification was successful. */
#define MSS_ACK 0x8

#define CRC_FOR_BUS(pac) ((BusPacket)pac)->crc = crc( ((char*)(pac))+2, 2 )
#define CRC_FOR_ACK(pac) ((AckPacket)pac)->crc = crc( ((char*)(pac))+2, 2 )
#define CRC_FOR_DAT(pac) ((DataPacket)pac)->crc = crc( ((char*)(pac))+2, 5+(((DataPacket)pac)->data_len) )

/* TODO: sizeof(mss_packet_type)+sizeof(mss_addr) zamiast 2? */ 

/**
 * Generic packet type, containing minimal set of data structures required to
 * be send over the mss network. Other packet types must include struct
 * generic_packet at the beginning of their body.
 * - crc is a crc16 cheksum of bytes from 3 to n of packet, where n is packet
 *   length.
 * - packet_type defines how tha packet would be interpreted. Generic packet
 *   is used directly by NRQ packets.
 */
typedef struct generic_packet {
    mss_crc16 crc;
    mss_packet_type packet_type;
} GenericPacket;

/** BUS-specific packet structure. */
typedef struct bus_packet {
    mss_crc16 crc;
    mss_packet_type packet_type;
    mss_addr slave_addr;
} BusPacket;

/** ACK-specific packet structure. */
typedef struct ack_packet {
    mss_crc16 crc;
    mss_packet_type packet_type;
    mss_num number;
} AckPacket;

/** DAT-specific packet structure. */
typedef struct data_packet {
    mss_crc16 crc;
    mss_packet_type packet_type;
    mss_addr src_addr;
    mss_addr dst_addr;
    mss_num number;
    mss_size data_len;
    char[ MSS_DATA_PER_PACKET ] data;
} DataPacket;

/** Union of all packets. */
typedef union mss_packet {
    GenericPacket generic, nrq;
    BusPacket bus;
    DataPacket dat;
    AckPacket ack;
} MssPacket;


/* Constant packets. */

extern const GenericPacket MSS_NRQ_PACKET;


/* Error codes. */

/** No error. */
#define MSS_OK           0
/** Bad checksum. */
#define MSS_BAD_CRC     -1
/** Timeout reached. */
#define MSS_TIMEOUT     -2
/** Unrecognised type of frame. */
#define MSS_BAD_TYPE    -3

/**
 * Receives a packet from the mss network. This function performs a crc16
 * packet validation.
 * @param packet A buffer in which received packet would be stored.
 * @param timeout number of bytes function will wait until a BOF is received or
 *        a timeout is reached. Negative value means infinite wait.
 * @return Zero (MSS_OK) when packet was successfuly read, or negative value
 *        otherwise. Errors might be MSS_BAD_CRC, MSS_TIMEOUT or MSS_BAD_TYPE.
 */
int receive_mss_packet( MssPacket* packet, int timeout );

/**
 * Sends a packet. This functions blocks until a packet is send, which will
 * happen as soon as local slave will gain access to the bus.
 * @param packet A packet to be send.
 * @return Zero (MSS_OK).
 */
int send_mss_packet( MssPacket* packet );

#endif  /* _MSS_BUS_PACKET_H_ */
