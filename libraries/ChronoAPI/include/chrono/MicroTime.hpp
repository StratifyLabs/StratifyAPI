/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef CHRONO_API_CHRONO_MICRO_TIME_HPP_
#define CHRONO_API_CHRONO_MICRO_TIME_HPP_

#include <mcu/types.h>
#include <time.h>

namespace chrono {

class MicroTime;

class Hours {
public:
  explicit Hours(s32 value) { m_value = value; }
  u32 hours() const { return m_value; }
  static Hours invalid() { return Hours(static_cast<u32>(-1)); }

private:
  u32 m_value;
};

class Minutes {
public:
  explicit Minutes(s32 value) { m_value = value; }
  u32 minutes() const { return m_value; }
  static Minutes invalid() { return Minutes(static_cast<u32>(-1)); }

private:
  u32 m_value;
};

class Seconds {
public:
  explicit Seconds(s32 value) { m_value = value; }
  u32 seconds() const { return m_value; }
  static Seconds invalid() { return Seconds(static_cast<u32>(-1)); }

private:
  u32 m_value;
};

class Milliseconds {
public:
  explicit Milliseconds(s32 value) { m_value = value; }
  u32 milliseconds() const { return m_value; }
  static Milliseconds invalid() { return Milliseconds(static_cast<u32>(-1)); }

private:
  u32 m_value;
};

class Nanoseconds {
public:
  explicit Nanoseconds(s32 value) { m_value = value; }
  u32 nanoseconds() const { return m_value; }
  static Nanoseconds invalid() { return Nanoseconds(static_cast<u32>(-1)); }

private:
  u32 m_value;
};

class ClockTimer;
class ClockTime;

/*! \details Defines the type for a chrono::MicroTime value. */
typedef u32 micro_time_t;

/*! \brief MicroTime Class
 * \details The MicroTime class is used for keeping track
 * of microsecond accurate time intervals. It uses
 * a 32-bit value so it is good for 4 billion microseconds
 * (or about 66 minutes).
 *
 * It is very handy for converting between microseconds,
 * milliseconds, and seconds. It also serves
 * to remove ambiguity when specifying short time intervals.
 *
 * For example:
 * ```
 * void set_period(const Microseconds & microseconds); //un-ambiguous and nice
 * code completion void set_period(u32 value); //the units here are not clear
 * void set_period_milliseconds(u32 value); //this is better but adds complexity
 * ```
 *
 *
 * ```
 * //md2code:include
 * #include <sapi/chrono.hpp>
 * ```
 *
 */
class MicroTime {
public:
  /*! \details Constructs a Microseconds object using a u32 microsecond value.
   *
   *
   * The default initial value is zero.
   *
   * ```
   * //md2code:main
   * //must be explicitly constructed
   * Microseconds duration = Microseconds(5);
   * printf("Duration is %ld microseconds\n", duration.microseconds());
   * ```
   *
   */
  explicit MicroTime(u32 microseconds = 0) {
    m_value_microseconds = microseconds;
  }

  /*! \details Constructs a Microseconds object from a chrono::ClockTime value.
   */
  MicroTime(const ClockTime &clock_time);

  /*! \details Constructs a Microseconds object from a chrono::Seconds value.
   *
   * ```
   * //md2code:main
   * Microseconds duration = Seconds(5); //converted to microseconds
   * printf("Duration is %ld microseconds\n", duration.microseconds());
   * ```
   *
   */
  MicroTime(const Seconds &seconds) {
    m_value_microseconds = seconds.seconds() * 1000000UL;
  }

  /*! \details Constructs a Microseconds object from a chrono::Milliseconds
   * value.
   *
   * ```
   * //md2code:main
   * Microseconds duration = Milliseconds(3000); //converted to microseconds
   * printf("Duration is %ld microseconds\n", duration.microseconds());
   * ```
   *
   */
  MicroTime(const Milliseconds &milliseconds) {
    m_value_microseconds = milliseconds.milliseconds() * 1000UL;
  }

  /*! \details Constructs a Microseconds object from a chrono::Nanoseconds
   * value.
   *
   * ```
   * //md2code:main
   * Microseconds duration = Nanoseconds(3000); //converted to microseconds
   * Microseconds zero = Nanoseconds(999); //converted using truncation -- goes
   * to zero printf("Duration is %ld microseconds\n", duration.microseconds());
   * printf("Zero is %ld microseconds\n", zero.microseconds());
   * ```
   *
   */
  MicroTime(const Nanoseconds &nanoseconds) {
    m_value_microseconds = nanoseconds.nanoseconds() / 1000;
  }

  /*! \details Constructs a Microseconds object from the current value of a
   * chrono::Timer. */
  MicroTime(const ClockTimer &timer);

  /*! \details Returns true if the time is set to a valid value.
   *
   */
  bool is_valid() const { return m_value_microseconds != static_cast<u32>(-1); }

  /*! \details Returns a MicroTime object set to the invalid time value. */
  static MicroTime invalid() { return MicroTime(static_cast<u32>(-1)); }

  /*! \details Assignment addition to another MicroTime object. */
  MicroTime &operator+=(const MicroTime &a) {
    m_value_microseconds += a.microseconds();
    return *this;
  }

  /*! \details Assignment addition to another MicroTime object. */
  MicroTime &operator-=(const MicroTime &a) {
    m_value_microseconds -= a.microseconds();
    return *this;
  }

  MicroTime operator+(const MicroTime &a) const {
    return MicroTime(microseconds() + a.microseconds());
  }

  MicroTime operator-(const MicroTime &a) const {
    return MicroTime(microseconds() - a.microseconds());
  }

  /*! \details Compares equality to another MicroTime object. */
  bool operator==(const MicroTime &a) const {
    return microseconds() == a.microseconds();
  }

  /*! \details Compares inequality to another MicroTime object. */
  bool operator!=(const MicroTime &a) const {
    return microseconds() != a.microseconds();
  }

  /*! \details Compares > to another MicroTime object. */
  bool operator>(const MicroTime &a) const {
    return microseconds() > a.microseconds();
  }

  /*! \details Compares < to another MicroTime object. */
  bool operator<(const MicroTime &a) const {
    return microseconds() < a.microseconds();
  }

  /*! \details Compares >= to another MicroTime object. */
  bool operator>=(const MicroTime &a) const {
    return microseconds() >= a.microseconds();
  }

  /*! \details Compares <= to another MicroTime object. */
  bool operator<=(const MicroTime &a) {
    return microseconds() <= a.microseconds();
  }

  MicroTime &operator<<(const Seconds &a) { return (*this) = a; }
  MicroTime &operator<<(const Milliseconds &a) { return (*this) = a; }
  MicroTime &operator<<(const MicroTime &a) { return (*this) = a; }
  MicroTime &operator<<(const Nanoseconds &a) { return (*this) = a; }

  /*! \details Returns the value in seconds. */
  u32 seconds() const { return microseconds() / 1000000UL; }

  /*! \details Returns the value in microseconds. */
  micro_time_t microseconds() const { return m_value_microseconds; }

  /*! \details Returns the value in milliseconds. */
  u32 milliseconds() const { return microseconds() / 1000UL; }

  /*! \details Waits for the value of the microtime.
   *
   */
  MicroTime &wait();

private:
  micro_time_t m_value_microseconds;
};

using Microseconds = MicroTime;

void wait(const MicroTime &duration);

inline Seconds operator"" _seconds(unsigned long long int value) {
  return Seconds(value);
}

inline Milliseconds operator"" _milliseconds(unsigned long long int value) {
  return Milliseconds(value);
}

inline Microseconds operator"" _microseconds(unsigned long long int value) {
  return Microseconds(value);
}

} // namespace chrono

#endif /* CHRONO_API_CHRONO_MICRO_TIME_HPP_ */
