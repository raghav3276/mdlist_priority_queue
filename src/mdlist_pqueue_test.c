#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>

#include "mdlist_pqueue_coarse_lock.h"

#define TEST_DEBUG

#define N_NODES		3
#define N_THREADS	32
#define	N_THREAD_LOOPS	10

sem_t thread_wait_sem;

struct mdlist_pqueue_head_coarse *myhead;

void *test_thread(void *arg)
{
	int i, j;

	struct mdlist_pqueue_node *nodes[N_NODES];
	struct mdlist_pqueue_node *test_node;

	for (j = 0; j < N_THREAD_LOOPS;  j++) {
#ifdef TEST_DEBUG
		printf("------------------------------------------\n");
		printf("j = %d\n", j);
#endif

		/* Examples of nodes created dynamically */
		for (i = 0; i < N_NODES; i++) {
			nodes[i] = mdlist_pqueue_alloc_node(i + 0x12345678);
			if (nodes[i] && !mdlist_pqueue_coarse_enq(myhead, nodes[i])) {
#ifdef TEST_DEBUG
				printf("Enqueue: 0x%x\n", i + 0x12345678);
#endif
			}
		}

		/* Test if the list contains the nodes with the keys */
		for (i = 0; i < N_NODES; i++) {
			if (mdlist_pqueue_coarse_contains(myhead, i + 0x12345678)) {
#ifdef TEST_DEBUG
				printf("Contains: 0x%x\n", i + 0x12345678);
#endif
			}
		}

		/* Dequeue operations */
		test_node = mdlist_pqueue_coarse_deq_key(myhead, 0x12346679);
		if (test_node) {
#ifdef TEST_DEBUG
			printf("dequeue_key_node: 0x%x\n", test_node->key);
#endif
		}

		for (i = 0; i < N_NODES + 1; i++) {
			test_node = mdlist_pqueue_coarse_deq(myhead);
			if (test_node) {
#ifdef TEST_DEBUG
				printf("dequeue_node: 0x%x\n", test_node->key);
#endif
			}
		}

		for (i = 0; i < N_NODES; i++) {
			/* Leave the following line commented, as the same deallocated
			 * are being created as new nodes which resulted in an infinite
			 * traversal.
			 */
			/* TODO: Need to fix this issue */
//			free(nodes[i]);
		}
	}

	sem_post(&thread_wait_sem);

	return NULL;
}

int main()
{
	int i;
	pthread_t thrd;

	myhead = mdlist_pqueue_coarse_alloc();
	if (!myhead)
		return -errno;

	sem_init(&thread_wait_sem, 0, 0);

	for (i = 0; i < N_THREADS; i++) {
		pthread_create(&thrd, NULL, test_thread, NULL);
	}

	for (i = 0; i < N_THREADS; i++)
		sem_wait(&thread_wait_sem);

	mdlist_pqueue_coarse_dealloc(myhead);

	return 0;
}
