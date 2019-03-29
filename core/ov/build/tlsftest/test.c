
#include "libov/ov_config.h"
#include "libov/tlsf.h"
#include <valgrind/memcheck.h>
#include <sys/mman.h>
//#include "valg_dummy.h"

int tlsf_valgrind_check_access(void* mem_pool);

#define POOLSIZE 1024*1024*2
#define LOOP 50
#define USESTACK 0

static void print_vbits(char* bits, size_t length){

	printf("vbits of %p length %lu\n", bits, length);

	for(int i=0; i<length; i++){
		printf("%02X", 0xff&bits[i]);
		if((i%2)==1)
			printf(" ");
	}
	printf("\n");
}

static void check_addable(void* ptr, size_t length){
	VALGRIND_DISABLE_ERROR_REPORTING;
	void* res = (void*)VALGRIND_CHECK_MEM_IS_ADDRESSABLE(ptr, length);
	VALGRIND_ENABLE_ERROR_REPORTING;

	if(!res)
		printf("length is addressable\n");
	else
		printf("only the first %lu bytes are addressable\n", res-ptr);
}

int main () {

#if USESTACK
	char pool[POOLSIZE] = {0};
#else
	char* pool = NULL;
#endif
	char testpool[50];
	void* pb = NULL;
	void* ptr[LOOP];
	int test = 0;
	char buf[2000] = {0};
	int rc = 0;
	char data[] = "lkashdfkashdlgkhaslikdgbhklsadbglksuhadflkhsadlifhvlisahdvilushbdlivbsiadufhiasuhdfiuhsdighuaisdhbgfisauhdfizbxcfgbzxiobufviadugfiaushfoiuhsadfigasgduhsapduhfpsauhdfpouhaspdguhopsaudhgfpsuahfiuhsadifguhpasiudhfpiasudhfpiuahsdgipuhpasduhgfpiasuhdfpiuhasoigduhaspiduhpasuhdfpauhsdpifguhaspdfuhpiasuhdgpiuashdpgfuhaspidfhapsudfhpasudghpihasdfg";

#if ! USESTACK
//	pool = mmap(NULL, POOLSIZE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
//	pool = sbrk(POOLSIZE);
	pool = malloc(POOLSIZE);
#endif

	VALGRIND_MAKE_MEM_NOACCESS(testpool, 50);
	VALGRIND_MAKE_MEM_UNDEFINED(testpool,5);
	VALGRIND_MAKE_MEM_DEFINED(testpool, 3);

	rc = VALGRIND_GET_VBITS(testpool, buf, 5);

	printf("get vbits returned %i\n", rc);

	print_vbits(buf, 6);

	check_addable(testpool, 5);
	check_addable(testpool, 7);

	VALGRIND_MAKE_MEM_UNDEFINED(pool, POOLSIZE);

	init_memory_pool(POOLSIZE, pool);

	tlsf_set_pool(ov_heap, pool);

	for(int i=0; i<LOOP; i++){
		ptr[i] = tlsf_malloc(128, ov_heap);
	}


	pb=ptr[20];
	ptr[20] = tlsf_malloc(112, ov_heap);
	memcpy(ptr[20], data, 111);
	*(char*)(ptr[20]+111) = 0;
	ptr[20] = tlsf_realloc(ptr[20], 200, ov_heap);

	ptr[1] = tlsf_realloc(ptr[1], 64, ov_heap);

//	VALGRIND_MAKE_MEM_NOACCESS((ptr[6])+120, 8);
//
//	printf("access length: %li\n", (long) (VALGRIND_CHECK_MEM_IS_ADDRESSABLE(ptr[6],128) - (long) ptr[6]));

	ptr[6] = tlsf_realloc(ptr[6], 250, ov_heap);

	tlsf_free(ptr[12], ov_heap);
	ptr[12] = NULL;
	ptr[11] = tlsf_realloc(ptr[11], 160, ov_heap);

	tlsf_free(ptr[17], ov_heap);
	ptr[17] = NULL;
	ptr[16] = tlsf_realloc(ptr[16], 64, ov_heap);

	for(int i=0; i<LOOP; i++){
		for(int j=0; j<128; j++){
			if (((char*) ptr[i])+j == 0)
				(void) i;
		}
	}

	for(int i=0; i<LOOP; i++){
		tlsf_free(ptr[i], ov_heap);
		ptr[i] = NULL;
	}

	tlsf_valgrind_check_access(pool);

	get_used_size(pool);
	get_free_size(pool);
	get_max_size(pool);
	get_fragmentation(pool);

	destroy_memory_pool(pool);

	return 0;
}
