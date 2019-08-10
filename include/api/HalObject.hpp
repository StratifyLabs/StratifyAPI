#ifndef SAPI_API_HAL_OBJECT_HPP_
#define SAPI_API_HAL_OBJECT_HPP_

#include "WorkObject.hpp"
#include "InfoObject.hpp"
#include "../fs/File.hpp"

namespace api {

/*! \brief Hal Info Object
 * \details Hal Objects for storing information like
 * device attributes, device information and pin assignment
 * structures.
 *
 * \sa hal namespace
 *
 */
class HalInfoObject : public virtual InfoObject {

};

/*! \brief Hal Work Object
 * \details The Hal Work Object class
 * is the parent of all Hal objects.
 *
 * \sa hal namespace
 *
 */
class HalWorkObject : public fs::File {
public:

};



}

#endif // SAPI_API_HAL_OBJECT_HPP_
