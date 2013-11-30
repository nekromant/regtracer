#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void print_bitdiff(uint32_t a, uint32_t b);


void usage()
{
	printf("Simple bitdiff utility\n");
	printf("Usage: bitdiff a b\n");
	exit(1);
}
int main(int argc, char* argv[])
{
	uint32_t a,b;
	if (argc < 3)
		usage();
	sscanf(argv[1], "0x%x", &a);
	sscanf(argv[2], "0x%x", &b);
	print_bitdiff(a,b);
	return 0;
}

