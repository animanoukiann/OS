#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>


int main()
{
	int pipe_fd[2];
	pid_t ID;

	if (pipe(pipe_fd) != 0)
	{
		perror("Cannot create pipe!");
		exit(EXIT_FAILURE);
	}

	pid_t p1 = fork();
	if (p1 == -1)
	{
		perror("Error while forking");
		exit(EXIT_FAILURE);
	}
	else if (p1 == 0)
	{
		close(pipe_fd[0]);
		ID = getpid();
		int written_bytes = write(pipe_fd[1], &ID, sizeof(ID));
		if (written_bytes == -1)
		{
			perror("Cannot write!");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		p1 = fork();
		if (p1 == -1)
		{
			perror("Error while forking");
			exit(EXIT_FAILURE);
		}
		else if (p1 == 0)
		{
			close(pipe_fd[1]);
			int read_bytes = read(pipe_fd[0], &ID, sizeof(ID));
			if (read_bytes == -1)
			{
				perror("Cannor read!");
				exit(EXIT_FAILURE);
			}
			printf("The ID of process N2 is %d\n", ID);
		}
	}
	wait(NULL);
	wait(NULL);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}

