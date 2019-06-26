#ifndef SAPI_API_INET_OBJECT_HPP_
#define SAPI_API_INET_OBJECT_HPP_

#include "WorkObject.hpp"
#include "InfoObject.hpp"

namespace api {

/*! \brief Inet Information Object
 *
 * \details
 * \sa inet namespace
 *
 */
class InetInfoObject : public virtual InfoObject {

};

/*! \brief Inet Work Object
 * \details
 * \sa inet namespace
 */
class InetWorkObject : public virtual WorkObject {

};


}

#endif // SAPI_API_INET_OBJECT_HPP_
