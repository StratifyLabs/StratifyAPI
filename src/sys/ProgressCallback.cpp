/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "sys/ProgressCallback.hpp"

using namespace sys;

ProgressCallback::ProgressCallback(){
	m_update = 0;
	m_context = 0;
}

ProgressCallback::ProgressCallback(callback_t callback, void * context){
	m_update = callback;
	m_context = context;
}

bool ProgressCallback::update(int value, int total) const {
	if( m_update ){
		return m_update(m_context, value, total);
	}
	//do not abort the operation
	return false;
}

int ProgressCallback::update_function(const void * context, int value, int total){
	const ProgressCallback * progress_callback = (const ProgressCallback*)context;
	if( progress_callback ){
		return progress_callback->update(value, total);
	}
	return 0;
}
