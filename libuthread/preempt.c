#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include "private.h"
#include "uthread.h"

#define time 10000 //time in microseconds = 1/HZs
#define time_start 0 //start time
static struct sigaction sa;

/* Signal handler = timer interrupt handler, will force the currently running thread to yield */
void alarm_handler(){
	uthread_yield();
}

void preempt_disable(void)
{
	sigset_t ss;
	sigemptyset(&ss);
    sigaddset(&ss,  SIGVTALRM);
    sigprocmask(SIG_BLOCK, &ss, NULL);
}

void preempt_enable(void)
{
	sigset_t ss;
	sigemptyset(&ss);
    sigaddset(&ss,  SIGVTALRM);
    sigprocmask(SIG_UNBLOCK, &ss, NULL);
}

void preempt_start(void)
{
	/*Set up signal*/
	memset (&sa, 0, sizeof (sa));
	sa.sa_handler = alarm_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGVTALRM, &sa, NULL);
	
	/*Set up timer*/
    struct itimerval timer;
    timer.it_value.tv_sec = time_start;
 	timer.it_value.tv_usec = time;
 	timer.it_interval.tv_sec = time_start;
 	timer.it_interval.tv_usec = time;
	
	setitimer(ITIMER_VIRTUAL, &timer, NULL);
}

void preempt_stop(void)
{
	/* Reset signal */
	sa.sa_flags = SA_RESETHAND;
	sigaction(SIGVTALRM, &sa, NULL);
}