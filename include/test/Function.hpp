#ifndef TEST_FUNCTION_HPP
#define TEST_FUNCTION_HPP

#include <mcu/types.h>
#include <cstdio>
#include <cstring>
#include <limits.h>

#include "../sys/Timer.hpp"

#include "Case.hpp"

namespace test {

template<typename return_type, typename...args> class Function {
public:

    Function(const char * function_name, return_type (*function)(args...)){
        m_function = function;
        printf("\"%s\": {\n", function_name);
        m_is_first = true;
    }

    ~Function(){
        printf("\n}\n");
    }

    return_type execute(const char * case_name, return_type expected_value, int expected_errno, args... arguments){
        sys::Timer t;
        return_type return_value;
        errno = 0;
        t.start();
        return_value = m_function(arguments...);
        t.stop();

        if( m_is_first == false ){
            printf(",\n");
        } else {
            m_is_first = false;
        }
        printf("\"%s\": {", case_name);


        if( (return_value != expected_value) || (expected_errno != errno) ){
            m_result = false;
            printf("\"result\": \"failed\",\n", case_name);
            printf("\"expected return\": \"%d\",\n", expected_value);
            printf("\"actual return\": \"%d\",\n", return_value);
            printf("\"expected errno\": \"%d\",\n", expected_errno);
            printf("\"actual errno\": \"%d\",\n", errno);
        } else {
            m_result = true;
            printf("\"result\": \"success\",\n");
        }

        m_execution_time = t.calc_usec();
        printf("\"execution time\": \"%ld\"\n", m_execution_time);
        printf("}");
        return return_value;
    }

    u32 execution_time() const {
        return m_execution_time;
    }

    bool result() const {
        return m_result;
    }


private:
    u32 m_execution_time;
    bool m_result;
    bool m_is_first;
    return_type (*m_function)(args...);
};

}

#endif // TEST_FUNCTION_HPP
