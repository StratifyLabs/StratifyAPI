#ifndef SAPI_API_SM_OBJECT_HPP_
#define SAPI_API_SM_OBJECT_HPP_

/*! \cond */

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

/*! \endcond */

#endif // SAPI_API_SM_OBJECT_HPP_
