/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
/* Copyright 2016 tgil All Rights Reserved */

#include "thread/Mutex.hpp"
#include "chrono.hpp"

#include <time.h>

using namespace thread;

Mutex::Attributes::Attributes() {
  validate();
  if (pthread_mutexattr_init(&m_item) < 0) {
    invalidate();
  }
}

Mutex::Attributes::~Attributes() {
  pthread_mutexattr_destroy(&m_item);
  invalidate();
}

Mutex::Attributes &Mutex::Attributes::set_priority_ceiling(int value) {
#if !defined __android
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL("", pthread_mutexattr_setprioceiling(&m_item, value));
#endif
  return *this;
}

Mutex::Attributes &Mutex::Attributes::set_protocol(Protocol value) {
#if !defined __android
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL(
    "",
    pthread_mutexattr_setprotocol(&m_item, static_cast<int>(value)));
#endif
  return *this;
}

Mutex::Attributes &Mutex::Attributes::set_process_shared(bool value) {
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL(
    "",
    pthread_mutexattr_setpshared(&m_item, static_cast<int>(value)));
  return *this;
}

Mutex::Attributes &Mutex::Attributes::set_type(Type value) {
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL(
    "",
    pthread_mutexattr_settype(&m_item, static_cast<int>(value)));
  return *this;
}

int Mutex::Attributes::get_priority_ceiling() const {
  int ret = 0;
#if !defined __android
  API_RETURN_VALUE_IF_ERROR(-1);
  API_SYSTEM_CALL("", pthread_mutexattr_getprioceiling(&m_item, &ret));
#endif
  return ret;
}

int Mutex::Attributes::get_protocol() const {
  int ret = 0;
#if !defined __android
  API_RETURN_VALUE_IF_ERROR(-1);
  API_SYSTEM_CALL("", pthread_mutexattr_getprotocol(&m_item, &ret));
#endif
  return ret;
}

int Mutex::Attributes::get_type() const {
  int ret;
  API_RETURN_VALUE_IF_ERROR(-1);
  API_SYSTEM_CALL("", pthread_mutexattr_gettype(&m_item, &ret));
  return ret;
}

bool Mutex::Attributes::get_process_shared() const {
  int ret = 0;
  API_RETURN_VALUE_IF_ERROR(false);
  API_SYSTEM_CALL("", pthread_mutexattr_getpshared(&m_item, &ret));
  return ret;
}

Mutex::Mutex() {
  Mutex::Attributes attr;
  API_RETURN_IF_ERROR();
  API_SYSTEM_CALL("", pthread_mutex_init(&m_mutex, &(attr.m_item)));
}

Mutex::Mutex(const Mutex::Attributes &attr) { set_attributes(attr); }

Mutex::~Mutex() {
  API_RETURN_IF_ERROR();
  API_SYSTEM_CALL("", pthread_mutex_destroy(&m_mutex));
}

Mutex &Mutex::set_attributes(const Attributes &attr) {
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL("", pthread_mutex_init(&m_mutex, &(attr.m_item)));
  return *this;
}

Mutex &Mutex::lock() {
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL("", pthread_mutex_lock(&m_mutex));
  return *this;
}

#if !defined __link
Mutex &Mutex::lock_timed(const chrono::ClockTime &clock_time) {
  API_RETURN_VALUE_IF_ERROR(*this);
  ClockTime calc_time = ClockTime::get_system_time();
  calc_time += clock_time;
  API_SYSTEM_CALL("", pthread_mutex_timedlock(&m_item, calc_time));
  return *this;
}
#endif

bool Mutex::try_lock() {
  API_RETURN_VALUE_IF_ERROR(false);
  if (pthread_mutex_trylock(&m_mutex) == 0) {
    return true;
  }
  API_RESET_ERROR();
  return false;
}

Mutex &Mutex::unlock() {
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL("", pthread_mutex_unlock(&m_mutex));
  return *this;
}
