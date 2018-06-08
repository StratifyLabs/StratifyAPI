#ifndef API_SYS_OBJECT_HPP
#define API_SYS_OBJECT_HPP

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

#endif // API_SYS_OBJECT_HPP
