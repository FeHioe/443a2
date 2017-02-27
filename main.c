#include "merge.h"
int phase1(char *filename, int total_mem, int block_size);

int main (int argc, char **argv) {
	//process and validate command-line arguments
	
	char *filename = argv[1];
	int total_mem = atoi(argv[2]);
	int block_size = atoi(argv[3]);
	
	MergeManager manager;	

	int sublist_num = phase1(filename, total_mem, block_size);

	printf("number of sublist: %d\n", sublist_num);

	return merge_runs (&manager, total_mem, block_size, sublist_num);
}
