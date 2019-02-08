#include <stdio.h>
#include <stdlib.h>
#include <uthread.h>

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
	for(int i=0; ;i++)
	{
		printf(".");
	}
	return 0;
}
