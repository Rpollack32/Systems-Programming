#ifndef MYMALLOC_H
#define MYMALLOC_H

#define MEMORY_SIZE 4096

#define malloc( x ) mymalloc( x, __FILE__, __LINE__ )
#define free( x ) myfree( x, __FILE__, __LINE__ )

void initialize(); // Initializes the  block of memory
void *mymalloc(size_t size, const char *file_name, int line_number); // Modified implementation of malloc()
void myfree(void *ptr, const char *file_name, int line_number); // Modified implementation of free()

#endif
