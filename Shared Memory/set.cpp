#include "header.hpp"

const int BUFFER_SIZE = 1024;

void set()
{
    int size = 4096;
    const char *name = "/myshm";
    int fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, size);
    void *ptr = (int *)mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    char buffer[BUFFER_SIZE];

    while (true) 
    {
	    int bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
	    if (bytes_read <= 0)
		    break;
	    if (bytes_read > size)
	    {
		    std::cerr << "Error: code has failed." << std::endl;
		    exit(EXIT_FAILURE);
	    }
	    std::memcpy(ptr, buffer, bytes_read);
	    std::memset((char *)ptr + bytes_read, 0, size - bytes_read);
	    sleep(5);
    }
    munmap(ptr, size);
    destroy(name);
}
