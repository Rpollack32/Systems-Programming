#include <stdio.h>
#include "mymalloc.h"

typedef struct{
	int size;		  // size of the block	
	int prev_size;		  // size of the previous block
	int is_allocated;	  // block is allocated: NO: 0  YES: 1
	int is_last;		  // block is the last block: NO: 0  YES: 1
}Metadata;

void initialize();
Metadata *next(Metadata *cur);    // Metadata structure repesentating the next block of memory
Metadata *prev(Metadata *cur);	  // Metadata structure repesentating the previous block of memory

static char myblock[MEMORY_SIZE]; // An array that simulates virtual memory

Metadata *first_block = (Metadata*)&myblock[0];  // pointer to the first metadata block of memory

/**
 * Replaces the existing malloc() function
 *
 * parameters:
 *  size_t req_size: Requested size of the memory block
 *  char* file_name:  File name that invoked this method
 *  int line_number:    The line number of the calling file name
 *
 * returns:
 *  A pointer to the allocated memory block or NULL if an error condition is encountered. 
 *
 */
void *mymalloc(size_t req_size,const char* file_name, int line_number){

	// Check to see if the request memory allocation is larger than what is available, if so return an error
	if(req_size + sizeof(Metadata) > MEMORY_SIZE){
		printf("Error in %s, line %d: Not enough space to allocate memory\n", file_name, line_number);
		return NULL;
	}

	// If the requested size is 0 return an error
	if ( req_size == 0 ) {
		printf("Error in %s, line %d: Requested memory allocation must be at least 1 byte\n", file_name, line_number);
		return NULL;
        }
	
	Metadata* cur = first_block; //Start from the first metadata block

	// Check to see if the request block can be allocated within the virtual memory
	while(!(cur < first_block || (char *)cur > &myblock[MEMORY_SIZE-1])){
		// If current  block is not allocated and if it fits, allocate the meeoy
		if(!cur->is_allocated && cur->size>=req_size){
			int old_size = cur->size;
			int old_is_last = cur->is_last;
			cur->is_allocated = 1;
			Metadata* nxt = next(cur);			

			//if the space left is enough for one block
			if((cur->size - req_size) >= (sizeof(Metadata)+1)) {
				cur->size = req_size;
				Metadata* new_metadata;
				char* temp = (char*)cur;
				temp += (sizeof(Metadata)) / sizeof(char);
				temp += cur->size;
				new_metadata = (Metadata*)temp;

				//set attributes for new metadata
				new_metadata->is_allocated = 0;
				new_metadata->size = old_size-sizeof(Metadata)-cur->size;
				new_metadata->is_last = old_is_last;
				cur->is_last = 0;
				new_metadata->prev_size = cur->size;
				nxt->prev_size = new_metadata->size;
			}
			char* ret;
			ret = (char*)cur;
			ret += sizeof(Metadata)/sizeof(char);
			return ret;
		}else if(cur->is_last){
			// Could not find space to allocate
			printf("Error in %s, line %d: Not enough space to allocate requested memory\n", file_name, line_number);
			return NULL;
		}else{
			//move to the next metadata bloack
			cur = next(cur);
		}
	}
}

/**
 * Replaces the existing free() function
 *
 * parameters
 *  void* ptr:   Pointer to the address of the memory block to be freed
 *  char* file_name:  File name that invoked this method
 *  int line_number:    The line number of the calling file name
 * 
 * returns:
 *  	nothing
 */
void myfree(void* ptr, const char* file_name, int line_number){

	// Check that the pointer is NULL, if it is return an error 
	if(ptr == NULL){
		printf("Error in %s, line %d: Cannot free a Null pointer.\n", file_name, line_number);
		return;
	}

	Metadata* meta_ptr = (Metadata*)(ptr - sizeof(Metadata));
	if(meta_ptr < first_block || (char *)ptr > &myblock[MEMORY_SIZE-1]){
		printf("Error in %s, line %d: Not a valid pointer.\n", file_name, line_number);
		return;
	}
	
	// If the pointer is not alloacted return an error
	if(!meta_ptr->is_allocated){
		printf("Error in %s, line %d: Can not free unallocated memory.\n", file_name, line_number);
		return;
	}
	
	meta_ptr->is_allocated = 0;
	int new_size;
	
	//merge right
	Metadata* nxt = next(meta_ptr);
	if(meta_ptr->is_last == 0 && nxt->is_allocated == 0){
		new_size = meta_ptr->size + nxt->size + sizeof(Metadata);
		meta_ptr->size = new_size;
		meta_ptr->is_last = nxt->is_last;

		if(!meta_ptr->is_last){
			Metadata* new_nxt = next(meta_ptr);
			new_nxt->prev_size = meta_ptr->size;
		}
	}
			
	//merge left
	Metadata* prv = prev(meta_ptr);
	if(meta_ptr != first_block && prv->is_allocated == 0){
		prv->size += meta_ptr->size + sizeof(Metadata);
		prv->is_last = meta_ptr->is_last;
		if(!meta_ptr->is_last){
			Metadata* new_nxt = next(meta_ptr);
			new_nxt->prev_size = prv->size;
		}
	}
}

// Set up the initial metadata block of memory
void initialize(){
	Metadata* initial_metadata = first_block;
	initial_metadata->size = MEMORY_SIZE - sizeof(Metadata);
	initial_metadata->prev_size = 0;
	initial_metadata->is_allocated = 0;
	initial_metadata->is_last = 1;
}

//Return the metadata pointer of the next block	
//Check if is_last is true prior before calling this method
Metadata *next(Metadata* cur){
	char* ret;
	ret = (char*)cur;
	ret += sizeof(Metadata);
	ret += cur->size;
	return (Metadata*)ret;
}


//Returns the metadata pointer of the previous block 
Metadata *prev(Metadata* cur){
	char* ret;
	ret = (char*)cur;
	ret -= cur->prev_size;
	ret -= sizeof(Metadata);
	return (Metadata*)ret;
}
