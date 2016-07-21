#include "String.h"

#include <iostream>

namespace TinySTL {
    const size_t string::npos;
    
    string::string(const string &str, size_t pos, size_t len){
        len = changeVarWhenEuqalNPOS(len, str.size(), pos);
        allocateAndCopy(str.start_ + pos, str.start_ + pos + len);
    }
    string::string(const char* s){
        allocateAndCopy(s, s + strlen(s));
    }
    string::string(const char* s, size_t n){
        allocateAndCopy(s, s+n);
        
    }
    string::string(size_t n, char c){
        allocateAndFillN(n, c);
    }
    
    string::string(string&& str){
        moveData(str);
    }
    
    string::~string(){
        destroyAndDeallocate();
    }
    
    string& string::operator=(const TinySTL::string &str){
        if(this != &str){
            destroyAndDeallocate();
            allocateAndCopy(str.start_, str.finish_);
        }
        return *this;
    }
    
    string& string::operator=(char c){
        destroyAndDeallocate();
        allocateAndFillN(1, c);
        return *this;
    }
    
    string& string::operator=(const char *s){
        destroyAndDeallocate();
        allocateAndCopy(s, s+strlen(s));
        return *this;
    }
    
    string& string::operator=(TinySTL::string &&str){
        if(this != &str){
            moveData(str);
        }
        return *this;
    }
    
    void string::resize(size_t n){
        resize(n,value_type());
    }
    
    void string::resize(size_t n, char c){
        if(n < size()){
            dataAllocator::destroy(start_ + n, finish_);
            finish_ = start_ + n;
        }
        else if(n > size() && n <=capacity()){
            auto lengthOfInsert = n - size();
            finish_ = std::uninitialized_fill_n(finish_, lengthOfInsert, c);
        }
        else if(n > capacity()){
            auto lengthOfInsert = n - size();
            iterator newStart = dataAllocator::allocate(getNewCapacity(lengthOfInsert));
            iterator newFinish = std::uninitialized_copy(begin(), end(), newStart);
            newFinish = std::uninitialized_fill_n(newFinish, lengthOfInsert, c);
            
            destroyAndDeallocate();
            start_ = newStart;
            finish_ = newFinish;
            endOfStorage_ = start_ + n;
        }
    }
    
    void string::reserve(size_t n){
        if(n <= capacity()){
            return ;
        }
        iterator newStart = dataAllocator::allocate(n);
        iterator newFinish = std::uninitialized_copy(begin(), end(), newStart);
        destroyAndDeallocate();
        start_ = newStart;
        finish_ = newFinish;
        endOfStorage_ = start_ + n;
    }
    
    //*************[ insert() ] *************
    
    string& string::insert(size_t pos, const string& str){
        insert(begin()+ pos, str.begin(), str.end());
        return *this;
    }
    string& string::insert(size_t pos, const string& str, size_t subpos, size_t sublen){
        insert(begin() + pos, str.begin()+subpos, str.begin() + subpos + sublen);
        return *this;
    }
    string& string::insert(size_t pos, const char* s){
        insert(begin() + pos, s, s + strlen(s));
        return *this;
    }
    string& string::insert(size_t pos, const char* s, size_t n){
        insert(begin() + pos, s, s + n);
        return *this;
    }
    
    string& string::insert(size_t pos, size_t n, char c){
        insert(begin()+pos, n, c);
        return *this;
    }
    
    string::iterator string::insert(iterator p, size_t n, char c){
        auto lengthOfLeft = capacity() - size();
        if(n <= lengthOfLeft){
            for(iterator it = finish_-1; it >= p; --it){
                *(it + n) = *(it);
            }
            std::uninitialized_fill_n(p, n, c);
            finish_ += n;
            return (p + n);
            
        }
        else{
            return insert_aux_filln(p, n, c);
        }
    }
    string::iterator string::insert(iterator p, char c){
        return insert(p, 1, c);
    }
    
    
    string& string::append(const string& str){
        *(this) += str;
        
        return *this;
    }
    string& string::append(const string& str, size_t subpos, size_t sublen){
        sublen = changeVarWhenEuqalNPOS(sublen, str.size(), subpos);
        insert(size(),str,subpos,sublen);
        return *this;
    }
    string& string::append(const char* s){
        insert(size(), s);
        return *this;
    }
    string& string::append(const char* s, size_t n){
        insert(size(), s, n);
        return *this;
    }
    string& string::append(size_t n,char c){
        insert(size(), n, c);
        return *this;
    }
    
    /************ [operator+] **************/
    
    string& string::operator+=(const string& str){
        insert(size(), str);
        return *this;
    }
    string& string::operator+=(const char* s){
        insert(size(), s);
        return *this;
    }
    string& string::operator+=(char c){
        insert(end(),c);
        return *this;
    }
    
    
    /************ [replace] **************/
    
    string& string::replace(size_t pos, size_t len, const string& str){
        replace(begin()+pos , begin() + pos + len, str.start_, str.finish_);
        return *this;
    }
    string& string::replace(iterator i1, iterator i2, const string& str){
        replace(i1, i2, str.start_, str.finish_);
        return *this;
    }
    string& string::replace(size_t pos, size_t len, const string& str, size_t subpos, size_t sublen){
        replace(begin()+pos , begin() + pos + len, str.start_ + subpos, str.start_ + subpos + sublen);
        return *this;
    }
    string& string::replace(size_t pos, size_t len, const char* s){
        replace(begin()+pos , start_ + pos + len, s, s + strlen(s));
        return *this;
    }
    string& string::replace(iterator i1, iterator i2, const char* s){
        replace(i1, i2, s, s + strlen(s));
        return *this;
    }
    string& string::replace(size_t pos, size_t len, const char* s, size_t n){
        replace(begin() + pos , begin() + pos + len, s, s + n);
        return *this;
    }
    string& string::replace(iterator i1, iterator i2, const char* s, size_t n){
        replace(i1, i2, s, s+n);
        return *this;
    }
    string& string::replace(size_t pos, size_t len, size_t n, char c){
        auto ptr = erase(begin()+pos, begin() + pos + len);
        insert(ptr, n, c);
        return *this;
    }
    string& string::replace(iterator i1, iterator i2, size_t n, char c){
        auto ptr = erase(i1, i2);
        insert(ptr, n, c);
        return *this;
    }
    
    
    /************ [find] **************/
    
    size_t string::find_aux(const_iterator cit, size_t pos, size_t lengthOfS, size_t cond)const{
        size_t i, j;
        for(i = pos; i != cond; ++i){
            for(j = 0; j != lengthOfS; ++j){
                if(*(begin()+i+j) != cit[j])
                    break;
            }
            if(j == lengthOfS)
                return i;
        }
        return npos;
    }
    
    size_t string::find(const char* s, size_t pos, size_t n) const{
        //size_t lengthOfS = strlen(s);
        return find_aux(s, pos, n, size());
    }
    
    size_t string::find(const string& str, size_t pos) const{
        size_t lengthOfS = str.size();
        if(lengthOfS > size() - pos){
            return npos;
        }
        return find_aux(str.cbegin(), pos, lengthOfS, size());
        
    }
    
    size_t string::find(const char* s, size_t pos) const{
        return find(s, pos, strlen(s));
    }
    
    size_t string::find(char c, size_t pos) const{
        for(auto cit = cbegin() + pos; cit != cend(); ++cit){
            if(*cit == c){
                return cit - cbegin();
            }
        }
        return npos;
    }
    
    /************ [compare] **************/
    int string::compare_aux(size_t pos, size_t len, const_iterator cit, size_t subpos, size_t sublen)const{
        size_t i,j;
        for(i = 0, j = 0; i < len && j < sublen; ++i, ++j){
            if((*this)[pos+i] < cit[subpos+i])
                return -1;
            else if((*this)[pos+i] > cit[subpos+i])
                return 1;
        }
        if(i == len && j == len)
            return 0;
        else if(i == len)
            return -1;
        else
            return 1;
    }
    
    int string::compare(const string& str) const{
        return compare_aux(0, size(), str.begin(), 0, str.size());
    }
    
    int string::compare(size_t pos, size_t len, const string& str) const{
        return compare_aux(pos, len, str.begin(), 0, str.size());
    }
    
    int string::compare(size_t pos, size_t len, const string& str, size_t subpos, size_t sublen) const{
        return compare_aux(pos, len, str.begin(), subpos, sublen);
    }
    
    int string::compare(const char* s) const{
        return compare_aux(0, size(), s, 0, strlen(s));
    }
    
    int string::compare(size_t pos, size_t len, const char* s) const{
        return compare_aux(pos, len, s, 0, strlen(s));
    }
    
    int string::compare(size_t pos, size_t len, const char* s, size_t n) const{
        return compare_aux(pos, len, s, 0, n);
    }
    
    
    /************ [_private_] **************/
    
    void string::moveData(string& str){
        start_ = str.start_;
        finish_ = str.finish_;
        endOfStorage_ = str.endOfStorage_;
        str.start_ = str.finish_ = str.endOfStorage_ = 0;
    }
    
    
    void string::destroyAndDeallocate(){
        dataAllocator::destroy(start_, finish_);
        dataAllocator::deallocate(start_, endOfStorage_ - start_);
    }
    
    
    
    void string::allocateAndFillN(size_t n, char c){
        start_ = dataAllocator::allocate(n);
        finish_ = std::uninitialized_fill_n(start_,n,c);
        /** ToDo: TinySTL::uninitialized_fill_n  */
        endOfStorage_ = finish_;
    }
    
    void string::string_aux(size_t n, char c, std::true_type){
        allocateAndFillN(n, c);
    }
    
    
    
    bool string::isContained(char ch, const_iterator first, const_iterator last) const{
        for(auto cit = first; cit != last; cit++){
            if(*cit == ch)
                return true;
        }
        return false;
    }
    size_t string::changeVarWhenEuqalNPOS(size_t var, size_t minuend, size_t minue) const{
        return (var == npos ? minuend - minue : var);
    }
    
    
    string::iterator string::insert_aux_filln(iterator p, size_t n, value_type c){
        auto newCapacity = getNewCapacity(n);
        iterator newStart = dataAllocator::allocate(newCapacity);
        iterator newFinish = std::uninitialized_fill_n(start_, n, c);
        auto res = newFinish;
        newFinish = std::uninitialized_copy(p,finish_,newFinish);
        destroyAndDeallocate();
        start_ = newStart;
        finish_ = newFinish;
        endOfStorage_ = start_ + newCapacity;
        return res;
        
    }
    
    /* new capacity we need  */
    string::size_type string::getNewCapacity(string::size_type len)const{
        size_type oldCapacity = endOfStorage_ - start_;
        auto res = std::max(oldCapacity, len);
        auto newCapacity = oldCapacity + res;
        return newCapacity;
    }
    
}