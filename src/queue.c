
#include <string.h>
#include <stdlib.h>
#include "queue.h"

int q_init (queue *q, size_t len)
{
    if (NULL == q)
        return -1;  /* NULL pointer dereference */
    if (0 == len)
        return -1;  /* zero length queue */

    memset(q, 0, sizeof(queue));
    q->max_len = len;

    if (NULL == (q->buffer = calloc(len, sizeof(queue_t))))
        return -1;  /* no memory */

    return 0;
}

void q_destroy (queue *q)
{
    free(q->buffer);
    memset(q, 0, sizeof(queue));
}

int q_enqueue (queue* q, queue_t x)
{
    if (NULL == q)
        return -1;  /* NULL pointer dereference */
    if (q->max_len == q->size)
        return -1;  /* queue is full */

    q->buffer[q->tail] = x;

    if (q->max_len-1 == q->tail)
        q->tail = 0;
    else
        ++q->tail;

    ++q->size;

    return 0;
}

queue_t q_dequeue (queue *q)
{
    queue_t x;

    if (NULL == q)
        return -1;  /* NULL pointer dereference */

    if (0 == q->size)
        return -1;  /* queue is empty */

    x = q->buffer[q->head];

    if (q->max_len-1 == q->head)
        q->head = 0;
    else
        ++q->head;

    --q->size;

    return x;
}

