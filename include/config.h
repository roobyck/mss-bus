
#ifndef _MSS_BUS_CONFIG_H_
#define _MSS_BUS_CONFIG_H_

/** Default timeout used by library for message receive function. */
#define MSS_TIMEOUT 5

/** Defines max number of slaves managed by the master. */
#define MSS_MAX_SLAVES 64

/** Defines max address value that could be assigned to a slave. */
#define MSS_MAX_ADDR 254

/** A broadcast address. */
#define MSS_BROADCAST_ADDR MSS_MAX_ADDR+1

/** Definiuje maksymalna ilosc bajtow niesiona przez jeden pakiet typu DAT. */
#define MSS_DATA_PER_PACKET 10

/** If not 0, library would print debug data into stdout. */
#define MSS_VERBOSE_MODE 1 

#endif  /* _MSS_BUS_CONFIG_H_ */
