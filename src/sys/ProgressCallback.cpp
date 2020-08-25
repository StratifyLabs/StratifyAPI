/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "sys/ProgressCallback.hpp"

using namespace sys;

ProgressCallback::ProgressCallback(){}

bool ProgressCallback::update(int value, int total) const {
	if( m_callback ){
		return m_callback(context(), value, total);
	}
	//do not abort the operation
	return false;
}

int ProgressCallback::update_function(const void * context, int value, int total){
	if( context == nullptr ){
		return 0;
	}
	return ((ProgressCallback*)context)->update(value,total);
}
