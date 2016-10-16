#ifndef MDLIST_PQUEUE_H_
#define MDLIST_PQUEUE_H_

#include <stdint.h>

#define MDLIST_PQUEUE_N_DIMENSIONS	3

/* All the sizes are in bits */
#define	MDLIST_PQUEUE_DIM_1_SIZE	8
#define	MDLIST_PQUEUE_DIM_2_SIZE	12
#define	MDLIST_PQUEUE_DIM_3_SIZE	12

#define MDLIST_PQUEUE_KEY_SIZE		\
	(MDLIST_PQUEUE_DIM_1_SIZE +	\
	MDLIST_PQUEUE_DIM_2_SIZE +	\
	MDLIST_PQUEUE_DIM_3_SIZE)

#define MDLIST_PQUEUE_TOP_DIM_BIT_SHIFT	\
	(MDLIST_PQUEUE_DIM_2_SIZE +	\
	MDLIST_PQUEUE_DIM_3_SIZE)

/*
 * mdlist_pqueue_node: Represents a node in the mdlist. Node in each
 * dimension, including the leaf nodes would be of this type.
 *
 * @key: The key for a particular node. For the leaf nodes, it stores
 * the entire key, but for the other dimensions, it holds the values
 * corresponding to the bit masking of that particular dimension.
 * @next: Pointer used to traverse in the same dimension.
 * @child: Pointer used to go to the next dimension.
 */
struct mdlist_pqueue_node {
	uint32_t key;

	struct mdlist_pqueue_node *next;
	struct mdlist_pqueue_node *child;
};

/*
 * mdlist_pqueue_head: Represents an mdlist for priority queue.
 *
 * @child: An array of pointers of type struct mdlist_pqueue_node.
 * As this is the first dimension, all the pointers are stored as
 * an array for O(1) access.
 */
struct mdlist_pqueue_head {
	struct mdlist_pqueue_node *child[1 << MDLIST_PQUEUE_DIM_1_SIZE];
};


/*
 * mdlist_pqueue_enq: Function to enqueue a node. The nodes are inserted
 * in a sorted order, higher priority (lower key) to lower priority
 * (higher key). Returns 0 on success, else a negative error code.
 *
 * @head: The head pointer representing this mdlist priority queue
 * @node: The node that has to be enqueued. The node should be created
 * dynamically using mdlist_pqueue_alloc_node(). Or if created statically,
 * should be initialized with mdlist_pqueue_init_node().
 */
extern int mdlist_pqueue_enq(struct mdlist_pqueue_head *head,
		struct mdlist_pqueue_node *node);

/* mdlist_pqueue_deq: Dequeue the node with the highest priority (least key value)
 * Returns the pointer to the dequeued node. If the list is empty, returns NULL.
 *
 * @head: The head pointer representing this mdlist priority queue.
 */
extern struct mdlist_pqueue_node *mdlist_pqueue_deq(
		struct mdlist_pqueue_head *head);

/* mdlist_pqueue_deq: Dequeue the node with highest priority for the given key
 * Returns the pointer to the dequeued node. If the list is empty, returns NULL.
 *
 * @head: The head pointer representing this mdlist priority queue.
 * @key: The node with the key that has to be dequeued.
 */
extern struct mdlist_pqueue_node *mdlist_pqueue_deq_key(
		struct mdlist_pqueue_head *head, uint32_t key);

/*
 * mdlist_pqueue_contains: Returns the pointer to the node in the mdlist queue.
 * Returns NULL if not found.
 *
 * @head: The head pointer representing this mdlist priority queue.
 * @key: The node with the key that has to be searched.
 */
extern struct mdlist_pqueue_node *mdlist_pqueue_contains(
		struct mdlist_pqueue_head *head, uint32_t key);

/* mdlist_pqueue_alloc_node: Node allocated dynamically (and initialized)
 *
 * @head: The head pointer representing this mdlist priority queue.
 * @key: The key with which the newly created node has to be initialized.
 */
extern struct mdlist_pqueue_node *mdlist_pqueue_alloc_node(uint32_t key);

/* mdlist_pqueue_init_node: Node if allocated statically, need to be
 *  initialized.
 *
 * @node: Pointer to the node that has to be initialized.
 * @key: The key with which the node has to be initialized.
 */
extern void mdlist_pqueue_init_node(struct mdlist_pqueue_node *node, uint32_t key);

#endif /* MDLIST_PQUEUE_H_ */
