
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "mss-bus.h"

int main (int argc, char** argv)
{
    int i, sa_size;
    uint8_t slave_addrs[64];
    
    if (argc < 2) {
	fprintf(stderr, "ERROR: No slave(s) address(es) given!\n");
	return -1;
    }
	
    for (i = 1; i < argc; ++i)
	slave_addrs[i-1] = atoi(argv[i]);
    sa_size = i-1;
    
    printf("Slaves available: ");
    for(i = 0; i < sa_size; ++i)
	printf("%d ", slave_addrs[i]);
    printf("\n");

    mss_run_master(slave_addrs, sa_size);

    return 0;
}

