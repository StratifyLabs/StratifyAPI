#ifndef API_INFO_OBJECT_HPP
#define API_INFO_OBJECT_HPP


#include "ApiObject.hpp"

namespace api {

/*! \brief API Information Object
 * \details Classes that inherit from
 * information objects are used for static
 * data storage and access. They don't do any
 * work that could cause errors.
 *
 *
 */
class InfoObject : public virtual ApiObject {

};

}

#endif // API_INFO_OBJECT_HPP
