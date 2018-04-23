#ifndef API_SM_OBJECT_HPP
#define API_SM_OBJECT_HPP

#include "WorkObject.hpp"
#include "InfoObject.hpp"

namespace api {

/*! \brief State Machine Information Object
 *
 */
class SmInfoObject : public virtual InfoObject {

};

/*! \brief State Machine Work Object Class
 */
class SmWorkObject : public virtual WorkObject {

};


}

#endif // API_SM_OBJECT_HPP
