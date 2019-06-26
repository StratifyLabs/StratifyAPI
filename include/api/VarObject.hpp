#ifndef SAPI_API_VAR_OBJECT_HPP_
#define SAPI_API_VAR_OBJECT_HPP_

#include "WorkObject.hpp"
#include "InfoObject.hpp"

namespace api {

/*! \brief Var Infomation Object
 *
 * \details
 * \sa var namespace
 *
 */
class VarInfoObject : public virtual InfoObject {

};

/*! \brief Var Work Object
 *
 * \details
 * \sa var namespace
 *
 */
class VarWorkObject : public virtual WorkObject {

};


}
#endif // SAPI_API_VAR_OBJECT_HPP_
