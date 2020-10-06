/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

// sl sdk.copyright:message="'Copyright 2011-2020 Tyler Gilbert and Stratify
// Labs, Inc; see LICENSE.md for rights.'",path=StratifyAPI,prefix='/*! \file */
// //',filter=cmake_link?cmake_arm?rapidjson?rapidxml --verbose=message

#ifndef API_API_HPP_
#define API_API_HPP_

#include "ApiMacros.hpp"

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
  static const char *version() { return "3.22.1"; }

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
template <typename A, const A *initial_value> class Api : public ApiObject {
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

#define API_ASSERT(a) api::api_assert(a, __PRETTY_FUNCTION__, __LINE__);
void api_assert(bool value, const char *function, int line);

enum class ErrorCode {
  none,
  permissions,
  no_entity,
  no_such_process,
  io_error,
  bad_file_number,
  no_memory,
  busy,
  invalid_value
};

#define API_ASSIGN_ERROR_CODE(error_code_value, result_value)                  \
  result().assign(__LINE__, error_code_value, result_value)

class Result {
public:
  ErrorCode error_code() const {
    if (m_value < 0) {
      return static_cast<ErrorCode>((m_value * -1) & 0xff);
    }

    return ErrorCode::none;
  }

  bool is_error() const { return m_value < 0; }

  int value() const { return m_value; }

  int line_number() const {
    if (m_value < 0) {
      return (m_value * -1) >> 24;
    }
    return 0;
  }

  const char *error_code_description() const;

  int assign(int line, ErrorCode error_code, int value) {
    if (value >= 0) {
      m_value = value;
    } else {
      m_value = -1 * ((line << 8) | static_cast<int>(error_code));
    }
    return value;
  }

  void reset_result() { m_value = 0; }

protected:
private:
  int m_value;
};

class Object {
public:
  Result &result() { return m_result; }
  Result result() const { return m_result; }
  static void exit_fatal(const char *message);
  static const char *error_code_description(ErrorCode error_code);

private:
  Result m_result;
};

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
