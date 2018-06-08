#ifndef API_SGFX_OBJECT_HPP
#define API_SGFX_OBJECT_HPP

#include "WorkObject.hpp"
#include "InfoObject.hpp"
#include "../var/Data.hpp"

namespace api {

/*! \brief Sgfx Information Object
 *
 * \details
 * \sa sgfx namespace
 *
 */
class SgfxInfoObject : public virtual InfoObject {

};

/*! \brief Sgfx Work Object
 * \details The Sgfx Object Class is the parent class
 * of all work objects in the sgfx namespace.
 *
 * \sa sgfx namespace
 */
class SgfxWorkObject : public virtual WorkObject {

};

/*! \brief Sgfx Data Object
 * \details The Sgfx Data Object Class is the parent class
 * of all data objects in the sgfx namespace.
 *
 * \sa sgfx namespace
 *
 */
class SgfxDataObject : public var::Data {

};


}

#endif // API_SGFX_OBJECT_HPP
