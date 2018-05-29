//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include <errno.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#if !defined __link
#include <reent.h>
#include <mcu/arch.h>
#endif

#include "var/Data.hpp"
using namespace var;

//This is here so that by default that data can point to a null value rather than be a null value
const int Data::m_zero_value = 0;

//this value corresponds to the malloc chunk size used in Stratify OS
//this may be something that could be determined through a system call
#define MIN_CHUNK_SIZE 56

u32 Data::minimum_size(){
    return MIN_CHUNK_SIZE;
}

Data::Data(){
    zero();
}

Data::Data(void * mem, u32 s, bool readonly){
    zero();
    set(mem, s, readonly);
}

Data::Data(u32 s){
    zero();
    alloc(s);
}


Data::Data(const Data & a){
    zero();
    copy(a);
}

Data& Data::operator=(const Data & a){
    copy(a);
    return *this;
}


int Data::free(){
    if( needs_free() ){
        ::free(m_mem_write);
    }
    zero();
    return 0;
}

Data::~Data(){
    free();
}

void Data::copy(const Data & a){
    if( a.is_internally_managed() ){
        //is the new object taking ownership or making a copy
        if( a.is_transfer_ownership() ){
            //set this memory to the memory of a
            set(a.data(), a.capacity(), false);

            //setting needs free on this and clearing it on a will complete the transfer
            set_needs_free();
            a.clear_needs_free();
        } else {

            //allocate memory for a copy of a
            set_capacity(a.capacity());
            if( data() && (capacity() >= a.capacity()) ){
                memcpy(data(), a.data(), a.capacity());
            }
        }
    } else {
        set(a.data(), a.capacity(), a.is_read_only());
    }
}

void Data::set(void * mem, u32 s, bool readonly){

    //free the data if it was previously allocated dynamically
    free();

    m_mem_write = mem;
    clear_needs_free();
    m_capacity = s;
    if( m_mem_write ){
        this->m_mem = m_mem_write;
    } else {
        mem = (void*)&m_zero_value;
    }

    if( readonly ){
        m_mem_write = 0;
    }
}

void Data::zero(){
    m_mem = &m_zero_value;
    m_mem_write = 0;
    clear_needs_free();
    m_capacity = 0;
}


int Data::alloc(u32 s, bool resize){

    void * new_data;

    if( ( !needs_free() ) && (m_mem != &m_zero_value) ){
        //this data object can't be resized -- it was created using a pointer (not dyn memory)
        return -1;
    }

    if( s == 0 ){
        zero();
        return 0;
    }

    s = ((s + MIN_CHUNK_SIZE-1) / MIN_CHUNK_SIZE) * MIN_CHUNK_SIZE;

    new_data = malloc(s);
    if( set_error_number_if_null(new_data) == 0 ){
        return -1;
    }

    if( resize ){
        memcpy(new_data, m_mem, s > m_capacity ? m_capacity : s);
    }

    free();

    m_mem_write = new_data;
    set_needs_free();
    m_mem = m_mem_write;
    m_capacity = s;

    return 0;
}

int Data::set_capacity(u32 s){
    if( s <= capacity() ){
        return 0;
    }

    return alloc(s, true);
}

void Data::clear(){ fill(0); }

void Data::fill(unsigned char d){
    if( m_mem_write ){
        memset(m_mem_write, d, capacity());
    }
}

int Data::print(u32 o_flags) const {
    const s8 * ptrs8 = (const s8*)data_const();
    const s16 * ptrs16 = (const s16*)data_const();
    const s32 * ptrs32 = (const s32*)data_const();
    const u8 * ptru8 = (const u8*)data_const();
    const u16 * ptru16 = (const u16*)data_const();
    const u32 * ptru32 = (const u32*)data_const();

    int size;
    if( o_flags & PRINT_32 ){
        size = capacity() / 4;
    } else if( o_flags & PRINT_16 ){
        size = capacity() / 2;
    } else {
        size = capacity();
    }

    int i;
    for(i=0; i < size; i++){
        printf("[%d]=", i);
        if( o_flags & PRINT_HEX ){
            if( o_flags & PRINT_32 ){
                printf("%lX", ptru32[i]);
            } else if( o_flags & PRINT_16 ){
                printf("%X", ptru16[i]);
            } else {
                printf("%X", ptru8[i]);
            }
            printf(" ");
        }
        if( o_flags & PRINT_UNSIGNED ){
            if( o_flags & PRINT_32 ){
                printf("%lu", ptru32[i]);
            } else if( o_flags & PRINT_16 ){
                printf("%u", ptru16[i]);
            } else {
                printf("%u", ptru8[i]);
            }
            printf(" ");
        }
        if( o_flags & PRINT_SIGNED ){
            if( o_flags & PRINT_32 ){
                printf("%ld", ptrs32[i]);
            } else if( o_flags & PRINT_16 ){
                printf("%d", ptrs16[i]);
            } else {
                printf("%d", ptrs8[i]);
            }
            printf(" ");
        }
        if( o_flags & PRINT_CHAR ){
            if( ptru8[i] == '\n' ){
                printf(" \\n");
            } else if( ptru8[i] == '\r' ){
                printf(" \\r");
            } else if( ptru8[i] == 0 ){
                printf(" null");
            } else if( ptru8[i] < 128){
                printf(" %c", ptru8[i]);
            }
        }
        printf("\n");
    }
    return capacity();
}

void Data::swap_byte_order(int size){

#if !defined __link
    if( size == 4 ){
        u32 * p = (u32*)data();
        if( p ){
            u32 i;
            for(i=0; i < capacity()/4; i++){
                p[i] = __REV(p[i]);
            }
        } else {
            set_error_number(EINVAL);
        }
    } else {
        u16 * p = (u16*)data();
        if( p ){
            u16 i;
            for(i=0; i < capacity()/2; i++){
                p[i] = __REV16(p[i]);
            }
        } else {
            set_error_number(EINVAL);
        }
    }
#endif

}


