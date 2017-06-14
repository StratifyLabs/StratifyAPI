//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "var/Ring.hpp"

namespace var {


Ring::Ring(u8 * buf, size_t size){
	m_buf = buf;
	m_size = size;
	m_head = 0;
	m_tail = 0;
	m_is_overflow_allowed = true;
}


int Ring::write(const u8 * buf, size_t nbytes){
	u32 i;
	size_t free_bytes;

	if( !m_is_overflow_allowed ){
		free_bytes = calc_free();
		if( nbytes > free_bytes ){
			nbytes = free_bytes;
		}
	}

	for(i=0; i < nbytes; i++){
		m_buf[m_head] = buf[i];
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

int Ring::read(u8 * buf, size_t size){
	u32 i;

	for(i=0; i < size; i++){
		if( m_tail != m_head ){
			buf[i] = m_buf[m_tail];
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

size_t Ring::calc_used() const {
	if( m_head >= m_tail ){
		return m_head - m_tail;
	} else {
		return m_size - ( m_tail - m_head );
	}
}

size_t Ring::calc_free() const {
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
