#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "queue.h"
#include "private.h"
#include "uthread.h"

enum State {running = 0, ready = 1, blocked = 2, zombie = 3, terminated = 4};
queue_t cur_threads;
struct uthread_tcb curr_thread;
struct uthread_tcb {
	/* TODO Phase 2 */
	ucontext_t thread_context;
	void* top_of_stack;
	int curr_state;
	uthread_func_t func;
	void* arg;
};

struct uthread_tcb *uthread_current(void)
{
	/* TODO Phase 2 */
	return &curr_thread;

}

void uthread_yield(void)
{
	/* TODO Phase 2 */
	void* prev_thread = uthread_current();
	queue_dequeue(cur_threads, (void**)prev_thread);
	queue_enqueue(cur_threads, &prev_thread);
	void* next_thread = uthread_current();
	uthread_ctx_switch(prev_thread, next_thread);
	
}

void uthread_exit(void)
{
	/* TODO Phase 2 */
}

int uthread_create(uthread_func_t func, void *arg) //Done, I think?
{
	/* TODO Phase 2 */
	void* thread_stack;
	struct uthread_tcb thread_tcb;

	thread_stack = uthread_ctx_alloc_stack();

	if(thread_stack == NULL) 
		return -1;

	thread_tcb.curr_state = 0;
	thread_tcb.func = func;
	thread_tcb.arg = arg;
	if(uthread_ctx_init(&thread_tcb.thread_context, thread_stack, func, arg) == -1) 
		return -1;

	cur_threads = queue_create();
	if(cur_threads == NULL)
		return -1;

	if(queue_enqueue(cur_threads, &thread_tcb) == -1) 
		return -1;
	
	return 0;
}

int uthread_start(uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
	uthread_create(func, arg);
	
	while(queue_length(cur_threads) != 0) {
		struct uthread_tcb* prev_thread = uthread_current();
		if(prev_thread->curr_state == 3) { //Thread was completed
			uthread_ctx_destroy_stack(prev_thread->top_of_stack);
			prev_thread->curr_state = 4;
			queue_delete(cur_threads, prev_thread);		
		}
		uthread_yield();

		
			
	
	}
	
	return 0;
	
}

void uthread_block(void)
{
	/* TODO Phase 2/3 */
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	/* TODO Phase 2/3 */
}

