/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
// Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#include "thread/Thread.hpp"
#include "chrono.hpp"

#include <cstdio>
#include <errno.h>
using namespace thread;

Thread::Thread(const Options &options) {
  init(options.stack_size(), options.detach_state() == DetachState::detached);
}

Thread::~Thread() {
  // what if thread is still running?
  if (is_running()) {
    // must be stopped or assert
  }

  if (!is_id_error()) {
    pthread_attr_destroy(&m_pthread_attr);
  }
}

bool Thread::is_valid() const { return !is_id_pending() && !is_id_error(); }

int Thread::init(int stack_size, bool detached) {
  set_id_error();

  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::io_error,
    pthread_attr_init(&m_pthread_attr));

  if (status().is_error()) {
    return -1;
  }

  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::io_error,
    pthread_attr_setstacksize(&m_pthread_attr, stack_size));

  if (status().is_error()) {
    return -1;
  }

  if (detached == true) {

    API_ASSIGN_ERROR_CODE(
      api::ErrorCode::io_error,
      pthread_attr_setdetachstate(&m_pthread_attr, PTHREAD_CREATE_DETACHED));

    if (status().is_error()) {
      return -1;
    }
  } else {

    API_ASSIGN_ERROR_CODE(
      api::ErrorCode::io_error,
      pthread_attr_setdetachstate(&m_pthread_attr, PTHREAD_CREATE_JOINABLE));

    if (status().is_error()) {
      return -1;
    }
  }

  set_id_pending();
  return 0;
}

Thread &Thread::set_stacksize(int size) {

  if (is_running()) {
    API_ASSIGN_ERROR_CODE(api::ErrorCode::busy, -1);
    return *this;
  }

  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::io_error,
    pthread_attr_setstacksize(&m_pthread_attr, size));

  return *this;
}

int Thread::get_stacksize() {
  u32 stacksize = 0;
  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::io_error,
    pthread_attr_getstacksize(&m_pthread_attr, (size_t *)&stacksize));
  return stacksize;
}

Thread::DetachState Thread::get_detachstate() {
  int value = 0;
  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::io_error,
    pthread_attr_getdetachstate(&m_pthread_attr, &value));
  return static_cast<DetachState>(value);
}

Thread &Thread::set_detachstate(DetachState value) {

  if (is_running()) {
    API_ASSIGN_ERROR_CODE(api::ErrorCode::busy, -1);
    return *this;
  }

  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::io_error,
    pthread_attr_setdetachstate(&m_pthread_attr, static_cast<int>(value)));

  return *this;
}

Thread &Thread::set_priority(int prio, Sched::Policy policy) {
  if (is_valid()) {
    struct sched_param param = {0};
    param.sched_priority = prio;
    API_ASSIGN_ERROR_CODE(
      api::ErrorCode::io_error,
      pthread_setschedparam(m_id, static_cast<int>(policy), &param));
  } else {
    API_ASSIGN_ERROR_CODE(api::ErrorCode::busy, -1);
  }
  return *this;
}

int Thread::get_priority() {

  if (is_valid()) {
    struct sched_param param;
    int policy;
    API_ASSIGN_ERROR_CODE(
      api::ErrorCode::io_error,
      pthread_getschedparam(m_id, &policy, &param));

    if (status().is_error()) {
      return -1;
    }

    return param.sched_priority;
  }

  API_ASSIGN_ERROR_CODE(api::ErrorCode::busy, -1);
  return -1;
}

Thread &Thread::cancel() {
  API_ASSIGN_ERROR_CODE(api::ErrorCode::io_error, pthread_cancel(id()));
  return *this;
}

int Thread::set_cancel_type(CancelType cancel_type) {
  return pthread_setcanceltype(static_cast<int>(cancel_type), nullptr);
}

int Thread::set_cancel_state(CancelState cancel_state) {
  return pthread_setcancelstate(static_cast<int>(cancel_state), nullptr);
}

int Thread::get_policy() {
  struct sched_param param;
  int policy;
  if (is_valid()) {

    API_ASSIGN_ERROR_CODE(
      api::ErrorCode::io_error,
      pthread_getschedparam(m_id, &policy, &param));

    if (status().is_error()) {
      return -1;
    }
  } else {
    API_ASSIGN_ERROR_CODE(api::ErrorCode::busy, -1);
    return -1;
  }
  return policy;
}

Thread &Thread::create(const CreateOptions &options) {
  reset();

  if (status().is_error()) {
    return *this;
  }

  if (!is_id_pending()) {
    API_ASSIGN_ERROR_CODE(api::ErrorCode::busy, -1);
    return *this;
  }

  if (
    API_ASSIGN_ERROR_CODE(
      api::ErrorCode::io_error,
      pthread_attr_setschedpolicy(
        &m_pthread_attr,
        static_cast<int>(options.policy())))
    < 0) {
    return *this;
  }

  struct sched_param param = {0};
  param.sched_priority = options.priority();
  if (
    API_ASSIGN_ERROR_CODE(
      api::ErrorCode::io_error,
      pthread_attr_setschedparam(&m_pthread_attr, &param))
    < 0) {
    return *this;
  }

  // First create the thread
  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::io_error,
    pthread_create(
      &m_id,
      &m_pthread_attr,
      options.function(),
      options.argument()));

  return *this;
}

bool Thread::is_running() {
  // check to see if the thread is running
  if (is_id_pending() || is_id_error()) {
    return false;
  }
  if (pthread_kill(m_id, 0) == 0) {
    return true;
  }

  reset();
  return false;
}

Thread &Thread::wait(void **ret, const chrono::Microseconds &interval) {

  void *dummy;

  if (is_valid()) {

    // if thread is joinable, then join it
    if (is_joinable()) {
      if (ret != 0) {
        join(ret);
      } else {
        join(&dummy);
      }
    } else {
      // just keep sampling until the thread completes
      while (is_running()) {
        interval.wait();
      }
    }
  }
  return *this;
}

Thread &Thread::reset() {

  if (is_id_pending()) {
    return *this;
  }

  if (is_valid() && (pthread_kill(m_id, 0) < 0)) {
    bool detached = !is_joinable();
    u32 stacksize = get_stacksize();

    if (
      API_ASSIGN_ERROR_CODE(
        api::ErrorCode::io_error,
        pthread_attr_destroy(&m_pthread_attr))
      < 0) {
      return *this;
    }

    init(stacksize, detached);
    return *this;
  }

  API_ASSIGN_ERROR_CODE(api::ErrorCode::busy, -1);
  return *this;
}

int Thread::join(Thread &thread_to_join, void **result) {
  void *tmp_ptr;
  void **ptr;
  if (result == nullptr) {
    ptr = &tmp_ptr;
  } else {
    ptr = result;
  }
  int local_result = pthread_join(thread_to_join.id(), ptr);
  if (local_result == 0) {
    // resets the thread that just completed
    thread_to_join.is_running();
  }
  return local_result;
}

Thread &Thread::join(void **value) {
  API_ASSIGN_ERROR_CODE(api::ErrorCode::io_error, join(*this, value));
  return *this;
}

bool Thread::is_joinable() {
  return get_detachstate() == DetachState::joinable;
}
