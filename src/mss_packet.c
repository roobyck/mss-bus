
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include "libser.h"
#include "mss_packet.h"

int receive_mss_packet (int fd, MssPacket* packet, int timeout)
{
    struct timeval tv = {1,0};
    uint8_t buffer[sizeof(*packet)];

    memset(packet, 0, sizeof(*buffer));

    while (--timeout) {
        /* TODO */
    }

    return MSS_TIMEOUT;
}

int send_mss_packet (int fd, MssPacket* packet)
{
    size_t packet_size;
    size_t sent;

    switch(packet->type) {
        case MSS_BUS:
            packet_size = MSS_BUS_SIZE; break;
        case MSS_NRQ:
            packet_size = MSS_NRQ_SIZE; break;
        case MSS_DAT:
            packet_size = MSS_DAT_SIZE + ((DataPacket*)packet)->data_len; break;
        case MSS_ACK:
            packet_size = MSS_ACK_SIZE; break;
        default:
            return -1;
    }

    while (packet_size)
    {
        libser_flush(fd);

        sent = libser_write(fd, packet, packet_size);
        packet_size -= sent;
        packet += sent;

        usleep(100);
    }

    return 0;
}

