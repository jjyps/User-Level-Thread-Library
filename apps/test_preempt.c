#include <assert.h>
#include <stdio.h>
#include <preempt.c>
#include <unistd.h>
#include <uthread.h>

void thread2(void *arg) {
    printf("Hello im thread2...");
    printf("im going to be annoying and run forever\n");
    while(1) {
        printf("forever\n");
    }
}
void thread1(void *arg) {
    printf("Hello I am thread 1, im going to be nice and yield\n");
    uthread_create(thread2, NULL);
    uthread_yield();
    printf("Im back, never should've trusted thread 2\n");
    exit(0);
}
int main(void) {
    uthread_start(thread1, NULL);
    return 0;
}
