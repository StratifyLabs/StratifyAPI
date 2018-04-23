#ifndef CALC_CALCOBJECT_HPP
#define CALC_CALCOBJECT_HPP

#include "WorkObject.hpp"
#include "InfoObject.hpp"

namespace api {

/*! \brief Format Infomation Object.
 *
 */
class CalcInfoObject : public virtual InfoObject {

};

/*! \brief Format Work Object Class
 *
 */
class CalcWorkObject : public virtual WorkObject {

};

}

#endif // CALC_CALCOBJECT_HPP
