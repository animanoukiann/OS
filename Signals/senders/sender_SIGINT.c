#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>

int main()
{
	int pid;
	scanf("%d", &pid);
	if (kill(pid, SIGINT) == -1)
	{
		perror("KILL");
		exit(EXIT_FAILURE);
	}
}
