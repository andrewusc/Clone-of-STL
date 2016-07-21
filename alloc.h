
#ifndef __TinySTL__alloc__
#define __TinySTL__alloc__

#include <stdio.h>
#include <cstdlib>
namespace TinySTL{
    /*
    ** Space allocator
    */
    class alloc{
    private:
        enum EAlign { ALIGN = 8 };
        enum EMaxBytes { MAXBYTES = 128};
        enum ENFreeLists{ NFREELISTS = (EMaxBytes::MAXBYTES)/(EAlign::ALIGN)};
        enum ENObjs{ NOBJS = 20 };
    private:
        union obj{
            union obj *next;
            char client[1];
        };
        /* maintian a free-list[16], available chunks with size 8*(i+1) are linked at free_list[i]  */
        static obj *free_list[ENFreeLists::NFREELISTS];
    
    private:
        static char *start_free; //the start of pool
        static char *end_free;   //the end of pool
        static size_t heap_size;  // current size of heap_size
    private:
        static size_t ROUND_UP(size_t bytes){ // 将bytes以8为倍数向上取整
            return (bytes + EAlign::ALIGN - 1) & ~(EAlign::ALIGN - 1);
        }
        
        static size_t FREELIST_INDEX(size_t bytes){  // find which free_list to use based on the size of bytes
            return ((bytes + EAlign::ALIGN-1)/EAlign::ALIGN - 1);
        }
        
        static void *refill(size_t n); // take spaces from the pool
        
        static char *chunk_alloc(size_t size, size_t & nobjs);  // 分配一大块空间，可以容纳nobjs个大小为size的区块
        
    public:
        static void *allocate(size_t bytes);
        static void deallocate(void *ptr,size_t bytes);
        static void *reallocate(void *ptr, size_t old_sz, size_t new_sz);
        
    };
    
}

#endif /* defined(__TinySTL__alloc__) */
