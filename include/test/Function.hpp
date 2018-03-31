#ifndef TEST_FUNCTION_HPP
#define TEST_FUNCTION_HPP

#include <mcu/types.h>
#include <errno.h>
#include <cstdio>
#include <cstring>
#include <limits.h>

#include "Test.hpp"

namespace test {

template<typename return_type, typename...args> class Function : public Test {
public:

    Function(const char * function_name, return_type (*function)(args...)) : Test(function_name){
        m_function = function;
    }

    ~Function(){}

    return_type execute(const char * case_name, return_type expected_value, int expected_errno, args... arguments){
        return_type return_value;
        bool result;
        errno = 0;

        open_case(case_name);

        case_timer().start();
        return_value = m_function(arguments...);
        case_timer().stop();


        if( (return_value != expected_value) || (expected_errno != errno) ){
            result = false;
            set_case_value("expected return", expected_value);
            set_case_value("actual return", return_value);
            set_case_value("expected errno", expected_errno);
            set_case_value("actual errno", errno);
        } else {
            result = true;
        }

        close_case(result);
        return return_value;
    }


private:
    return_type (*m_function)(args...);
};

}

#endif // TEST_FUNCTION_HPP
