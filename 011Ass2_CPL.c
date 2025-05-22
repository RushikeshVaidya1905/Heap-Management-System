#include <stdio.h>
#define METADATA_SIZE sizeof(Metadata)
#define FREE_SPACE_SIZE 40000

typedef enum {false, true} bool;
//This is metadat structure used to manage space
typedef struct Metadata_tag{
    int size;
    bool free;
    struct Metadata_tag* prev;
} Metadata;

char free_space[FREE_SPACE_SIZE];

Metadata* freeListHead;
int InitialisedFlag = 1;
int size = 40000;

//Initialising the space at start
void Initialise(){
    freeListHead = (Metadata*)free_space;
    freeListHead->free = true;
    freeListHead->size = FREE_SPACE_SIZE - METADATA_SIZE;
    freeListHead->prev = NULL;
    printf("The status is size: %d, free: %d\n", freeListHead->size, freeListHead->free);
}

Metadata* split(Metadata* curr, int required_size ){
    //keeping the next pointer as after spliting and assigning it as free
    Metadata* next = (Metadata*)((void*)curr + METADATA_SIZE + required_size);
    next->free = true;
    next->size = curr->size - required_size - METADATA_SIZE;
    next->prev = curr;
    curr->size = required_size;
    curr->free = false;
    return curr;
}

void* allocate(int required_size){
    //Initilising the memory at the start
    if(InitialisedFlag){
        Initialise();
        InitialisedFlag = 0;
    }
    //Traversing the array 
    Metadata* curr = freeListHead;
    void* retVal;

    //Traversing whole heap for checking the free if the curr location goes out of bound then no space
    while ((void*)curr <= (void*)&free_space[FREE_SPACE_SIZE-1]) { // first-fit
        //checking for the free space
        if (curr->free == true && curr->size >= required_size && curr->size < required_size + METADATA_SIZE) { // internal fragmentation
            curr->free = false;
            retVal = (void*)(curr + 1);
            size = size - ( required_size + sizeof(Metadata));
            return retVal;
        } 
        //If large memory is there then split the memory
        else if (curr->free == true && curr->size >= required_size + METADATA_SIZE) {
            curr = split(curr, required_size);
            retVal = (void*)(curr + 1);
            size = size - ( required_size + sizeof(Metadata));
            return retVal;
        } 
        //If no space is free then traverse to the next block
        else {
            curr = (Metadata*)((char*)curr + METADATA_SIZE + curr->size);
        }
    }

    //At end if no space is available
    printf("No free space for memory allocation\n");
    return NULL;
}

void merge_blocks(Metadata* curr){
    //Using the previous block and next block
    Metadata* prev = curr->prev;
    Metadata* next = (Metadata*)((char*)curr + METADATA_SIZE + curr->size);

    //if the prev is not null and prev is free then merge it with the curr block and meake it free
    if(prev != NULL && prev->free == true){
        prev->size += METADATA_SIZE + curr->size;
        curr = prev;
    }

    //if the next block is also free then merge the current and next block as well
    if((void*)next <= (void*)&free_space[FREE_SPACE_SIZE-1] && next->free == true){
        curr->size += METADATA_SIZE + next->size;
    }
}

void de_allocate(void* loc){
    //if the location is out of bound then print out of bounds
    if(loc < (void*)free_space || loc >= (void*)(free_space + FREE_SPACE_SIZE)){
        printf("Out of bounds\n");
    }
    //else merge the given block with neighbour free members and if no free neighbour then it won't merge
    else{
        Metadata* curr = (Metadata*)loc - 1;
        curr->free = true;
        size = size + ( curr->size + sizeof(Metadata));
        merge_blocks(curr);
    }
}

int main(){
    //Initial specifications of the heap
    printf("Metadata Size: %d\n", METADATA_SIZE);
    printf("Heap Size : %d\n", FREE_SPACE_SIZE);
    printf("Start of free_space :\t%p\n", free_space);
    printf("End of free_space :\t%p\n", &free_space[FREE_SPACE_SIZE-1]);
    
    int* arr1 = (int*)allocate(5 * sizeof(int));
    printf("%p\n", arr1);
    printf("Size remained after allocation %d\n",size);

    int* arr2 = (int*)allocate(9988);
    printf("%p\n", arr2);
    printf("Size remained after allocation %d\n",size);

    int* arr3 = (int*)allocate(9988);
    printf("%p\n", arr3);
    printf("Size remained after allocation %d\n",size);

    int* arr4 = (int*)allocate(9968);
    printf("%p\n", arr4);
    printf("Size remained after allocation %d\n",size);

    de_allocate(arr2);
    printf("Size remained after deallocation %d\n",size);

    int* arr5 = (int*)allocate(sizeof(int)*4);
    printf("%p\n", arr5);
    printf("Size remained after allocation %d\n",size);

    int* arr6 = (int*)allocate(9968);
    printf("%p\n", arr6);
    printf("Size remained after allocation %d\n",size);

    de_allocate(arr4);
    printf("Size remained after deallocation %d\n",size);
    
    int* arr7 = (int*)allocate(9968);
    printf("%p\n", arr7);
    printf("Size remained after allocation %d\n",size);
    return 0;
}
 