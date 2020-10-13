/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef CHRONO_API_CHRONO_TIME_HPP_
#define CHRONO_API_CHRONO_TIME_HPP_

#include <ctime>

#include "var/String.hpp"

#include "MicroTime.hpp"

namespace chrono {

/*! \brief Time Class
 * \details This class is for accessing the current time as well
 * as adding and subtracting times and dates.
 *
 * The time is based on the value of the RTC in the system. The RTC
 * will keep the same value even when the device is reset. This class
 * should provide accurate access to the current data and time.
 *
 * It holds a c style time_t value that can be managed which is
 * a 32-bit value with accuracy to the second.
 *
 * \code
 * Time now; //current time
 * Time ten_minutes(0, 10, 0); //duration of 10 minutes
 *
 * now += ten_minutes; //adds ten minutes to now
 * \endcode
 *
 */
class DateTime {
public:
  /*! \details Constructs using current time. */
  DateTime();

  explicit DateTime(const time_t &t) { m_time = t; }

  DateTime(
    var::StringView time_string,
    var::StringView format = "%Y-%m-%d %H:%M:%S");

  bool is_valid() { return m_time != 0; }

  static DateTime current_time() { return get_system_time(); }

  DateTime operator+(const DateTime &a) const {
    DateTime result;
    result.m_time = m_time + a.m_time;
    return result;
  }

  DateTime operator-(const DateTime &a) const {
    DateTime result;
    result.m_time = m_time - a.m_time;
    return result;
  }

  bool operator==(const DateTime &a) const { return m_time == a.m_time; }
  bool operator!=(const DateTime &a) const { return m_time != a.m_time; }
  bool operator>(const DateTime &a) const { return m_time > a.m_time; }
  bool operator<(const DateTime &a) const { return m_time < a.m_time; }
  bool operator>=(const DateTime &a) const { return m_time >= a.m_time; }
  bool operator<=(const DateTime &a) const { return m_time <= a.m_time; }

  DateTime age() const { return DateTime::get_system_time() - *this; }

  /*! \details Adds to the current value. */
  DateTime &operator+=(const DateTime &a);
  /*! \details Assigns another Time. */
  DateTime &operator=(const DateTime &a);
  /*! \details Assigns another time value (time_t). */
  DateTime &operator=(u32 a);
  /*! \details Subtracts from the current value. */
  DateTime &operator-=(const DateTime &a);

  /*! \details Assigns the system time of day to the time stored in this object
   * and returns the current system time.
   */
  static DateTime get_system_time();

  /*! \details Sets the current value.
   *
   */
  DateTime &set_time(time_t tm) {
    m_time = tm;
    return *this;
  }

  /*! \details Returns the time value (number of seconds since epoch). */
  time_t ctime() const { return m_time; }
  time_t time() const { return m_time; }

  /*! \details Returns seconds (from 0 to 59). */
  u32 second() const;
  /*! \details Returns minutes (from 0 to 59). */
  u32 minute() const;
  /*! \details Returns hours (from 0 to 23). */
  u32 hour() const;

private:
  time_t m_time;
};

class Date {
public:
  explicit Date(const DateTime &date_time);

  /*! \details Gets the name of the month. */
  const char *month_name() const;

  /*! \details Returns seconds (from 0 to 59). */
  u32 second() const { return m_tm.tm_sec; }
  /*! \details Returns minutes (from 0 to 59). */
  u32 minute() const { return m_tm.tm_min; }
  /*! \details Returns hours (from 0 to 23). */
  u32 hour() const { return m_tm.tm_hour; }

  /*! \details Returns the day of month (from 1 to 31). */
  u32 day() const { return m_tm.tm_mday; }
  /*! \details Returns the day of week (from 1 to 7). */
  u32 weekday() const { return m_tm.tm_wday; }
  /*! \details Returns the day of the year (1 to 366). */
  u32 yearday() const { return m_tm.tm_yday; }
  /*! \details Returns the month (from 1 to 12). */
  u32 month() const { return m_tm.tm_mon + 1; }
  /*! \details Returns the year (e.g. 2014) */
  u32 year() const { return m_tm.tm_year; }

  /*! \details Converts the time to a struct tm. */
  const struct tm &get_tm() const { return m_tm; }

private:
  struct tm m_tm;
};

} // namespace chrono

inline chrono::DateTime operator"" _weeks(unsigned long long int value) {
  return chrono::DateTime(value * 7 * 24 * 3600UL);
}

inline chrono::DateTime operator"" _days(unsigned long long int value) {
  return chrono::DateTime(value * 24 * 3600UL);
}

inline chrono::DateTime operator"" _hours(unsigned long long int value) {
  return chrono::DateTime(value * 3600UL);
}

inline chrono::DateTime operator"" _minutes(unsigned long long int value) {
  return chrono::DateTime(value * 60UL);
}

namespace printer {
class Printer;
Printer &operator<<(Printer &printer, const chrono::DateTime &a);
} // namespace printer

#endif /* CHRONO_API_CHRONO_TIME_HPP_ */
