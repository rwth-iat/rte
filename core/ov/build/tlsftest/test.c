
#include "libov/ov_config.h"
#include "libov/tlsf.h"
#include <valgrind/memcheck.h>
#include <sys/mman.h>
//#include "valg_dummy.h"


#define POOLSIZE 1024*1024
#define LOOP 2
#define USESTACK 0

int main () {

#if USESTACK
	char pool[POOLSIZE] = {0};
#else
	char* pool = NULL;
#endif
	void* pb = NULL;
	void* ptr[200];
	char bits[50] = {0};
	int test = 0;
	char buf[2000];

#if ! USESTACK
	pool = mmap(NULL, POOLSIZE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
#endif

	init_memory_pool(POOLSIZE, &pool);

	tlsf_set_pool(ov_heap, &pool);

	pb = tlsf_malloc(20, ov_heap);

	for(int i=0; i<LOOP; i++){
		ptr[i] = tlsf_malloc((i*351+13531)%1000, ov_heap);
		//memcpy(buf, ptr[i], 2000);
		memset(ptr[i], 0xa5, 2000);
	}
	for(int i=0; i<LOOP; i++){
		if(i%2)
		tlsf_free(ptr[i], ov_heap);
	}

	destroy_memory_pool(pool);

	return 0;
}
