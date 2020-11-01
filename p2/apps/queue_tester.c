#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <queue.h>

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

/* Create */
void test_create(void)
{
	fprintf(stderr, "*** TEST create ***\n");

	TEST_ASSERT(queue_create() != NULL);
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
	int data = 3, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_simple ***\n");

	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &data);
}
/* Tests queue_length*/
void test_queue_length(void) {
    queue_t q;
    int data1 = 1, data2 = 2, *ptr;
    int length;

    fprintf(stderr, "*** TEST queue_length ***\n");

    q = queue_create();
    length = queue_length(q);

    TEST_ASSERT(length == 0);
    queue_enqueue(q, &data1);
    queue_enqueue(q, &data2);
    length = queue_length(q);
    TEST_ASSERT(length == 2);
    queue_dequeue(q, (void**)&ptr);
    TEST_ASSERT(length == 1);
}
/* Tests queue_destroy with success*/
void test_queue_destroy_success(void) {
    queue_t q;

    fprintf(stderr, "*** TEST queue_destroy_success ***\n");

    q = queue_create();
    TEST_ASSERT(queue_destroy(q) == 0);

}
/* Tests queue_destroy with empty queue*/
void test_queue_destroy_not_empty(void) {
    queue_t q;
    int data = 1;

    fprintf(stderr, "*** TEST queue_destroy_not_empty ***\n");
    q = queue_create();
    enqueue_queue(q, &data);
    TEST_ASSERT(queue_destroy(q) == -1);

}
/*Tests queue_destroy with NULL queue */
void test_queue_destroy_null(void) {
    queue_t q;

    fprintf(stderr, "*** TEST queue_destroy_null ***\n");

    TEST_ASSERT(queue_destroy(q) == -1);
}
/* Tests queue_enqueue when data is NULL */
void test_queue_enqueue_null_data(void) {
    queue_t q;
    int data = NULL;

    fprintf(stderr, "*** TEST queue_enqueue_null_data ***\n");

    q = queue_create();
    TEST_ASSERT(queue_enqueue(q, &data) == -1);
}
/* Tests queue_enqueue when queue is null */
void test_queue_enqueue_null_queue(void) {
    queue_t q;
    int data = 1;

    fprintf(stderr, "*** TEST queue_enqueue_null_queue ***\n");

    TEST_ASSERT(queue_enqueue(q, &data) == -1);
}
/* Tests queue_dequeue when queue is empty*/
void test_queue_dequeue_empty_queue(void) {
    queue_t q;
    int *ptr;

    fprintf(stderr, "*** TEST queue_dequeue_empty_queue ***\n");
    q = queue_create();
    TEST_ASSERT(queue_dequeue(q, (void**)&ptr) == -1);
}
/*Tests queue_dequeue when queue is null*/
void test_queue_dequeue_null_queue(void) {
    queue_t q;
    int *ptr;

    fprintf(stderr, "*** TEST queue_dequeue_null_queue ***\n");
    TEST_ASSERT(queue_dequeue(q, (void**)&ptr) == -1);
}
/*Tests that queue_deque removes oldest item*/
void test_queue_deque_remove_oldest(void) {
    queue_t q;
    int *ptr;
    int datas[5] = {1,2,3,4,5};

    fprintf(stderr, "*** TEST queue_deque_remove_oldest***\n");

    q = queue_create();
    for(int i = 0; i < 5; i++) {
        queue_enqueue(q, &datas[i]);
    }
    queue_dequeue(q,(void**)&ptr);
    TEST_ASSERT(ptr == &datas[0]);
}
/*Tests deque/enque with large number of elements*/
void test_queue_large(void) {
    queue_t q;
    int* ptr;
    int* datas = (int*)malloc(1000* sizeof(int));

    for(int i = 0; i < 1000; i++) {
        datas[i] = i;
    }

    fprintf(stderr, "*** TEST queue_large***\n");
    
    q = queue_create();
    for(int i = 0; i < 1000; i++) {
        queue_enqueue(q, &datas[i]);
    }
    TEST_ASSERT(queue_length(q) == 1000);

    for(int i = 0; i < 1000; i++) {
        queue_dequeue(q,(void**)&ptr);
    }
    TEST_ASSERT(queue_length(q) == 0);
    TEST_ASSERT(ptr == &datas[0]);
    free(datas);
}
/* Tests queue_delete with data that doesnt exist*/
void test_queue_delete_data_doesnt_exist(void) {
    queue_t q;
    int data1 = 1, data2 = 2;
    fprintf(stderr, "*** TEST queue_delete_data_doesnt_exist***\n");

    q = queue_create();
    queue_enqueue(q, &data1);
    TEST_ASSERT(queue_delete(q,&data2) == -1);
}
/* Tests queue_delete when data is null */
void test_queue_delete_null_data(void) {
    queue_t q;
    int data1 = 1, data2 = NULL;
    fprintf(stderr, "*** TEST queue_delete_null_data***\n");

    q = queue_create();
    queue_enqueue(q, &data1);
    TEST_ASSERT(queue_delete(q,&data2) == -1);
}
/*Tests queue_delete when queue is null */
void test_queue_delete_null_queue(void) {
    queue_t q;
    int data1 = 1;
    fprintf(stderr, "*** TEST queue_delete_null_queue***\n");

    TEST_ASSERT(queue_delete(q,&data1) == -1);
}
/*Tests queue_delete when asked to delete a exisiting element*/
void test_queue_delete_simple(void) {
    queue_t q;
    int data1 = 1, data2 = 2;

    fprintf(stderr, "*** TEST queue_delete_simple***\n");

    q = queue_create();
    queue_enqueue(q, &data1);
    queue_enqueue(q, &data2);
    queue_delete(q,&data1);
    TEST_ASSERT(queue_delete_simple(q,&data1) == -1);
    TEST_ASSSERT(queue_length(q) == 1);
}
/* Tests queue_delete with large queue*/
void test_queue_delete_large(void) {
    queue_t q;
    int* datas = (int*)malloc(1000* sizeof(int));

    for(int i = 0; i < 1000; i++) {
        datas[i] = i;
    }

    fprintf(stderr, "*** TEST queue_delete_large***\n");

    q = queue_create();
    for(int i = 0; i < 1000; i++) {
        queue_enqueue(q, &datas[i]);
    }

    queue_delete(q,&datas[5]);
    TEST_ASSERT(queue_delete(q,&datas[5]) == -1);
    datas[7] = 1;
    queue_enqueue(q, &datas[7]);
    TEST_ASSERT(queue_enqueue(q, &datas[7]) == 0);
    free(datas);
}
/*Tests queue_iterate */
static void even_to_zero(void *data) {
    int*a = (int*)data;
    if(*a % 2 == 0)
        *a = 0;
}
void test_queue_iterate(void) {
    queue_t q;
    int datas [] = {1,2,3,4,5,6,7,8,9,10};

    fprintf(stderr, "*** TEST queue_iterate***\n");

    q = queue_create();
    for(int i = 0; i < 10; i++) {
        queue_enqueue(q, &datas[i]);
    }

    queue_iterate(q,even_to_zero);
    TEST_ASSERT(datas[1] == 0 && datas[3] == 0 && datas[5] == 0 && datas[7] == 0 && datas[10] == 0);
}
/*Tests queue_iterate when queue is NULL */
void test_queue_iterate_null_queue(void) {
    queue_t q;

    fprintf(stderr, "*** TEST queue_iterate_null_queue***\n");

    TEST_ASSERT(queue_iterate(q,even_to_zero) == -1);
}
/*Tests queue_iterate when multiple elements get deleted */
queue_t qItr;
static void odd_to_zero(void *data) {
    int*a = (int*)data;
    if(*a % 2)
        *a = 0;
    else
        queue_delete(qItr, data);   
}
void test_queue_iterate_delete(void) {
    int datas [] = {1,2,3,4,5,6,7,8,9,10};

    fprintf(stderr, "*** TEST queue_iterate_delete***\n");

    qItr = queue_create();
    for(int i = 0; i < 10; i++) {
        queue_enqueue(qItr, &datas[i]);
    }

    queue_iterate(qItr,odd_to_zero);
    TEST_ASSERT(queue_length(qItr) == 5);
    TEST_ASSERT(datas[0] == 0 && datas[2] == 0 && datas[4] == 0 && datas[6] == 0 && datas[8] == 0);
}
int main(void)
{
	test_create();
	test_queue_simple();

	return 0;
}
