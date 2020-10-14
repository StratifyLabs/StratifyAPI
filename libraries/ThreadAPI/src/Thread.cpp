/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
// Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#include "thread/Thread.hpp"
#include "chrono.hpp"

#include <cstdio>
#include <errno.h>
using namespace thread;

Thread::Thread(const Construct &options) {
  create(options);
}

Thread::~Thread() {
  // what if thread is still running?
  if (is_running()) {
    // must be stopped or assert
  }
}

bool Thread::is_valid() const { return !is_id_pending() && !is_id_error(); }

Thread &Thread::cancel() {
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL("", pthread_cancel(id()));
  return *this;
}

Thread &Thread::set_cancel_type(CancelType cancel_type) {
  API_RETURN_VALUE_IF_ERROR(*this);
  int old = 0;
  API_SYSTEM_CALL(
    "",
    pthread_setcanceltype(static_cast<int>(cancel_type), &old));
  return *this;
}

Thread &Thread::set_cancel_state(CancelState cancel_state) {
  API_RETURN_VALUE_IF_ERROR(*this);
  int old = 0;
  API_SYSTEM_CALL(
    "",
    pthread_setcancelstate(static_cast<int>(cancel_state), &old));
  return *this;
}

void Thread::create(const Construct &options) {
  API_RETURN_IF_ERROR();
  API_ASSERT(options.function() != nullptr);

  set_id_pending();

  pthread_attr_t pthread_attr;

  API_SYSTEM_CALL("", pthread_attr_init(&pthread_attr));
  API_RETURN_IF_ERROR();

  API_SYSTEM_CALL(
    "",
    pthread_attr_setstacksize(&pthread_attr, options.stack_size()));
  API_RETURN_IF_ERROR();

  API_SYSTEM_CALL(
    "",
    pthread_attr_setdetachstate(
      &pthread_attr,
      static_cast<int>(options.detach_state())));
  API_RETURN_IF_ERROR();

  m_detach_state = options.detach_state();

  API_SYSTEM_CALL(
    "",
    pthread_attr_setschedpolicy(
      &pthread_attr,
      static_cast<int>(options.policy())));

  API_RETURN_IF_ERROR();

  struct sched_param param = {0};
  param.sched_priority = options.priority();
  API_SYSTEM_CALL("", pthread_attr_setschedparam(&pthread_attr, &param));
  API_RETURN_IF_ERROR();

  // First create the thread
#if defined __link
  int result =
#endif
    API_SYSTEM_CALL(
      "",
      pthread_create(
        &m_id,
        &pthread_attr,
        options.function(),
        options.argument()));

  pthread_attr_destroy(&pthread_attr);

#if defined __link
  if (result == 0) {
    m_status = 0;
  }
#endif
}

bool Thread::is_running() {
  API_RETURN_VALUE_IF_ERROR(false);

  // check to see if the thread is running
  if (is_id_pending() || is_id_error()) {
    return false;
  }

  if (pthread_kill(m_id, 0) == 0) {
    return true;
  }
  API_RESET_ERROR();

  destroy();
  return false;
}

Thread &Thread::wait(void **ret, chrono::MicroTime interval) {

  if (is_valid()) {

    // if thread is joinable, then join it
    if (is_joinable()) {
      join(ret);
    } else {
      // just keep sampling until the thread completes
      while (is_running()) {
        chrono::wait(interval);
      }
    }
  }
  return *this;
}

void Thread::destroy() {
#if defined __link
  m_status = id_completed;
#else
  m_id = id_completed;
#endif
}

Thread &Thread::join(void **value) {
  API_RETURN_VALUE_IF_ERROR(*this);

  void *tmp_ptr;
  void **ptr = value == nullptr ? &tmp_ptr : value;

  const int local_result = API_SYSTEM_CALL("", pthread_join(id(), ptr));
  if (local_result == 0) {
    // resets the thread that just completed

    is_running();
  }
  return *this;
}
