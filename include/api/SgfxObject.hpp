#ifndef API_SGFX_OBJECT_HPP
#define API_SGFX_OBJECT_HPP

#include "WorkObject.hpp"
#include "InfoObject.hpp"
#include "../var/Data.hpp"

namespace api {

/*! \brief Sgfx Informatin Object
 *
 */
class SgfxInfoObject : public virtual InfoObject {

};

/*! \brief Sgfx Object Class
 * \details The Sgfx Object Class is the parent class
 * of all objects in the sgfx namespace.
 */
class SgfxWorkObject : public virtual WorkObject {

};

/*! \brief Sgfx Object Class
 * \details The Sgfx Object Class is the parent class
 * of all objects in the sgfx namespace.
 */
class SgfxDataObject : public var::Data {

};


}

#endif // API_SGFX_OBJECT_HPP
