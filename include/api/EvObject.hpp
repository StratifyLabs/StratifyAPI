#ifndef API_EV_OBJECT_HPP
#define API_EV_OBJECT_HPP

#include "WorkObject.hpp"
#include "InfoObject.hpp"

namespace api {

/*! \brief Event Informatin Object
 *
 */
class EvInfoObject : public virtual InfoObject {

};

/*! \brief Event Object Class
 */
class EvWorkObject : public virtual WorkObject {

};


}

#endif // API_EV_OBJECT_HPP
