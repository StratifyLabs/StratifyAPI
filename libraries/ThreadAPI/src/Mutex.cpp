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

Mutex::Attributes &Mutex::Attributes::set_prio_ceiling(int value) {
#if !defined __android
  if (pthread_mutexattr_setprioceiling(&m_item, value) < 0) {
    invalidate();
  }
#endif
  return *this;
}

Mutex::Attributes &Mutex::Attributes::set_protocol(Protocol value) {
#if !defined __android
  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::io_error,
    pthread_mutexattr_setprotocol(&m_item, static_cast<int>(value)));
#endif
  return *this;
}

Mutex::Attributes &Mutex::Attributes::set_pshared(bool value) {
  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::io_error,
    pthread_mutexattr_setpshared(&m_item, static_cast<int>(value)));
  return *this;
}

Mutex::Attributes &Mutex::Attributes::set_type(Type value) {
  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::io_error,
    pthread_mutexattr_settype(&m_item, static_cast<int>(value)));
  return *this;
}

int Mutex::Attributes::get_prio_ceiling() const {
  int ret = 0;
#if !defined __android
  pthread_mutexattr_getprioceiling(&m_item, &ret);
#endif
  return ret;
}

int Mutex::Attributes::get_protocol() const {
  int ret = 0;
#if !defined __android
  pthread_mutexattr_getprotocol(&m_item, &ret);
#endif
  return ret;
}

int Mutex::Attributes::get_type() const {
  int ret;
  pthread_mutexattr_gettype(&m_item, &ret);
  return ret;
}

bool Mutex::Attributes::get_pshared() const {
  int ret;
  pthread_mutexattr_getpshared(&m_item, &ret);
  return ret;
}

Mutex::Mutex() {
  Mutex::Attributes attr;
  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::no_memory,
    pthread_mutex_init(&m_item, &(attr.m_item)));
}

Mutex::Mutex(const Mutex::Attributes &attr) { set_attributes(attr); }

Mutex &Mutex::set_attributes(const Attributes &attr) {
  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::io_error,
    pthread_mutex_init(&m_item, &(attr.m_item)));
  return *this;
}

Mutex &Mutex::lock() {
  API_ASSIGN_ERROR_CODE(api::ErrorCode::no_memory, pthread_mutex_lock(&m_item));
  return *this;
}

#if !defined __link
Mutex &Mutex::lock_timed(const chrono::ClockTime &clock_time) {
  ClockTime calc_time = ClockTime::get_system_time();
  calc_time += clock_time;
  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::no_memory,
    pthread_mutex_timedlock(&m_item, calc_time));
  return *this;
}
#endif

Mutex &Mutex::try_lock() {
  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::no_memory,
    pthread_mutex_trylock(&m_item));
  return *this;
}

Mutex &Mutex::unlock() {
  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::no_memory,
    pthread_mutex_unlock(&m_item));
  return *this;
}
