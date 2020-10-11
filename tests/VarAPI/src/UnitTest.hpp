
#include <cstdio>
#include <mcu/types.h>

#include "api/api.hpp"
#include "chrono.hpp"
#include "sys.hpp"
#include "test/Test.hpp"
#include "thread.hpp"
#include "var.hpp"

using S = var::String;
using SV = var::StringView;
using D = var::Data;
using V = var::View;

using B64 = var::Base64;

class UnitTest : public test::Test {
public:
  UnitTest(var::StringView name) : test::Test(name) {}

  bool execute_class_api_case() {
    if (!string_api_case()) {
      return false;
    }

    if (!string_view_api_case()) {
      return false;
    }

    if (!base64_api_case()) {
      return false;
    }

    return true;
  }

  bool base64_api_case() {
    TEST_EXPECT(Base64().encode(var::StringView("1")) == "MQ==");
    TEST_EXPECT(Base64().encode(var::StringView("ab")) == "YWI=");
    TEST_EXPECT(Base64().encode(var::StringView("234")) == "MjM0");
    TEST_EXPECT(Base64().encode(var::StringView("5678")) == "NTY3OA==");
    TEST_EXPECT(Base64().encode(var::StringView("9abcd")) == "OWFiY2Q=");
    TEST_EXPECT(Base64().encode(var::StringView("efghij")) == "ZWZnaGlq");
    TEST_EXPECT(Base64().encode(var::StringView("KLMNOPQ")) == "S0xNTk9QUQ==");
    TEST_EXPECT(Base64().encode(var::StringView("rstuvwxy")) == "cnN0dXZ3eHk=");

    return true;
  }

  bool string_view_api_case() {
    {
      SV sv;
      TEST_EXPECT(sv.is_empty());
      TEST_EXPECT(!sv.is_null());
      TEST_EXPECT(sv.cstring() != nullptr);
    }

    {
      SV sv("");
      TEST_EXPECT(sv.is_empty());
      TEST_EXPECT(!sv.is_null());
      TEST_EXPECT(sv.cstring() != nullptr);
    }

    {
      SV sv("testing");
      TEST_EXPECT(!sv.is_empty());
      TEST_EXPECT(!sv.is_null());
      TEST_EXPECT(sv.cstring() != nullptr);
      TEST_EXPECT(sv.cstring()[1] == 'e');
      TEST_EXPECT(sv.cstring()[sv.length()] == 0);
      TEST_EXPECT(sv == "testing");
      TEST_EXPECT(!(sv != "testing"));
      TEST_EXPECT(sv.front() == 't');
      TEST_EXPECT(sv.back() == 'g');
      TEST_EXPECT(sv.at(0) == 't');
      TEST_EXPECT(sv.at(1) == 'e');
      TEST_EXPECT(sv.at(2) == 's');
      TEST_EXPECT(sv.at(3) == 't');
      TEST_EXPECT(sv.at(4) == 'i');
      TEST_EXPECT(sv.at(5) == 'n');
      TEST_EXPECT(sv.at(6) == 'g');
      TEST_EXPECT(sv.length() == (sizeof("testing") - 1));

      TEST_EXPECT(sv.get_substring_at_position(2) == "sting");
      TEST_EXPECT(sv.get_substring_with_length(4) == "test");

      TEST_EXPECT(sv.find('e') == 1);
      TEST_EXPECT(sv.find('x') == SV::npos);
      TEST_EXPECT(sv.reverse_find('e') == 1);
      TEST_EXPECT(sv.reverse_find('x') == SV::npos);

      TEST_EXPECT(sv.find("e") == 1);
      TEST_EXPECT(sv.find("x") == SV::npos);
      TEST_EXPECT(sv.reverse_find("e") == 1);
      TEST_EXPECT(sv.reverse_find("x") == SV::npos);
      TEST_EXPECT(sv.find_first_of("abcde") == 1);
      TEST_EXPECT(sv.find_first_of("wxyz") == SV::npos);
      TEST_EXPECT(sv.find_first_not_of("abcde") == 0);
      TEST_EXPECT(sv.find_first_not_of("test") == 4);

#if RUN_NOT_PASSING
      TEST_EXPECT(sv.find_last_of("rst") == 3);
      TEST_EXPECT(sv.find_last_not_of("in") == 6);
#endif

      TEST_EXPECT(
        sv.get_substring(SV::GetSubstring().set_position(2).set_length(3))
        == "sti");

      size_t i = 0;
      for (auto c : sv) {
        TEST_EXPECT(c == sv.at(i++));
      }

      TEST_EXPECT(sv.to_long() == 0);

      TEST_EXPECT(sv.pop_front(4) == "ing");
    }

    {
      SV sv("105");
      TEST_EXPECT(sv.to_long() == 105);
      TEST_EXPECT(sv.to_unsigned_long() == 105);
      TEST_EXPECT(sv.to_long(SV::Base::decimal) == 105);
      TEST_EXPECT(sv.to_unsigned_long(SV::Base::decimal) == 105);
    }

    {
      SV sv("-500");
      TEST_EXPECT(sv.to_long() == -500);
    }

    {
      SV sv("0.456");
      TEST_EXPECT(sv.to_float() == 0.456f);
    }

    {
      SV sv("1E6");
      TEST_EXPECT(sv.to_float() == 1E6f);
    }

    {
      SV sv("123E-3");
      TEST_EXPECT(sv.to_float() == 123E-3f);
    }

    {
      SV sv("0666");
      TEST_EXPECT(sv.to_long(SV::Base::octal) == 0666);
      TEST_EXPECT(sv.to_unsigned_long(SV::Base::octal) == 0666);
    }

    {
      SV sv("abcd");
      TEST_EXPECT(sv.to_long(SV::Base::hexidecimal) == 0xabcd);
      TEST_EXPECT(sv.to_unsigned_long(SV::Base::hexidecimal) == 0xabcd);
    }

    {
      SV sv;
      TEST_EXPECT(sv.is_empty());
      TEST_EXPECT(!sv.is_null());
      sv.set_null();
      TEST_EXPECT(sv.is_empty());
      TEST_EXPECT(sv.is_null());
      TEST_EXPECT(SV().set_null().is_null());
    }

    return true;
  }

  bool string_api_case() {
    {
      S s("construct");
      TEST_EXPECT(s == "construct");
    }

    return true;
  }

private:
  const void *m_original_context = nullptr;
};
