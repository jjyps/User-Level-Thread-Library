#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "queue.h"
#include "queue_helpers.h"
#include "private.h"
#include "uthread.h"
#include "context.c"
typedef struct uthread_tcb tcb;
enum State {running = 0,ready = 1,blocked = 2,zombie = 3,terminated = 4};
queue_t threads;
//queue_t running_q;
//queue_t ready_q;
//queue_t q;
tcb *curr_thread;

struct uthread_tcb
{
	/* TODO Phase 2 */
	int curr_state;
	uthread_ctx_t* thread_context;
	void* top_of_stack;
};

struct uthread_tcb *uthread_current(void)
{
	/* TODO Phase 2 */
	return curr_thread;
}

void uthread_yield(void)
{
	/* TODO Phase 2 */
	
	tcb* prev_thread = NULL;
	tcb* next_thread = NULL;

	queue_dequeue(threads,(void**)&next_thread);
	next_thread->curr_state = 0;

	prev_thread = uthread_current();
	prev_thread->curr_state = 1;
	queue_enqueue(threads,prev_thread);
	curr_thread = next_thread;

	//curr_thread = (struct uthread_tcb*)get_head(threads);
	//uthread_ctx_t* prev_ctx = next_thread->thread_context;
	//uthread_ctx_t* next_ctx = curr_thread->thread_context;
	
	uthread_ctx_switch(prev_thread->thread_context, next_thread->thread_context);
}

void uthread_exit(void)
{
	/* TODO Phase 2 */
	curr_thread->curr_state = 3;
}

int uthread_create(uthread_func_t func, void *arg) 
{
	/* TODO Phase 2 */
	tcb* new_thread = malloc(sizeof(tcb));

	new_thread->curr_state = 1;
	void* new_stack = uthread_ctx_alloc_stack();
	if(new_stack == NULL)
		return -1;
	
	new_thread->thread_context = malloc(sizeof(uthread_ctx_t));
	if(uthread_ctx_init(new_thread->thread_context, new_stack, func, arg) == -1)
		return -1;


	new_thread->curr_state = 0;
	queue_enqueue(threads, &new_thread);
	if(queue_length(threads) == 1) {
		curr_thread = (struct uthread_tcb*)get_head(threads);
	}
	uthread_ctx_bootstrap(func, arg);
	
	return 0;
}

int uthread_start(uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
	threads = queue_create();
	if(uthread_create(func, arg) == -1) 
		return -1;
	

	while(1) {

		if(curr_thread->curr_state == 3) {
			uthread_ctx_destroy_stack(curr_thread->top_of_stack);
			tcb zombie_thread;
			curr_thread->curr_state = 4;
			queue_dequeue(threads, (void**)&zombie_thread);
		}
		if(queue_length(threads) == 0) 
			break;


	}
	queue_destroy(threads);
	return 0;
}

void uthread_block(void)
{
	/* TODO Phase 2/3 */
	curr_thread->curr_state = 2;
	uthread_yield();
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	/* TODO Phase 2/3 */
	uthread->curr_state = 1;
}