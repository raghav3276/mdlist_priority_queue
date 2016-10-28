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
	new_node->child = new_node->next = NULL;

	return new_node;
}

/* Returns the node pointer to the next dimension (2D) */
static struct mdlist_pqueue_node *mdlist_pqueue_get_2d_node(
		struct mdlist_pqueue_head *head, uint32_t key)
{
	uint32_t key_1d = mdlist_pqueue_get_1d_key(key);
	struct mdlist_pqueue_node *node_2d = NULL;
	struct mdlist_pqueue_node *curr = head->child[key_1d];
	uint32_t key_2d = mdlist_pqueue_get_2d_key(key);

	if (!curr || key_2d < curr->key) {
		/* The address holder in the array is empty */
		node_2d = mdlist_pqueue_new_2d_node(key);
		if (!node_2d)
			return NULL;

		head->child[key_1d] = node_2d;

		/* Node replaced the existing first node */
		if (curr && key_2d < curr->key)
			node_2d->next = curr;

		/* Update the deq_idx if required */
		if (key_1d < head->deq_idx)
			head->deq_idx = key_1d;

	} else {
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

	new_node->key = mdlist_pqueue_get_3d_key(key);
	new_node->child = new_node->next = NULL;

	return new_node;
}

static struct mdlist_pqueue_node *mdlist_pqueue_get_3d_node(
		struct mdlist_pqueue_node *node_2d, uint32_t key)
{
	struct mdlist_pqueue_node *node_3d = NULL;
	struct mdlist_pqueue_node *curr = node_2d->child;
	uint32_t key_3d = mdlist_pqueue_get_3d_key(key);

	if (!curr || key_3d < curr->key) {
		node_3d = mdlist_pqueue_new_3d_node(key);
		if (!node_3d)
			return NULL;

		node_2d->child = node_3d;

		/* Node replaced the existing first node */
		if (curr && key_3d < curr->key)
			node_3d->next = curr;

	} else {
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

	for (;curr->next; curr = curr->next);

	curr->next = node;
}

int mdlist_pqueue_enq(struct mdlist_pqueue_head *head,
		struct mdlist_pqueue_node *node)
{
	uint32_t key = node->key;
	struct mdlist_pqueue_node *node_2d = NULL;
	struct mdlist_pqueue_node *node_3d = NULL;

	if (!node || !head)
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

/* Check if the portion of the key exists in the corresponding dimension */
static struct mdlist_pqueue_node *mdlist_pqueue_contains_node(
		struct mdlist_pqueue_node *node_d, uint32_t key_d)
{
	struct mdlist_pqueue_node *curr = node_d;

	if (!curr)
		return NULL;

	while (curr->key < key_d) {
		curr = curr->next;
		if (!curr)
			break;
	}

	if (curr && key_d == curr->key)
		return curr;
	else
		return NULL;
}

static struct mdlist_pqueue_node *mdlist_pqueue_contains_2d_node(
		struct mdlist_pqueue_node *node_1d, uint32_t key)
{
	uint32_t key_2d = mdlist_pqueue_get_2d_key(key);

	return mdlist_pqueue_contains_node(node_1d, key_2d);
}

static struct mdlist_pqueue_node *mdlist_pqueue_contains_3d_node(
		struct mdlist_pqueue_node *node_2d, uint32_t key)
{
	uint32_t key_3d = mdlist_pqueue_get_3d_key(key);

	return mdlist_pqueue_contains_node(node_2d, key_3d);
}

struct mdlist_pqueue_node *mdlist_pqueue_contains(
		struct mdlist_pqueue_head *head, uint32_t key)
{
	uint32_t key_1d = mdlist_pqueue_get_1d_key(key);
	struct mdlist_pqueue_node *node_2d = NULL;
	struct mdlist_pqueue_node *node_3d = NULL;

	if (!head)
		return NULL;

	if (!head->child[key_1d])
		return NULL;

	node_2d = mdlist_pqueue_contains_2d_node(head->child[key_1d], key);
	if (!node_2d)
		return NULL;

	node_3d = mdlist_pqueue_contains_3d_node(node_2d->child, key);
	if (!node_3d)
		return NULL;

	return mdlist_pqueue_contains_node(node_3d->child, key);
}

void mdlist_pqueue_update_dqueue_idx(struct mdlist_pqueue_head *head)
{
	uint8_t i;
	uint32_t n_1d_keys = (1 << MDLIST_PQUEUE_DIM_1_SIZE) - 1;

	for (i = 0; i < n_1d_keys && !head->child[i]; i++);

	/* List is empty! */
	if (i == n_1d_keys)
		head->deq_idx = 0xff;
	else
		head->deq_idx = i;
}

struct mdlist_pqueue_node *mdlist_pqueue_deq_key(struct mdlist_pqueue_head *head,
		uint32_t key)
{
	uint32_t key_1d = mdlist_pqueue_get_1d_key(key);
	struct mdlist_pqueue_node *node_2d = NULL;
	struct mdlist_pqueue_node *node_3d = NULL;
	struct mdlist_pqueue_node *node = NULL;

	/* The function is similar to that of the mdlist_pqueue_contains().
	 * But, here we are redefining the function as the pointers node_2d
	 * and node_3d are required for the unlink process.
	 * The unlink follows the same approach as that of the regular dequeue
	 * operation.
	 */

	if (!head && !head->child[key_1d])
		return NULL;

	node_2d = mdlist_pqueue_contains_2d_node(head->child[key_1d], key);
	if (!node_2d)
		return NULL;

	node_3d = mdlist_pqueue_contains_3d_node(node_2d->child, key);
	if (!node_3d)
		return NULL;

	node = mdlist_pqueue_contains_node(node_3d->child, key);

	if (!node)
		return NULL;

	/* Undo the links */
	node_3d->child = node->next;
	if (!node_3d->child) {
		node_2d->child = node_3d->next;
		free(node_3d);
	}

	if (!node_2d->child) {
		head->child[key_1d] = node_2d->next;
		free(node_2d);
	}

	/* Update the deq_idx if the last node of this array element is deleted */
	if (!head->child[head->deq_idx])
		mdlist_pqueue_update_dqueue_idx(head);

	return node;
}

struct mdlist_pqueue_node *mdlist_pqueue_deq(struct mdlist_pqueue_head *head)
{
	uint8_t deq_idx;
	struct mdlist_pqueue_node *node_2d = NULL;
	struct mdlist_pqueue_node *node_3d = NULL;
	struct mdlist_pqueue_node *node = NULL;

	if (!head)
		return NULL;

	deq_idx = head->deq_idx;

	/* List empty: If the dequeue index is pointing to NULL, it indicates that
	 * no enqueue has happened */
	if (!head->child[deq_idx])
		return NULL;

	/* Extract the node */
	node_2d = head->child[deq_idx];
	node_3d = node_2d->child;
	node = node_3d->child;

	/* Undo the links */
	node_3d->child = node->next;
	if (!node_3d->child) {
		node_2d->child = node_3d->next;
		free(node_3d);
	}

	if (!node_2d->child) {
		head->child[deq_idx] = node_2d->next;
		free(node_2d);
	}

	/* Update the deq_idx if the last node of this array element is deleted */
	if (!head->child[deq_idx])
		mdlist_pqueue_update_dqueue_idx(head);

	return node;
}

void mdlist_pqueue_init_node(struct mdlist_pqueue_node *node, uint32_t key)
{
	node->key = key;
	node->child = node->next = NULL;
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

void mdlist_pqueue_init_head(struct mdlist_pqueue_head *head)
{
	/* Initialize it to the maximum value for the log to work */
	head->deq_idx = 0xff;
}
