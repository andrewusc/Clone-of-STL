//
//  Construct.h
//  TinySTL
//
//  Created by 宇 杨 on 16/7/6.
//  Copyright (c) 2016年 edu.self. All rights reserved.
//

#ifndef TinySTL_Construct_h
#define TinySTL_Construct_h

#include <new>

#include "TypeTraits.h"

namespace TinySTL {
    template<class T1, class T2>
    inline void construct(T1 *ptr1, const T2& value){
        new(ptr1) T1(value);
    }
    
    template<class T>
    inline void destroy(T *ptr){
        ptr->~T();
    }
    
    template<class ForwardIterator>
    inline void _destroy(ForwardIterator first, ForwardIterator last, _true_type){}
    
    template<class ForwardIterator>
    inline void _destroy(ForwardIterator first, ForwardIterator last, _false_type){
        for (; first != last; ++first){
            destroy(&*first);
        }
    }
    
    template<class ForwardIterator>
    inline void destroy(ForwardIterator first, ForwardIterator last){
        typedef typename _type_traits<ForwardIterator>::is_POD_type is_POD_type;
        _destroy(first, last, is_POD_type());
    }
    
}





#endif
