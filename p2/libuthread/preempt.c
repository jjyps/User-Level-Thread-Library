#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

/* Signal handler = timer interrupt handler, will force the currently running thread to yield */
void timer_interrupt_handler(int signum){
	uthread_yield();
}

void preempt_disable(void)
{
	/* TODO Phase 4 */
}

void preempt_enable(void)
{
	/* TODO Phase 4 */
}

void preempt_start(void)
{
	/* TODO Phase 4 */
	struct sigacttion sa;
	sa.sa_handler = timer_interrupt_handler;
}

void preempt_stop(void)
{
	/* TODO Phase 4 */
}

