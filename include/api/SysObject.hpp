#ifndef SAPI_API_SYS_OBJECT_HPP_
#define SAPI_API_SYS_OBJECT_HPP_

#include "WorkObject.hpp"
#include "InfoObject.hpp"

namespace api {

/*! \brief System Info Object
 *
 * \details
 * \sa sys namespace
 *
 */
class SysInfoObject : public virtual InfoObject {

};

/*! \brief System Work Object
 *
 * \details
 * \sa sys namespace
 *
 */
class SysWorkObject : public virtual WorkObject {

};



}

#endif // SAPI_API_SYS_OBJECT_HPP_
