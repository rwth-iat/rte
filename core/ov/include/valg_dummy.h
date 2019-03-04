#ifndef __valgdummy
#define __valgdummy

#ifndef COMPILE
#define VALGRIND_MAKE_MEM_NOACCESS(_qzz_addr,_qzz_len)
#define VALGRIND_MAKE_MEM_UNDEFINED(_qzz_addr,_qzz_len)
#define VALGRIND_MAKE_MEM_DEFINED(_qzz_addr,_qzz_len)
#define VALGRIND_MAKE_MEM_DEFINED_IF_ADDRESSABLE(_qzz_addr,_qzz_len)
#define VALGRIND_CREATE_BLOCK(_qzz_addr,_qzz_len, _qzz_desc)
#define VALGRIND_DISCARD(_qzz_blkindex)
#define VALGRIND_CHECK_MEM_IS_ADDRESSABLE(_qzz_addr,_qzz_len)
#define VALGRIND_CHECK_MEM_IS_DEFINED(_qzz_addr,_qzz_len)
#define VALGRIND_CHECK_VALUE_IS_DEFINED(__lvalue)
#define VALGRIND_DO_LEAK_CHECK
#define VALGRIND_DO_ADDED_LEAK_CHECK
#define VALGRIND_DO_CHANGED_LEAK_CHECK
#define VALGRIND_DO_QUICK_LEAK_CHECK
#define VALGRIND_COUNT_LEAKS(leaked, dubious, reachable, suppressed)
#define VALGRIND_COUNT_LEAK_BLOCKS(leaked, dubious, reachable, suppressed)
#define VALGRIND_GET_VBITS(zza,zzvbits,zznbytes)
#define VALGRIND_SET_VBITS(zza,zzvbits,zznbytes)
#define VALGRIND_DISABLE_ADDR_ERROR_REPORTING_IN_RANGE(_qzz_addr,_qzz_len)
#define VALGRIND_ENABLE_ADDR_ERROR_REPORTING_IN_RANGE(_qzz_addr,_qzz_len)
#endif

#endif
