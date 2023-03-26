#include "header.hpp"

void print(int fd, const char *name)
{
	int size = 4096;
	void *ptr = (int *)mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	int count = 0;
	while (1)
	{
		std::cout << (char *)ptr << std::endl;
		sleep(5);
	}
	close(fd);
	munmap(ptr, size);
}

