#ifndef TEST_FUNCTION_HPP
#define TEST_FUNCTION_HPP

#include <mcu/types.h>
#include <errno.h>
#include <cstdio>
#include <cstring>
#include <limits.h>

#include "Test.hpp"

namespace test {

/*! \brief Function Test Class Template
 * \details The Function template class is designed
 * to test any arbitrary C or C++ function (not member
 * methods, just regular functions).
 *
 * The following is an example that tests open()
 * for various error conditions.
 *
 * \code
 *
 * #include <sapi/test.hpp>
 *
 * //because open() has variable arguments we wrap it to make it work with this class
 * int test_open(const char * path, int o_flags, int o_mode){
 *   return open(path, o_flags, o_mode);
 * }
 *
 * bool is_test_enabled = true;
 *
 * Test::initilize("test name", "0.1");
 *
 * if( is_test_enabled ){
 *   //when test is constructed
 *   Function<int, const char *, int, int> open_function_test("open", test_open);
 *
 *   open_function_test.execute("NO EXIST Read only", -1, ENOENT, "/home/non-existing-file.txt", O_RDONLY, 0);
 *   open_function_test.execute("NO EXIST Read Write", -1, ENOENT, "/home/non-existing-file.txt", O_RDWR, 0);
 *   open_function_test.execute("NO EXIST Read Write", -1, ENAMETOOLONG, "/home/non-existing-file-too-long-too-long-too-long-too-long-too-long-too-long-too-long-too-long-too-long-too-long.txt", O_RDWR, 0);
 *
 *   //open_function_test will call the deconstructor here before Test::finalize()
 * }
 *
 *
 * Test::finalize();
 *
 *
 * \endcode
 *
 *
 */
template<typename return_type, typename...args> class Function : public Test {
public:

    /*! \details Constructs a new function test object.
     *
     * @param test_name The name of the test
     * @param function A function pointer to the function that will be tested
     *
     */
    Function(const char * test_name, return_type (*function)(args...)) : Test(test_name){
        m_function = function;
    }

    ~Function(){}

    /*! \details Executes a test case.
     *
     * @param case_name The name of the case
     * @param expected_value The expected return value
     * @param expected_errno The expected error number
     * @param args The arguments to pass to the test function
     * @return The value that the tested function returns
     *
     */
    return_type execute_case(const char * case_name, return_type expected_value, int expected_errno, args... arguments){
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
