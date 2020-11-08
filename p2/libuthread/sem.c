#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"
#include "uthread.h"

struct semaphore {
	/* TODO Phase 3 */
	queue_t blocked;
	size_t count;
};

sem_t sem_create(size_t count)
{
	/* TODO Phase 3 */
	sem_t sem = malloc(sizeof(sem_t));
	if(!sem)
		return NULL;
	sem->count = count;
	sem->blocked = queue_create();
	return sem;
}

int sem_destroy(sem_t sem)
{
	/* TODO Phase 3 */
	if (sem == NULL || queue_length(sem->blocked) != -1)
		return -1;
	
	free(sem);
	return 0;
}

int sem_down(sem_t sem)
{
	/* TODO Phase 3 */
	if(sem == NULL)
		return -1;

	sem->count--;
	return 0;
}

int sem_up(sem_t sem)
{
	/* TODO Phase 3 */
	if(sem == NULL)
		return -1;

	sem->count++;
	return 0;
}

