/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#include "api/api.hpp"
// #include "var/String.hpp"

#include <pthread.h>

#include <cstdio>
#include <cstring>

#define ERROR_CODE_CASE(c)                                                     \
  case c:                                                                      \
    return MCU_STRINGIFY(c)

using namespace api;

const char *ApiInfo::version() {
  return MCU_STRINGIFY(__PROJECT_VERSION_MAJOR) "." MCU_STRINGIFY(
    __PROJECT_VERSION_MINOR) "." MCU_STRINGIFY(__PROJECT_VERSION_PATCH);
}

const char *ApiInfo::git_hash() { return SOS_GIT_HASH; }

void api::api_assert(bool value, const char *function, int line) {
  if (!value) {
    printf("assertion %s():%d\n", function, line);
#if defined __link && !defined __win32
    void *array[200];
    size_t size;
    size = backtrace(array, 200);
    backtrace_symbols_fd(array, size, stderr->_file);
#endif
    ::abort();
  }
}

PrivateExecutionContext ExecutionContext::m_private_context;

Error &PrivateExecutionContext::get_error() {
  if (&(errno) == m_error.m_signature) {
    return m_error;
  }

  if (m_error_list == nullptr) {
    m_error_list = new std::vector<Error>();
    API_ASSERT(m_error_list != nullptr);
    m_error_list->push_back(Error(&(errno)));
  }

  for (Error &error_context : *m_error_list) {
    if (m_error.m_signature == &(errno)) {
      return m_error;
    }
  }

  API_ASSERT(true);
  return m_error;
}

void PrivateExecutionContext::update_error_context(
  int line,
  const char *message) {
  strncpy(m_error.m_message, message, Error::m_message_size);
  m_error.m_line_number = line;
  m_error.m_error_number = errno;
  m_error.capture_backtrace();
  errno = -1;
}

#define RESULT_ERROR_CODE_CASE(c)                                              \
  case ErrorCode::c:                                                           \
    return MCU_STRINGIFY(c)

void ExecutionContext::exit_fatal(const char *message) {
  printf("fatal error: %s\n", message);
  exit(1);
}

ProgressCallback::ProgressCallback() {}

bool ProgressCallback::update(int value, int total) const {
  if (m_callback) {
    return m_callback(context(), value, total);
  }
  // do not abort the operation
  return false;
}

int ProgressCallback::update_function(
  const void *context,
  int value,
  int total) {
  if (context == nullptr) {
    return 0;
  }
  return ((ProgressCallback *)context)->update(value, total);
}
