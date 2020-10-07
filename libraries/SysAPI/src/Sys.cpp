/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
// Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#if !defined __link
#include <sos/sos.h>
#endif

#include <sos/link.h>

#include "printer/Printer.hpp"
#include "sys/Sys.hpp"
#include "sys/Trace.hpp"

printer::Printer &
printer::operator<<(printer::Printer &printer, const sys::TraceEvent &a) {
  var::String id;
  chrono::ClockTime clock_time;
  clock_time = a.timestamp();
  switch (a.id()) {
  case LINK_POSIX_TRACE_FATAL:
    id = "fatal";
    break;
  case LINK_POSIX_TRACE_CRITICAL:
    id = "critical";
    break;
  case LINK_POSIX_TRACE_WARNING:
    id = "warning";
    break;
  case LINK_POSIX_TRACE_MESSAGE:
    id = "message";
    break;
  case LINK_POSIX_TRACE_ERROR:
    id = "error";
    break;
  default:
    id = "other";
    break;
  }
  printer.key(
    "timestamp",
    F32U ".%06ld",
    clock_time.seconds(),
    clock_time.nanoseconds() / 1000UL);
  printer.key("id", id);
  printer.key("thread", "%d", a.thread_id());
  printer.key("pid", "%d", a.pid());
  printer.key("programAddress", "0x%lX", a.program_address());
  printer.key("message", a.message());
  return printer;
}

printer::Printer &
printer::operator<<(printer::Printer &printer, const sys::Sys::Info &a) {
  printer.key("name", a.name());
  printer.key("serialNumber", a.serial_number().to_string());
  printer.key("hardwareId", F3208X, a.hardware_id());
  if (a.name() != "bootloader") {
    printer.key("projectId", a.id());
    if (a.team_id().is_empty() == false) {
      printer.key("team", a.team_id());
    }
    printer.key("bspVersion", a.bsp_version());
    printer.key("sosVersion", a.sos_version());
    printer.key("cpuArchitecture", a.cpu_architecture());
    printer.key("cpuFrequency", F32D, a.cpu_frequency());
    printer.key("applicationSignature", F32X, a.application_signature());

    printer.key("bspGitHash", a.bsp_git_hash());
    printer.key("sosGitHash", a.sos_git_hash());
    printer.key("mcuGitHash", a.mcu_git_hash());
  }
  return printer;
}

using namespace sys;

SerialNumber::SerialNumber() { m_serial_number = {0}; }

SerialNumber SerialNumber::from_string(var::StringView str) {
  SerialNumber ret;
  u32 len = strnlen(str.cstring(), 8 * 4);
  if (len == 8 * 4) {
#if defined __link
    sscanf(
      str.cstring(),
      "%08X%08X%08X%08X",
#else
    sscanf(
      str.cstring(),
      "%08lX%08lX%08lX%08lX",
#endif
      &ret.m_serial_number.sn[3],
      &ret.m_serial_number.sn[2],
      &ret.m_serial_number.sn[1],
      &ret.m_serial_number.sn[0]);
  }
  return ret;
}

SerialNumber::SerialNumber(const var::String &str) {
  SerialNumber serial_number = from_string(str);
  memcpy(&m_serial_number, &serial_number.m_serial_number, sizeof(mcu_sn_t));
}

bool SerialNumber::operator==(const SerialNumber &serial_number) {
  return memcmp(
           &serial_number.m_serial_number,
           &m_serial_number,
           sizeof(mcu_sn_t))
         == 0;
}

var::String SerialNumber::to_string() const {
  var::String ret;
  ret.format(
    F3208X F3208X F3208X F3208X,
    m_serial_number.sn[3],
    m_serial_number.sn[2],
    m_serial_number.sn[1],
    m_serial_number.sn[0]);
  return ret;
}

Sys::Sys(FSAPI_LINK_DECLARE_DRIVER)
  : FileAccess<Sys>(
    "/dev/sys",
    fs::OpenMode::read_write() FSAPI_LINK_INHERIT_DRIVER) {}

var::String Sys::launch(
  const LaunchOptions &options,
  const api::ProgressCallback *progress_callback) {
#if defined __link
  return var::String();
#else
  var::String result;
  result.resize(PATH_MAX);
  if (
    ::launch(
      options.path().cstring(),
      result.to_char(),
      options.arguments().cstring(),
      options.application_flags(),
      options.ram_size(),
      api::ProgressCallback::update_function,
      (void *)(progress_callback), // pointer to the object
      nullptr                      // environment not implemented
      )
    < 0) {
    return var::String();
  }
  return result;
#endif
}

var::String Sys::install(
  var::StringView path,
  enum Appfs::flags options, // run in RAM, discard on exit
  int ram_size) {
  return install(path, options, ram_size, nullptr);
}

var::String Sys::install(
  var::StringView path,
  enum Appfs::flags options, // run in RAM, discard on exit
  int ram_size,
  const api::ProgressCallback *progress_callback) {
#if defined __link
  return var::String();
#else
  var::String result;
  result.resize(PATH_MAX);
  if (
    ::install(
      path.cstring(),
      result.to_char(),
      options,
      ram_size,
      api::ProgressCallback::update_function,
      progress_callback)
    < 0) {
    return var::String();
  }
  return result;
#endif
}

int Sys::free_ram(var::StringView path) {
  int fd;
  int ret;
#if defined __link
  if ((fd = link_open(link_driver.argument(), path.cstring(), O_RDONLY)) < 0) {
    return -1;
  }
  ret = link_ioctl(link_driver.argument(), fd, I_APPFS_FREE_RAM);
  link_close(link_driver.argument(), fd);
#else
  if ((fd = ::open(path.cstring(), O_RDONLY)) < 0) {
    return -1;
  }
  ret = ::ioctl(fd, I_APPFS_FREE_RAM);
  ::close(fd);
#endif
  return ret;
}

int Sys::reclaim_ram(var::StringView path) {
  int fd;
  int ret;
#if defined __link
  if ((fd = link_open(link_driver.argument(), path.cstring(), O_RDONLY)) < 0) {
    return -1;
  }
  ret = link_ioctl(link_driver.argument(), fd, I_APPFS_RECLAIM_RAM);
  link_close(link_driver.argument(), fd);
#else
  if ((fd = ::open(path.cstring(), O_RDONLY)) < 0) {
    return -1;
  }
  ret = ::ioctl(fd, I_APPFS_RECLAIM_RAM);
  ::close(fd);
#endif
  return ret;
}

#if !defined __link

var::String Sys::get_version() {
  return var::String(get_info().system_version());
}

var::String Sys::get_kernel_version() {
  return var::String(get_info().kernel_version());
}

void Sys::powerdown(const chrono::MicroTime &duration) {
  ::powerdown(duration.seconds());
}

int Sys::hibernate(const chrono::MicroTime &duration) {
  return ::hibernate(duration.seconds());
}

int Sys::request(int request, void *arg) {
  return kernel_request(request, arg);
}

void Sys::reset() {
  int fd = ::open("/dev/core", O_RDWR);
  core_attr_t attr;
  attr.o_flags = CORE_FLAG_EXEC_RESET;
  ::ioctl(fd, I_CORE_SETATTR, &attr);
  ::close(fd); // incase reset fails
}

int Sys::get_board_config(sos_board_config_t &config) {
  return ioctl(I_SYS_GETBOARDCONFIG, &config).status().value();
}

#endif

Sys::Info Sys::get_info() {
  sys_info_t sys_info;
  ioctl(I_SYS_GETINFO, &sys_info);
  if (status().is_error()) {
    return Sys::Info();
  }
  return Sys::Info(sys_info);
}

bool Sys::is_authenticated() {
  return ioctl(I_SYS_ISAUTHENTICATED, nullptr).status().value();
}

sys_secret_key_t Sys::get_secret_key() {
  sys_secret_key_t result = {0};
  ioctl(I_SYS_GETSECRETKEY, &result);
  return result;
}

SerialNumber Sys::get_serial_number() { return get_info().serial_number(); }


sys_id_t Sys::get_id() {
  sys_id_t result = {0};
  ioctl(I_SYS_GETID, &result);
  return result;
}
