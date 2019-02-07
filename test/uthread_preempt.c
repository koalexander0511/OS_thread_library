/*
 * Thread creation and yielding test
 *
 * Tests the creation of multiples threads and the fact that a parent thread
 * should get returned to before its child is executed. The way the printing,
 * thread creation and yielding is done, the program should output:
 *
 * thread1
 * thread2
 * thread3
 */

#include <stdio.h>
#include <stdlib.h>
#include <uthread.h>
#include "preempt.h"

int retval;

int thread3(void* arg)
{
	while(1);

	uthread_exit(3);
	return 0;
}

int thread2(void* arg)
{
	while(1);

	uthread_exit(20+retval);
	return 0;
}

int thread1(void* arg)
{
	while(1);

	uthread_exit(100+retval);
	return 0;
}

int main(void)
{
	uthread_t id1,id2,id3;

	preempt_start();

	id1 = uthread_create(thread1, NULL);
	id2 = uthread_create(thread2, NULL);
	id3 = uthread_create(thread3, NULL);

	while(1);

	uthread_join(id1, &retval);
	uthread_join(id2, &retval);
	uthread_join(id3, &retval);

	return 0;
}
