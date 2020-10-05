/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
//Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc


#include "sys/Signal.hpp"
using namespace sys;


int Signal::set_handler(const SignalHandler & handler) const {
#if defined __win32
	signal_function_callback_t ptr = handler.sigaction()->sa_handler;
	::signal(m_signo, ptr);
#else
	if( handler.sigaction()->sa_flags & SIGNAL_SIGINFO_FLAG ){
		return set_error_number_if_error(::sigaction(m_signo, handler.sigaction(), 0));
	} else {
		_sig_func_ptr ptr = (_sig_func_ptr)handler.sigaction()->sa_handler;
		::signal(m_signo, ptr);
	}
#endif
	return 0;
}


