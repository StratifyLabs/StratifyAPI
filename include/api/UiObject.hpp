#ifndef SAPI_API_UI_OBJECT_HPP_
#define SAPI_API_UI_OBJECT_HPP_

#include "WorkObject.hpp"
#include "InfoObject.hpp"

namespace api {

/*! \brief User Interface Information Object
 *
 * \details
 * \sa ui namespace
 *
 */
class UiInfoObject : public virtual InfoObject {

};

/*! \brief User Interface Work Object
 *
 * \details
 * \sa ui namespace
 */
class UiWorkObject : public virtual WorkObject {

};


}

#endif // SAPI_API_UI_OBJECT_HPP_
