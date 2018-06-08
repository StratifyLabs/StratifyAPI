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
class HalWorkObject : public sys::File {
public:
#if defined __link
    HalWorkObject(link_transport_mdriver_t * d=0) : File(d){}
#endif

};



}

#endif // API_HAL_OBJECT_HPP
