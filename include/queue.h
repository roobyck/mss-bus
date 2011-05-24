
#ifndef __QUEUE_H
#define __QUEUE_H

#include <stdlib.h>

typedef int queue_t;    /* queue element type */

typedef
struct queue {
    queue_t *buffer;    /* buffer (queue) */
    size_t head;        /* queue head */
    size_t tail;        /* queue tail */
    size_t size;        /* number of elements in queue */
    size_t max_len;     /* maximum queue length */
}
queue;

int q_init (queue*, size_t);
void q_destroy (queue*);
int q_enqueue (queue*, queue_t);
queue_t q_dequeue (queue *);

#endif  /* __QUEUE_H */

