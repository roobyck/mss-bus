
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mss-bus.h"

int main (int argc, char** argv)
{
    uint8_t slave_addr;
    uint8_t sender;
    char msg[100];
    int msg_size;
    int bcast;
    
    if (argc != 2) {
    fprintf(stderr, "ERROR: No slave address given!\n");
    return -1;
    }

    slave_addr = atoi(argv[1]);

    mss_init( "/dev/ttyS1", B9600 );
    mss_init_slave(slave_addr);
    printf("Slave: #%d (recv)\n", slave_addr);

    msg_size = mss_slave_recv(&sender, msg, &bcast);

    msg[msg_size] = '\0';
    printf("Msg: %s\n", msg);

    mss_close();
    
    return 0;
}

