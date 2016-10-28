#include <stdlib.h>
#include <errno.h>

#include "mdlist_pqueue_coarse_lock.h"

int mdlist_pqueue_coarse_enq
	(struct mdlist_pqueue_head_coarse *mdlist_queue_coarse_head,
			struct mdlist_pqueue_node *node)
{
	int retval;

	if (!mdlist_queue_coarse_head || !node)
		return -EINVAL;

	pthread_mutex_lock(&mdlist_queue_coarse_head->lock);

	retval = mdlist_pqueue_enq(&mdlist_queue_coarse_head->head, node);

	pthread_mutex_unlock(&mdlist_queue_coarse_head->lock);

	return retval;
}

struct mdlist_pqueue_node *mdlist_pqueue_coarse_contains(
		struct mdlist_pqueue_head_coarse *mdlist_queue_coarse_head, uint32_t key)
{
	struct mdlist_pqueue_node *node;

	if (!mdlist_queue_coarse_head)
		return NULL;

	pthread_mutex_lock(&mdlist_queue_coarse_head->lock);

	node = mdlist_pqueue_contains(&mdlist_queue_coarse_head->head, key);

	pthread_mutex_unlock(&mdlist_queue_coarse_head->lock);

	return node;
}

struct mdlist_pqueue_node *mdlist_pqueue_coarse_deq(
		struct mdlist_pqueue_head_coarse *mdlist_queue_coarse_head)
{
	struct mdlist_pqueue_node *node;

	if (!mdlist_queue_coarse_head)
		return NULL;

	pthread_mutex_lock(&mdlist_queue_coarse_head->lock);

	node = mdlist_pqueue_deq(&mdlist_queue_coarse_head->head);

	pthread_mutex_unlock(&mdlist_queue_coarse_head->lock);

	return node;
}

struct mdlist_pqueue_node *mdlist_pqueue_coarse_deq_key(
		struct mdlist_pqueue_head_coarse *mdlist_queue_coarse_head, uint32_t key)
{
	struct mdlist_pqueue_node *node;

	if (!mdlist_queue_coarse_head)
		return NULL;

	pthread_mutex_lock(&mdlist_queue_coarse_head->lock);

	node = mdlist_pqueue_deq_key(&mdlist_queue_coarse_head->head, key);

	pthread_mutex_unlock(&mdlist_queue_coarse_head->lock);

	return node;
}

struct mdlist_pqueue_head_coarse *mdlist_pqueue_coarse_alloc(void)
{
	struct mdlist_pqueue_head_coarse *mdlist_coarse;

	mdlist_coarse = (struct mdlist_pqueue_head_coarse *)
			malloc(sizeof(*mdlist_coarse));
	if (!mdlist_coarse)
		return NULL;

	pthread_mutex_init(&mdlist_coarse->lock, NULL);
	mdlist_pqueue_init_head(&mdlist_coarse->head);

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
