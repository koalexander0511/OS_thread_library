#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <queue.h>

/* Callback function that increments items by a certain value */
static int inc_item(void *data, void *arg) {
	int *a = (int*)data;
	int inc = (int)(long)arg;
	*a += inc; return 0;
}

/* Callback function that finds a certain item according to its value */
static int find_item(void *data, void *arg) {
	int *a = (int*)data;
	int match = (int)(long)arg;
	if (*a == match) 
		return 1;
	return 0; 
}

void test_create_destroy_enqueue_dequeue_length_delete(void) {

	assert(queue_destroy(NULL) == -1); 
	assert(queue_enqueue(NULL, NULL) == -1);

	queue_t q = queue_create();
	assert(queue_length(q) == 0);
	assert(queue_destroy(q) == 0);

	int data[5] = { 1, 2, 3, 4, 5};

	q = queue_create();
	assert(queue_enqueue(NULL, 0) == -1); 
	assert(queue_enqueue(q, NULL) == -1);
	assert(queue_enqueue(q, data + 0) == 0);

	assert(queue_length(q) == 1);

	
	int **data_ptr = malloc(sizeof(int*));
	assert(queue_dequeue(q, (void**)0) == -1);
	assert(queue_dequeue(0, (void**)data_ptr) == -1);
	assert(queue_dequeue(q, (void**)data_ptr) == 0);
	assert(**(int**)data_ptr == 1);

	assert(queue_length(q) == 0);

	assert(queue_enqueue(q, data + 1) == 0);
	assert(queue_enqueue(q, data + 2) == 0);
	assert(queue_enqueue(q, data + 3) == 0);

	assert(queue_length(q) == 3);

	assert(queue_delete(q, data + 4) == -1);

	assert(queue_delete(q, data + 2) == 0);

	assert(queue_length(q) == 2);

	assert(queue_dequeue(q, (void**)data_ptr) == 0);
	assert(**(int**)data_ptr == 2);

	assert(queue_dequeue(q, (void**)data_ptr) == 0);
	assert(**(int**)data_ptr == 4);

	assert(queue_dequeue(q, (void**)data_ptr) == -1);
}
	
void test_iterator(void) {
	queue_t q;
	int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; 
	int i;
	int *ptr;

	/* Initialize the queue and enqueue items */
	q = queue_create();
	for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
		queue_enqueue(q, &data[i]);

	assert(queue_length(q) == 10);

	/* Add value '1' to every item of the queue */
	queue_iterate(q, inc_item, (void*)1, NULL);
	assert(data[0] == 2);

	/* Find and get the item which is equal to value '5' */
	ptr = NULL;
	queue_iterate(q, find_item, (void*)5, (void**)&ptr); assert(ptr != NULL);

	assert(*ptr == 5);
	assert(ptr == &data[3]);
}


int main(void)
{
	printf("Testing Queue...\n");
	test_create_destroy_enqueue_dequeue_length_delete();
	test_iterator();

	return 0;
}
