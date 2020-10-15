
#include <cstdio>
#include <mcu/types.h>

#include "api/api.hpp"
#include "chrono.hpp"
#include "crypto.hpp"
#include "fs.hpp"
#include "printer.hpp"
#include "sys.hpp"
#include "test/Test.hpp"
#include "var.hpp"

class UnitTest : public test::Test {
public:
  UnitTest(var::StringView name) : test::Test(name) {}

  bool execute_class_api_case() { return true; }

private:
};
