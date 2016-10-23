#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>

#include "mdlist_pqueue_coarse_lock.h"

#define N_THREADS	1
#define	N_THREAD_LOOP	10
#define N_DYNAMIC_NODES	5

sem_t sem_end_thread;

struct mdlist_pqueue_head_coarse *myhead;

void *thrd_func(void *arg)
{
	int i, node_id;
	struct mdlist_pqueue_node node1;
	struct mdlist_pqueue_node *nodes[N_DYNAMIC_NODES];
	struct mdlist_pqueue_node *test_node;
	unsigned long int thread_id = (unsigned long int) pthread_self();

	for (i = 0; i < N_THREAD_LOOP; i++) {
		/* Example of node created statically */
		mdlist_pqueue_init_node(&node1, thread_id + i + 1);
		mdlist_pqueue_coarse_enq(myhead, &node1);

		/* Examples of nodes created dynamically */
		for (node_id = 0; node_id < N_DYNAMIC_NODES; node_id++) {
			nodes[node_id] = mdlist_pqueue_alloc_node(thread_id + i + node_id);
			if(nodes[node_id])
				mdlist_pqueue_coarse_enq(myhead, nodes[node_id]);
		}

		/* Test if the list contains the nodes with the keys */
		for (node_id = 0; node_id < N_DYNAMIC_NODES; node_id++) {
			if (mdlist_pqueue_coarse_contains(myhead, thread_id + i + node_id))
				printf("Contains: 0x%lx\n", thread_id + i + node_id);
		}

		/* Perform dequeue on the nodes */
		for (node_id = 0; node_id < N_DYNAMIC_NODES; node_id++) {
			test_node = mdlist_pqueue_coarse_deq(myhead);
			if (test_node)
				printf("dequeue_node: 0x%x\n", test_node->key);
		}

		for (node_id = 0; node_id < N_DYNAMIC_NODES; node_id++)
			free(nodes[node_id]);
	}

	sem_post(&sem_end_thread);

	return NULL;
}

int main()
{
	int i;
	pthread_t thrd;

	myhead = mdlist_pqueue_coarse_alloc();
	if (!myhead)
		return errno;

	sem_init(&sem_end_thread, 0, 0);

	for (i = 0; i < N_THREADS; i++) {
		pthread_create(&thrd, NULL, thrd_func, NULL);
	}

	for (i = 0; i < N_THREADS; i++)
		sem_wait(&sem_end_thread);


	mdlist_pqueue_coarse_dealloc(myhead);

	return 0;
}
