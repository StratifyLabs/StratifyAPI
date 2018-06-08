#ifndef API_VAR_OBJECT_HPP
#define API_VAR_OBJECT_HPP

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
#endif // API_VAR_OBJECT_HPP
