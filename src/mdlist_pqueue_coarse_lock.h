#ifndef MDLIST_PQUEUE_COARSE_LOCK_H_
#define MDLIST_PQUEUE_COARSE_LOCK_H_

#include "mdlist_pqueue.h"
#include <pthread.h>

struct mdlist_pqueue_head_coarse {
	struct mdlist_pqueue_head head;
	pthread_mutex_t lock;
};

extern struct mdlist_pqueue_head_coarse *mdlist_pqueue_coarse_alloc(void);

extern void mdlist_pqueue_coarse_dealloc
	(struct mdlist_pqueue_head_coarse *mdlist_pqueue_coarse);

extern void mdlist_pqueue_coarse_init
	(struct mdlist_pqueue_head_coarse *mdlist_pqueue_coarse);


#endif /* MDLIST_PQUEUE_COARSE_LOCK_H_ */
