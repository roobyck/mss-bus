#include "libser.h"
#include <stdio.h>
#include "packet.h"
#include "stdlib.h"

#define DESC 1

int libser_open (const char *device, int speed) {
	printf("libser_open()\n");
	return 1;
}

void libser_close (int fd) {
	printf("libser_close()\n");
}

void libser_flush (int fd) {}

int libser_setrts (int fd, int onoff) { return 0; }

int serial_isempty (int fd) { return 0; }

char* buff_=0;
size_t count_=0;
int progress_ = 0;

void char2out(char c) {
	if( c == MSS_BOF )
		printf("[BOF]");
	else if( c == MSS_BUS )
		printf("[BUS]");
	else if( c == MSS_NRQ )
		printf("[NRQ]");
	else if( c == MSS_DAT )
		printf("[DAT]");
	else if( c == MSS_ACK )
		printf("[ACK]");
	else
		printf("%c",c);
}

int libser_read (int fd, char *buff, size_t len, struct timeval *tv) {
	//printf("libser_read(): filling buffer with data previously sent...\n");
	if( progress_ == -1 ) {
		*(buff) = MSS_BOF;
		progress_ = 0;
		return 1;
	}
	int i,j;
	for( i = progress_, j=0; i<progress_+len; ++i, ++j) {
		*(buff+j) = *(buff_+i);
	}
	progress_ = i;
	return len;
}

int libser_write (int fd, const void * buff, size_t count) {
	printf("libser_write(): ");

	int i;
	const char* d = (const char*) buff;

	short cr = *((short*) (buff));

	printf("[CRC: %i]",(int)cr);

	if(buff_!=0) free(buff_);
	buff_ = malloc( sizeof(char) * count );

	for( i=0; i<count; ++i) {
		if( i!=0 && i!=1 ) char2out(*(d+i));
		*(buff_+i) = *(d+i);
	}

	progress_=-1;

	printf("\n");
	return count;
}
