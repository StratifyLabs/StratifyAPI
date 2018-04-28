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
    Function(const char * test_name, return_type (*function)(args...), Test * parent = 0) : Test(test_name, parent){
        m_function = function;
    }

    ~Function(){}

    /*! \details Executes a test case.
     *
     * @param case_name The name of the case
     * @param expected_value The expected return value
     * @param expected_errno The expected error number
     * @param arguments The arguments to pass to the test function
     * @return The value that the tested function returns
     *
     */
    return_type execute_case_with_expected_return(const char * case_name, return_type expected_value, int expected_errno, args... arguments){
        return_type return_value;
        bool result = true;
        errno = 0;

        open_case(case_name);

        case_timer().start();
        return_value = m_function(arguments...);
        case_timer().stop();


        if( return_value != expected_value ){
            result = false;
            print_case_message("expected return %d != actual return %d", expected_value, return_value);
        } else {
            print_case_message("expected value returned");
        }

        if( expected_errno > 0 ){
            if( expected_errno != errno ){
                result = false;
                print_case_message("expected errno %d != actual errno %d", expected_errno, errno);
            } else {
                print_case_message("expected errno match: %d", errno);
            }
        }

        close_case(result);
        return return_value;
    }

    return_type execute_case_with_less_than_zero_on_error(const char * case_name, int expected_errno, args... arguments){
        return_type return_value;
        bool result = true;
        errno = 0;

        open_case(case_name);

        case_timer().start();
        return_value = m_function(arguments...);
        case_timer().stop();

        if( expected_errno > 0 ){

            if( return_value >= 0 ){
                print_case_message("expected an error but returned: %d", return_value);
                result = false;
            }

            if( expected_errno != errno ){
                result = false;
                print_case_message("expected errno %d != actual errno %d", expected_errno, errno);
            } else {
                print_case_message("expected errno match: %d", errno);
            }

        } else {

            if( return_value < 0 ){
                result = false;
                print_case_message("expected success but ret: %d errno: %d", return_value, errno);
            } else {
                print_case_message("returned: %d", return_value);
            }

        }

        close_case(result);
        return return_value;
    }


private:
    return_type (*m_function)(args...);
};

}

#endif // TEST_FUNCTION_HPP
