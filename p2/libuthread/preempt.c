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
static struct sigaction sa;
/* Signal handler = timer interrupt handler, will force the currently running thread to yield */
//void timer_interrupt_handler(int signum){
//	uthread_yield();
//}

void preempt_disable(void)
{
	/* TODO Phase 4 */
	sigset_t ss;
	sigemptyset(&ss);
    sigaddset(&ss,  SIGVTALRM);
    sigprocmask(SIG_BLOCK, &ss, NULL);
	
}

void preempt_enable(void)
{
	/* TODO Phase 4 */
	sigset_t ss;
	sigemptyset(&ss);
    sigaddset(&ss,  SIGVTALRM);
    sigprocmask(SIG_UNBLOCK, &ss, NULL);
	
}

void preempt_start(void)
{
	/* TODO Phase 4 */
	
	//sa.sa_handler = timer_interrupt_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGVTALRM, &sa, NULL);
	//alarm(1/HZ);
}

void preempt_stop(void)
{
	/* TODO Phase 4 */
	sa.sa_flags = SA_RESETHAND;
	sigaction(SIGVTALRM, &sa, NULL);
}