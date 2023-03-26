#include "header.hpp"

void destroy(const char *name)
{
	shm_unlink(name);
}
