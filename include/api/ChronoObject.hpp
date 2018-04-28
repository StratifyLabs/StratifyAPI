#ifndef API_CHRONO_OBJECT_HPP
#define API_CHRONO_OBJECT_HPP

#include "WorkObject.hpp"
#include "InfoObject.hpp"

namespace api {

/*! \brief Chrono Information Object
 *
 */
class ChronoInfoObject : public virtual InfoObject {

};

/*! \brief Chrono Work Object Class
 */
class ChronoWorkObject : public virtual WorkObject {
public:

    /*! \details Wait for the specified number of seconds. */
    static void wait_seconds(u32 timeout);

    /*! \details This method will delay the specified number of milliseconds.
     *
     * @param timeout Time to wait in milliseconds
     */
    static void wait_milliseconds(u32 timeout);

    /*! \details This method will delay the specified number of microseconds.
     *
     * @param timeout Time to wait in microseconds
     */
    static void wait_microseconds(u32 timeout);
};


}

#endif // API_UI_OBJECT_HPP
