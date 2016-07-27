#include <stdio.h>
#include <stdlib.h>
#include "mem_pool.h"
#include "spinlock.h"


static struct lock_cache_line lock_line = {
    .lock = 0
};

void* mem_pool_create(int num)
{
    void *pool;

    pool = malloc(num*MEM_POOL_SIZE);
    if(pool == NULL){
        printf("malloc failed\n");
        return NULL;
    }
    return pool;
}

int mem_pool_init(void** pool,int num)
{
    int i;//for loop

    for(i = 0 ; i < num; i ++){
        *((char**)(*pool + i*MEM_POOL_SIZE)) = *pool + (i + 1)*MEM_POOL_SIZE;
    }

    return 0;
}


void * mem_alloc_lock(void** pool)
{
    void * tmp;
    spinlock(&(lock_line.lock));

    tmp =  (void*)(*pool);
    *pool = *(void**)(*pool);

    spinunlock(&(lock_line.lock));
    
    return tmp;
}

void  mem_free_lock(void * ptr,void **pool)
{
    spinlock(&(lock_line.lock));
    *(char**)ptr = (void*)(*pool);
    *pool = ptr;
    spinunlock(&(lock_line.lock));
    return ;
}


int main()
{
    void *pool;
    int i = 0;
    pool = mem_pool_create(1024);
    mem_pool_init(&pool,1024);
    for( i = 0; i < 1024; i ++){
        void * tmp;
        tmp = mem_alloc_lock(&pool);
        printf("%p\n");
    }

    return 0;
}
