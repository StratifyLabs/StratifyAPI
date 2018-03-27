/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef MICRO_TIME_HPP_
#define MICRO_TIME_HPP_

#include <mcu/types.h>
#include <time.h>

#ifdef __MCU_ONLY__
#include "hal/Tmr.hpp"
#endif

namespace sys {

/*! \details Defines the type for a sys::MicroTime value. */
typedef u32 micro_time_t;

/*! \brief MicroTime Class
 * \details The MicroTime class is used for keeping track
 * of microsecond accurate time intervals. It uses
 * a 32-bit value so it is good for 4B microseconds
 * (or about 66 minutes).
 *
 * It is very handy for converting between microseconds (usec),
 * milliseconds (msec), and seconds (sec). It also serves
 * to remove ambiguity when specified short time intervals.
 *
 * For example:
 * \code
 * void set_period(const MicroTime & micro_time); //un-ambiguous and nice code completion
 * void set_period(u32 value); //the units here are not clear
 * void set_period_msec(u32 value); //this is better but adds complexity
 * \endcode
 *
 */
class MicroTime {
public:

    /*! \details Constructs a MicroTime object using a u32 value.
     *
     * The default initial value is zero.
     *
     */
    MicroTime(u32 microseconds = 0){ m_value_microseconds = microseconds; }

    /*! \details Create a MicroTime object from a second value. */
    static MicroTime from_seconds(u32 sec){ return MicroTime(sec*1000000UL); }
    static MicroTime from_sec(u32 sec){ return from_seconds(1000000UL); }


    /*! \details Create a MicroTime object from a millisecond value. */
    static MicroTime from_milliseconds(u32 msec){ return MicroTime(msec*1000UL); }
    static MicroTime from_msec(u32 msec){ return from_milliseconds(msec); }

    /*! \details Create a MicroTime object from a microsecond value.
     *
     * This method is only provided for completeness. The following objects
     * will be initialized with the same value because the constructor
     * value assumes microseconds.
     *
     * \code
     * MicroTime time_from_usec(MicroTime::from_usec(1000));
     * MicroTime time_usec(1000);
     * \endcode
     *
     */
    static MicroTime from_microseconds(u32 microseconds){ return microseconds; }
    static MicroTime from_usec(u32 usec){ return usec; }

    /*! \details Returns the usec() value when
     * the comipler wants to convert to an unsigned 32-bit value.
     *
     * This allows times to be easily added, subtracted, and compared
     * as a u32.
     *
     */
    operator micro_time_t () const { return microseconds(); }


    MicroTime & operator += (const MicroTime & micro_time){
        m_value_microseconds += micro_time.usec();
        return *this;
    }

    MicroTime & operator -= (const MicroTime & micro_time){
        m_value_microseconds -= micro_time.usec();
        return *this;
    }

    /*! \details Sets the value of the time in seconds.
     *
     * @param sec The number of seconds.
     *
     */
    void set_seconds(u32 sec){ set_usec(sec*1000000UL); }
    void set_sec(u32 sec){ set_seconds(sec); }

    /*! \details Sets the value of the time in milliseconds.
     *
     * @param msec The value to assign in milliseconds.
     *
     */
    void set_milliseconds(u32 msec){ set_usec(msec*1000UL); }
    void set_msec(u32 msec){ set_milliseconds(msec); }

    /*! \details Sets the value of the time in microseconds.
     *
     * @param microseconds The value in microseconds
     *
     */
    void set_microseconds(micro_time_t microseconds){ m_value_microseconds = microseconds; }
    void set_usec(micro_time_t usec){ set_microseconds(usec); }

    /*! \details Returns the value in seconds. */
    u32 seconds() const { return microseconds() / 1000000UL; }
    u32 sec() const { return seconds(); }

    /*! \details Returns the value in microseconds. */
    micro_time_t microseconds() const { return m_value_microseconds; }
    micro_time_t usec() const { return microseconds(); }

    /*! \details Returns the value in milliseconds. */
    u32 milliseconds() const { return usec() / 1000UL; }
    u32 msec() const { return milliseconds(); }

private:
    micro_time_t m_value_microseconds;
};

}

#endif /* MICRO_TIME_HPP_ */
