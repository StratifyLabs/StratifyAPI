#ifndef API_SM_OBJECT_HPP
#define API_SM_OBJECT_HPP

#include "WorkObject.hpp"
#include "InfoObject.hpp"

namespace api {

/*! \brief State Machine Information Object
 *
 * \details
 * \sa sm namespace
 */
class SmInfoObject : public virtual InfoObject {

};

/*! \brief State Machine Work Object
 *
 * \details
 * \sa sm namespace
 */
class SmWorkObject : public virtual WorkObject {

};


}

#endif // API_SM_OBJECT_HPP
