#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>

void destroy(const char* name);
void init();
void print(int fd, const char *name);
void set();
