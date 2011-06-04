
#ifndef __MSS_PACKET_H
#define __MSS_PACKET_H

#include <stdint.h>
#include "crc.h"

/* ======= Packet types ======= */
#define MSS_BUS     0x01
#define MSS_NRQ     0x02
#define MSS_DAT     0x04
#define MSS_ACK     0x08

/* ======= Packet sizes ======= */
#define MSS_BUS_SIZE    5
#define MSS_NRQ_SIZE    4
#define MSS_DAT_SIZE    8
#define MSS_ACK_SIZE    5

/* ======= Macros for calling CRC16 functions ======= */
#define CRC_FOR_BUS(pac)    crc16((pac)+3, MSS_BUS_SIZE-3)
#define CRC_FOR_ACK(pac)    crc16((pac)+3, MSS_ACK_SIZE-3)
#define CRC_FOR_DAT(pac)    crc16((pac)+3, MSS_DAT_SIZE-3+(pac->data_len))

/* ======= Packet structures ======= */
typedef struct generic_packet {
    uint8_t  bof;
    uint16_t crc16;
    uint8_t  type;
    uint8_t  zero[14];
} __attribute__((packed))
GenericPacket;

typedef struct bus_packet {
    uint8_t  bof;
    uint16_t crc16;
    uint8_t  type;
    uint8_t  slave_addr;
    uint8_t  zero[13];
} __attribute__((packed))
BusPacket;

typedef struct data_packet {
    uint8_t  bof;
    uint16_t crc16;
    uint8_t  type;
    uint8_t  frame_nr;
    uint8_t  src_addr;
    uint8_t  dst_addr;
    uint8_t  data_len;
    uint8_t  data[10];
} __attribute__((packed))
DataPacket;

typedef struct ack_packet {
    uint8_t  bof;
    uint16_t crc16;
    uint8_t  type;
    uint8_t  frame_nr;
    uint8_t  zero[13];
} __attribute__((packed))
AckPacket;

typedef GenericPacket NrqPacket;
typedef GenericPacket MssPacket;

/* ======= Error codes ======= */

/** No error. */
#define MSS_OK           0
/** Bad checksum. */
#define MSS_BAD_CRC     -1
/** Timeout reached. */
#define MSS_TIMEOUT     -2
/** Unrecognised type of frame. */
#define MSS_BAD_TYPE    -3

/* ======= Functions for mss-bus protocol  ======= */

/**
 * Receives a packet from the mss network. This function performs a crc16
 * packet validation.
 * @param packet A buffer in which received packet would be stored.
 * @param timeout number of bytes function will wait until a BOF is received or
 *        a timeout is reached. Negative value means infinite wait.
 * @return Zero (MSS_OK) when packet was successfuly read, or negative value
 *        otherwise. Errors might be MSS_BAD_CRC, MSS_TIMEOUT or MSS_BAD_TYPE.
 */
int receive_mss_packet (int fd, MssPacket* packet, int timeout);

/**
 * Sends a packet. This functions blocks until a packet is send, which will
 * happen as soon as local slave will gain access to the bus.
 * @param packet A packet to be send.
 * @return Zero (MSS_OK).
 */
int send_mss_packet (int fd, MssPacket* packet);

#endif  /* __MSS_PACKET_H */

