#include<assert.h>
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
enum State { running = 0,ready = 1,blocked = 2,zombie = 3,terminated = 4, idle = 5 };
static queue_t run_threads;
static struct uthread_tcb *curr_thread;
//static struct uthread_tcb* idle_thread;
struct uthread_tcb
{
	/* TODO Phase 2 */
	ucontext_t thread_context;
	void *top_of_stack;
	int curr_state;
	uthread_func_t func;
	void *arg;
};

struct uthread_tcb *uthread_current(void)
{
	/* TODO Phase 2 */
	return curr_thread;
}

void uthread_yield(void)
{
	/* TODO Phase 2 */
	struct uthread_tcb *prev_thread = uthread_current();
	void *temp_thread;
	queue_dequeue(run_threads, (void **)&temp_thread);
	queue_enqueue(run_threads, &prev_thread);
	struct uthread_tcb *next_thread = (struct uthread_tcb *)get_head(run_threads);
	curr_thread = next_thread;
	uthread_ctx_switch(&prev_thread->thread_context, &next_thread->thread_context);
}

void uthread_exit(void)
{
	/* TODO Phase 2 */
	curr_thread->curr_state = 3;
}

int uthread_create(uthread_func_t func, void *arg) //Done, I think?
{
	/* TODO Phase 2 */
	void *thread_stack;
	struct uthread_tcb thread_tcb;

	thread_stack = uthread_ctx_alloc_stack();

	if (thread_stack == NULL)
		return -1;

	thread_tcb.curr_state = 0;
	thread_tcb.func = func;
	thread_tcb.arg = arg;
	if (uthread_ctx_init(&thread_tcb.thread_context, thread_stack, func, arg) == -1)
		return -1;

	run_threads = queue_create();
	if (run_threads == NULL)
		return -1;

	if (queue_enqueue(run_threads, &thread_tcb) == -1)
		return -1;
	return 0;
}

int uthread_start(uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
	/* initialize first thread */
	uthread_create(func, arg);
	curr_thread = (struct uthread_tcb *)get_head(run_threads);
	curr_thread->curr_state = 5;

	/*Loop until there are no more threads */
	while (queue_length(run_threads) != 0)
	{
		/* if thread is not ready to run then yield */
		if ((curr_thread->curr_state != 1 && curr_thread->curr_state != 5) || (curr_thread->curr_state == 5 && queue_length(run_threads) > 1))
		{
			uthread_yield();
		}

		/* execute function */
		func(arg);
		curr_thread->curr_state = 0;
		uthread_exit();
		/*Thread was completed then deallocate stack, delete from queue & yield to next one */
		if (curr_thread->curr_state == 3)
		{
			uthread_ctx_destroy_stack(curr_thread->top_of_stack);
			curr_thread->curr_state = 4;
			struct uthread_tcb *prev_thread;
			queue_dequeue(run_threads, (void **)&prev_thread);
			if (queue_length(run_threads) == 0)
				break;
			uthread_yield();
		}
	}
	queue_destroy(run_threads);

	return 0;
}

void uthread_block(void)
{
	/* TODO Phase 2/3 */
	curr_thread->curr_state = 2;
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	/* TODO Phase 2/3 */
	uthread->curr_state = 1;
}
