#include <stdio.h>
#include <stdlib.h>

#include "mdlist_pqueue.h"

struct mdlist_pqueue_head myhead;

struct mdlist_pqueue_node node1;
struct mdlist_pqueue_node *node2, *node3, *node4, *node5, *node6;
struct mdlist_pqueue_node *test_node;

int main()
{
	int i;

	/* Example of node created statically */
	mdlist_pqueue_init_node(&node1, 0x12345678);
	mdlist_pqueue_enq(&myhead, &node1);

	/* Examples of nodes created dynamically */
	node2 = mdlist_pqueue_alloc_node(0x12346678);
	if(node2)
		mdlist_pqueue_enq(&myhead, node2);

	node3 = mdlist_pqueue_alloc_node(0x12346679);
	if (node3)
		mdlist_pqueue_enq(&myhead, node3);

	node4 = mdlist_pqueue_alloc_node(0x12345679);
	if (node4)
		mdlist_pqueue_enq(&myhead, node4);

	node5 = mdlist_pqueue_alloc_node(0x11345679);
	if (node5)
		mdlist_pqueue_enq(&myhead, node5);

	node6 = mdlist_pqueue_alloc_node(0x12346679);
	if (node6)
		mdlist_pqueue_enq(&myhead, node6);

	/* Test if the list contains the nodes with the keys */
	if (mdlist_pqueue_contains(&myhead, 0x12345678))
		printf("Contains: 0x%x\n", 0x12345678);

	if (mdlist_pqueue_contains(&myhead, 0x12346678))
		printf("Contains: 0x%x\n", 0x12346678);

	if (mdlist_pqueue_contains(&myhead, 0x12346679))
		printf("Contains: 0x%x\n", 0x12346679);

	if (mdlist_pqueue_contains(&myhead, 0x12345679))
		printf("Contains: 0x%x\n", 0x12345679);

	if (mdlist_pqueue_contains(&myhead, 0x11345679))
		printf("Contains: 0x%x\n", 0x11345679);

	if (mdlist_pqueue_contains(&myhead, 0x11111111))
		printf("Contains: 0x%x\n", 0x11111111);

	if (mdlist_pqueue_contains(&myhead, 0x12346679))
		printf("Contains: 0x%x\n", 0x12346679);

	/* Dequeue operations */
	for (i = 0; i < 7; i++) {
		test_node = mdlist_pqueue_deq(&myhead);
		if (test_node)
			printf("dequeue_node: 0x%x\n", test_node->key);
	}

	free(node2);
	free(node3);
	free(node4);
	free(node5);

	return 0;
}
