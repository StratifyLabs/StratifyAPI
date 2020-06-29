/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef SAPI_TEST_TEST_HPP_
#define SAPI_TEST_TEST_HPP_

#include <cstdarg>
#if defined __link
#include <sos/link.h>
#include <errno.h>
#endif

#include "../api/TestObject.hpp"
#include "../chrono/MicroTime.hpp"
#include "../chrono/Timer.hpp"
#include "../sys/Cli.hpp"
#include "../var/ConstString.hpp"

namespace test {

#define TEST_EXPECT(a, T, b, c) a.expect<T>(__PRETTY_FUNCTION__, __LINE__, b, c)
#define TEST_EXPECT_NOT(a, T, b, c) a.expect_not<T>(__PRETTY_FUNCTION__, __LINE__, b, c)
#define TEST_ASSERT(a, T, b, c) do { if( a.expect<T>(__PRETTY_FUNCTION__, __LINE__, b, c) == false ){ return case_result(); } } while(0)
#define TEST_ASSERT_NOT(a, T, b, c) do { if( a.expect_not<T>(__PRETTY_FUNCTION__, __LINE__, b, c) == false ){ return case_result(); } } while(0)

#define TEST_THIS_EXPECT(T, b, c) this->expect<T>(__PRETTY_FUNCTION__, __LINE__, b, c)
#define TEST_THIS_EXPECT_NOT(T, b, c) this->expect_not<T>(__PRETTY_FUNCTION__, __LINE__, b, c)
#define TEST_THIS_EXPECT_ERROR(b,c) this->expect_error(__PRETTY_FUNCTION__, __LINE__, b, c)
#define TEST_THIS_EXPECT_ERROR_NULL(b,c) this->expect_error_null(__PRETTY_FUNCTION__, __LINE__, b, c)
#define TEST_THIS_ASSERT(T, b, c) do { if( this->expect<T>(__PRETTY_FUNCTION__, __LINE__, b, c) == false ){ return case_result(); } } while(0)
#define TEST_THIS_ASSERT_NOT(T, b, c) do { if( this->expect_not<T>(__PRETTY_FUNCTION__, __LINE__, b, c) == false ){ return case_result(); } } while(0)


/*! \brief Test Class
 * \details The Test class is designed to
 * be inherited to implement the following
 * tests:
 *
 * - API Tests: Should be designed to ensure the API is documented correctly and works as expected
 * - Performance Tests: Should be designed to run the same set of operations over and over
 * - Stress Tests: Should be designed to stress the system and check for unexpected behavior
 * - Additional Tests: Used as needed by inheriting classes to run additional tests
 *
 * \code
 *
 * #include <sapi/test.hpp>
 *
 * u32 o_execute_flags = Test::EXECUTE_ALL;
 * Test::initialize(cli.name(), cli.version());
 * if( o_flags & DATA_TEST_FLAG ){
 *   Test test("generic test");  //Must be constructed after Test::inialize()
 *   test.execute(o_execute_flags);
 *   //At this point test will be deconstructed -- must happen before Test::finalize()
 * }
 * Test::finalize();
 * \endcode
 *
 * See the [sostest](https://github.com/StratifyLabs/sostest) suite of applications for examples (like
 * [DataTest](https://github.com/StratifyLabs/sostest/blob/master/api-var-test/src/DataTest.cpp)).
 *
 * Here is an example report in JSON format:
 *
 * \code
 * {
 *   "system": {
 *     "name": "Stratify Alpha",
 *     "arch": "v7em_f4sh",
 *     "sys version": "1.5",
 *     "kernel version": "3.5.0",
 *     "memory size": "16384",
 *     "id": "-KZKdVwMXIj6vTVsbX56",
 *     "serial": "F5001942555D5A3EAEAE24601801F01A"
 *   },
 *   "test": {
 *    "name": "api-var-test",
 *    "version": "0.1"
 *   },
 *   "var::Data": {
 *     "api": {
 *       "msg-0": "fill test complete",
 *       "msg-1": "alloc test complete",
 *       "result": true,
 *       "microseconds": 10900.0
 *     },
 *     "result": true,
 *     "microseconds": 11172.0
 *   }
 * }
 *
 * \endcode
 *
 *
 */
class Test : public api::WorkObject {
public:

	using Name = arg::Argument<const var::String &, struct TestNameTag>;
	using Version = arg::Argument<const var::String &, struct TestVersionTag>;
	using VersionEncodedString = Version;
	using GitHash = arg::Argument<const var::String &, struct TestGitHashTag>;

	/*! \details Initializes the test report.
	  *
	  * This must be called before any tests are even
	  * constructed.
	  *
	  */
	static void initialize(
			Name name,
			Version version,
			GitHash git_hash = GitHash("")
			);

	/*! \details Finalizes the test report.
	  *
	  * This should be called after the last test
	  * has been deconstructed.
	  *
	  */
	static void finalize();


	/*! \details Parse command line options for running test types.
	  *
	  * This method searches for the following options and returns
	  * a value that has which tests to execute.
	  *
	  * - -execute_all
	  * - -api
	  * - -performance
	  * - -stress
	  * - -additional
	  *
	  * \code
	  * #include <sapi/test.hpp>
	  * #include <sapi/sys.hpp>
	  *
	  * int main(int argc, char * argv[]){
	  *   Cli cli(argc, argv);
	  *   u32 o_execute_flags = Test::parse_options(cli);
	  *
	  *   Test::initialize(cli.name(), cli.version());
	  *
	  *   if( o_execute_flags ){
	  *     Test test("do nothing");
	  *     test.execute(o_execute_flags);
	  *   }
	  *
	  *   Test::finalize();
	  *
	  *   return 0;
	  * }
	  *
	  * \endcode
	  *
	  */
	static u32 parse_execution_flags(const sys::Cli & cli);
	static u32 parse_test(const sys::Cli & cli, const var::String & name, u32 test_flag);


	/*! \details Constructs a new test object.
	  *
	  * @param name The name of the test
	  * @param parent The parent test or zero
	  *
	  * <b>The test must be constructed after Test::initialize().</b>
	  *
	  * \code
	  *
	  * //the WRONG way
	  * int main(int argc, char * argv[]){
	  *   Test my_test("my empty test"); //DON'T CONSTRUCT BEFORE Test::initialize()
	  *   Test::initialize();
	  *   my_test.execute();
	  *   Test::finalize();
	  *
	  *   return 0; //my_test is deconstructed after Test::finalize() -- WRONG!
	  * }
	  *
	  * //the RIGHT way
	  * int main(int argc, char * argv[]){
	  *   bool is_run_test = true;
	  *   Test::initialize();
	  *   if(is_run_test){
	  *     Test my_test("my empty test"); //DON'T CONSTRUCT BEFORE Test::initialize()
	  *     my_test.execute();
	  *     //my_test will deconstruct here BEFORE Test::finalize() --RIGHT
	  *   }
	  *   Test::finalize();
	  *   return 0;
	  * }
	  *
	  * \endcode
	  *
	  * If a Test is constructed inside the case of another test, "this" should
	  * be used as the value of the parent.
	  *
	  */
	Test(const var::String & name, Test * parent = 0);

	/*! \details Deconstructs the test object.
	  *
	  * The object must be deconstructed before Test::finalize() is
	  * executed.
	  */
	~Test();


	enum test_flags {
		execute_api /*! API Test Execution flag */ = (1<<0),
		execute_stress /*! Stress Test Execution flag */ = (1<<1),
		execute_performance /*! Performance Test Execution flag */ = (1<<2),
		execute_additional /*! Additional Test Execution flag */ = (1<<3),
		execute_all_types = execute_api | execute_stress | execute_performance | execute_additional,
		execute_all /*! Execute all test */ = static_cast<u32>(-1)
	};

	void execute(const sys::Cli & cli);

	/*! \details Executes the tests specified by \a o_flags.
	  *
	  * @param o_flags Bitmask of the tests to execute (e.g., Test::EXECUTE_API)
	  *
	  */
	virtual void execute(int o_flags = execute_all){
		if( o_flags & execute_api ){ execute_api_case(); }
		if( o_flags & execute_stress ){ execute_stress_case(); }
		if( o_flags & execute_performance ){ execute_performance_case(); }
		if( o_flags & execute_additional ){ execute_additional_cases(); }
	}

	/*! \details Executes the API test case. */
	void execute_api_case();
	/*! \details Executes the performance test case. */
	void execute_performance_case();
	/*! \details Executes the stress test case. */
	void execute_stress_case();
	/*! \details Executes additional test cases.
	  *
	  * By default there are not additional cases. The
	  * inheriting class should reimplement
	  * this method it additional test cases are used.
	  *
	  * \code
	  * void MyClassTest::execute_additional_cases(){
	  *   open_case("first test case");
	  *   set_case_message("message", "test case message");
	  *   close_case(true); //case passed
	  *
	  *   open_case("second test case");
	  *   set_case_message("message", "test case message");
	  *   close_case(false); //case failed
	  * }
	  *
	  * \endcode
	  *
	  */
	virtual void execute_additional_cases();

	/*! \details Executes the class api test.
	  *
	  * This method should be overridden by the inheriting
	  * class.
	  *
	  */
	virtual bool execute_class_api_case();

	/*! \details Executes the class performance test.
	  *
	  * This method should be overridden by the inheriting
	  * class.
	  *
	  */
	virtual bool execute_class_performance_case();

	/*! \details Executes the class stress test.
	  *
	  * This method should be overridden by the inheriting
	  * class.
	  *
	  */
	virtual bool execute_class_stress_case();


	/*! \details Returns the results of the test.
	  *
	  * If any single test case has a false result,
	  * the result of the test is false.  Otherwise,
	  * the result is true.
	  *
	  */
	bool result() const { return m_test_result; }

	/*! \details Returns the current value of the case result.
	  *
	  * When a case is opened, the case result is set to true (passing).
	  *
	  * Calling set_case_failed() or print_case_failed() will set
	  * the case result value to false.
	  *
	  * case_result() is the default return value of close_case().
	  *
	  */
	bool case_result() const { return m_case_result; }

	/*! \details Sets the current case result to failed. */
	void set_case_failed() { m_case_result = false; }

	/*! \details Closes the test case using the
	  * current case_result() value.
	  *
	  */
	void close_case();

	template<typename T> bool expect(
			const char * function,
			unsigned int line,
			const T & a,
			const T & b
			){
		if( a == b ){
			return true;
		}
		print_case_failed("%s:%d: value not expected", function, line);
		return false;
	}

	template<typename T> bool expect_not(
			const char * function,
			unsigned int line,
			const T & a,
			const T & b
			){
		if( !(a == b) ){
			return true;
		}
		print_case_failed("%s:%d: value not expected not", function, line);
		return false;
	}

	bool expect_error(
			const char * function,
			unsigned int line,
			int result,
			int expected_error
			){
		if( result < 0 ){
			if( expected_error == errno ){
				return true;
			}
			print_case_failed("%s : %d expected errno %d, got %d",
									function,
									line,
									expected_error,
									errno);
		} else {
			print_case_failed("%s : %d expected result < 0, got %d",
									function,
									line,
									result);
		}
		return false;
	}

	bool expect_error_null(
			const char * function,
			unsigned int line,
			void * result,
			int expected_error
			){
		if( result == nullptr ){
			if( expected_error == errno ){
				return true;
			}
			print_case_failed("%s : %d expected errno %d, got %d",
									function,
									line,
									expected_error,
									errno);
		} else {
			print_case_failed("%s : %d expected result null result, got %p",
									function,
									line,
									result);
		}
		return false;
	}


protected:

	friend class LeakGuard;
	/*! \details Returns a reference to the case timer.
	  *
	  * The case timer is used to time each case. It
	  * is started and stopped automatically.
	  *
	  * Test methods can access the value of the case timer
	  * while the test is running.
	  *
	  */
	const chrono::Timer & case_timer() const { return m_case_timer; }
	chrono::Timer & case_timer(){ return m_case_timer; }


	u32 score() const;

	/*! \details Prints a message to the test report.
	  *
	  * @param fmt Formatted string with variable arguments.
	  *
	  * The key to the message is assigned automatically as
	  * - msg-0
	  * - msg-1
	  * - msg-2
	  * - msg-...
	  *
	  */
	void print_case_message(const char * fmt, ...);

	void print_case_score();

	void print_case_failed(const char * fmt, ...);
	void print_case_failed(const api::Result & result, int line = 0){
		if( line ){
			print_case_failed("failed on line %d with return value %d and errno %d",
									line, result.return_value(), result.error_number());
		} else {
			print_case_failed("failed with return value %d and errno %d",
									result.return_value(), result.error_number());
		}
	}


	/*! \details Prints a message to the test report.
	  *
	  * @param key The key value for the saved message
	  * @param fmt Formatted string with variable arguments.
	  *
	  *
	  */
	void print_case_message_with_key(const var::String & key, const char * fmt, ...);

	int indent() const { return m_indent_count; }

	const var::String & name() const {
		return m_name;
	}

private:

	bool m_test_result = true;
	bool m_case_result = true;
	chrono::Timer m_case_timer;
	u32 m_test_duration_microseconds;
	u32 m_case_message_number = 0;
	u32 m_indent_count = 1;
	var::String m_name;
	Test * m_parent = nullptr;
	static bool m_is_initialized;
	static bool m_all_test_result;
	static u32 m_all_test_duration_microseconds;

	void vprint_case_message(const var::String & key, const char * fmt, va_list args);

	void print(const char * fmt, ...);
	static void print_indent(int indent, const char * fmt, ...);
	static void vprint_indent(int indent, const char * fmt, va_list args);

	void reset_indent(){
		m_indent_count = 0;
	}

	void increment_indent(){
		m_indent_count++;
	}

	void decrement_indent(){
		if( m_indent_count ){
			m_indent_count--;
		}
	}

	friend class Case;
	void open_case(const var::String & case_name);
	void close_case(bool result);
};

}

#endif // SAPI_TEST_TEST_HPP_
