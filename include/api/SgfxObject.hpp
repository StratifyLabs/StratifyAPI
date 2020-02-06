/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef SAPI_API_SGFX_OBJECT_HPP_
#define SAPI_API_SGFX_OBJECT_HPP_

#include <sapi/sg.h>
#include "WorkObject.hpp"
#include "InfoObject.hpp"
#include "../var/Data.hpp"
#include "../sys/requests.h"

namespace api {

typedef Api<sg_api_t, SGFX_API_REQUEST> SgfxApi;

class SgfxObject {
public:
	static SgfxApi & api(){ return m_api; }
private:
	static SgfxApi m_api;
};

/*! \brief Sgfx Information Object
 *
 * \details
 * \sa sgfx namespace
 *
 */
class SgfxInfoObject : public virtual InfoObject, public SgfxObject {

};

/*! \brief Sgfx Work Object
 * \details The Sgfx Object Class is the parent class
 * of all work objects in the sgfx namespace.
 *
 * \sa sgfx namespace
 */
class SgfxWorkObject : public virtual WorkObject, public SgfxObject {

};


}

#endif // SAPI_API_SGFX_OBJECT_HPP_
