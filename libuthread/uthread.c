#define _XOPEN_SOURCE 600

#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "context.h"
#include "preempt.h"
#include "queue.h"
#include "uthread.h"

enum State {running, done, ready, blocked};
uthread_t THREAD_ID_COUNT = 0;

struct uthread_tcb {
	uthread_t id;
	int state;
	uthread_ctx_t *context;
	void* stack_addr;
	int retval;
};

queue_t thread_queue;
queue_t zombies;
struct uthread_tcb *curr_thread;

void uthread_yield(void)
{	
	
	struct uthread_tcb *prev;

	//If no other thread is waiting to be executed, just keep executing this thread
	if(queue_length(thread_queue) == 0)
		return;
	if(curr_thread->state == running)
		curr_thread->state = ready;

	if(curr_thread->state == done)
	{	
		/*DEBUGMODE*/printf("Thread %d dead, put into zombie queue...\n", curr_thread->id);
		queue_enqueue(zombies, (void*)curr_thread);
	}
	else
		queue_enqueue(thread_queue, (void*)curr_thread);

	prev = curr_thread;

	queue_dequeue(thread_queue, (void**)&curr_thread);

	/*DEBUGMODE*/printf("Thread %d yielding to thread %d...\n",prev->id,curr_thread->id);

	if(curr_thread->state == ready)
		curr_thread->state = running;
	
	uthread_ctx_switch(prev->context, curr_thread->context);
}

uthread_t uthread_self(void)
{
	return curr_thread->id;
}

//sets main() as the main thread
void create_main_thread(){
	zombies = queue_create();
	thread_queue = queue_create();

	struct uthread_tcb *main_thread = malloc (sizeof(struct uthread_tcb));
	main_thread->context = malloc(sizeof(uthread_ctx_t));

	main_thread->state = running;
	main_thread->id = THREAD_ID_COUNT;

	curr_thread = main_thread;

	THREAD_ID_COUNT++;
}

int uthread_create(uthread_func_t func, void *arg)
{
	if (func == NULL)
		return -1;

	if(THREAD_ID_COUNT == 0)
		create_main_thread();

	struct uthread_tcb *thread = malloc (sizeof(struct uthread_tcb));
	thread->context = malloc(sizeof(uthread_ctx_t));

	thread->stack_addr = uthread_ctx_alloc_stack();
	uthread_ctx_init(thread->context, thread->stack_addr, func, arg);

	thread->state = ready;
	thread->id = THREAD_ID_COUNT;
	
	queue_enqueue(thread_queue, (void*)thread);

	THREAD_ID_COUNT++;
	return thread->id;
}

void uthread_exit(int retval)
{
	/*DEBUGMODE*/printf("Thread %d exiting...\n",curr_thread->id);
	curr_thread->state = done;
	curr_thread->retval = retval;
	uthread_yield();
}

// Callback function that finds thread according to its tid
static int find_tcb(void *data, void *args) {
	struct uthread_tcb * tcb = (struct uthread_tcb*)data;
	uthread_t * tid = (uthread_t*)args;
	if(tcb->id == *tid)
		return 1;

	return 0; 
}

int uthread_join(uthread_t tid, int *retval)
{
	/*DEBUGMODE*/printf("Thread %d called join to get thread %d...\n",curr_thread->id,tid);
	//exit with error if trying to join main thread, or with itself
	if(tid == 0 || tid == curr_thread->id)
		return -1;

	/*DEBUGMODE*/printf("Thread %d looking for thread %d...\n",curr_thread->id,tid);
	struct uthread_tcb * target = NULL;
	queue_iterate(zombies, find_tcb, (void*)&tid, (void**)&target);
	
	//keep pushing this thread in the queue with blocked state while the child thread is running
	while(target == NULL)
	{	
		curr_thread->state = blocked;
		uthread_yield();
		/*DEBUGMODE*/printf("Thread %d looking for thread %d...\n",curr_thread->id,tid);
		queue_iterate(zombies, find_tcb, (void*)&tid, (void**)&target);
	}

	/*DEBUGMODE*/printf("Thread %d found thread %d...\n",curr_thread->id,tid);

	//child is done executing, time to get some shit done
	*retval = target->retval;
	uthread_ctx_destroy_stack(target->stack_addr);
	queue_delete(zombies, target);


	curr_thread->state = running;
	uthread_yield();
	return 0;
}
