#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void handler(int num)
{
	static int counter = 0;
	if (num == SIGUSR2)
		counter++;
	if (num == SIGINT)
	{
		printf("The total number of SIGUSR2 signal is  %d\n", counter);
		exit(0);
	}
}

int main()
{
	signal(SIGUSR2, handler);
	signal(SIGINT, handler);
	signal(SIGUSR1, SIG_IGN);
	while(1)
	{
		printf("I am still alive\n");
		sleep(5);
	}
}
