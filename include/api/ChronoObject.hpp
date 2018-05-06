#ifndef API_CHRONO_OBJECT_HPP
#define API_CHRONO_OBJECT_HPP

#include "WorkObject.hpp"
#include "InfoObject.hpp"

namespace chrono{
class MicroTime;
class ClockTime;
class Time;
}

namespace api {

/*! \brief Chrono Information Object
 *
 */
class ChronoInfoObject : public virtual InfoObject {
public:
    /*! \details Wait for the specified number of seconds. */
    static void wait_seconds(u32 timeout);

    /*! \details This method will delay the specified number of milliseconds.
     *
     * @param timeout Time to wait in milliseconds
     */
    static void wait_milliseconds(u32 timeout){
        return wait_microseconds(timeout*1000UL);
    }

    /*! \details This method will delay the specified number of microseconds.
     *
     * @param timeout Time to wait in microseconds
     */
    static void wait_microseconds(u32 timeout);

    /*! \details This method delays based on a chrono::ClockTime value. */
    static void wait(const chrono::ClockTime & clock_time);
    /*! \details This method delays based on a chrono::MicroTime value. */
    static void wait(const chrono::MicroTime & micro_time);
    /*! \details This method delays based on a chrono::Time value. */
    static void wait(const chrono::Time & time);
};

/*! \brief Chrono Work Object Class
 */
class ChronoWorkObject : public virtual WorkObject {
public:

    /*! \details See chrono::ChronoInfoObject::wait_seconds();
     */
    static void wait_seconds(u32 timeout){ ChronoInfoObject::wait_seconds(timeout); }

    /*! \details See chrono::ChronoInfoObject::wait_milliseconds();
     */
    static void wait_milliseconds(u32 timeout){ wait_microseconds(timeout*1000UL); }

    /*! \details See chrono::ChronoInfoObject::wait_microseconds();
     */
    static void wait_microseconds(u32 timeout){ ChronoInfoObject::wait_microseconds(timeout); }

};


}

#endif // API_UI_OBJECT_HPP
