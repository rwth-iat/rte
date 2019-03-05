
#include "libov/ov_config.h"
#include "libov/tlsf.h"
#include <valgrind/memcheck.h>
#include <sys/mman.h>
//#include "valg_dummy.h"

int tlsf_valgrind_check_access(void* mem_pool);

#define POOLSIZE 1024*50
#define LOOP 300
#define USESTACK 0

int main () {

#if USESTACK
	char pool[POOLSIZE] = {0};
#else
	char* pool = NULL;
#endif
	void* pb = NULL;
	void* ptr[LOOP];
	char bits[50] = {0};
	int test = 0;
	char buf[2000];

#if ! USESTACK
	//pool = mmap(NULL, POOLSIZE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	pool = malloc(POOLSIZE);
#endif

	VALGRIND_MAKE_MEM_UNDEFINED(pool, POOLSIZE);

	init_memory_pool(POOLSIZE, pool);

	tlsf_set_pool(ov_heap, pool);

	pb = tlsf_malloc(20, ov_heap);

	for(int i=0; i<LOOP; i++){
		ptr[i] = tlsf_malloc((i*351+13531)%1000, ov_heap);
		//memcpy(buf, ptr[i], 2000);
		memset(ptr[i], 0xa5, (i*351+13531)%1000);
	}
	for(int i=0; i<LOOP; i++){
		if(i%7==0){
			tlsf_free(ptr[i], ov_heap);
			ptr[i] = NULL;
		}
	}

	for(int i=0; i<LOOP; i++){
		if(i%3==0 && !ptr[i]){
			tlsf_malloc((i*753+973)%1000, ov_heap);
		}
	}

	for(int i=0; i<LOOP; i++){
		if(i%7){
			tlsf_free(ptr[i], ov_heap);
			ptr[i] = NULL;
		}
	}

	for(int i=0; i<LOOP; i++){
		if(i%3){
			tlsf_realloc(ptr[i], (i*6513+651)%1020, ov_heap);
			ptr[i] = NULL;
		}
	}

	tlsf_valgrind_check_access(pool);

	get_used_size(pool);
	get_free_size(pool);
	get_max_size(pool);
	get_fragmentation(pool);

	destroy_memory_pool(pool);

	return 0;
}
