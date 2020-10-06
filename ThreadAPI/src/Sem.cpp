// Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for
// rights.

#if !defined __link

#include "ThreadAPI/Sem.hpp"

using namespace thread;

Sem::Sem() { m_handle = nullptr; }

Sem &Sem::close() {
  API_ASSIGN_ERROR_CODE(api::ErrorCode::io_error, sem_close(m_handle));
  return *this;
}

Sem &Sem::finalize() {
  API_ASSIGN_ERROR_CODE(api::ErrorCode::io_error, sem_destroy(m_handle));
  return *this;
}

int Sem::get_value() const {
  int value;
  int ret;
  ret = sem_getvalue(m_handle, &value);
  if (ret < 0) {
    return ret;
  }
  return value;
}

Sem &Sem::initialize(sem_t *sem, int pshared, unsigned int value) {
  m_handle = sem;
  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::io_error,
    sem_init(m_handle, pshared, value));
}

Sem &Sem::open(const var::StringView &name, const OpenOptions &options) {
  m_handle = sem_open(
    name.cstring(),
    options.o_flags(),
    options.mode(),
    options.value());
  if (m_handle == nullptr) {
    API_ASSIGN_ERROR_CODE(api::ErrorCode::io_error, -1);
  }
  return *this;
}

Sem &Sem::create(const var::StringView &name, int value, Exclusive exclusive) {
  int o_flags = O_CREAT;
  if (exclusive == Exclusive::yes) {
    o_flags |= O_EXCL;
  }
  return open(
    name,
    OpenOptions().set_o_flags(o_flags).set_mode(0666).set_value(value));
}

Sem &Sem::post() {
  API_ASSIGN_ERROR_CODE(api::ErrorCode::io_error, sem_post(m_handle));
  return *this;
}

Sem &Sem::wait_timed(const chrono::ClockTime &timeout) {
  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::io_error,
    sem_timedwait(m_handle, timeout.timespec()));
  return *this;
}

Sem &Sem::try_wait() {
  API_ASSIGN_ERROR_CODE(api::ErrorCode::io_error, sem_trywait(m_handle));
  return *this;
}

Sem &Sem::unlink(const var::StringView &name) {
  API_ASSIGN_ERROR_CODE(api::ErrorCode::io_error, sem_unlink(name.cstring()));
  return *this;
}

Sem &Sem::wait() {
  API_ASSIGN_ERROR_CODE(api::ErrorCode::io_error, sem_wait(m_handle));
  return *this;
}

#endif
