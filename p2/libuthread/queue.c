#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "queue.h"
#include "queue_helpers.h"
typedef struct node node_t;
struct node {
	void* value;
	node_t *next;
	node_t *prev;
};

struct queue {
	/* TODO Phase 1 */
	node_t *head;
	node_t *tail;
	int length;
};

queue_t queue_create(void)
{
	/* TODO Phase 1 */
	queue_t new_q = malloc(sizeof(struct queue));
	if(!new_q)
		return NULL;

	new_q->head = NULL;
	new_q->tail = NULL;
	new_q->length = 0;
	return new_q;
}
/* for this function I think he only wants us to deallocate memory of the queue itself, not the elements bc he says if queue is not empty then -1*/
int queue_destroy(queue_t queue) 
{
	/* TODO Phase 1 */
	if(queue == NULL || queue_length(queue) != 0) // Not sure what it meant by return -1 if @queue is not empty?
		return -1;
	free(queue);
	// free(queue->head->value);
	// while (queue_length(queue) != -1){
	// 	free(queue->head->value);
	// 	node_t *temp;
	// 	temp = queue->head;
	// 	queue->head = queue->head->next;

	// 	if(queue->head)
	// 		queue->head->prev = NULL;
	// 	else
	// 		queue->tail = NULL;
	// 	queue->length--;
	// 	free(temp);
	// }
	
	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
	/* TODO Phase 1 */
	if(queue == NULL || data  == NULL)
		return -1;

	node_t *new_node = malloc(sizeof(struct node));
	if(!new_node)
		return -1;

	// empty queue
	new_node->value = data;
	if(queue_length(queue) == 0){
		queue->head = queue->tail = new_node;
		new_node->next = NULL;
		new_node->prev = NULL;
	}
	// existing queue
	else{
		queue->tail->next = new_node;
		queue->tail = new_node;
		new_node->next = NULL;
		new_node->prev = queue->tail;
	}

	queue->length++;
	//free(new_node);
	return 0;
}


int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */
	if(queue == NULL || data  == NULL || queue->length == 0)
		return -1;

	*data = queue->head->value;
	node_t *temp = queue->head;
	queue->head->value = NULL;
	queue->head = queue->head->next;
	
	
	if(queue->head != NULL)
		queue->head->prev = NULL;
	//  no more node left in  queue
	else
		queue->tail = NULL;

	queue->length--;
	free(temp);
	return 0;
}

// Not quite sure with this one
int queue_delete(queue_t queue, void *data) //Seg faults on queue_iterate_delete -> fixed by removing else on line 129
{
	/* TODO Phase 1 */
	if(queue == NULL || data  == NULL)
		return -1;

	while(queue->head != NULL){
		if(queue->head->value == data){
			
			node_t *temp = queue->head;
			temp->value = NULL;
			temp->prev = queue->head->prev;

			if(temp->next != NULL)
				temp->next = queue->head->next;
			// else
			// 	temp->next->prev = NULL; // temp->tail
				
			queue->length--;
			temp = NULL;
			free(temp);
			return 0;
		}
		queue->head = queue->head->next;
	}

	return -1;
}

int queue_iterate(queue_t queue, queue_func_t func) 
{
	/* TODO Phase 1 */
	if(queue == NULL || func == 0)
		return -1;

	// from tail to head, if no more head done
	while(queue->head != NULL){
		func(queue->head->value);
		//queue->head = NULL;
		queue->head = queue->head->next;
	}

	return 0;
}

int queue_length(queue_t queue)
{
	/* TODO Phase 1 */
	if(queue == NULL) 
		return -1;
	else
		return (queue->length);
}
void* get_head(queue_t queue) {
	return queue->head;
}
void* get_next(queue_t queue) {
	return queue->head->next;
}
