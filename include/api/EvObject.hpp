#ifndef API_EV_OBJECT_HPP
#define API_EV_OBJECT_HPP

#include "WorkObject.hpp"
#include "InfoObject.hpp"

namespace api {

/*! \brief Event Information Object
 *
 * \details
 * \sa ev namespace
 *
 */
class EvInfoObject : public virtual InfoObject {

};

/*! \brief Event Object Class
 *
 * \details
 * \sa ev namespace
 */
class EvWorkObject : public virtual WorkObject {

};


}

#endif // API_EV_OBJECT_HPP
