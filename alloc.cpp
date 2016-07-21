//
//  alloc.cpp
//  TinySTL
//
//  Created by 宇 杨 on 16/7/1.
//  Copyright (c) 2016年 edu.self. All rights reserved.
//

#include "alloc.h"

namespace TinySTL {
    char *alloc::start_free = 0;
    char *alloc::end_free = 0;
    size_t alloc::heap_size = 0;
    
    alloc::obj *alloc::free_list[alloc::ENFreeLists::NFREELISTS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    void *alloc::allocate(size_t bytes){
        if (bytes > EMaxBytes::MAXBYTES){  // the new space is greater that 128 bytes, we just call malloc.
            return malloc(bytes);
        }
        size_t index = FREELIST_INDEX(bytes);
        obj *list = free_list[index];
        if (list){        //there is free space from list
            free_list[index] = list->next;
            return list;
        }
        else{//此list没有足够的空间，需要从内存池里面取空间
            return refill(ROUND_UP(bytes));
        }
    }
    
    void alloc::deallocate(void *ptr, size_t bytes){
        if(bytes > EMaxBytes::MAXBYTES){
            free(ptr);
        }
        else{
            size_t index = FREELIST_INDEX(bytes);
            obj *node = static_cast<obj *>(ptr);
            node->next = free_list[index];
            free_list[index] = node;
        }
    }
    
    void* alloc::reallocate(void *ptr , size_t old_sz, size_t new_sz){
        deallocate(ptr, old_sz);
        ptr = allocate(new_sz);
        return ptr;
    }
    
    void *alloc::refill(size_t bytes){
        size_t nobjs = ENObjs::NOBJS;
        char* chunk = chunk_alloc(bytes, nobjs);
        
        if(nobjs == 1){
            return chunk;
        }
        else{
            obj *current_obj = 0, *next_obj = 0;
            
            obj **my_free_list = free_list + FREELIST_INDEX(bytes);
            obj *result = (obj *)(chunk);
            *my_free_list = next_obj = (obj *)(chunk + bytes);
            
            for(int i = 1; ; i++){// 将取出的多余空间加入到相应的free list 中去
                current_obj = next_obj;
                next_obj = (obj *)((char *)next_obj + bytes);
                if(nobjs == i+1){
                    current_obj->next = 0;
                    break;
                }
                else{
                    current_obj->next = next_obj;
                }
            }
            return result;
        }
        
    }
    
    char *alloc::chunk_alloc(size_t bytes, size_t& nobjs){  // bytes is already adjusted to be the multiple of 8
        
        char *result = 0;
        size_t total_bytes = bytes * nobjs;
        size_t bytes_left = end_free - start_free;
        
        if(bytes_left >= total_bytes){
            result = start_free;
            start_free = start_free + total_bytes;
            return result;
        }
        else if(bytes_left >= bytes){
            nobjs = bytes_left / bytes;
            total_bytes = nobjs * bytes;
            result = start_free;
            start_free += total_bytes;
            return result;
        }
        else{
            size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size);
            if(bytes_left > 0){
                obj **my_free_list = free_list + FREELIST_INDEX(bytes_left);
                ((obj *) start_free)->next = *my_free_list;
                *my_free_list = (obj *)start_free;
            }
            start_free = (char *)malloc(bytes_to_get);
            if(!start_free){
                obj **my_free_list = 0, *p = 0;
                for(int i = 0; i <= EMaxBytes::MAXBYTES; i += EAlign::ALIGN){
                    my_free_list = free_list + FREELIST_INDEX(i);
                    p = *my_free_list;
                    if(p != 0){
                        *my_free_list = p->next;
                        start_free = (char *)p;
                        end_free = start_free + 1;
                        return chunk_alloc(bytes, nobjs);
                    }
                }
                end_free = 0;
                
            }
            heap_size += bytes_to_get;
            end_free = start_free + bytes_to_get;
            return chunk_alloc(bytes, nobjs);
        }
    }
}

