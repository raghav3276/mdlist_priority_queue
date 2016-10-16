#include <errno.h>
#include <stdlib.h>

#include "mdlist_pqueue.h"

static inline uint32_t mdlist_pqueue_get_1d_key(uint32_t key)
{
	return (key >> MDLIST_PQUEUE_TOP_DIM_BIT_SHIFT);
}

static inline uint32_t mdlist_pqueue_get_2d_key(uint32_t key)
{
	return ((key >> MDLIST_PQUEUE_DIM_3_SIZE) &
			((1 << MDLIST_PQUEUE_DIM_2_SIZE) - 1));
}

static inline uint32_t mdlist_pqueue_get_3d_key(uint32_t key)
{
	return (key & ((1 << MDLIST_PQUEUE_DIM_3_SIZE) - 1));
}

static struct mdlist_pqueue_node *mdlist_pqueue_new_2d_node(uint32_t key)
{
	struct mdlist_pqueue_node *new_node;

	new_node = (struct mdlist_pqueue_node *) malloc(sizeof(*new_node));
	if(!new_node)
		return NULL;

	new_node->key = mdlist_pqueue_get_2d_key(key);

	return new_node;
}

/* Returns the node pointer to the next dimension (2D) */
static struct mdlist_pqueue_node *mdlist_pqueue_get_2d_node(
		struct mdlist_pqueue_head *head, uint32_t key)
{
	uint32_t key_1d = mdlist_pqueue_get_1d_key(key);
	struct mdlist_pqueue_node *node_2d = NULL;

	if (!head->child[key_1d]) {
		/* The address holder in the array is empty */
		node_2d = mdlist_pqueue_new_2d_node(key);
		if (!node_2d)
			return NULL;

		head->child[key_1d] = node_2d;
	} else {
		struct mdlist_pqueue_node *curr = head->child[key_1d];
		uint32_t key_2d = mdlist_pqueue_get_2d_key(key);
		uint32_t curr_key_2d = curr->key;

		/* Travel until the 2d_key is found */
		while (curr_key_2d < key_2d) {
			if (!curr->next)
				break;

			curr = curr->next;
			curr_key_2d = mdlist_pqueue_get_2d_key(curr->key);
		}

		if (curr->key != key_2d) {
			node_2d = mdlist_pqueue_new_2d_node(key);
			if (!node_2d)
				return NULL;

			node_2d->next = curr->next;
			curr->next = node_2d;
		} else {
			node_2d = curr;
		}
	}

	return node_2d;
}

static struct mdlist_pqueue_node *mdlist_pqueue_new_3d_node(uint32_t key)
{
	struct mdlist_pqueue_node *new_node;

	new_node = (struct mdlist_pqueue_node *) malloc(sizeof(*new_node));
	if(!new_node)
		return NULL;

	new_node->key = mdlist_pqueue_get_2d_key(key);

	return new_node;
}

static struct mdlist_pqueue_node *mdlist_pqueue_get_3d_node(
		struct mdlist_pqueue_node *node_2d, uint32_t key)
{
	struct mdlist_pqueue_node *node_3d = NULL;

	if (!node_2d->child) {
		node_3d = mdlist_pqueue_new_3d_node(key);
		if (!key)
			return NULL;

		node_2d->child = node_3d;
	} else {
		struct mdlist_pqueue_node *curr = node_2d->child;
		uint32_t key_3d = mdlist_pqueue_get_3d_key(key);
		uint32_t curr_key_3d = curr->key;

		/* Travel until the 2d_key is found */
		while (curr_key_3d < key_3d) {
			if (!curr->next)
				break;

			curr = curr->next;
			curr_key_3d = mdlist_pqueue_get_3d_key(curr->key);
		}

		if (curr->key != key_3d) {
			node_3d = mdlist_pqueue_new_3d_node(key);
			if (!node_3d)
				return NULL;

			node_3d->next = curr->next;
			curr->next = node_3d;
		} else {
			node_3d = curr;
		}
	}

	return node_3d;
}

/* Adds as the last node in the 3rd dimension */
static void mdlist_pqueue_add_node(struct mdlist_pqueue_node *node_3d,
		struct mdlist_pqueue_node *node, uint32_t key)
{
	struct mdlist_pqueue_node *curr = node_3d->child;

	if (!curr) {
		node_3d->child = node;
		return;
	}

	for(;curr->next; curr = curr->next);

	curr->next = node;
}

int mdlist_pqueue_enq(struct mdlist_pqueue_head *head,
		struct mdlist_pqueue_node *node)
{
	uint32_t key = node->key;
	struct mdlist_pqueue_node *node_2d = NULL;
	struct mdlist_pqueue_node *node_3d = NULL;

	if (!node)
		return -EINVAL;

	node_2d = mdlist_pqueue_get_2d_node(head, key);
	if (!node_2d)
		return -ENOMEM;

	node_3d = mdlist_pqueue_get_3d_node(node_2d, key);
	if (!node_3d)
		return -ENOMEM;

	mdlist_pqueue_add_node(node_3d, node, key);

	return 0;
}

int mdlist_pqueue_deq(struct mdlist_pqueue_head *head, uint32_t key)
{
	return 0;
}

int mdlist_pqueue_contains(struct mdlist_pqueue_head *head, uint32_t key)
{
	return 0;
}

void mdlist_pqueue_init_node(struct mdlist_pqueue_node *node, uint32_t key)
{
	node->key = key;
}

struct mdlist_pqueue_node *mdlist_pqueue_alloc_node(uint32_t key)
{
	struct mdlist_pqueue_node *node;

	node = (struct mdlist_pqueue_node *) malloc(sizeof(*node));
	if (!node)
		return NULL;

	mdlist_pqueue_init_node(node, key);

	return node;
}

