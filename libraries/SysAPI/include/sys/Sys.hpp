/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef SAPI_SYS_SYS_HPP_
#define SAPI_SYS_SYS_HPP_

#if !defined __link
#include <sos/sos.h>
#endif

#include <sos/dev/sys.h>
#include <sos/link.h>

#include "Appfs.hpp"
#include "Trace.hpp"
#include "chrono/DateTime.hpp"
#include "fs/File.hpp"

namespace sys {

class Sys;

/*! \brief Serial Number class
 * \details The SerialNumber class holds a value for an
 * MCU serial number.
 *
 * Stratify OS supports reading the serial number directly
 * from the chip. This class makes doing so as simply as possible.
 *
 */
class SerialNumber {
public:
  /*! \details Constructs an empty serial number. */
  SerialNumber();

  /*! \details Constructs a serial number for an array of u32 values. */
  explicit SerialNumber(const u32 serial_number[4]) {
    memcpy(m_serial_number.sn, serial_number, sizeof(u32) * 4);
  }

  /*! \details Constructs a serial number from an mcu_sn_t. */
  explicit SerialNumber(const mcu_sn_t serial_number)
    : m_serial_number(serial_number) {}

  /*! \details Constructs this serial number from \a str. */
  explicit SerialNumber(const var::String &str);

  /*! \details Returns true if a valid serial number is held. */
  bool is_valid() const { return at(0) + at(1) + at(2) + at(3) != 0; }

  /*! \details Returns the u32 section of the serial number specified by *idx*.
   */
  u32 at(u32 idx) const {
    if (idx >= 4) {
      idx = 3;
    }
    return m_serial_number.sn[idx];
  }

  /*! \details Compares this strig to \a serial_number. */
  bool operator==(const SerialNumber &serial_number);

  /*! \details Converts the serial number to a string. */
  var::String to_string() const;

  /*! \details Returns a serial number object from a string type. */
  static SerialNumber from_string(var::StringView str);

private:
  mcu_sn_t m_serial_number;
};

/*! \brief Sys Class
 * \details This class allows access to system attributes and functions.
 */
class Sys : public fs::FileAccess<Sys> {
public:

  /*! \brief System Information Class
   * \details This class holds the system information.
   *
   *
   * \code
   * #include <sapi/sys.hpp>
   * SysInfo info = SysInfo::get(); //grab system information
   * Printer p;
   * p << info; //print system information using the printer
   * \endcode
   *
   */
  class Info {
    friend class Sys;

  public:
    /*! \details Constructs an empty SysInfo object. */
    Info() { clear(); }

    /*! \details Constructs an object from *info*. */
    explicit Info(const sys_info_t &info) : m_info(info) {}

    operator const sys_info_t &() const { return m_info; }

    /*! \details Returns true if the object is valid. */
    bool is_valid() const { return cpu_frequency() != 0; }

    /*! \details Returns the OS package project ID. */
    var::StringView  id() const { return m_info.id; }
    /*! \details Returns the team ID of the system. */
    var::StringView  team_id() const { return m_info.team_id; }
    /*! \details Returns the name of the system. */
    var::StringView  name() const { return m_info.name; }
    /*! \details Returns the system version. */
    var::StringView  system_version() const { return m_info.sys_version; }
    /*! \details Returns the board support package version (same as
     * system_version()). */
    var::StringView  bsp_version() const { return m_info.sys_version; }
    /*! \details Returns the Stratify OS version version. */
    var::StringView  sos_version() const { return m_info.kernel_version; }
    /*! \details Returns the kernel version (same as sos_version()). */
    var::StringView  kernel_version() const { return m_info.kernel_version; }

    /*! \details Returns the CPU architecture.
     *
     * Applications that are installed on the system must
     * have a compatible architecture.
     *
     *
     */
    var::StringView  cpu_architecture() const { return m_info.arch; }

    /*! \details Returns the CPU core clock frequency in Hertz. */
    u32 cpu_frequency() const { return m_info.cpu_freq; }

    /*! \details Returns the signature for the system call table.
     *
     * Applications that are installed must have a compatible signature.
     *
     */
    u32 application_signature() const { return m_info.signature; }

    /*! \details Returns the GIT Hash of the OS package as built. */
    var::StringView  bsp_git_hash() const { return m_info.bsp_git_hash; }
    /*! \details Returns the Stratify OS library used to link the OS package. */
    var::StringView  sos_git_hash() const { return m_info.sos_git_hash; }
    /*! \details Returns the Stratify OS MCU library used to link the OS
     * package.
     */
    var::StringView  mcu_git_hash() const { return m_info.mcu_git_hash; }

    u32 o_flags() const { return m_info.o_flags; }

    var::StringView  arch() const { return m_info.arch; }
    var::StringView  stdin_name() const { return m_info.stdin_name; }
    var::StringView  stdout_name() const { return m_info.stdout_name; }
    var::StringView  trace_name() const { return m_info.trace_name; }
    u32 hardware_id() const { return m_info.hardware_id; }

    SerialNumber serial_number() const { return SerialNumber(m_info.serial); }

    void clear() { memset(&m_info, 0, sizeof(m_info)); }

  private:
    sys_info_t m_info;
  };

  Sys(FSAPI_LINK_DECLARE_DRIVER_NULLPTR);

#if !defined __link

  /*! \details Gets the version (system/board version).
   *
   * @param version The destination string for the version
   * @return Zero on success
   */
  var::String get_version();

  /*! \details Gets the version (kernel version).
   *
   * @param version The destination string for the version
   * @return Zero on success
   */
  var::String get_kernel_version();

  /*! \details Loads the board configuration provided as
   * part of the board support package.
   *
   * @param config A reference to the destination object
   * @return Zero on success
   *
   * The object must be opened before calling this method.
   *
   * \sa open()
   */
  int get_board_config(sos_board_config_t &config);

#endif

  /*! \details Loads the current system info.
   *
   *
   */
  Info get_info() const;

  bool is_authenticated() const;

  sys_secret_key_t get_secret_key() const;

  SerialNumber get_serial_number() const;

  /*! \details Loads the cloud kernel ID.
   *
   * @param id A reference to the destination data
   * @return Less than zero if the operation failed
   *
   * The object must be opened before calling this method.
   *
   */
  sys_id_t get_id() const;

private:
};

} // namespace sys

namespace printer {
class Printer;
Printer &operator<<(Printer &printer, const sys::Sys::Info &a);
class TraceEvent;
Printer &operator<<(Printer &printer, const sys::TraceEvent &a);
} // namespace printer

#endif /* SAPI_SYS_SYS_HPP_ */
