#ifndef API_TEST_OBJECT_HPP
#define API_TEST_OBJECT_HPP

#include "WorkObject.hpp"
#include "InfoObject.hpp"

namespace api {

/*! \brief Test Informatin Object
 *
 */
class TestInfoObject : public virtual InfoObject {

};

/*! \brief Test Object Class
 */
class TestWorkObject : public virtual WorkObject {

};


}

#endif // API_TEST_OBJECT_HPP
