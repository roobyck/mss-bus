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

char buff_[256];
size_t count_=0;
int min_ = 0;
int max_ = 0;

void clear_fake_libser() {
	min_=max_=0;
}

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
	else if( (unsigned char)c == 0xBF )
		printf("[BOF]");
	else
		printf("%c",c);
}

int libser_read (int fd, char *buff, size_t len, struct timeval *tv) {
	//printf("libser_read(): filling buffer with data previously sent...\n");
	int j;
	for( j=0; j<len; ++j) {
		if(min_ == max_) {
			return j;
		}
		*(buff+j) = *(buff_+min_);
		min_ = (min_+1)%256;
	}
	return len;
}

int libser_write (int fd, const void * buff, size_t count) {
	printf("libser_write(): ");

	int i;
	const char* d = (const char*) buff;

	if( count > 1 ) {
		short cr = *((short*) (buff));
		printf("[CRC: %i]",(int)cr);
	} else if(*((unsigned char*)buff) == 0xBF)
		printf("[BOF]");

	for( i=0; i<count; ++i) {

		if( ((max_+i)%256 == min_-1) || ((max_+i)==256 && (min_==0)) ) {
			printf("buffer is full.\n");
			max_ = (max_+i) % 256;
			return i;
		}

		if( i!=0 && i!=1 ) char2out(*(d+i));
		*(buff_+((max_+i)%256)) = *(d+i);
	}
	max_=(max_+i)%256;
	printf("\n");
	return count;
}
