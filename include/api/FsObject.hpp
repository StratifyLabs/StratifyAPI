/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
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
