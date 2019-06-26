#ifndef SAPI_API_TEST_OBJECT_HPP_
#define SAPI_API_TEST_OBJECT_HPP_

#include "WorkObject.hpp"
#include "InfoObject.hpp"

namespace api {

/*! \brief Test Information Object
 *
 * \details
 * \sa test namespace
 *
 */
class TestInfoObject : public virtual InfoObject {

};

/*! \brief Test Work Object
 *
 * \details
 * \sa test namespace
 */
class TestWorkObject : public virtual WorkObject {

};


}

#endif // SAPI_API_TEST_OBJECT_HPP_
