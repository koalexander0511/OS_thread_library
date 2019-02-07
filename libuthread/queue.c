#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

typedef struct node {
	struct node *next;
	void *data;
} node;

struct queue {
	node *head, *tail;
	int length;
};

queue_t queue_create(void)
{
	queue_t queue = malloc(sizeof(struct queue));
	struct queue q = { NULL, NULL, 0 };
	*queue = q;
	return queue;
}

int queue_destroy(queue_t queue)
{
	if(queue == NULL || queue->length > 0)
		return -1;
	
	free(queue);
	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
	if(queue == NULL || data == NULL)
		return -1;

	node* item = malloc(sizeof(node));
	struct node i = { NULL, data };
	*item = i;

	if(queue->length == 0) {
		queue->head = item;
		queue->tail = item;
	} 
	else {
		queue->tail->next = item;
		queue->tail = item;
	}

	queue->length++;
	
	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	if(queue == NULL || data == NULL || queue->length == 0)
		return -1;
	
	*data = queue->head->data;

	queue->head = queue->head->next;
	queue->length--;

	return 0;
}

int queue_delete(queue_t queue, void *data)
{
	if(queue == NULL || data == NULL || queue->length == 0)
		return -1;

	if(queue->head->data == data) {
		queue->head = queue->head->next;
		queue->length--;
		return 0;
	} 

	node* curr = queue->head;

	while(curr->next != NULL) {
		if(curr->next->data == data) {
			node* toDelete = curr->next;
			curr->next = toDelete->next;

			free(toDelete);
			queue->length--;

			return 0;
		}
	
		else
			curr = curr->next;
	}

	return -1;
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	if(queue == NULL || func == NULL)
		return -1;

	node* curr = queue->head;
	while(curr != NULL) {
		if(func(curr->data, arg)) {
			if(data != NULL) {
				*data = curr->data;
			}
			break;
		}
		curr = curr->next;
	}

	return 0;
}

int queue_length(queue_t queue)
{
	return queue->length;
}