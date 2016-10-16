#include <stdio.h>
#include <stdlib.h>

#include "mdlist_pqueue.h"

struct mdlist_pqueue_head myhead;

struct mdlist_pqueue_node node1;
struct mdlist_pqueue_node *node2, *node3, *node4;

int main()
{
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

	free(node2);
	free(node3);
	free(node4);

	return 0;
}
