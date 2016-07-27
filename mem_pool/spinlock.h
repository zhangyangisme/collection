

static inline int spinlock_try_once(volatile int *lock_ptr)
{
   int res = __sync_bool_compare_and_swap(lock_ptr, 0, 1);
   return res;
}
 
#define SPIN_LOCK_TRY_TIMES 10
static inline int spinlock_try_ntimes(volatile int *lock_ptr, int n)
{
   int res = 0;
   while(n){
       res = __sync_bool_compare_and_swap(lock_ptr, 0, 1);
       if(res == 1) {
           break;
       }
       n--;
   }
   return res;
}

static inline void spinlock(volatile int *lock_ptr)
{
    while (!__sync_bool_compare_and_swap(lock_ptr, 0, 1))
        ; // Do nothing.
}

static inline void spinunlock(volatile int *lock_ptr)
{
    asm volatile ("" ::: "memory");
    *lock_ptr = 0;
}

