/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#ifndef SAPI_TEST_TEST_HPP_
#define SAPI_TEST_TEST_HPP_

#include <cstdarg>
#if defined __link
#include <errno.h>
#include <sos/link.h>
#endif

#include "chrono/Time.hpp"
#include "chrono/Timer.hpp"
#include "printer/Printer.hpp"
#include "sys/Cli.hpp"
#include "var/Data.hpp"
#include "var/StringView.hpp"

namespace test {

#define TEST_EXPECT(T, b, c)                                                   \
  this->expect<T>(__PRETTY_FUNCTION__, __LINE__, b, c)
#define TEST_ASSERT(T, b, c)                                                   \
  do {                                                                         \
    if (this->expect<T>(__PRETTY_FUNCTION__, __LINE__, b, c) == false) {       \
      return case_result();                                                    \
    }                                                                          \
  } while (0)

struct TestFlags {
  enum class ExecuteFlags : u32 {
    none = 0,
    api /*! API Test Execution flag */ = (1 << 0),
    stress /*! Stress Test Execution flag */ = (1 << 1),
    performance /*! Performance Test Execution flag */ = (1 << 2),
    all_types = 0x7,
    all /*! Execute all test */ = static_cast<u32>(-1)
  };
};

API_OR_NAMED_FLAGS_OPERATOR(TestFlags, ExecuteFlags)

class Test : public api::Object, public TestFlags {
public:
  class Options {
    API_AC(Options, var::String, name);
    API_AC(Options, var::String, version);
    API_AC(Options, var::String, git_hash);
    API_AF(Options, printer::Printer *, printer, nullptr);
  };

  /*! \details Initializes the test report.
   *
   * This must be called before any tests are even
   * constructed.
   *
   */
  static void initialize(const Options &options);

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
  static ExecuteFlags parse_execution_flags(const sys::Cli &cli);
  static u32
  parse_test(const sys::Cli &cli, var::StringView name, u32 test_flag);

  Test(var::StringView name);
  ~Test();

  void execute(const sys::Cli &cli);

  /*! \details Executes the tests specified by \a o_flags.
   *
   * @param o_flags Bitmask of the tests to execute (e.g., Test::EXECUTE_API)
   *
   */
  void execute(ExecuteFlags execute_flags = ExecuteFlags::all) {
    if (execute_flags & ExecuteFlags::api) {
      execute_api_case();
    }
    if (execute_flags & ExecuteFlags::stress) {
      execute_stress_case();
    }
    if (execute_flags & ExecuteFlags::performance) {
      execute_performance_case();
    }
  }

  /*! \details Executes the API test case. */
  void execute_api_case();
  /*! \details Executes the performance test case. */
  void execute_performance_case();
  /*! \details Executes the stress test case. */
  void execute_stress_case();

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
  void set_case_failed() {
    m_case_result = false;
    m_test_result = false;
    m_final_result = false;
  }

  template <typename T>
  bool expect(const char *function, unsigned int line, const T &a, const T &b) {
    if (a == b) {
      return true;
    }
    printer().error(
      "failed",
      var::String().format("%s:%d: value not expected", function, line));
    set_case_failed();
    return false;
  }

protected:
  const chrono::Timer &case_timer() const { return m_case_timer; }
  chrono::Timer &case_timer() { return m_case_timer; }

  static u32 get_score(u32 microseconds);

  var::StringView name() const { return m_name; }

  static printer::Printer &printer() { return *m_printer; }

private:
  bool m_test_result = true;
  bool m_case_result = true;
  var::DataInfo m_case_data_info;
  var::DataInfo m_test_data_info;
  chrono::Timer m_case_timer;
  u32 m_test_duration_microseconds;
  var::String m_name;
  Test *m_parent = nullptr;

  static var::DataInfo m_final_data_info;
  static bool m_final_result;
  static u32 m_final_duration_microseconds;
  static printer::Printer *m_printer;

  friend class Case;
  void open_case(var::StringView case_name);
  void close_case();
};

} // namespace test

#endif // SAPI_TEST_TEST_HPP_
