//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include <cstdlib>
#include <cstring>
#include "fmt/Son.hpp"

using namespace fmt;


#if defined __link
Son::Son(void * driver, u16 max_depth, son_stack_t * stack){
	memset(&m_son, 0, sizeof(m_son));
	m_stack_size = max_depth;
	if( stack == 0 ){
		m_is_stack_needs_free = true;
		m_stack = (son_stack_t*)malloc(max_depth * sizeof(son_stack_t));
	} else {
		m_is_stack_needs_free = false;
		m_stack = stack;
	}
	set_driver(driver);
}
#else
Son::Son(u16 max_depth, son_stack_t * stack){
	memset(&m_son, 0, sizeof(m_son));
	m_stack_size = max_depth;
	if( stack == 0 ){
		m_is_stack_needs_free = true;
		m_stack = (son_stack_t*)malloc(max_depth * sizeof(son_stack_t));
	} else {
		m_is_stack_needs_free = false;
		m_stack = stack;
	}
}
#endif

Son::~Son(){
	if( m_is_stack_needs_free ){
		free(m_stack);
	}
}

