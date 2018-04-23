#ifndef API_DRAW_OBJECT_HPP
#define API_DRAW_OBJECT_HPP

#include "WorkObject.hpp"
#include "InfoObject.hpp"

namespace api {

/*! \brief Draw Information Object
 *
 */
class DrawInfoObject : public virtual InfoObject {

};

/*! \brief State Machine Work Object Class
 */
class DrawWorkObject : public virtual WorkObject {

};


}

#endif // API_DRAW_OBJECT_HPP
