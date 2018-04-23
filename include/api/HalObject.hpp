#ifndef API_HAL_OBJECT_HPP
#define API_HAL_OBJECT_HPP

#include "WorkObject.hpp"
#include "InfoObject.hpp"
#include "../sys/File.hpp"

namespace api {

/*! \brief Hal Info Object
 * \details Hal Objects for storing information like
 * device attributes, device information and pin assignment
 * structures.
 *
 */
class HalInfoObject : public virtual InfoObject {

};

/*! \brief Hal Object Class
 * \details The Hal Object class
 * is the parent of all Hal objects.
 *
 */
class HalWorkObject : public sys::File {

};



}

#endif // API_HAL_OBJECT_HPP
