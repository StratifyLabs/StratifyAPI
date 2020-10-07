/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef SAPI_SYS_AIO_HPP_
#define SAPI_SYS_AIO_HPP_

#if !defined __link

#include <aio.h>
#include <cstring>
#include <errno.h>

#include "chrono/Time.hpp"
#include "var/View.hpp"

#include "File.hpp"

namespace hal {
class Device;
}

namespace fs {

/*! \brief Asynchronous IO Class */
/*! \details The Asynchronous IO class is used for performing asynchronous
 * operations on hardware devices.  When calling synchronous IO, the read/write
 * function is called and then returns when the operation is complete.  With
 * Asynchronous IO, the read/write function returns immediately and the Aio
 * class can be used to see when the operation completes.
 *
 * On Stratify OS, synchronous operations will always cause the running thread
 * to yield the processor. This can be avoided by using the Aio class.  Consider
 * the following example:
 *
 * \code
 * #include <sapi/hal.hpp>
 * #include <sapi/sys.hpp>
 * #include <sapi/var.hpp>
 *
 * int main(int argc, char * argv[]){
 * 	String buf = "Hello World!\n";
 * 	Uart uart(0); //we will read/write data to UART port 0
 * 	Aio aio(buf.data(), buf.size()); //aio uses buf as it's data
 *
 * 	uart.init(); //init the UART with default settings
 *
 *  //First a synchronous write
 * 	uart.write(buf.c_str(), buf.size());  //this will return when the bytes
 * are written
 *  //While bytes are writing, the processor will be used on another thread
 *
 *  //Now the AIO version
 * 	uart.write(aio);
 *  //This thread keeps the processor -- so we can do something else in parallel
 *
 * 	while( aio.done() == false ){
 *  	Timer::wait_milliseconds(5); //wait for the operation to complete
 * 	}
 *
 * 	return 0;
 * }
 * \endcode
 *
 *
 */
class Aio : public api::Object {
  friend class hal::Device;

public:
  /*! \details Constructs an empy AIO object. */
  Aio() { memset(&m_aio_var, 0, sizeof(struct aiocb)); }

  explicit Aio(const var::View &reference) {
    m_aio_var.aio_buf = reference.to_void();
    m_aio_var.aio_nbytes = reference.size();
    m_aio_var.aio_offset = 0;
    m_aio_var.aio_sigevent.sigev_notify = SIGEV_NONE;
  }

  Aio(size_t location, const var::View &item) {
    m_aio_var.aio_buf = item.to_void();
    m_aio_var.aio_nbytes = item.size();
    m_aio_var.aio_offset = location;
    m_aio_var.aio_sigevent.sigev_notify = SIGEV_NONE;
  }

  using AiocbList = var::Vector<struct aiocb *const>;

  /*! \details Blocks until all transfers in list have completed or timeout is
   * reached.
   *
   * @param list A list of AIO structures
   * @param nent The number of entities in the list
   * @param timeout_usec Timeout in microseconds to block (0 to block
   * indefinitely)
   *
   *
   */
  static int suspend(
    const AiocbList &list,
    const chrono::Microseconds &timeout = chrono::Microseconds(0)) {
    struct timespec timeout;
    timeout.tv_sec = timeout_usec / 1000000;
    timeout.tv_nsec = (timeout_usec % 1000000) * 1000;
    if (timeout_usec == 0) {
      return aio_suspend(list.data(), list.count(), 0);
    } else {
      return aio_suspend(list.data(), list.count(), &timeout);
    }
  }

  /*! \details Blocks until the operation completes or timeout is reached.
   *
   * @param timeout_usec The timeout in microseconds (0 to block indefinitely)
   *
   *
   */
  Aio &suspend(const chrono::Microseconds &timeout = chrono::Microseconds(0)) {
    struct aiocb *const list[1] = {&m_aio_var};
    API_ASSIGN_RESULT(
      api::ErrorCode::io_error,
      suspend(list, 1, timeout.microseconds()));
    return *this;
  }

  /*! \details Returns the buffer pointer. */
  volatile void *buffer() const { return m_aio_var.aio_buf; }

  /*! \details Sets the buffer pointer.
   *
   * @param buf A pointer to the data buffer
   * @param nbytes The number of bytes to transfer
   *
   */
  Aio &refer_to(const var::View &item) {
    m_aio_var.aio_buf = (volatile void *)item.to_void();
    m_aio_var.aio_nbytes = item.size();
    return *this;
  }

  /*! \details Returns the number of bytes to transfer. */
  int nbytes() const { return m_aio_var.aio_nbytes; }

  /*! \details Returns the offset (or channel for Dac, Adc, Pwm, etc). */
  int offset() const { return m_aio_var.aio_offset; }

  /*! \details Sets the offset (or channcel for Dac, Adc, Pwm, etc). */
  Aio &set_offset(int offset) {
    m_aio_var.aio_offset = offset;
    return *this;
  }

  /*! \details Returns the return value of the operation. */
  int ret() { return aio_return(&m_aio_var); }

  /*! \details Returns the error number of the operation. */
  int error() { return aio_error(&m_aio_var); }

  /*! \details Returns true if operation is complete. */
  bool is_done() const { return !(aio_error(&m_aio_var) == EINPROGRESS); }

  /*! \details Returns true if operation is still in progress. */
  bool is_busy() const { return (aio_error(&m_aio_var) == EINPROGRESS); }

  /*! \details Causes the calling thread to receive a signal when the operation
   * completes.
   *
   * @param number The signal number (ie SIGTERM) use -1 to prevent a signal
   * from being sent
   * @param value The signal value (passed as an argument to the handler if
   * using siginfo)
   * @return Zero on success
   */
  Aio &set_signal(int number, int value) {
    if (number >= 0) {
      m_aio_var.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
      m_aio_var.aio_sigevent.sigev_signo = number;
      m_aio_var.aio_sigevent.sigev_value.sival_int = value;
    } else {
      m_aio_var.aio_sigevent.sigev_notify = SIGEV_NONE;
    }
    return *this;
  }

  Aio &set_signal_number(int number) {
    if (number >= 0) {
      m_aio_var.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
      m_aio_var.aio_sigevent.sigev_signo = number;
    } else {
      m_aio_var.aio_sigevent.sigev_notify = SIGEV_NONE;
    }
    return *this;
  }

  Aio &set_signal_value(int value) {
    m_aio_var.aio_sigevent.sigev_value.sival_int = value;
    return *this;
  }

  Aio &set_location(int location) {
    m_aio_var.aio_offset = location;
    return *this;
  }

private:
  struct aiocb m_aio_var;
};

} // namespace fs

#endif

#endif /* SAPI_SYS_AIO_HPP_ */
