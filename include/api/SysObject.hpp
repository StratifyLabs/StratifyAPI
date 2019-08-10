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

namespace sys {
typedef api::Argument<const pid_t&> Pid;
typedef api::Argument<const u32&> StackSize;
}

#endif // SAPI_API_SYS_OBJECT_HPP_
