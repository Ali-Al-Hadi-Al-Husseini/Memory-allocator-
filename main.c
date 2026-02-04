#include <stdio.h>
#include <assert.h>
#include <stdbool.h>


#define CAPACITY 640000

char heap[CAPACITY] = {0};
size_t used_size = 0;


void *heap_allocate(size_t size){

    assert( used_size + size <= CAPACITY);
    void *result = heap + used_size;
    used_size += size ;

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
