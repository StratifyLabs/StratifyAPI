/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#include "api/api.hpp"
// #include "var/String.hpp"

#include <pthread.h>

#include <cstdio>
#include <cstring>

#if defined __link
#include <string>
#define API_MINIMUM_CHUNK_SIZE 1024
#define API_MALLOC_CHUNK_SIZE 1024
#if !defined __win32
#include <execinfo.h>
#endif
#else
#include <mcu/arch.h>
#define API_MALLOC_CHUNK_SIZE MALLOC_CHUNK_SIZE
#define API_MINIMUM_CHUNK_SIZE (MALLOC_CHUNK_SIZE - 12)
#endif

#define ERROR_CODE_CASE(c)                                                     \
  case c:                                                                      \
    return MCU_STRINGIFY(c)

using namespace api;

const char *ApiInfo::git_hash() { return SOS_GIT_HASH; }

const char *ApiInfo::operating_system_name() {
#if defined __link

#if defined __macosx
  return "macosx";
#elif defined __win32
  return "windows";
#elif defined __linux
  return "linux";
#else
  return "unknown";
#endif

#else
  return "stratifyos";
#endif
}

const char *ApiInfo::system_processor() {
#if defined __processor_i386 || defined __processor_x86
  return "i386";
#elif defined __processor_x86_64 || defined __processor_AMD64
  return "x86_64";
#elif defined __processor_arm || defined __processor_armv7l
  return "arm32";
#elif defined __processor_aarch64
  return "arm64";
#else
  return "unknown";
#endif
}

bool ApiInfo::is_windows() {
  return strcmp(operating_system_name(), "windows") == 0;
}
bool ApiInfo::is_macosx() {
  return strcmp(operating_system_name(), "macosx") == 0;
}
bool ApiInfo::is_linux() {
  return strcmp(operating_system_name(), "linux") == 0;
}
bool ApiInfo::is_stratify_os() {
  return strcmp(operating_system_name(), "stratifyos") == 0;
}

bool ApiInfo::is_processor_i386() {
  return strcmp(system_processor(), "i386") == 0;
}
bool ApiInfo::is_processor_x86_64() {
  return strcmp(system_processor(), "x86_64") == 0;
}
bool ApiInfo::is_processor_arm32() {
  return strcmp(system_processor(), "arm32") == 0;
}
bool ApiInfo::is_processor_arm64() {
  return strcmp(system_processor(), "arm64") == 0;
}

u32 ApiInfo::malloc_start_chunk_size() { return API_MINIMUM_CHUNK_SIZE; }
u32 ApiInfo::malloc_chunk_size() { return API_MALLOC_CHUNK_SIZE; }

const char *ApiInfo::user_data_path() {

#if defined __link

  static std::string result;
  result = std::string();

  std::string path;
  if (is_windows()) {
    path = getenv("LocalAppData");
    result += path;
  }

  if (is_macosx()) {
    // read env home variable
    path = getenv("HOME");
    result += path + "/Library/Application Support";
  }

  if (is_linux()) {
    path = getenv("HOME");
    result += path + "/.sl";
  }

  return result.c_str();
#else
  return "/home";
#endif
}

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

ErrorContext &Status::error_context() {
  if (&(errno) == m_error_context.m_context) {
    return m_error_context;
  }

  if (m_error_context_list == nullptr) {
    m_error_context_list = new std::vector<ErrorContext>();
    API_ASSERT(m_error_context_list != nullptr);
    m_error_context_list->push_back(ErrorContext(&(errno)));
  }

  for (ErrorContext &error_context : *m_error_context_list) {
    if (error_context.m_context == &(errno)) {
      return error_context;
    }
  }

  API_ASSERT(true);
  return m_error_context;
}

#define RESULT_ERROR_CODE_CASE(c)                                              \
  case ErrorCode::c:                                                           \
    return MCU_STRINGIFY(c)

const char *Object::error_code_description(ErrorCode error_code) {
  switch (error_code) {
    RESULT_ERROR_CODE_CASE(none);
    RESULT_ERROR_CODE_CASE(permissions);
    RESULT_ERROR_CODE_CASE(no_entity);
    RESULT_ERROR_CODE_CASE(no_such_process);
    RESULT_ERROR_CODE_CASE(io_error);
    RESULT_ERROR_CODE_CASE(bad_file_number);
    RESULT_ERROR_CODE_CASE(busy);
    RESULT_ERROR_CODE_CASE(no_memory);
    RESULT_ERROR_CODE_CASE(aborted);
    RESULT_ERROR_CODE_CASE(invalid_value);
    RESULT_ERROR_CODE_CASE(missing_system_api);
  }
  return "unknown";
}

const char *Status::error_code_description() const {
  return Object::error_code_description(error_code());
}

void Object::exit_fatal(const char *message) {
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
