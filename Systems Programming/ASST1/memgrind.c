#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include "mymalloc.h"

// Number of iterations the workload should be run
#define Iterations 50

// Declare function that runs the simple tests that should catch basic errors
void simpletests();

// Declare Workload functions that will be used as part of this test
void workloadtests();
void workloadA();
void workloadB();
void workloadC();
void workloadD();
void workloadE();

// Function which run each workload and captures the execution time in milliseconds
double workload_timer(void (*function)());

// Variables to hold the total execution time for each workflow
double sumA;
double sumB;
double sumC;
double sumD;
double sumE;

void main(){
	
	//simpletests();
	workloadtests();
}


/*
 * The malloc() and free() implementations that will catch including the following
 * error conditions and return NULL and print out an error message with the calling
 * application and the line number of the call.
 *
 * 1. Trying to malloc 0 bytes
 * 2. free()ing addresses that are not pointers
 * 3. free()ing pointers that are not allocated by malloc
 * 4. Redundant free()ing of the same pointer
 * 5. Saturation of dynamic memory by trying to allocte more memory than is available
 *
 * parameters:
 *       none
 *
 * returns:
 *       nothing
*/
void simpletests(){

	initialize();

	char *ptr = (char *)malloc( 0 );	

	// Simple test A
	//int x;
	//free ( (int*)x );

	// Simple test B
	char *p = (char *)malloc( 200 );
	free ( p + 10);

	int *y;
	free ( y );

	// Simple test C
	char *q = (char *)malloc(100);
	free( q );
	free( q );

	char *r = (char *)malloc(100);
	free( r );
	r = (char *)malloc(100);
	free( r );

	// Simple test D
	char *s = (char *)malloc(5000);
}


/*
 * Run all the workloads, one after the other, 50 times. Record the run time for each
 * workload and store in an array When all iterations of all the workloads have been run,
 * calculate the team time for each workload to execute and output them in sequence.
 *
 * parameters:
 *       none
 *
 * returns:
 *       nothing
*/
void workloadtests(){

	initialize();

	// Run the entire sets of workloads 50 times
	for(int i = 0; i < Iterations; i++){
	  sumA += workload_timer( workloadA );
	  sumB += workload_timer( workloadB );
	  sumC += workload_timer( workloadC );
	  sumD += workload_timer( workloadD );
	  sumE += workload_timer( workloadE );
        }

        printf("Mean time to run workload A is %.6f\n", sumA / Iterations);
        printf("Mean time to run workload B is %.6f\n", sumB / Iterations);
        printf("Mean time to run workload C is %.6f\n", sumC / Iterations);
        printf("Mean time to run workload D is %.6f\n", sumD / Iterations);
        printf("Mean time to run workload E is %.6f\n", sumE / Iterations);
}


/*
 * The purpose of the function is to run the following test:
 * malloc() 1 byte and immediately free it - do this 120 times
 *
 * parameters:
 *       none
 *
 * returns:
 *       nothing
*/ 
void workloadA(){
	
	for(int i = 0; i < 120; i++){
		void* ptr = malloc(1);
		free(ptr);
	}
}


/*
 * The purpose of the function is to run the following test:
 * malloc() 1 byte, store the pointer in an array - do this 120 times
 * Once you've malloc(ed) 120 byte chuncks, then free() the 120 1 byte pointers one by one
 *
 * parameters:
 *       none
 *
 * returns:
 *       nothing
*/ 
void workloadB(){

	void *pointers[120];
	
	int i;
	for(i = 0; i < 120; i++){
		pointers[i] = malloc(1);
	}
	
	for(i = 0; i < 120; i++){
		free(pointers[i]);
	}
}


/*
 * 240 times, randomly choose between a 1 byte mallos() or free()ing one of the 
 * malloc()ed pointersKeeps track of each operation so that the function will 
 * eventually malloc() 120 bytes, in total Keeps track of each operation so that
 * the function will eventually free() all pointers 
 *
 * parameters:
 *       none
 *
 * returns:
 *       nothing
*/ 
void workloadC(){
	void *pointers[240];
	int counter=0;
	int size=0;
	int number=0;
	
	while(counter < 240){
		number = rand() % 2;
		
		//if number is 0 or if the array is empty, malloc 1 new byte
		if(number == 0 || size == 0){
			pointers[size] = malloc(1);
			counter++;
			size++;
		}else{
		//Choose a random entry and free it, then fill in the gap with the top entry
			int entry = rand() % size;
			if(entry == (size - 1)){
				free(pointers[entry]);
				pointers[entry] = 0;
			}else{
				free(pointers[entry]);
				pointers[entry] = pointers[size-1];
				pointers[size-1] = 0;
			}size--;
		}
	}
	
	//free any remaining pointers
	while(size > 0){
		free(pointers[size-1]);
		size--;
	}
}


/*
 *  This workload attempts to randomly allocate memory sizes between 0 and 10 bytes, 
 *  storing the pointers in an array. Once a single call to malloc() fails, the   
 *  workload will stop trying to allocate more memory and free all of the pointers.
 *
 * parameters:
 *       none
 *
 * returns:
 *       nothing
 */ 
void workloadD(){

	char *pointer[1000];	
	int index = 0;
	bool success = true;

	//allocate memory until malloc fails
	while( success ){
		int random_number = rand()%10 + 1;
		pointer[index] = malloc(random_number);

		//if an error occurs with malloc, the pointer will be NULL
		if(pointer[index] == NULL){
			success = false;
			continue;
		}
		index++;
	}

	//free all pointers
	for(int i = 0; i < index; i++){
		free(pointer[i]);
	}
}


/*
 * Loop 10 times through a series of malloc() and free() workload calls
 * - Allocate memory blocks until its reach's the end of the virtual memory available
 * - Free every other memory block
 * - Allocate memory blocks in chunks of 25
 * - Free all allocated memory
 *
 * parameters:
 *       none
 *
 * returns:
 *       nothing
*/
void workloadE(){

	int *pointers[100];

  	for (int i = 0; i < 10; i++){
    		int numberOfBlocks = 1;
    		int currentMallocIndex = 0;
    		int currentFreeIndex = 0;

    		// Allocate memory blocks until its reach's the end of the virtual memory available
    		while ((pointers[currentMallocIndex] = malloc(50)) != NULL ){
      			numberOfBlocks++;
      			currentMallocIndex++;
    		}

   		 // Free every other memory block
     		for (currentFreeIndex = 0; currentFreeIndex < currentMallocIndex; currentFreeIndex += 2){
      			free(pointers[currentFreeIndex]);
      			pointers[currentFreeIndex] = NULL;
    		}

    		// Allocate memory blocks in chunks of 25
    		for (currentMallocIndex = 0; currentMallocIndex < numberOfBlocks-1; currentMallocIndex += 2){
      			pointers[currentMallocIndex] = malloc(25);
    		}

    		// Free all allocated memory
    		for (currentFreeIndex = 0; currentFreeIndex < numberOfBlocks-1; currentFreeIndex += 1){
      			free(pointers[currentFreeIndex]);
      			pointers[currentFreeIndex] = NULL;
    		}
 	 }
}

/*
 * Executes a workload and captures the execution time in milliseconds
 *
 * paramters:
 *     *function():  A workload that takes in no parameters
 * returns:
  *    A double that stores the time it takes to excute the function
 */
double workload_timer(void (*function)()){

	struct timeval start, end;

	// Time the execution of the function call
	gettimeofday(&start, NULL);
	function();
	gettimeofday(&end, NULL);

        // Determine the time it took to execute the function and return it
        double timer = (end.tv_sec - start.tv_sec) * 1000.0;
	timer += (end.tv_usec - start.tv_usec) / 1000.0;
        return timer;
}
