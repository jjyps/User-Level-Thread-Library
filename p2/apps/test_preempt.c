#include <assert.h>
#include <stdio.h>
#include <preempt.c>
#include <uthread.h>
#include <unistd.h>

#define TEST_ASSERT(assert)				\
do {									\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {						\
		printf("PASS\n");				\
	} else	{							\
		printf("FAIL\n");				\
		exit(1);						\
	}									\
} while(0)
void thread4(void* arg) {
    printf("Thread 4: to infinity and beyond\n");
    while(1) {
        
    }
}
void thread3(void* arg) {
    uthread_create(thread2, NULL);
    printf("Hi I am thread 3\n");
    while(1) {
        
    }
}
void thread2(void* arg) {
    printf("Hi I am thread 2\n");
}
void thread1(void* arg) {
    uthread_create(thread2, NULL);
    printf("Hi I am thread 1\n");
}
void test_preempt_start_and_stop() {
    fprintf(stderr, "\n*** TEST preempt_start and preempt_stop ***\n");

    uthread_start(thread1, NULL);
    struct uthread_tcb* tcb1 = uthread_current();
    preempt_start();
    pause();
    TEST_ASSERT(tcb1 != uthread_current());
    prempt_stop();
    uthread_yield();
    TEST_ASSERT(tcb1 == uthread_current());
    uthread_exit();
    uthread_exit();

}
void test_preempt_disable_and_enable() {
    fprintf(stderr, "*** TEST preempt_enable and preempt_disable ***\n");
    uthread_start(thread1, NULL);
    preempt_disable();
    TEST_ASSERT(raise(SIGVTALRM) != 0);
    preempt_enable();
    TEST_ASSERT(raise(SIGVTALRM) == 0);  
}

int main(void) {

    return 0;
}
