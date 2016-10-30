#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>

#include "mdlist_pqueue_coarse_lock.h"

//#define MDLIST_PQUEUE_DEBUG

#define N_NODES		3
#define N_THREADS	32
#define	N_THREAD_LOOPS	10

sem_t thread_wait_sem;

struct mdlist_pqueue_head_coarse *myhead;

void *test_thread(void *file)
{
	struct mdlist_pqueue_node *node;
	unsigned long val;
	FILE *trace_fp;
	char tr_line[16];

	trace_fp = fopen((char *) file, "r");
	if (!trace_fp) {
		perror("Unable to open the trace file ");
		goto out;
	}

	while (fgets(tr_line, 16, trace_fp)) {
		if ('d' == tr_line[0]) {
			node = mdlist_pqueue_coarse_deq(myhead);
			if (!node) {
#ifdef MDLIST_PQUEUE_DEBUG
				printf("Queue empty\n");
#endif
			} else {
#ifdef MDLIST_PQUEUE_DEBUG
				printf("Dequeue: 0x%x\n", node->key);
#endif
			}
		} else if('e' == tr_line[0]) {
			sscanf(tr_line + 1, "%lu", &val);

			node = mdlist_pqueue_alloc_node((uint32_t) val);
			if (!node)
				continue;

			if (mdlist_pqueue_coarse_enq(myhead, node)) {
#ifdef MDLIST_PQUEUE_DEBUG
				printf("Enqueue failed\n");
#endif
			} else {
#ifdef MDLIST_PQUEUE_DEBUG
				printf("Enqueue: 0x%x\n", val);
#endif
			}
		} else if('c' == tr_line[0]) {
			sscanf(tr_line + 1, "%lu", &val);

			node = mdlist_pqueue_coarse_contains(myhead, (uint32_t) val);
#ifdef MDLIST_PQUEUE_DEBUG
			if (node) {
				printf("Contains: 0x%x\n", node->key);
			}
#endif

		} else {
			fprintf(stderr, "Unknown trace record: %s\n", tr_line);
		}
	}

out:
	sem_post(&thread_wait_sem);

	return NULL;
}

int main(int argc, char *argv[])
{
	unsigned int i, n_threads;
	pthread_t thrd;

	if (argc != 3) {
		printf("Usage: mdlist_pqueue_test <num_threads> <tracefile>\n");
		return 0;
	}

	sscanf(argv[1], "%d", &n_threads);

	myhead = mdlist_pqueue_coarse_alloc();
	if (!myhead)
		return -errno;

	sem_init(&thread_wait_sem, 0, 0);

	for (i = 0; i < n_threads; i++) {
		pthread_create(&thrd, NULL, test_thread, argv[2]);
	}

	for (i = 0; i < n_threads; i++)
		sem_wait(&thread_wait_sem);

	mdlist_pqueue_coarse_dealloc(myhead);

	return 0;
}
