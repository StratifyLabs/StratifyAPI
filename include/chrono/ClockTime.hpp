/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef CHRONO_CLOCK_TIME_HPP_
#define CHRONO_CLOCK_TIME_HPP_

#include <mcu/types.h>
#include <time.h>

#include "../api/ChronoObject.hpp"

namespace chrono {

class MicroTime;

/*! \brief Clock Time Class
 * \details The ClockTime class is a helper class for using
 * struct timespec times. This is a 64-bit time with a 32-bit
 * seconds element and a 32-bit nanoseconds element.
 *
 *
 */
class ClockTime : public api::ChronoWorkObject {
public:

    /*! \details Constructs a MicroTime object using a u32 value.
     *
     * The default initial value is zero.
     *
     */
    ClockTime(s32 seconds, s32 nanoseconds){
        assign(seconds, nanoseconds);
    }

    /*! \details Constructs a clock time object based on the timespec. */
    ClockTime(const struct timespec & nano_time){
        m_value = nano_time;
    }

    /*! \details Contructs an object from a micro time object. */
    ClockTime(const MicroTime & micro_time);

    /*! \details Constructs a zero value ClockTime object. */
    ClockTime(){ reset(); }

    enum {
        REALTIME /*! Realtime clock ID used with get_time() and get_resolution() */ = CLOCK_REALTIME
    };

    /*! \details Assigns the value of CLOCK_REALTIME to this object */
    int get_time(int clock_id = REALTIME);

    static ClockTime get_resolution(int clock_id = REALTIME);

    /*! \details Resets the value of the clock to zero. */
    void reset(){
        m_value.tv_sec = 0; m_value.tv_nsec = 0;
    }

    /*! \details Sets the value of the clock.
     *
     * @param seconds The seconds value
     * @param nanoseconds The nanosecond value
     *
     */
    void set(s32 seconds, s32 nanoseconds){
        m_value.tv_sec = seconds; m_value.tv_nsec = nanoseconds;
    }

    /*! \details Returns a pointer to the struct timespec.
     *
     * This allows the object to be passed directly to functions
     * that required a pointer to struct timespec (read-only)
     *
     */
    operator const struct timespec * () const { return &m_value; }

    /*! \details Returns a pointer to the struct timespec.
     *
     * This allows the object to be passed directly to functions
     * that required a pointer to struct timespec (read-write)
     *
     */
    operator struct timespec * (){ return &m_value; }

    /*! \details Returns true if the time is set to a valid value.
     *
     */
    bool is_valid() const {
        return *this != invalid();
    }

    /*! \details Returns a MicroTime object set to the invalid time. */
    static ClockTime invalid(){ return ClockTime((u32)-1, (u32)-1); }

    /*! \details Returns true if this is greater than \a a. */
    bool operator > (const ClockTime & a) const;
    /*! \details Returns true if this is less than \a a. */
    bool operator < (const ClockTime & a) const;
    /*! \details Returns true if this is greater than or equal to \a a. */
    bool operator >= (const ClockTime & a) const;
    /*! \details Returns true if this is less than or equal to \a a. */
    bool operator <= (const ClockTime & a) const;
    /*! \details Returns true if this is equal to \a a. */
    bool operator == (const ClockTime & a) const;
    /*! \details Returns true if this is not equal to \a a. */
    bool operator != (const ClockTime & a) const;

    /*! \details Returns the sum of this object and \a a. */
    ClockTime operator + (const ClockTime & a) const { return add(*this, a); }
    /*! \details Returns the difference of this object and \a a. */
    ClockTime operator - (const ClockTime & a) const { return subtract(*this, a); }


    /*! \details Adds \a to this and assigned to this. */
    ClockTime & operator += (const ClockTime & a){
        *this = add(*this, a);
        return *this;
    }

    /*! \details Subracts \a from this and assigned to this. */
    ClockTime & operator -= (const ClockTime & a){
        //subtract nano time
        *this = subtract(*this, a);
        return *this;
    }

    /*! \details Returns the seconds component. */
    s32 seconds() const { return m_value.tv_sec; }
    /*! \details Returns the nanoseconds component. */
    s32 nanoseconds() const { return m_value.tv_nsec; }

    /*! \details Returns a pointer to a strut timespec. */
    struct timespec * timespec(){ return &m_value; }
    /*! \details Returns a pointer to a strut timespec (read-only). */
    const struct timespec * timespec() const { return &m_value; }


private:
    void assign(s32 seconds, s32 nanoseconds);
    static ClockTime add(const ClockTime & a, const ClockTime & b);
    static ClockTime subtract(const ClockTime & a, const ClockTime & b);

    struct timespec m_value;
};

}

#endif /* CHRONO_NANO_TIME_HPP_ */
