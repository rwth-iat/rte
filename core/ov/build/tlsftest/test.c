
#include "libov/ov_config.h"
#include "libov/tlsf.h"
#include "memcheck.h"

#define POOLSIZE 1024*1024

int main () {

	char pool[POOLSIZE] = {0};
	void* pb = NULL;

	init_memory_pool(POOLSIZE, &pool);

	tlsf_set_pool(ov_heap, &pool);

	pb = tlsf_malloc(20, ov_heap);

	memset(pb, 0, 26);

	tlsf_free(pb, ov_heap);

	pb = malloc(20);

	*((int*)pb) = 20;

	VALGRIND_MAKE_MEM_NOACCESS(pb, 20);

	*((int*)pb) = 5;

	VALGRIND_MAKE_MEM_DEFINED(pb, 20);

	*((int*)pb) = 6;

	free(pb);

	return 0;
}
