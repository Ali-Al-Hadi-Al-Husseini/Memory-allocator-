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
    memory_chunk key = { .start = ptr};
    memory_chunk *result = bsearch(&key, list->chunks,list->size, sizeof (memory_chunk), chunk_compare);
    
    if (result == NULL){
        return -1;
    }
 
    assert(list->chunks <= result);
    return ( result - list->chunks);


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

    assert(index < list->size);
    for(int i = index; i < list->size; i++){
        list->chunks[i] = list->chunks[i+1];
    }
    list->size -- ;
    
}

void chunk_list_print(chunk_list *list){

    for(int i = 0; i < list->size; i ++){
        
        printf("index: %i chunk for %p and the size of %zu \n",i,list->chunks[i].start,list->chunks[i].size );
    }
}





char heap[MEM_CAPACITY] = {0};
size_t alloced_size = 0;

chunk_list mem_chunks = {0}; 
chunk_list freed_chunks = {0};

// these func checks the chunks to see if any are mergable 
int merge_freed_chunks(void){
    int i = 0;

    while(i < freed_chunks.size - 1  ){
        // adds the size of chunk[i] to its pointer to know if the next chunk[i+1] is right after it 
        bool mergable = freed_chunks.chunks[i].start + freed_chunks.chunks[i].size == freed_chunks.chunks[i+1].start  ;
        if (mergable){
            freed_chunks.chunks[i+1].size += freed_chunks.chunks[i].size;
            freed_chunks.chunks[i+1].start = freed_chunks.chunks[i].start;
            chunk_list_remove(&freed_chunks,i);

        }else{
            i ++;
        }
    }
    printf("gola amigo i: %i",i);
}
void *check_freed_chunks(size_t size) {
    for (int i = 0; i < freed_chunks.size; i++) {
        if (freed_chunks.chunks[i].size >= size) {
            size_t chunk_size = freed_chunks.chunks[i].size;
            void *chunk_ptr = freed_chunks.chunks[i].start;

            chunk_list_insert(&mem_chunks, chunk_ptr, chunk_size);

            if (chunk_size - size == 0) {
                chunk_list_remove(&freed_chunks, i);
            } else {
                // Shrinking the chunk
                freed_chunks.chunks[i].size -= size;
                freed_chunks.chunks[i].start += size;
            }
            return chunk_ptr;
        }
    }
    return NULL; 
}

void *heap_allocate(size_t size){

    if(size <= 0){
        return NULL;
    }
    // checking to see if there is enough space left 
    if ( alloced_size + size <= MEM_CAPACITY){

        void *result = heap + alloced_size;
        alloced_size += size ;

        chunk_list_insert(&mem_chunks,result , size);
        return result;
    }
    void *result = check_freed_chunks(size);
    if(result != NULL){
        return result;
    }

    merge_freed_chunks();

    result = check_freed_chunks(size);
    return result;



}

void heap_free(void *ptr ){
    
    int index = chunk_list_find(&mem_chunks,ptr);
    chunk_list_insert(&freed_chunks,ptr,mem_chunks.chunks[index].size);
    chunk_list_remove(&mem_chunks,index);
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

    // a sudo chunk for testing 
    // int x = 1 ;
    // memory_chunk  test  ;
    // test.size = 32;
    // test.start = &x;


    // int res1 = chunk_list_find(&mem_chunks, &test);
    // int res2 = chunk_list_find(&mem_chunks, mem_chunks.chunks[0].start);
    // printf(" res1: %i \n res2: %i\n",res1,res2);

    // heap_free(mem_chunks.chunks[10].start);
    // printf("removing %s from memchunks \n",mem);
    // chunk_list_print(&mem_chunks);
    // printf("printing freed chunks \n");

    // chunk_list_print(&freed_chunks);


    //testing merge 
    void *to_free[10];
    for (int i = 5;i < 10; ++i){
        to_free[i-5]=  mem_chunks.chunks[i].start;
    }
    for (int i = 15;i < 20; ++i){
        to_free[i-10]=  mem_chunks.chunks[i].start;
    }
    for (int i = 0;i < 10; ++i){
        heap_free(to_free[i]);
    }
    // chunk_list_print(&mem_chunks);
    printf("++++++++++++++freed+++++++++++++\n");
    printf("++++++++++++++++++++++++++++++++\n");


    chunk_list_print(&freed_chunks);
    merge_freed_chunks();
    printf("++++++++++++++freed+++++++++++++\n");
    printf("++++++++++++++++++++++++++++++++\n");
    chunk_list_print(&freed_chunks);
   
    printf("++++++++++++++++++++++++++++++++\n");
    chunk_list_print(&mem_chunks);
    return 0 ; 
}
