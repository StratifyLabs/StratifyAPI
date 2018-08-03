#ifndef API_INET_OBJECT_HPP
#define API_INET_OBJECT_HPP

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

#endif // API_INET_OBJECT_HPP
