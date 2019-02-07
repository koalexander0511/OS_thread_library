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

int threadC(void* arg)
{
	printf("\n-----------------------------------\n");
	printf("\nTHREAD %d TYPE C EXECUTING....\n", uthread_self());
	printf("\n-----------------------------------\n");
	
	uthread_exit(uthread_self());
	return 0;
}

int threadB(void* arg)
{
	printf("\n-----------------------------------\n");
	printf("\nTHREAD %d TYPE B EXECUTING....\n", uthread_self());
	printf("\n-----------------------------------\n");
	
	uthread_exit(uthread_self());
	return 0;
}

int threadA(void* arg)
{
	printf("\n-----------------------------------\n");
	printf("\nTHREAD %d TYPE A EXECUTING....\n", uthread_self());
	printf("\n-----------------------------------\n");
	
	uthread_exit(uthread_self());
	return 0;
}

int main(void)
{
	preempt_start();

	for(int i=0;;i++)
	{
		printf("%d ",count);
		if(count == 50)
		{
			preempt_disable();
			preempt_enable();
		}
	}
	return 0;
}
