/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include <cstdlib>
#include <cstring>
#include "fmt/Son.hpp"

using namespace fmt;

SonApi Son::m_api;


Son::Son(u16 max_depth, son_stack_t * stack){
	if( api().is_valid() == false ){
		exit_fatal("son api missing");
	}
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


Son::~Son(){
	if( m_is_stack_needs_free ){
		free(m_stack);
	}
}

