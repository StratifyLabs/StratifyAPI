//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "var/Ring.hpp"

namespace var {


Ring::Ring(u8 * buf, u32 size) : Data(buf, size){
	m_size = size;
	m_head = 0;
	m_tail = 0;
	m_is_overflow_allowed = true;
}

Ring::Ring(u32 size) : Data(size){
    m_size = size;
    m_head = 0;
    m_tail = 0;
    m_is_overflow_allowed = true;
}


int Ring::write(const u8 * buf, u32 nbytes){
	u32 i;
	u32 free_bytes;

	if( !m_is_overflow_allowed ){
		free_bytes = calc_free();
		if( nbytes > free_bytes ){
			nbytes = free_bytes;
		}
	}

	for(i=0; i < nbytes; i++){
        cdata()[m_head] = buf[i];
		m_head++;
		if( m_head == m_tail ){
			m_tail++;
			if( m_tail == m_size ){
				m_tail = 0;
			}
		}
		if( m_head == m_size ){
			if( m_tail == 0 ){
				m_tail++;
			}
			m_head = 0;
		}
	}
	return nbytes;
}

int Ring::read(u8 * buf, u32 size){
	u32 i;

	for(i=0; i < size; i++){
		if( m_tail != m_head ){
            buf[i] = cdata()[m_tail];
			m_tail++;
			if( m_tail == m_size ){
				m_tail = 0;
			}
		} else {
			return i;
		}
	}

	return i;
}

u32 Ring::calc_used() const {
	if( m_head >= m_tail ){
		return m_head - m_tail;
	} else {
		return m_size - ( m_tail - m_head );
	}
}

u32 Ring::calc_free() const {
	if( m_head >= m_tail ){
		return m_size - (m_head - m_tail);
	} else {
		return (m_tail - m_head);
	}
}


void Ring::flush(void){
	m_head = 0;
	m_tail = 0;
}


} /* namespace var */
