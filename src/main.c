
#include "packet.h"
#include "stdlib.h"
#include "stdio.h"

void check_bus(MssPacket* packet,MssPacket* rpacket) {

	printf("BUS:\n");

	packet->bus.packet_type = MSS_BUS;
	packet->bus.slave_addr = 's';
	CRC_FOR_BUS( packet );

	send_mss_packet( packet );
	int receive_res = receive_mss_packet( rpacket, 5 );
	printf("Receiving a packet previously sent, result: %i\n", receive_res);
	send_mss_packet( rpacket );

	printf("\n");
}

void check_nrq(MssPacket* packet,MssPacket* rpacket) {

	printf("NRQ:\n");

	unsigned char mss_nrq_blabla = MSS_NRQ;
	GenericPacket MSS_NRQ_PACKET = { crc16(&mss_nrq_blabla,1,0), MSS_NRQ };

	packet->nrq.packet_type = MSS_NRQ;
	packet->nrq.crc = MSS_NRQ_PACKET.crc;

	send_mss_packet( packet );
	int receive_res = receive_mss_packet( rpacket, 5 );
	printf("Receiving a packet previously sent, result: %i\n", receive_res);
	send_mss_packet( rpacket );

	printf("\n");
}

void check_dat(MssPacket* packet,MssPacket* rpacket) {

	printf("DAT:\n");
	packet->dat.packet_type = MSS_DAT;
	packet->dat.data_len = 3;
	packet->dat.dst_addr = 'd';
	packet->dat.src_addr = 's';
	packet->dat.number = 'n';
	packet->dat.data[0] = 'l';
	packet->dat.data[1] = 'o';
	packet->dat.data[2] = 'l';
	CRC_FOR_DAT( packet );

	send_mss_packet( packet );
	int receive_res = receive_mss_packet( rpacket, 5 );
	printf("Receiving a packet previously sent, result: %i\n", receive_res);
	send_mss_packet( rpacket );

	printf("\n");
}

int main() {

	MssPacket* packet = malloc( sizeof(MssPacket) );
	MssPacket* rpacket = malloc( sizeof(MssPacket) );

	check_bus(packet,rpacket);
	check_nrq(packet,rpacket);
	check_dat(packet,rpacket);
	//check_bus(packet,rpacket);

	// Bah, 2*sizeof(MssPacket) memory leaked.

	return 0;
}
