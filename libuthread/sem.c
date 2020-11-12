#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"
#include "uthread.h"

struct semaphore {
	queue_t blocked;
	size_t count;
};

sem_t sem_create(size_t count)
{
	sem_t sem = malloc(sizeof(sem_t));
	if(!sem)
		return NULL;
	preempt_enable();
	sem->count = count;
	sem->blocked = queue_create();
	return sem;
}

int sem_destroy(sem_t sem)
{
	if (sem == NULL || queue_destroy(sem->blocked) == -1)
		return -1;
	
	preempt_enable();
	free(sem);
	return 0;
}

int sem_down(sem_t sem)
{
	if(sem == NULL)
		return -1;
		
	/* No available resource cause the calling thread blocked */
	if(sem->count == 0){
		struct uthread_tcb *caller;
		caller = uthread_current();
		queue_enqueue(sem->blocked, caller);
		uthread_block();
	}

	sem->count--;
	return 0;
}

int sem_up(sem_t sem)
{
	if(sem == NULL)
		return -1;

	/* If the waiting list not empty, release resource causing the first thread unblocked */
	if(queue_length(sem->blocked) != 0){
		struct uthread_tcb *first;
		queue_dequeue(sem->blocked, (void**)&first);
		uthread_unblock(first);
	}

	sem->count++;
	return 0;
}

