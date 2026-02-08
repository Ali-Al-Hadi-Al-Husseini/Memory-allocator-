#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>


#define MEM_CAPACITY 640000
#define CHUNKS_CAPACITY 2048

typedef struct  
{
    void *start;
    size_t size;

}memory_chunk;

int chunk_compare(const void *a, const void *b){
    const memory_chunk *chunk_a = a;
    const memory_chunk *chunk_b = b;

    return (chunk_a->start - chunk_b -> start);
}
typedef struct{
    
    size_t size;
    memory_chunk chunks[CHUNKS_CAPACITY];

}chunk_list;


int chunk_list_find( chunk_list *list, void *ptr){

    void *result = bsearch(ptr, list->chunks,list->size, sizeof (memory_chunk), chunk_compare);

    if (result == NULL){
        return -1;
    }
    return 1;

}


int chunk_list_insert( chunk_list *list,void *ptr , size_t size){

    assert( list->size < CHUNKS_CAPACITY);
    
    list->chunks[list->size].start = ptr;
    list->chunks[list->size].size = size;


    // inseting a chunk in sorted order O(n)
    for(int i= list->size; i > 0; i--){
        if (list->chunks[i].start >list->chunks[i - 1 ].start){
            break;
        }
        memory_chunk temp_chunk =  list->chunks[i];
        list->chunks[i] =  list->chunks[i-1];
        list->chunks[i-1] = temp_chunk;
       
    }

    list->size ++ ;

    return 1 ;
    
}

int chunk_list_remove( chunk_list *list, size_t index){
    (void)  list;
    (void) index;
 
    assert(false && "chunk_list_remove not implemented yet ");
    
}

void chunk_list_print(chunk_list *list){

    for(int i = 0; i < list->size; i ++){
        
        printf("chunk at %p ans the size of %ul \n",list->chunks[i].start,list->chunks[i].size );
    }
}




char heap[MEM_CAPACITY] = {0};
size_t alloced_size = 0;

chunk_list mem_chunks = {0}; 
chunk_list freed_chunks = {0};



void *heap_allocate(size_t size){

    if(size <= 0){
        return NULL;
    }
    // checking to see if there is enough space left 
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
    // printf("%s\n", mem);

    for (int i = 27;i < 50; ++i){
        mem= heap_allocate(i);
    }
    // chunk_list_print(&mem_chunks);
    // printf("i want to find %p \n",&mem_chunks.chunks[0]);

    // 
    int x = 1 ;
    memory_chunk  test  ;
    test.size = 32;
    test.start = &x;


    int res1 = chunk_list_find(&mem_chunks, &test);
    int res2 = chunk_list_find(&mem_chunks, &mem_chunks.chunks[0]);
    printf(" res1: %i \n res2: %i\n",res1,res2);
   
    return 0 ; 
}
