#ifndef API_TEST_OBJECT_HPP
#define API_TEST_OBJECT_HPP

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

#endif // API_TEST_OBJECT_HPP
