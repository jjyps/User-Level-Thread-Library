#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "context.c"
#include "queue.h"
#include "uthread.h"
#include "private.h"

typedef struct uthread_tcb tcb;		
enum State {running = 0, ready = 1, blocked = 2, zombie = 3, terminated = 4} ;
static queue_t threads;	
static tcb* curr_thread;

struct uthread_tcb {
	/* TODO Phase 2 */
	uthread_ctx_t *thread_context;
	void* new_stack;
	int curr_state;
	
};

struct uthread_tcb *uthread_current(void)
{
	/* TODO Phase 2 */
	return curr_thread;
}

void uthread_yield(void)
{
	/* TODO Phase 2 */
	/* Current thread which will be previous thread & next thread, oldest thread in queue */
	tcb *prev_thread = uthread_current();
	tcb *next_thread = NULL;

	prev_thread->curr_state = ready;
	preempt_disable();
	queue_dequeue(threads, (void**) &next_thread);
	next_thread->curr_state = running;
	curr_thread = next_thread;
	

	// Prevent idle thread to be enqueued
	//if(prev_thread != idle)
	queue_enqueue(threads, prev_thread);
	preempt_enable();	
	uthread_ctx_switch(prev_thread->thread_context, next_thread->thread_context);
}

void uthread_exit(void)
{
	/* TODO Phase 2 */
	curr_thread->curr_state = terminated;
	/* Terminate the current state and yield to the next thread */
	uthread_yield();
}

int uthread_create(uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
	tcb* new_thread = malloc(sizeof(tcb));
	if (!new_thread)
		return -1;
	
	new_thread->new_stack = uthread_ctx_alloc_stack();
	if(new_thread->new_stack == NULL)
		return -1;

	new_thread->thread_context = malloc(sizeof(uthread_ctx_t));
	if (new_thread->thread_context == NULL)
		return -1;
	
	if (uthread_ctx_init(new_thread->thread_context, new_thread->new_stack, func, arg) == -1)
		return -1;

	new_thread->curr_state = ready;
	preempt_disable();
	queue_enqueue(threads, new_thread);
	preempt_enable();

	return 0;
}


int uthread_start(uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
	preempt_disable();
	threads = queue_create();
	preempt_enable();
	if (!threads)
		return -1;

	// Multithreading scheudling starts - Set current thread as idle thread
	tcb* idle = malloc(sizeof(tcb));
	if (!idle)
		return -1;

	idle->thread_context = malloc(sizeof(uthread_ctx_t));
	if (idle->thread_context == NULL) 
		return -1;

	curr_thread = idle;
	curr_thread->curr_state = running;
	preempt_start();
	if(uthread_create(func, arg) == -1)
		return -1;

	/* returns once all the threads have finished running */
	while(queue_length(threads) != 0) 
		uthread_yield();
	
	preempt_stop();
	return 0;
}


void uthread_block(void)
{
	curr_thread->curr_state = blocked;
	uthread_yield();
}


void uthread_unblock(struct uthread_tcb *uthread)
{
	uthread->curr_state = ready;
	preempt_disable();
	queue_enqueue(threads, uthread);
	preempt_enable();
}
