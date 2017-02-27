#include "merge.h"

/*
//manager fields should be already initialized in the caller
int merge_runs (MergeManager * merger){	
	int  result; //stores SUCCESS/FAILURE returned at the end	
	
	//1. go in the loop through all input files and fill-in initial buffers
	if (init_merge (merger)!=SUCCESS)
		return FAILURE;

	while (merger->current_heap_size > 0) { //heap is not empty
		HeapElement smallest;
		Record next; //here next comes from input buffer
		
		if(get_top_heap_element (merger, &smallest)!=SUCCESS)
			return FAILURE;

		result = get_next_input_element (merger, smallest.run_id, &next);
		
		if (result==FAILURE)
			return FAILURE;

		if(result==SUCCESS) {//next element exists, may also return EMPTY
			if(insert_into_heap (merger, smallest.run_id, &next)!=SUCCESS)
				return FAILURE;
		}		


		merger->output_buffer [merger->current_output_buffer_position].UID1=smallest.UID1;
		merger->output_buffer [merger->current_output_buffer_position].UID2=smallest.UID2;
		
		merger->current_output_buffer_position++;

        //staying on the last slot of the output buffer - next will cause overflow
		if(merger->current_output_buffer_position == merger-> output_buffer_capacity ) {
			if(flush_output_buffer(merger)!=SUCCESS) {
				return FAILURE;			
				merger->current_output_buffer_position=0;
			}	
		}
	
	}

	
	//flush what remains in output buffer
	if(merger->current_output_buffer_position > 0) {
		if(flush_output_buffer(merger)!=SUCCESS)
			return FAILURE;
	}
	
	clean_up(merger);
	return SUCCESS;	
}
*/

int get_top_heap_element (MergeManager * merger, HeapElement * result){
	HeapElement item;
	int child, parent;

	if(merger->current_heap_size == 0){
		printf( "UNEXPECTED ERROR: popping top element from an empty heap\n");
		return FAILURE;
	}

	*result=merger->heap[0];  //to be returned

	//now we need to reorganize heap - keep the smallest on top
	item = merger->heap [--merger->current_heap_size]; // to be reinserted 

	parent =0;
	while ((child = (2 * parent) + 1) < merger->current_heap_size) {
		// if there are two children, compare them 
		if (child + 1 < merger->current_heap_size && 
				(compare_heap_elements(&(merger->heap[child]),&(merger->heap[child + 1]))>0)) 
			++child;
		
		// compare item with the larger 
		if (compare_heap_elements(&item, &(merger->heap[child]))>0) {
			merger->heap[parent] = merger->heap[child];
			parent = child;
		} 
		else 
			break;
	}
	merger->heap[parent] = item;
	
	return SUCCESS;
}

int insert_into_heap (MergeManager * merger, int run_id, Record *input){

	HeapElement new_heap_element;
	int child, parent;

	new_heap_element.UID1 = input->UID1;
	new_heap_element.UID2 = input->UID2;
	new_heap_element.run_id = run_id;
	
	if (merger->current_heap_size == merger->heap_capacity) {
		printf( "Unexpected ERROR: heap is full\n");
		return FAILURE;
	}
  	
	child = merger->current_heap_size++; /* the next available slot in the heap */
	
	while (child > 0) {
		parent = (child - 1) / 2;
		if (compare_heap_elements(&(merger->heap[parent]),&new_heap_element)>0) {
			merger->heap[child] = merger->heap[parent];
			child = parent;
		} 
		else 
			break;
	}
	merger->heap[child]= new_heap_element;	
	return SUCCESS;
}


/*
** TO IMPLEMENT
*/

int init_merge (MergeManager * manager, int total_mem, int block_size, int sublist_num) {
	int i;

	// Initalize struct values
	manager->heap = (HeapElement *) malloc (sublist_num);

	manager->input_file_numbers = (int *) malloc (sublist_num);
	for (i=0; i < sublist_num; i++){
		manager->input_file_numbers[i] = i;
	};

	manager->output_buffer = (Record *) calloc (block_size/sizeof(Record), sizeof(Record));
	manager->curent_ouput_buffer_position = 0;
	//manager->output_buffer_capacity = ;
	
	manager->input_buffers = (Record **) malloc(sublist_num * sizeof(Record *));
	printf("input buf assigned\n");	
	strcpy(manager->input_prefix, "sublist");
	
	
	FILE *fp_read;
	int blocks_per_mem = total_mem/block_size;	
	
	printf("output buf assigned\n");	
	
	blocks_per_mem = blocks_per_mem - 1;
	int blocks_per_input_buf = blocks_per_mem / sublist_num;	

	if (blocks_per_input_buf < 1 ) {
		printf("Error: must have one block per input buffer.");
		exit(1);
	}; 	
	
	char str[1024];
	
	manager->current_input_file_positions = (int *) malloc(sublist_num * sizeof(int *));
	printf("current assigned\n");	
	
	for (i=0; i < sublist_num; i++){
		manager->input_buffers[i] = (Record *)malloc(blocks_per_input_buf * block_size);
		printf("input %d assigned\n", i);	
		
		sprintf(str, "sublist%d.dat", i);
		if (!(fp_read = fopen(str, "rb"))) {
    		printf("Error: could not open file for read.\n");
    		exit(1);
  		};
  	
  		manager->current_input_file_positions[i] = fread(manager->input_buffers[i], sizeof(Record), (blocks_per_input_buf * block_size)/sizeof(Record), fp_read);
		printf("read\n");		
	};	
	
	printf("first current_input_pos: %d\n", manager->current_input_file_positions[0]);
	for(i=0; i< (blocks_per_input_buf * block_size)/sizeof(Record)  ;i++){
		printf("printing index %d of first input buffer: %d\n", i, manager->input_buffers[0][i].UID2);
	};

	return SUCCESS;
}

int flush_output_buffer (MergeManager * manager) {
	return SUCCESS;
}

int get_next_input_element(MergeManager * manager, int file_number, Record *result) {
	return SUCCESS;
}

int refill_buffer (MergeManager * manager, int file_number) {
	return SUCCESS;
}

void clean_up (MergeManager * merger) {
	
}

int compare_heap_elements (HeapElement *a, HeapElement *b) {
	return 0;
}
