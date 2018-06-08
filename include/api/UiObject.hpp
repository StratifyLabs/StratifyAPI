#ifndef API_UI_OBJECT_HPP
#define API_UI_OBJECT_HPP

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

#endif // API_UI_OBJECT_HPP
