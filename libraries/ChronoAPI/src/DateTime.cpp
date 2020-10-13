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
printer::operator<<(printer::Printer &printer, const chrono::DateTime &a) {
  printer.key("time", var::NumberToString(a.time()));
  return printer;
}

using namespace chrono;

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

DateTime DateTime::get_system_time() {
  time_t t = ::time(0);
  if (t < 962668800) {
    t = 962668800;
  }
  return DateTime(t);
}

u32 DateTime::second() const { return m_time % 60; }

u32 DateTime::minute() const { return (m_time % 3600) / 60; }

u32 DateTime::hour() const { return m_time / 3600 % 24; }

Date::Date(const DateTime &date_time) {
  time_t ctime = date_time.ctime();
#if defined __win32
  struct tm *ptr;
  ptr = gmtime(&ctime);
  m_tm = *ptr;
#else
  gmtime_r(&ctime, &m_tm);
#endif
}

const char *Date::month_name() const {
  u32 mon = month();
  if (mon < 12) {
    return month_names[mon];
  }
  return "";
}
