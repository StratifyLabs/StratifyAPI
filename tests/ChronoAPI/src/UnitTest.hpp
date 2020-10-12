
#include <cstdio>
#include <mcu/types.h>

#include "api/api.hpp"
#include "chrono.hpp"
#include "fs.hpp"
#include "sys.hpp"
#include "test/Test.hpp"
#include "thread.hpp"
#include "var.hpp"

class UnitTest : public test::Test {
public:
  UnitTest(var::StringView name) : test::Test(name) {}

  bool execute_class_api_case() {
    if (!execute_clocktime_api_case()) {
      return false;
    }

    if (!execute_microtime_api_case()) {
      return false;
    }

    return true;
  }

  bool execute_microtime_api_case() {

    using MT = MicroTime;

    TEST_EXPECT(1_seconds == 1000_milliseconds);
    TEST_EXPECT(1_milliseconds == 1000_microseconds);
    TEST_EXPECT(5000000_microseconds == 5_seconds);

    {
      MT start(5_seconds);
      TEST_EXPECT(start + 2_seconds == 7_seconds);
      TEST_EXPECT(start + 10_milliseconds == 5010000_microseconds);
      TEST_EXPECT(start + 100_microseconds == 5000100_microseconds);
    }

    ClockTimer start = ClockTimer().start();
    wait(10_milliseconds);
    TEST_EXPECT(start.stop().milliseconds() >= 10);

    return true;
  }

  bool execute_clocktime_api_case() {
    using CT = chrono::ClockTime;

    CT now = CT::get_system_time();
    TEST_EXPECT(now <= CT::get_system_time());
    wait(50_milliseconds);
    ClockTime later = CT::get_system_time();
    TEST_EXPECT(later - CT(50_milliseconds) > now);

    CT resolution = CT::get_system_resolution();

    TEST_EXPECT(
      CT(1_milliseconds) + CT(100_microseconds) == CT(1100_microseconds));
    TEST_EXPECT(CT(5_seconds) + CT(3200_milliseconds) == CT(8200_milliseconds));

    TEST_EXPECT(CT(5_seconds) - CT(3000_milliseconds) == CT(2000_milliseconds));

    TEST_EXPECT(CT(12345678_microseconds).get_string() == "12.345678000");

    String unique = CT::get_unique_string();
    wait(1_milliseconds);
    TEST_EXPECT(unique != CT::get_unique_string());

    {
      CT ct = CT().set_seconds(10).set_nanoseconds(5);
      TEST_EXPECT(ct.seconds() == 10);
      TEST_EXPECT(ct.nanoseconds() == 5);
    }

    return true;
  }

private:
};
