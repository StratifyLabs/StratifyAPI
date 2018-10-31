#ifndef TEST_ENGINE_HPP
#define TEST_ENGINE_HPP

#include "Test.hpp"
#include "../var/Queue.hpp"

namespace test {

class Engine : public var::Queue<Test*> {
public:
	Engine();



};

}

#endif // ENGINE_HPP
