#include <stdio.h>
#include <assert.h>
#include <stdbool.h>


#define MEM_CAPACITY 640000
#define CHUNKS_CAPACITY 2048

typedef struct  
{
    void *start;
    size_t size;

}memory_chunk;

typedef struct{
    
    size_t size;
    memory_chunk chunks[CHUNKS_CAPACITY];

}chunk_list;


int chunk_list_find( chunk_list *list, void *ptr){
    (void)  list;
    (void) ptr;
 
    assert(false && "chunk_list_find not implemented yet ");

}


int chunk_list_insert( chunk_list *list,void *ptr , size_t size){
    
    (void)  list;
    (void) size;
 
    assert(false && "chunk_list_insert not implemented yet ");
    
}

int chunk_list_remove( chunk_list *list, size_t index){
    (void)  list;
    (void) index;
 
    assert(false && "chunk_list_remove not implemented yet ");
    
}





char heap[MEM_CAPACITY] = {0};
size_t alloced_size = 0;

chunk_list mem_chunks = {0}; 
chunk_list freed_chunks = {0};



void *heap_allocate(size_t size){

    if(size <= 0){
        return NULL;
    }

    assert( alloced_size + size <= MEM_CAPACITY);
    void *result = heap + alloced_size;
    alloced_size += size ;

    chunk_list_insert(&mem_chunks,result , size);


    return result;
}

void heap_free(void *ptr){
    (void) ptr;
    assert(false && "heap_free not implemented yet ");
}
//

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
