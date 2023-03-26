#include "header.hpp"

void init()
{
	int size = 4096;
	const char *name = "/mysm";
	int fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	ftruncate(fd, size);
	void *ptr = (int *)mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);

	munmap(ptr, size);
}
