/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
// Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#include "thread/Signal.hpp"
using namespace thread;

Signal &Signal::set_handler(const SignalHandler &handler) {
#if defined __win32
  signal_function_callback_t ptr = handler.sigaction()->sa_handler;
  ::signal(m_signo, ptr);
#else
  if (handler.sigaction()->sa_flags & SIGNAL_SIGINFO_FLAG) {
    API_ASSIGN_ERROR_CODE(
      api::ErrorCode::io_error,
      ::sigaction(m_signo, handler.sigaction(), 0));

  } else {
    _sig_func_ptr ptr = (_sig_func_ptr)handler.sigaction()->sa_handler;
    ::signal(m_signo, ptr);
  }
#endif
  return *this;
}
