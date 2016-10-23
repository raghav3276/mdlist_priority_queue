#ifndef MDLIST_PQUEUE_COARSE_LOCK_H_
#define MDLIST_PQUEUE_COARSE_LOCK_H_

#include "mdlist_pqueue.h"
#include <pthread.h>

struct mdlist_pqueue_head_coarse {
	struct mdlist_pqueue_head head;
	pthread_mutex_t lock;
};

extern int mdlist_pqueue_coarse_enq
	(struct mdlist_pqueue_head_coarse *mdlist_queue_coarse_head,
	struct mdlist_pqueue_node *node);

extern struct mdlist_pqueue_node *mdlist_pqueue_coarse_deq(
	struct mdlist_pqueue_head_coarse *mdlist_queue_coarse_head);

extern struct mdlist_pqueue_node *mdlist_pqueue_coarse_deq_key(
	struct mdlist_pqueue_head_coarse *mdlist_queue_coarse_head, uint32_t key);

extern struct mdlist_pqueue_node *mdlist_pqueue_coarse_contains(
	struct mdlist_pqueue_head_coarse *mdlist_queue_coarse_head, uint32_t key);

extern struct mdlist_pqueue_head_coarse *mdlist_pqueue_coarse_alloc(void);

extern void mdlist_pqueue_coarse_dealloc
	(struct mdlist_pqueue_head_coarse *mdlist_pqueue_coarse);

extern void mdlist_pqueue_coarse_init
	(struct mdlist_pqueue_head_coarse *mdlist_pqueue_coarse);


#endif /* MDLIST_PQUEUE_COARSE_LOCK_H_ */
