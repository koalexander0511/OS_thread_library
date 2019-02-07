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

int retval;

int thread3(void* arg)
{
	printf("\n-----------------------------------\n");
	printf("\nTHREAD %d TYPE 3 EXECUTING....\n", uthread_self());
	printf("\n-----------------------------------\n");
	uthread_exit(3);
	return 0;
}

int thread2(void* arg)
{
	uthread_join(uthread_create(thread3, NULL),&retval);
	printf("\n-----------------------------------\n");
	printf("\nTHREAD %d TYPE 2 EXECUTING....\n", uthread_self());
	printf("\n-----------------------------------\n");
	uthread_exit(20+retval);
	return 0;
}

int thread1(void* arg)
{
	uthread_join(uthread_create(thread2, NULL),&retval);
	printf("\n-----------------------------------\n");
	printf("\nTHREAD %d TYPE 1 EXECUTING....\n", uthread_self());
	printf("\n-----------------------------------\n");
	uthread_exit(100+retval);
	return 0;
}

int main(void)
{
	//thread1(): creates thread2(), and thread2() creates thread3(). The execution order should be 3->2->1
	//thread2(): creates thread3(). The execution order should be 3->2

	uthread_join(uthread_create(thread1, NULL), &retval);
	printf("thread1() @ main() done: Retval --> %d\n",retval);
	uthread_join(uthread_create(thread2, NULL), &retval);
	printf("thread2() @ main() done: Retval --> %d\n",retval);
	uthread_join(uthread_create(thread3, NULL), &retval);
	printf("thread3() @ main() done: Retval --> %d\n",retval);

	return 0;
}
