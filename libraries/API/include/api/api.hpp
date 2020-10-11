/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

// sl sdk.copyright:message="'Copyright 2011-2020 Tyler Gilbert and Stratify
// Labs, Inc; see LICENSE.md for rights.'",path=StratifyAPI,prefix='/*! \file */
// //',filter=cmake_link?cmake_arm?rapidjson?rapidxml --verbose=message

#ifndef API_API_HPP_
#define API_API_HPP_

#include <cstring>
#include <errno.h>

#include <vector>

#include "macros.hpp"

#if defined __link
#if !defined __win32
#include <execinfo.h>
#endif
#endif

/*!
 * \brief Application Programming Interface
 *
 * \details
 *
 * **Introduction**
 *
 * The Stratify API library contains classes that allow easy access
 * to Stratify OS hardware and POSIX constructs such as threads (sys::Thread),
 * mutexes (sys::Mutex), files (fs::File) and many more. It also contains
 * classes used to manage data (see the var namespace) in an embedded
 * friendly way.
 *
 * If you are just getting started with Stratify OS and the Stratify API,
 * try reading through some of the guides:
 *
 * - [Stratify OS](https://docs.stratifylabs.co/guides/Guide-Stratify-OS/)
 * - [Filesystems](https://docs.stratifylabs.co/guides/Guide-Filesystems/)
 * - [Threads](https://docs.stratifylabs.co/guides/Guide-Threads/)
 * - [CMake Projects](https://docs.stratifylabs.co/guides/Guide-CMake/)
 *
 * If you are ready to start tinkering, hal::Pin is a nice place to
 * start and allows you to read and write GPIO values. hal::Uart
 * can be used to access UART peripherals and fs::File is for
 * reading and writing files on any mounted filesystem.
 *
 * **Code Hierarchy**
 *
 * The api namespace contains all top level objects. All objects inherit
 * from api::ApiObject. Below api::ApiObject this is api::ApiWorkObject
 * and api::ApiInfoObject.
 *
 * **None of the classes that are part of the api
 * namespace should be directly declared.**
 *
 * Work objects include an error number and are the base for objects that do
 * work and make system calls.
 *
 * Info objects are used for storing and managing static data structures. They
 * don't make system calls and can't store errors. Info objects also include
 * classes with only static methods.
 *
 *
 */
namespace api {

/*! \brief ApiInfo Class
 * \details Provides inforamation abou the API library.
 *
 * This object is available in the api
 * namespace when including any object
 * or namespace within the StratifyAPI. For
 * this example, we will include `sapi/sys.hpp`
 * but any `sapi` header file will do.
 *
 * ```
 * //md2code:include
 * #include <sapi/sys.hpp>
 * ```
 */
class ApiInfo {
public:
  /*! \details Returns a pointer to a string
   * that shows the API version.
   *
   * ```
   * //md2code:main
   *	printf("The Stratify API version is %s\n",
   *	  api::ApiInfo::version()
   * );
   * ```
   *
   */
  static const char *version() { return "4.0.0-alpha"; }

  /*! \details Returns a c-style string pointer
   * to the git hash used to build the Stratify API.
   *
   * ```
   * //md2code:main
   *	printf("The Stratify API git hash is %s\n",
   *	  api::ApiInfo::git_hash()
   * );
   * ```
   *
   *
   */
  static const char *git_hash();

  /*! \details Returns a c-style string pointer
   * of the name of the operating system
   * the application is running on.
   *
   * ```
   * //md2code:main
   *	printf("The Stratify API application is running on %s\n",
   *	  api::ApiInfo::operating_system_name()
   * );
   * ```
   *
   * The names are:
   *
   * - `macosx`
   * - `windows`
   * - `linux`
   * - `stratifyos`
   *
   *
   */
  static const char *operating_system_name();

  static const char *system_processor();

  static bool is_processor_i386();
  static bool is_processor_x86_64();
  static bool is_processor_arm32();
  static bool is_processor_arm64();

  /*! \details Returns true if the application
   * is running on a windows system.
   *
   * ```
   * //md2code:main
   * if( api::ApiInfo::is_windows() ){
   *   printf("This is windows baby!\n");
   * }
   * ```
   *
   */
  static bool is_windows();

  /*! \details Returns true if the application
   * is running on a mac os x system.
   *
   * ```
   * //md2code:main
   * if( api::ApiInfo::is_macosx() ){
   *   printf("Running on macosx\n");
   * }
   * ```
   *
   */
  static bool is_macosx();

  /*! \details Returns true if the application
   * is running on a mac os x system.
   *
   * ```
   * //md2code:main
   * if( api::ApiInfo::is_linux() ){
   *   printf("Running on linux\n");
   * }
   * ```
   *
   */
  static bool is_linux();

  /*! \details Returns true if the application
   * is running on a Stratify OS system.
   *
   * ```
   * //md2code:main
   * if( api::ApiInfo::is_stratify_os() ){
   *   printf("Yep!\n");
   * }
   * ```
   *
   */
  static bool is_stratify_os();

  /*! \details Returns a directory path
   * that can be used to store application
   * data.
   *
   * For Stratify OS, this is `/home`.
   *
   */
  static const char *user_data_path();

  static u32 malloc_start_chunk_size();
  static u32 malloc_chunk_size();
};

#if defined __link
template <typename A, const A *initial_value> class Api {
#else
extern "C" const void *kernel_request_api(u32 request);
template <typename A, u32 request> class Api {
#endif
public:
  Api() { initialize(); }

  bool is_valid() {
    initialize();
    return m_api != nullptr;
  }

  Api &operator=(const A *value) {
    m_api = value;
    return *this;
  }

  const A *operator->() const { return m_api; }
  const A *api() const { return m_api; }

private:
  void initialize() {
    if (m_api == nullptr) {
#if defined __link
      m_api = initial_value;
#else
      m_api = (const A *)kernel_request_api(request);
#endif
    }
  }
  const A *m_api = nullptr;
};

class ErrorContext {
public:
  const char *message() const { return m_message; }
  int error_number() const { return m_error_number; }
  int line_number() const { return m_line_number; }

  void *signature() const { return m_signature; }

private:
  ErrorContext(void *signature) : m_signature(signature) {}
  friend class Status;
  static constexpr size_t m_message_size = 31;
  static constexpr size_t m_backtrace_count = 32;
  void *m_signature;
  int m_error_number = 0;
  int m_line_number = 0;
  char m_message[m_message_size + 1];
  void *m_backtrace[m_backtrace_count];

  inline void capture_backtrace() {
#if defined __link
#if !defined __win32
    backtrace(m_backtrace, m_backtrace_count);
#endif
#else
    // need to implement backtrace on StratifyOS v4
#endif
  }
};

class Status {
public:
  bool is_error() const { return value() < 0; }
  bool is_success() const { return value() >= 0; }

  int value() const { return errno; }

  ErrorContext &error_context();

  int system_call(int line, const char *message, int value) {
    if (value >= 0) {
      errno = value;
    } else {
      update_error_context(line, message);
    }
    return value;
  }

  template <typename T>
  T *system_call_null(int line, const char *message, T *value) {
    if (value == nullptr) {
      update_error_context(line, message);
    }
    return value;
  }

  void reset() { errno = 0; }

  size_t error_context_count() const {
    if (m_error_context_list) {
      return m_error_context_list->size() + 1;
    }
    return 1;
  }

private:
  friend class Object;
  Status() : m_error_context(&(errno)) {}
  ErrorContext m_error_context;
  std::vector<ErrorContext> *m_error_context_list = nullptr;

  void update_error_context(int line, const char *message) {
    strncpy(error_context().m_message, message, ErrorContext::m_message_size);
    error_context().m_line_number = line;
    error_context().m_error_number = errno;
    error_context().capture_backtrace();
    errno = -1;
  }
};

class Object {
public:
  static ErrorContext &error_context() { return m_status.error_context(); }
  static Status &status() { return m_status; }
  static void exit_fatal(const char *message);
  static void reset_error_context() { m_status.reset(); }

private:
  static Status m_status;
};

#define API_ASSERT(a) api::api_assert(a, __PRETTY_FUNCTION__, __LINE__);
void api_assert(bool value, const char *function, int line);

#define API_RETURN_VALUE_IF_ERROR(return_value)                                \
  if (api::Object::status().is_error()) {                                      \
    return return_value;                                                       \
  }

#define API_RETURN_IF_ERROR()                                                  \
  if (api::Object::status().is_error()) {                                      \
    return;                                                                    \
  }

#define API_SYSTEM_CALL(message_value, return_value)                           \
  api::Object::status().system_call(__LINE__, message_value, return_value)

#define API_SYSTEM_CALL_NULL(message_value, return_value)                      \
  api::Object::status().system_call_null(__LINE__, message_value, return_value)

#define API_RESET_ERROR() api::Object::status().reset()

#define API_RETURN_VALUE_ASSIGN_ERROR(                                         \
  return_value,                                                                \
  message_value,                                                               \
  error_number_value)                                                          \
  do {                                                                         \
    errno = error_number_value;                                                \
    api::Object::status().system_call(__LINE__, message_value, -1);            \
    return return_value;                                                       \
  } while (0)

#define API_RETURN_ASSIGN_ERROR(message_value, error_number_value)             \
  do {                                                                         \
    errno = error_number_value;                                                \
    api::Object::status().system_call(__LINE__, message_value, -1);            \
    return;                                                                    \
  } while (0)

/*! \brief ProgressCallback Class
 * \details The ProgressCallback class is used
 * by various methods to allow the application
 * to update the progress of an operation.
 *
 * ```
 * #include <sapi/sys.hpp>
 * #include <sapi/inet.hpp>
 * Printer p;
 *
 * p.set_progress_key("downloading");
 * HttpClient http_client;
 * DataFile data_file(File::APPEND);
 *
 * //download file to data_file and print the progress using #'s
 * http_client.get("http://some.url/file", data_file, p.progress_callback());
 * ```
 *
 */
class ProgressCallback {
public:
  /*! \details Defines the callback function prototype.
   *
   * The function has three arguments.  The first
   * is the context() member of this object. The first
   * integer is the current progress value and the second
   * is the total progress value.
   *
   * If the total (third argument) is zero, the operation is either complete
   * or aborted. If total is set to -1, the progress should be
   * shown as indeterminate.
   *
   */
  typedef bool (*callback_t)(void *, int, int);

  /*! \details Constructs an empty object. */
  ProgressCallback();

  static int indeterminate_progress_total() { return -1; }

  /*! \details Executes the callback if it is valid.
   *
   * This method is called within operations in order
   * to execute the provided callback.
   *
   * @param value The value of the progress of the operation
   * @param total The total possible progress for the operation
   * @return true to abort the operation or false to continue as normal
   *
   */
  bool update(int value, int total) const;

  static int update_function(const void *context, int value, int total);

private:
  API_AF(ProgressCallback, callback_t, callback, nullptr);
  API_AF(ProgressCallback, void *, context, nullptr);
};

} // namespace api

#endif // API_API_HPP_
