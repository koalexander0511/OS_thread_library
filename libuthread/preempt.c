#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include "preempt.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

float usec = 1000000/HZ;
sigset_t block_alarm;

void preempt_disable(void)
{
	sigprocmask (SIG_BLOCK, &block_alarm, NULL);
}

void preempt_enable(void)
{
	sigprocmask (SIG_UNBLOCK, &block_alarm, NULL);
}

void alarmHandler (int signum)
{
	uthread_yield();
}

void preempt_start()
{
	sigemptyset (&block_alarm);
	sigaddset (&block_alarm, SIGVTALRM);

	struct sigaction sa;
	struct itimerval timer;

	sa.sa_handler = &alarmHandler;
	sigemptyset (&sa.sa_mask);

	sa.sa_flags = 0;
	sigaction (SIGVTALRM, &sa, NULL);


	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = usec;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = usec;
	 
	setitimer (ITIMER_VIRTUAL, &timer, NULL);

}
