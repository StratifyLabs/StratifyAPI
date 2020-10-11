// Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for
// rights.

#if !defined __link

#include "thread/Sem.hpp"

using namespace thread;

Sem::Sem() { m_handle = nullptr; }

Sem &Sem::close() {
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL("", sem_close(m_handle));
  return *this;
}

Sem &Sem::finalize() {
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL("", sem_destroy(m_handle));
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
  API_RETURN_VALUE_IF_ERROR(*this);
  m_handle = sem;
  API_SYSTEM_CALL("", sem_init(m_handle, pshared, value));
  return *this;
}

Sem &Sem::open(var::StringView name, const Open &options) {
  API_RETURN_VALUE_IF_ERROR(*this);
  m_handle = sem_open(
    name.cstring(),
    options.o_flags(),
    options.mode(),
    options.value());
  if (m_handle == nullptr) {
    API_SYSTEM_CALL("", -1);
  }
  return *this;
}

Sem &Sem::create(var::StringView name, int value, Exclusive exclusive) {
  API_RETURN_VALUE_IF_ERROR(*this);
  int o_flags = O_CREAT;
  if (exclusive == Exclusive::yes) {
    o_flags |= O_EXCL;
  }
  return open(
    name,
    Open().set_o_flags(o_flags).set_mode(0666).set_value(value));
}

Sem &Sem::post() {
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL("", sem_post(m_handle));
  return *this;
}

Sem &Sem::wait_timed(const chrono::ClockTime &timeout) {
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL("", sem_timedwait(m_handle, timeout.timespec()));
  return *this;
}

Sem &Sem::try_wait() {
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL("", sem_trywait(m_handle));
  return *this;
}

Sem &Sem::unlink(var::StringView name) {
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL("", sem_unlink(name.cstring()));
  return *this;
}

Sem &Sem::wait() {
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL("", sem_wait(m_handle));
  return *this;
}

#else
int fs_api_fs_sem_unused;
#endif
