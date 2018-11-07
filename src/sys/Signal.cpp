//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include "sys/Signal.hpp"
using namespace sys;


int Signal::set_handler(const SignalHandler & handler) const {
	if( handler.sigaction()->sa_flags & SIGNAL_SIGINFO_FLAG ){
        return set_error_number_if_error(::sigaction(m_signo, handler.sigaction(), 0));
	} else {
		_sig_func_ptr ptr = handler.sigaction()->sa_handler;
        ::signal(m_signo, ptr);
		return 0;
	}
}

