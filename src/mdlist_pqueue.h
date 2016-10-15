#ifndef MDLIST_PQUEUE_H_
#define MDLIST_PQUEUE_H_

#include <stdint.h>

#define MDLIST_PQUEUE_N_DIMENSIONS	3

/* All the sizes are in bits */
#define	MDLIST_PQUEUE_DIM_1_SIZE	8
#define	MDLIST_PQUEUE_DIM_2_SIZE	12
#define	MDLIST_PQUEUE_DIM_3_SIZE	12

#define MDLIST_PQUEUE_KEY_SIZE		\
	(MDLIST_PQUEUE_DIM_1_SIZE +		\
	MDLIST_PQUEUE_DIM_2_SIZE +		\
	MDLIST_PQUEUE_DIM_3_SIZE)


struct mdlist_pqueue_node {
	uint32_t key;

	struct mdlist_pqueue_node *next;
	struct mdlist_pqueue_node *child;
};

struct mdlist_pqueue_head {
	struct mdlist_pqueue_node *child[1 << MDLIST_PQUEUE_DIM_1_SIZE];
};

extern int mdlist_pqueue_enq(struct mdlist_pqueue_head *head, uint32_t key);
extern int mdlist_pqueue_deq(struct mdlist_pqueue_head *head, uint32_t key);
extern int mdlist_pqueue_contains(struct mdlist_pqueue_head *head, uint32_t key);

#endif /* MDLIST_PQUEUE_H_ */
