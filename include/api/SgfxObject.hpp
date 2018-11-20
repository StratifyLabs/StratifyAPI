#ifndef API_SGFX_OBJECT_HPP
#define API_SGFX_OBJECT_HPP

#include <sapi/sg.h>
#include "WorkObject.hpp"
#include "InfoObject.hpp"
#include "../var/Data.hpp"
#include "../sys/requests.h"

namespace api {

typedef Api<sg_api_t, SAPI_API_REQUEST_SGFX> SgfxApi;

class SgfxObject {
public:
	static const SgfxApi & api(){ return m_api; }
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

#endif // API_SGFX_OBJECT_HPP
