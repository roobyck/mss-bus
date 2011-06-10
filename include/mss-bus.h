
#ifndef _MSS_BUS_H_
#define _MSS_BUS_H_

#include <stddef.h>
#include <termios.h>
#include "types.h"
#include "config.h"

/** No error. */
#define MSS_OK               0

/** Library not initialized. */
#define MSS_UNINITIALIZED   -4

/**
 * Must be called before any other usage of library occurs. Initializes serial
 * port.
 * @return MSS_OK on success, negative value otherwise.
 */
int mss_init (const char *device, int speed);

/**
 * Closes serial port.
 */
void mss_close ();

/**
 * Launches a mss-master which takes control over the network, allowing
 * connected slaves to exchange data. Slaves are staticly assigned to the
 * master. Note that mss_run_master is blocking function and will continue to
 * run master until mss_stop_master is called.
 * @param slaves A pointer to array of slaves managed by the master. Each entry
 *        contains address of one slave in the network.
 * @param slave_count Amount of slaves in slaves.
 * @return MSS_OK on success (when master was closed), or MSS_UNINITIALIZED if
 *        library is not initialized.
 */
int mss_run_master (const mss_addr* slaves, int slaves_count);

/**
 * Stops execution of mss-master.
 */
void mss_stop_master();

/**
 * Initializes slave on local machine and assigns given address to it.
 * @return MSS_OK on success, or MSS_UNINITIALIZED if library is not
 *        initialized.
 */
int mss_init_slave (mss_addr addr);

/**
 * Performs send operation over mss network. mss_slave_send blocks until a
 * transmission is complete. Note that it is not guaranteed that all data would
 * be send. In case of bus error the function ceases operation. Bus error
 * occurs when no acknowledgment is received after sending a data packet.
 * @param addr Address of machine which transmission targets (might be a
 *             MSS_BROADCAST_ADDR for a SDN broadcast transmission).
 * @param data Buffer containing data to be send.
 * @param data_len Number of bytes to be send.
 * @return Number of bytes sent. If all bytes were transmitted successfuly,
 *         return value equals data_len. MSS_UNINITIALZED is returned upon
 *         error caused by library not initialized.
 */
int mss_slave_send (mss_addr addr, const char* data, size_t data_len);

/**
 * Receives data over bus. Function blocks until data is received. Function
 * returns as soon as data packet is received. Therefore assumption is made
 * that buffer size is max data size allowed to be transported in data packet
 * (that is 10 bytes).
 * @param sender_addr Sender addr is written here.
 * @param buffer Message is written to the buffer.
 * @param is_broadcast Is set, if transmission was broadcast message; otherwise
 *        it is cleared.
 * @return Number of data bytes received. MSS_UNINITIALZED is returned upon
 *         error caused by library not initialized.
 */
int mss_slave_recv (mss_addr* sender_addr, char* buffer, int* is_broadcast);

#endif  /* _MSS_BUS_H_ */

