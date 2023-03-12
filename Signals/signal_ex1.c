#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


int main()
{
	pid_t p = fork();
	if (p == -1)
	{
		perror("Error while forking");
		exit(EXIT_FAILURE);
	}
	else if (p == 0)
	{
		int ppid = getppid();
		if (kill(ppid, SIGKILL) == -1)
		{
			perror("KILL");
			exit(EXIT_FAILURE);
		}
		printf("Child process killed parent process\n");
	}
	else
	{
		wait(NULL);
	}

}
