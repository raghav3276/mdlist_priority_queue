#include <stdlib.h>

#include "mdlist_pqueue_coarse_lock.h"

struct mdlist_pqueue_head_coarse *mdlist_pqueue_coarse_alloc(void)
{
	struct mdlist_pqueue_head_coarse *mdlist_coarse;

	mdlist_coarse = (struct mdlist_pqueue_head_coarse *)
			malloc(sizeof(*mdlist_coarse));
	if (!mdlist_coarse)
		return NULL;

	pthread_mutex_init(&mdlist_coarse->lock, NULL);

	return mdlist_coarse;
}

void mdlist_pqueue_coarse_dealloc
	(struct mdlist_pqueue_head_coarse *mdlist_pqueue_coarse)
{
	pthread_mutex_destroy(&mdlist_pqueue_coarse->lock);
	free(mdlist_pqueue_coarse);
}

void mdlist_pqueue_coarse_init
	(struct mdlist_pqueue_head_coarse *mdlist_pqueue_coarse)
{
	if (!mdlist_pqueue_coarse)
		return;

	pthread_mutex_init(&mdlist_pqueue_coarse->lock, NULL);
}
