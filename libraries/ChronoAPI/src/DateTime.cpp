/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
// Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#if defined __StratifyOS__
#include <fcntl.h>
#include <sos/dev/rtc.h>
#include <unistd.h>
#endif

#include "printer/Printer.hpp"

#include "chrono/ClockTimer.hpp"
#include "chrono/DateTime.hpp"

#if defined __win32
#include <iomanip>
#include <sstream>
#include <time.h>

static char *strptime(const char *s, const char *f, struct tm *tm) {
  // Isn't the C++ standard lib nice? std::get_time is defined such that its
  // format parameters are the exact same as strptime. Of course, we have to
  // create a string stream first, and imbue it with the current C locale, and
  // we also have to make sure we return the right things if it fails, or
  // if it succeeds, but this is still far simpler an implementation than any
  // of the versions in any of the C standard libraries.
  std::istringstream input(s);
  input.imbue(std::locale(setlocale(LC_ALL, nullptr)));
  input >> std::get_time(tm, f);
  if (input.fail()) {
    return nullptr;
  }
  return (char *)(s + input.tellg());
}

#endif

printer::Printer &
printer::operator<<(printer::Printer &printer, const chrono::MicroTime &a) {
  printer.key("duration", var::NumberToString(a.microseconds(), F32U "us"));
  return printer;
}

printer::Printer &
printer::operator<<(printer::Printer &printer, const chrono::DateTime &a) {
  printer.key("time", var::NumberToString(a.time()));
  return printer;
}

using namespace chrono;

MicroTime::MicroTime(const ClockTimer &timer) {
  m_value_microseconds = timer.microseconds();
}

MicroTime::MicroTime(const ClockTime &clock_time) {
  m_value_microseconds = clock_time.seconds() * 1000000UL
                         + (clock_time.nanoseconds() + 500) / 1000;
}

MicroTime &MicroTime::wait() {
  chrono::wait(*this);
  return *this;
}

constexpr static const char *month_names[] = {
  "January",
  "February",
  "March",
  "April",
  "May",
  "June",
  "July",
  "August",
  "September",
  "October",
  "November",
  "December"};

/*! \brief Construct using current time */
DateTime::DateTime() { m_time = 0; }

/*! \brief Construct using an amount of time */
DateTime::DateTime(const Seconds &sec, const Minutes &min, const Hours &hour) {
  set_time(sec, min, hour);
}

DateTime::DateTime(var::StringView time_string, var::StringView format) {
  struct tm tm = {0};
  if (strptime(time_string.cstring(), format.cstring(), &tm) != nullptr) {
    m_time = mktime(&tm);
  } else {
    m_time = 0;
  }
}

DateTime &DateTime::operator+=(const DateTime &a) {
  m_time += a.time();
  return *this;
}

DateTime &DateTime::operator=(const DateTime &a) {
  m_time = a.time();
  return *this;
}

DateTime &DateTime::operator=(u32 a) {
  m_time = a;
  return *this;
}

DateTime &DateTime::operator-=(const DateTime &a) {
  m_time -= a.time();
  return *this;
}

int DateTime::set_time_of_day(const DateTime &t) {
#if defined __StratifyOS__
  int ret;
  int fd = ::open("/dev/rtc", O_RDWR);
  if (fd < 0) {
    return -1;
  }
  struct tm t_data = t.get_tm();
  ret = ::ioctl(fd, I_RTC_SET, &t_data);
  close(fd);
  return ret;
#else
  return -1;
#endif
}

int DateTime::set_time_of_day() { return set_time_of_day(*this); }

DateTime DateTime::get_time_of_day() {
  time_t t = ::time(0);
  if (t < 962668800) {
    t = 962668800;
  }
  return DateTime(t);
}

DateTime &
DateTime::set_time(const Seconds &sec, const Minutes &min, const Hours &hour) {
  m_time = sec.seconds() + min.minutes() * 60 + hour.hours() * 3600;
  return *this;
}

u32 DateTime::second() const { return m_time % 60; }

u32 DateTime::minute() const { return (m_time % 3600) / 60; }

u32 DateTime::hour() const { return m_time / 3600 % 24; }

u32 DateTime::day() const { return get_tm().tm_mday; }
u32 DateTime::weekday() const { return get_tm().tm_wday; }
u32 DateTime::yearday() const { return get_tm().tm_yday; }
u32 DateTime::month() const { return get_tm().tm_mon + 1; }
u32 DateTime::year() const { return get_tm().tm_year + 1900; }

const char *DateTime::month_name() const {
  u32 mon = month();
  if (mon < 12) {
    return month_names[mon];
  }
  return "";
}

struct tm DateTime::get_tm() const {
  struct tm time_struct;
#if defined __win32
  struct tm *ptr;
  ptr = gmtime(&m_time);
  time_struct = *ptr;
#else
  gmtime_r(&m_time, &time_struct);
#endif
  return time_struct;
}
