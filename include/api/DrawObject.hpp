/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef SAPI_API_DRAW_OBJECT_HPP_
#define SAPI_API_DRAW_OBJECT_HPP_

#include "WorkObject.hpp"
#include "InfoObject.hpp"
#include "SgfxObject.hpp"

namespace api {

/*! \brief Draw Information Object
 *
 * \details
 * \sa draw namespace
 *
 */
class DrawInfoObject : public virtual InfoObject, public SgfxObject {

};

/*! \brief Draw Work Object
 * \details
 * \sa draw namespace
 */
class DrawWorkObject : public virtual WorkObject, public SgfxObject {

};


}

#endif // SAPI_API_DRAW_OBJECT_HPP_
