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
#include <unistd.h>

int retval;

int thread3(void* arg)
{
	printf("I am Thread 3 and I will yield!\n");
	uthread_yield();
	
	uthread_exit(0);

	return 0;
}

int thread2(void* arg)
{
	printf("I am Thread 2 and I will yield!\n");
	uthread_yield();
	
	uthread_exit(0);
	return 0;
}

int thread1(void* arg)
{
	while(1)

	return 0;
}

int main(void)
{
	uthread_t id2,id3;

	uthread_create(thread1, NULL);
	id2 = uthread_create(thread2, NULL);
	id3 = uthread_create(thread3, NULL);


	
	
	uthread_join(id3, &retval);
	uthread_join(id2, &retval);
	printf("Success!\nI'm not gonna collect thread 1 because he's an asshole!\n");

	return 0;
}
