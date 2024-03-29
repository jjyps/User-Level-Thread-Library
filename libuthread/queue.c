#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "queue.h"
typedef struct node node_t;
struct node {
	void* value;
	node_t *next;
	node_t *prev;
};

struct queue {
	node_t *head;
	node_t *tail;
	int length;
};

queue_t queue_create(void)
{
	queue_t new_q = malloc(sizeof(struct queue));
	if(!new_q)
		return NULL;

	new_q->head = NULL;
	new_q->tail = NULL;
	new_q->length = 0;
	return new_q;
}
int queue_destroy(queue_t queue) 
{
	if(queue == NULL || queue_length(queue) != 0)
		return -1;
	free(queue);
	return 0;
}
int queue_enqueue(queue_t queue, void *data)
{
	if(queue == NULL || data  == NULL)
		return -1;

	node_t *new_node = malloc(sizeof(struct node));
	if(!new_node)
		return -1;

	/* empty queue */
	new_node->value = data;
	if(queue_length(queue) == 0){
		queue->head = queue->tail = new_node;
		new_node->next = NULL;
		new_node->prev = NULL;
	}
	/* existing queue */
	else{
		queue->tail->next = new_node;
		queue->tail = new_node;
		new_node->next = NULL;
		new_node->prev = queue->tail;
	}

	queue->length++;
	return 0;
}
int queue_dequeue(queue_t queue, void **data)
{
	if(queue == NULL || data  == NULL || queue->length == 0)
		return -1;

	*data = queue->head->value;
	node_t *temp = queue->head;
	queue->head->value = NULL;
	queue->head = queue->head->next;
	
	
	if(queue->head != NULL)
		queue->head->prev = NULL;
	//  no more nodes left in  queue
	else
		queue->tail = NULL;

	queue->length--;
	free(temp);
	return 0;
}
int queue_delete(queue_t queue, void *data)
{
	if(queue == NULL || data  == NULL)
		return -1;

	while(queue->head != NULL){
		if(queue->head->value == data){
			
			node_t *temp = queue->head;
			temp->value = NULL;
			temp->prev = queue->head->prev;

			if(temp->next != NULL)
				temp->next = queue->head->next;
	
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
	if(queue == NULL || func == 0)
		return -1;

	/* iterate from head to tail until it reaches the end */
	while(queue->head != NULL){
		func(queue->head->value);
		queue->head = queue->head->next;
	}

	return 0;
}

int queue_length(queue_t queue)
{
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