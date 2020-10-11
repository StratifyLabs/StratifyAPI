#ifndef SYSAPI_SYS_SOS_HPP
#define SYSAPI_SYS_SOS_HPP

#include "api/api.hpp"

#include "Appfs.hpp"
#include "chrono/MicroTime.hpp"
#include "var/String.hpp"

namespace sys {

#if !defined __link
class Sos : public api::Object {
public:
  /*! \details Resets the device. */
  void reset();

  class Launch {
    API_ACCESS_COMPOUND(Launch, var::StringView, path);
    API_ACCESS_COMPOUND(Launch, var::StringView, arguments);
    API_ACCESS_COMPOUND(Launch, var::StringView, environment);
    API_ACCESS_FUNDAMENTAL(
      Launch,
      Appfs::Flags,
      application_flags,
      Appfs::Flags::is_default);
    API_ACCESS_FUNDAMENTAL(Launch, int, ram_size, 0);
    API_ACCESS_FUNDAMENTAL(
      Launch,
      const api::ProgressCallback *,
      progress_callback,
      nullptr);
  };

  var::String launch(const Launch &options) const;

  /*!
   * \details Installs an application from the data filesystem
   * to the application filesystem.
   *
   * \param path The path to the source application to install
   * \param options The installation flag options
   * \param ram_size The number of bytes to allow for the application's data
   * section
   *
   * \return The full path of the install location.
   */
  var::String install(
    var::StringView path,
    Appfs::Flags options
    = Appfs::Flags::is_default, // run in RAM, discard on exit
    int ram_size = 0) const;

  /*!
   * \details Installs an application from the data filesystem
   * to the application filesystem.
   *
   * \param path The path to the source file.
   * \param options The installation options.
   * \param ram_size The number of bytes to allow for the application's data
   * section. \param progress_callback A pointer to a callback to indicate the
   * installation progress. \return
   */
  var::String install(
    var::StringView path,
    Appfs::Flags options, // run in RAM, discard on exit
    int ram_size,
    const api::ProgressCallback *progress_callback) const;

  /*! \details Puts the kernel in powerdown mode.
   *
   * @param timeout_msec The number of milliseconds before the
   * device will power on (reset).  If this isn't supported,
   * the device will power off until reset by an external signal
   */
  void powerdown(const chrono::MicroTime &duration = chrono::Milliseconds(0));

  /*! \details Puts the kernel in hibernate mode.
   *
   * @param timeout_msec The number of milliseconds before the
   * device will wake up from hibernation.  If this isn't supported,
   * the device will stay in hibernation until woken up externally
   */
  const Sos &
  hibernate(const chrono::MicroTime &duration = chrono::Milliseconds(0)) const;

  /*! \details Executes a kernel request.
   *
   * @param req The request number
   * @param arg Argument pointer
   * @return The result of the execution of the request. (-1 if request is not
   * available)
   *
   * The kernel request must
   * be defined and implemented by the board support package.
   */
  int request(int req, void *argument = nullptr) const;

  /*! \details Redirects the standard output to the file specified.
   *
   * @param fd The file descriptor where the standard output should be directed.
   *
   * The file desriptor should be open and ready for writing. For example,
   * to redirect the standard output to the UART:
   *
   * \code
   * #include <sapi/sys.hpp>
   * #include <sapi/hal.hpp>
   *
   * Uart uart(0);
   * uart.init(); //initializes uart using default settings (if available)
   * Sys::redirect_stdout( uart.fileno() );
   * printf("This will be written to UART0\n");
   * \endcode
   *
   *
   */
  static void redirect_stdout(int fd) { _impure_ptr->_stdout->_file = fd; }

  /*! \details Redirects the standard input from the specified file descriptor.
   *
   * @param fd The open and readable file descriptor to use for standard input
   *
   * See Sys::redirect_stdout() for an example.
   *
   */
  static void redirect_stdin(int fd) { _impure_ptr->_stdin->_file = fd; }

  /*! \details Redirects the standard error from the specified file descriptor.
   *
   * @param fd The open and writable file descriptor to use for standard input
   *
   * See Sys::redirect_stdout() for an example.
   *
   */
  static void redirect_stderr(int fd) { _impure_ptr->_stderr->_file = fd; }
};
#endif

} // namespace sys

#endif // SYSAPI_SYS_SOS_HPP
