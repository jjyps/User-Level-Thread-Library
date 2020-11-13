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
enum State {running = 0, ready = 1, blocked = 2, terminated = 4};
static queue_t threads;	
static tcb* curr_thread; 
static tcb* idle;

struct uthread_tcb {
	uthread_ctx_t *thread_context;
	void* new_stack;
	int curr_state;
};
struct uthread_tcb *uthread_current(void)
{
	return curr_thread;
}

void uthread_yield(void)
{
	/* Current thread which will be previous thread & next thread, oldest thread in queue */
	tcb *prev_thread = uthread_current();
	tcb *next_thread = NULL;

	prev_thread->curr_state = ready;
	preempt_disable();
	queue_dequeue(threads, (void**) &next_thread);
	next_thread->curr_state = running;
	curr_thread = next_thread;

	/*idle thread shouldn't exit unless all threads are finished*/	
	if(next_thread == idle && queue_length(threads) > 1) {
		next_thread->curr_state = ready;
		queue_dequeue(threads, (void**) &next_thread);
		next_thread->curr_state = running;
		curr_thread = next_thread;
		queue_enqueue(threads,next_thread);
	}

	queue_enqueue(threads, prev_thread);
	uthread_ctx_switch(prev_thread->thread_context, next_thread->thread_context);
	preempt_enable();
}
void uthread_exit(void)
{
	/* Terminate the current thread and yield to the next one */
	curr_thread->curr_state = terminated;
	tcb* terminated_thread = uthread_current();

	uthread_yield();
	queue_delete(threads,terminated_thread);
	free(terminated_thread->thread_context);
	uthread_ctx_destroy_stack(terminated_thread->new_stack);
	free(terminated_thread);
	
}
int uthread_create(uthread_func_t func, void *arg)
{
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
	preempt_disable();
	threads = queue_create();
	preempt_enable();
	if (!threads)
		return -1;

	/* Multithreading scheudling starts - Set current thread as idle thread */
	idle = malloc(sizeof(tcb));
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
	while(queue_length(threads) != 0) {
		uthread_yield();
	}
	
	preempt_stop();
	free(idle->thread_context);
	uthread_ctx_destroy_stack(idle->new_stack);
	free(idle);
	
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
