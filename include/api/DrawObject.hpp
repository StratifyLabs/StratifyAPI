#ifndef API_DRAW_OBJECT_HPP
#define API_DRAW_OBJECT_HPP

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

#endif // API_DRAW_OBJECT_HPP
