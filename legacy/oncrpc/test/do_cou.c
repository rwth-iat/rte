#include <stdio.h>
#include <rpc/rpc.h>
#include "cou.h"

main(argc, argv)
int argc;
char *argv[];
{
CLIENT *cl;
void *dummy;
int i, val, final;

rpc_nt_init();

if (argc != 2) {
	fprintf(stderr, "usage: %s host\n", argv[0]);
	exit (1);
	}

while(1){
	
cl = clnt_create(argv[1], COUNTSERV, COUNTVERS, "tcp");

if (cl == NULL) {
	clnt_pcreateerror(argv[1]);
	exit (1);
	}
for (i = 0; i < 10; i++) {
	inc_1(dummy, cl);
	inc_1(dummy, cl);
	val = *dec_1(dummy, cl);
	printf("New value: %d\n", val);
}

val = val + 100;

printf("Set value: %d\n", val);
final = *set_1(&val, cl);
printf("Final value: %d\n", final);

clnt_destroy(cl);
};
rpc_nt_exit();

exit(0);
}
