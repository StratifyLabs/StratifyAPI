/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#include <cstring>
#include <sched.h>
#include <unistd.h>

#include "ThreadAPI/Sched.hpp"

namespace thread {

int Sched::get_priority_max(Policy value) {
  return sched_get_priority_max(static_cast<int>(value));
}

int Sched::get_priority_min(Policy value) {
  return sched_get_priority_min(static_cast<int>(value));
}

int Sched::get_priority(pid_t pid) {
  struct sched_param param;
  if (sched_getparam(pid, &param) < 0) {
    return -1;
  }
  return param.sched_priority;
}

int Sched::get_rr_interval(pid_t pid) {
  struct timespec t;
  memset(&t, 0, sizeof(t));
  sched_rr_get_interval(pid, &t);
  return t.tv_nsec / 1000;
}

Sched &Sched::set_scheduler(const SetSchedulerOptions &options) {
  struct sched_param param;
  param.sched_priority = options.priority();
  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::io_error,
    sched_setscheduler(
      options.pid(),
      static_cast<int>(options.policy()),
      &param));
  return *this;
}

} // namespace thread
