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

	fprintf(stderr, "\n*** TEST queue_simple ***\n");

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
    length = queue_length(q);
    TEST_ASSERT(length == 1);
}
/* Tests queue_destroy with success*/
void test_queue_destroy_success(void) {
    queue_t q;

    fprintf(stderr, "\n*** TEST queue_destroy_success ***\n");

    q = queue_create();
    TEST_ASSERT(queue_destroy(q) == 0);
}
/* Tests queue_destroy with empty queue*/
void test_queue_destroy_not_empty(void) {
    queue_t q;
    int data = 1;

    fprintf(stderr, "\n*** TEST queue_destroy_not_empty ***\n");

    q = queue_create();
    queue_enqueue(q, &data);
    TEST_ASSERT(queue_destroy(q) == -1);

}
/*Tests queue_destroy with NULL queue */
void test_queue_destroy_null(void) {
    queue_t q = NULL;

    fprintf(stderr, "\n*** TEST queue_destroy_null ***\n");

    TEST_ASSERT(queue_destroy(q) == -1);
}
/* Tests queue_enqueue when data is NULL */
void test_queue_enqueue_null_data(void) {
    queue_t q;
    int* data = NULL;

    fprintf(stderr, "\n*** TEST queue_enqueue_null_data ***\n");

    q = queue_create();
    TEST_ASSERT(queue_enqueue(q, data) == -1);
}
/* Tests queue_enqueue when queue is null */
void test_queue_enqueue_null_queue(void) {
    queue_t q = NULL;
    int data = 1;

    fprintf(stderr, "\n*** TEST queue_enqueue_null_queue ***\n");

    TEST_ASSERT(queue_enqueue(q, &data) == -1);
}
/* Tests queue_dequeue when queue is empty*/
void test_queue_dequeue_empty_queue(void) {
    queue_t q;
    int *ptr;

    fprintf(stderr, "\n*** TEST queue_dequeue_empty_queue ***\n");

    q = queue_create();
    TEST_ASSERT(queue_dequeue(q, (void**)&ptr) == -1);
}
/*Tests queue_dequeue when queue is null*/
void test_queue_dequeue_null_queue(void) {
    queue_t q = NULL;
    int *ptr;

    fprintf(stderr, "\n*** TEST queue_dequeue_null_queue ***\n");

    TEST_ASSERT(queue_dequeue(q, (void**)&ptr) == -1);
}
/*Tests that queue_deque removes oldest item*/
void test_queue_deque_remove_oldest(void) {
    queue_t q;
    int *ptr;
    int data[] = {1,2,3,4,5};
    int length_of_data = sizeof(data)/ sizeof(data[0]);

    fprintf(stderr, "\n*** TEST queue_deque_remove_oldest***\n");

    q = queue_create();
    for(int i = 0; i < length_of_data; i++) {
        queue_enqueue(q, &data[i]);
    }
    queue_dequeue(q,(void**)&ptr);
    TEST_ASSERT(ptr == &data[0]);
}
/*Tests deque/enque with large number of elements*/
void test_queue_large(void) {
    queue_t q;
    int* ptr;
    int length_of_data = 1000;
    int* data = (int*)malloc(length_of_data* sizeof(int));
    
    for(int i = 0; i < length_of_data; i++) {
        data[i] = i;
    }

    fprintf(stderr, "\n*** TEST queue_large***\n");
    
    q = queue_create();
    for(int i = 0; i < length_of_data; i++) {
        queue_enqueue(q, &data[i]);
    }
    TEST_ASSERT(queue_length(q) == length_of_data);

    for(int i = 0; i < length_of_data; i++) {
        queue_dequeue(q,(void**)&ptr);
    }
    TEST_ASSERT(queue_length(q) == 0);
    TEST_ASSERT(ptr == &data[length_of_data-1]);
    free(data);
}
/* Tests queue_delete with data that doesnt exist*/
void test_queue_delete_data_doesnt_exist(void) {
    queue_t q;
    int data1 = 1, data2 = 2;

    fprintf(stderr, "\n*** TEST queue_delete_data_doesnt_exist***\n");

    q = queue_create();
    queue_enqueue(q, &data1);
    TEST_ASSERT(queue_delete(q,&data2) == -1);
}
/* Tests queue_delete when data is null */
void test_queue_delete_null_data(void) {
    queue_t q;
    int data1 = 1, * data2 = NULL;

    fprintf(stderr, "\n*** TEST queue_delete_null_data***\n");

    q = queue_create();
    queue_enqueue(q, &data1);
    TEST_ASSERT(queue_delete(q,data2) == -1);
}
/*Tests queue_delete when queue is null */
void test_queue_delete_null_queue(void) {
    queue_t q = NULL;
    int data1 = 1;

    fprintf(stderr, "\n*** TEST queue_delete_null_queue***\n");

    TEST_ASSERT(queue_delete(q,&data1) == -1);
}
/*Tests queue_delete when asked to delete a exisiting element*/
void test_queue_delete_simple(void) {
    queue_t q;
    int data1 = 1, data2 = 2;

    fprintf(stderr, "\n*** TEST queue_delete_simple***\n");

    q = queue_create();
    queue_enqueue(q, &data1);
    queue_enqueue(q, &data2);
    queue_delete(q,&data1);
    TEST_ASSERT(queue_delete(q,&data1) == -1);
    TEST_ASSERT(queue_length(q) == 1);
}
/* Tests queue_delete with large queue*/
void test_queue_delete_large(void) {
    queue_t q;
    int length_of_data = 1000;
    int* data = (int*)malloc(length_of_data* sizeof(int));

    for(int i = 0; i < length_of_data; i++) {
        data[i] = i;
    }

    fprintf(stderr, "\n*** TEST queue_delete_large***\n");

    q = queue_create();
    for(int i = 0; i < length_of_data; i++) {
        queue_enqueue(q, &data[i]);
    }

    queue_delete(q,&data[5]);
    TEST_ASSERT(queue_delete(q,&data[5]) == -1);
    data[7] = 1;
    queue_enqueue(q, &data[7]);
    TEST_ASSERT(queue_enqueue(q, &data[7]) == 0);
    free(data);
}
/*Tests queue_iterate */
static void even_to_zero(void *data) {
    int*a = (int*)data;
    if(*a % 2 == 0)
        *a = 0;
}
void test_queue_iterate(void) {
    queue_t q;
    int data [] = {1,2,3,4,5,6,7,8,9,10};
    int length_of_data = sizeof(data) / sizeof(data[0]);

    fprintf(stderr, "\n*** TEST queue_iterate***\n");

    q = queue_create();
    for(int i = 0; i < length_of_data; i++) {
        queue_enqueue(q, &data[i]);
    }

    queue_iterate(q,even_to_zero);
    TEST_ASSERT(data[1] == 0 && data[3] == 0 && data[5] == 0 && data[7] == 0 && data[9] == 0);
}
/*Tests queue_iterate when queue is NULL */
void test_queue_iterate_null_queue(void) {
    queue_t q = NULL;

    fprintf(stderr, "\n*** TEST queue_iterate_null_queue***\n");

    TEST_ASSERT(queue_iterate(q,even_to_zero) == -1);
}
/*Tests queue_iterate when multiple elements get deleted */
queue_t qItr;
static void odd_to_zero(void *data) {
    int*a = (int*)data;
    if(*a % 2)
        *a = 0;
    
    if(*a == 10)
        queue_delete(qItr, data);   
}
void test_queue_iterate_delete(void) {
    int data [] = {1,2,3,4,5,6,7,8,9,10};
    int length_of_data = sizeof(data) / sizeof(data[0]);

    fprintf(stderr, "\n*** TEST queue_iterate_delete***\n");

    qItr = queue_create();
    for(int i = 0; i < length_of_data; i++) {
        queue_enqueue(qItr, &data[i]);
    }
    queue_iterate(qItr,odd_to_zero);
    TEST_ASSERT(queue_length(qItr) == 9);
    TEST_ASSERT(data[0] == 0 && data[2] == 0 && data[4] == 0 && data[6] == 0 && data[8] == 0);
}
int main(void)
{
	test_create(); 
	test_queue_simple(); 
    test_queue_length(); 
    test_queue_destroy_success(); 
    test_queue_destroy_not_empty(); 
    test_queue_destroy_null(); 
    test_queue_enqueue_null_data(); 
    test_queue_enqueue_null_queue(); 
    test_queue_dequeue_empty_queue(); 
    test_queue_dequeue_null_queue(); 
    test_queue_deque_remove_oldest();
    test_queue_large(); 
    test_queue_delete_data_doesnt_exist(); 
    test_queue_delete_null_data(); 
    test_queue_delete_null_queue(); 
    test_queue_delete_simple(); 
    test_queue_delete_large(); 
    test_queue_iterate();
    test_queue_iterate_null_queue();
    test_queue_iterate_delete();
	return 0;
}