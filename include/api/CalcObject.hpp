#ifndef SAPI_API_CALCOBJECT_HPP_
#define SAPI_API_CALCOBJECT_HPP_

#include "WorkObject.hpp"
#include "InfoObject.hpp"

namespace api {

/*! \brief Calc Infomation Object
 * \details
 *
 * \sa calc namespace
 */
class CalcInfoObject : public virtual InfoObject {

};

/*! \brief Calc Work Object
 * \details
 *
 * \sa calc namespace
 */
class CalcWorkObject : public virtual WorkObject {

};

}

#endif // SAPI_API_CALCOBJECT_HPP_
