//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "var/Ring.hpp"

namespace var {

RingBuffer::RingBuffer(void * buf, u32 size) : Data(buf, size){
	m_size = size;
	m_head = 0;
	m_tail = 0;
	m_is_overflow_allowed = true;
}

RingBuffer::RingBuffer(u32 size) : Data(size){
    m_size = size;
    m_head = 0;
    m_tail = 0;
    m_is_overflow_allowed = true;
}

u32 RingBuffer::calc_used() const {
    if( m_tail == m_size ){
        return m_size;
    } else if( m_head >= m_tail ){
        return m_head - m_tail;
    } else {
        return m_size - ( m_tail - m_head );
    }
}

u32 RingBuffer::calc_free() const {
    return m_size - calc_used();
}

void RingBuffer::flush(void){
	m_head = 0;
	m_tail = 0;
}

void RingBuffer::inc_tail(){
    m_tail++;
    if( m_tail == m_size ){
        m_tail = 0;
    }
}

void RingBuffer::inc_head(){
    m_head++;
    if( m_head == m_size ){
        m_head = 0;
    }

    if( m_head == m_tail ){
        m_tail = m_size; //buffer is full
    }
}

u32 RingBuffer::get_free_count(u32 count) const {
    if( !m_is_overflow_allowed ){
        u32 free_count;
        free_count = calc_free();
        if( count > free_count ){
            count = free_count;
        }
    }
    return count;
}


} /* namespace var */
