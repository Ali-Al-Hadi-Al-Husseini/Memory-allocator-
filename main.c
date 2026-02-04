#include <stdio.h>
#include <assert.h>
#include <stdbool.h>


#define MEM_CAPACITY 640000
#define AllOCED_CAPCITY 2048

typedef struct  
{
    void *start;
    size_t size;
}memory_chunk;



char heap[MEM_CAPACITY] = {0};
size_t alloced_size = 0;


memory_chunk mem_chunks [AllOCED_CAPCITY] = {0}; 
size_t mem_chunks_size = 0;



void *heap_allocate(size_t size){

    assert( alloced_size + size <= MEM_CAPACITY);
    void *result = heap + alloced_size;
    alloced_size += size ;

    return result;
}

void heap_free(void *ptr){
    (void) ptr;
    
}


int main(void)
{
    // test 
    char *mem = heap_allocate(26);

    for (int i = 0;i < 26; ++i){
        mem[i] = i + 'A';
    }

    printf("%s", mem);
    return 0 ; 
}
