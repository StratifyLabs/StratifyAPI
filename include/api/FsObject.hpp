#ifndef SAPI_API_FS_OBJECT_HPP_
#define SAPI_API_FS_OBJECT_HPP_

#include "WorkObject.hpp"
#include "InfoObject.hpp"

namespace api {

/*! \brief System Info Object
 *
 * \details
 * \sa sys namespace
 *
 */
class FsInfoObject : public virtual InfoObject {

};

/*! \brief System Work Object
 *
 * \details
 * \sa sys namespace
 *
 */
class FsWorkObject : public virtual WorkObject {

};



}

#endif // SAPI_API_FS_OBJECT_HPP_
