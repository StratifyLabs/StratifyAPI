/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
// Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#include "thread/Thread.hpp"
#include "chrono.hpp"

#include <cstdio>
#include <errno.h>
using namespace thread;

Thread::Attributes::Attributes() {
  API_RETURN_IF_ERROR();
  API_SYSTEM_CALL("", pthread_attr_init(&m_pthread_attr));
  set_inherit_sched(IsInherit::yes);
  set_stack_size(4096);
  set_detach_state(DetachState::detached);
}

Thread::Attributes::~Attributes() {
  API_RETURN_IF_ERROR();
  API_SYSTEM_CALL("", pthread_attr_destroy(&m_pthread_attr));
}

Thread::Attributes &Thread::Attributes::set_stack_size(size_t value) {
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL("", pthread_attr_setstacksize(&m_pthread_attr, value));
  return *this;
}

int Thread::Attributes::get_stack_size() const {
  API_RETURN_VALUE_IF_ERROR(-1);
  size_t stack_size = 0;
  API_SYSTEM_CALL("", pthread_attr_getstacksize(&m_pthread_attr, &stack_size));
  return stack_size;
}

Thread::Attributes &Thread::Attributes::set_detach_state(DetachState value) {
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL(
    "",
    pthread_attr_setdetachstate(&m_pthread_attr, static_cast<int>(value)));
  return *this;
}

Thread::DetachState Thread::Attributes::get_detach_state() const {
  API_RETURN_VALUE_IF_ERROR(DetachState::detached);
  int detach_state = 0;
  API_SYSTEM_CALL(
    "",
    pthread_attr_getdetachstate(&m_pthread_attr, &detach_state));
  return static_cast<DetachState>(detach_state);
}

Thread::Attributes &Thread::Attributes::set_inherit_sched(IsInherit value) {
  API_RETURN_VALUE_IF_ERROR(*this);
  int sched_value
    = (value == IsInherit::no) ? PTHREAD_EXPLICIT_SCHED : PTHREAD_INHERIT_SCHED;
  API_SYSTEM_CALL(
    "",
    pthread_attr_setinheritsched(&m_pthread_attr, sched_value));
  return *this;
}
Thread::IsInherit Thread::Attributes::get_inherit_sched() const {
  API_RETURN_VALUE_IF_ERROR(IsInherit::yes);
  int inherit_sched = 0;
  API_SYSTEM_CALL(
    "",
    pthread_attr_getdetachstate(&m_pthread_attr, &inherit_sched));
  return (inherit_sched == PTHREAD_EXPLICIT_SCHED) ? IsInherit::no
                                                   : IsInherit::yes;
}

Thread::Attributes &Thread::Attributes::set_scope(ContentionScope value) {
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL(
    "",
    pthread_attr_setscope(&m_pthread_attr, static_cast<int>(value)));
  return *this;
}

Thread::ContentionScope Thread::Attributes::get_scope() const {
  API_RETURN_VALUE_IF_ERROR(ContentionScope::process);
  int value = 0;
  API_SYSTEM_CALL("", pthread_attr_getdetachstate(&m_pthread_attr, &value));
  return static_cast<ContentionScope>(value);
}

Thread::Attributes &Thread::Attributes::set_sched_priority(int priority) {
  API_RETURN_VALUE_IF_ERROR(*this);
  struct sched_param param = {0};
  param.sched_priority = priority;
  API_SYSTEM_CALL("", pthread_attr_setschedparam(&m_pthread_attr, &param));
  return *this;
}

Thread::Attributes &Thread::Attributes::set_sched_policy(Sched::Policy value) {
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL(
    "",
    pthread_attr_setschedpolicy(&m_pthread_attr, static_cast<int>(value)));
  return *this;
}

Sched::Policy Thread::Attributes::get_sched_policy() const {
  API_RETURN_VALUE_IF_ERROR(Sched::Policy::round_robin);
  int value = 0;
  API_SYSTEM_CALL("", pthread_attr_getschedpolicy(&m_pthread_attr, &value));
  return static_cast<Policy>(value);
}

int Thread::Attributes::get_priority() const {
  API_RETURN_VALUE_IF_ERROR(-1);
  struct sched_param param = {0};
  API_SYSTEM_CALL("", pthread_attr_getschedparam(&m_pthread_attr, &param));
  return param.sched_priority;
}

Thread::Thread(const Construct &options, const Attributes &attributes) {
  API_RETURN_IF_ERROR();
  API_ASSERT(options.function() != nullptr);

  set_id_pending();

  m_detach_state = attributes.get_detach_state();

  // First create the thread
#if defined __link
  int result =
#endif
    API_SYSTEM_CALL(
      "",
      pthread_create(
        &m_id,
        &attributes.m_pthread_attr,
        options.function(),
        options.argument()));

#if defined __link
  if (result == 0) {
    m_private_context = 0;
  }
#endif
}

Thread::~Thread() {
  // what if thread is still running?
  API_ASSERT(is_running() == false);
}

Thread &Thread::set_sched_parameters(Sched::Policy policy, int priority) {
  API_RETURN_VALUE_IF_ERROR(*this);
  struct sched_param param = {0};
  param.sched_priority = priority;
  API_SYSTEM_CALL(
    "",
    pthread_setschedparam(id(), static_cast<int>(policy), &param));
  return *this;
}

int Thread::get_sched_policy() const {
  int policy;
  int priority;
  get_sched_parameters(policy, priority);
  return policy;
}

int Thread::get_sched_priority() const {
  int policy;
  int priority;
  get_sched_parameters(policy, priority);
  return priority;
}

int Thread::get_sched_parameters(int &policy, int &priority) const {
  API_RETURN_VALUE_IF_ERROR(-1);
  if (is_id_pending() || is_id_error()) {
    return -1;
  }
  struct sched_param param = {0};
  int result
    = API_SYSTEM_CALL("", pthread_getschedparam(id(), &policy, &param));
  priority = param.sched_priority;
  return result;
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
  m_private_context = id_completed;
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
