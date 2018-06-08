#ifndef API_DRAW_OBJECT_HPP
#define API_DRAW_OBJECT_HPP

#include "WorkObject.hpp"
#include "InfoObject.hpp"

namespace api {

/*! \brief Draw Information Object
 *
 * \details
 * \sa draw namespace
 *
 */
class DrawInfoObject : public virtual InfoObject {

};

/*! \brief Draw Work Object
 * \details
 * \sa draw namespace
 */
class DrawWorkObject : public virtual WorkObject {

};


}

#endif // API_DRAW_OBJECT_HPP
