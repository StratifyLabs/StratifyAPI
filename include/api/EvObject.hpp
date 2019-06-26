#ifndef SAPI_API_EV_OBJECT_HPP_
#define SAPI_API_EV_OBJECT_HPP_

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

#endif // SAPI_API_EV_OBJECT_HPP_
