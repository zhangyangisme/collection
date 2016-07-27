


#define MEM_POOL_SIZE 2048

struct lock_cache_line {
    volatile int lock;
    int padding[15];
};



void *mem_pool_create(int num);
int mem_pool_init(void** pool,int num);
void * mem_alloc_lock(void** pool);
void mem_free_lock(void* ptr,void** pool);
