#ifndef CALC_CALCOBJECT_HPP
#define CALC_CALCOBJECT_HPP

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

#endif // CALC_CALCOBJECT_HPP
