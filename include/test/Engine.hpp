#ifndef SAPI_TEST_ENGINE_HPP_
#define SAPI_TEST_ENGINE_HPP_

/*! \cond */

#include "Test.hpp"
#include "../var/Queue.hpp"

namespace test {

class Engine : public var::Queue<Test*> {
public:
	Engine();



};

}

/*! \endcond */


#endif // SAPI_TEST_ENGINE_HPP_
