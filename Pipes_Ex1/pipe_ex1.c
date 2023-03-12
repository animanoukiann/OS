#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>


int main()
{
 int pipe_fd_1[2];
 int pipe_fd_2[2];
 pid_t ID_1, ID_2;
 int written_bytes;
 int read_bytes;

 if (pipe(pipe_fd_1) != 0)
 {
  perror("Cannot create pipe!");
  exit(EXIT_FAILURE);
 }

 if (pipe(pipe_fd_2) != 0)
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
  close(pipe_fd_1[0]);
  close(pipe_fd_2[1]);
  ID_1 = getpid();
  printf("The real ID of process N1 is %d\n", ID_1);
  int written_bytes = write(pipe_fd_1[1], &ID_1, sizeof(ID_1));
  if (written_bytes == -1)
  {
   perror("Cannot write!");
   exit(EXIT_FAILURE);
  }
  read_bytes = read(pipe_fd_2[0], &ID_2, sizeof(ID_2));
  if (read_bytes == -1)
  {
   perror("Cannot read!");
   exit(EXIT_FAILURE);
  }
  printf("process N2 inside process N1 is %d\n", ID_2);
  return (0);
 }
  pid_t p2 = fork();
  if (p2 == -1)
  {
   perror("Error while forking");
   exit(EXIT_FAILURE);
  }
  else if (p2 == 0)
  {
   close(pipe_fd_1[1]);
   close(pipe_fd_2[0]);
   ID_2 = getpid();
   printf("The real ID of process N2 is %d\n", ID_2);
   written_bytes = write(pipe_fd_2[1], &ID_2, sizeof(ID_2));
   if (written_bytes == -1)
   {
    perror("Cannot write!");
    exit(EXIT_FAILURE);
   }
   read_bytes = read(pipe_fd_1[0], &ID_1, sizeof(ID_1));
   if (read_bytes == -1)
   {
    perror("Cannot read!");
    exit(EXIT_FAILURE);
   }
   printf("process N1 inside process N2 is %d\n", ID_1);
   return (0);
  }
  wait(NULL);
  wait(NULL);
}
