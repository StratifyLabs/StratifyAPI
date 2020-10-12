
#include <cstdio>
#include <mcu/types.h>

#include "api/api.hpp"
#include "chrono.hpp"
#include "fs.hpp"
#include "sys.hpp"
#include "test/Test.hpp"
#include "thread.hpp"
#include "var.hpp"

using S = var::String;
using SV = var::StringView;
using D = var::Data;
using V = var::View;
using T = var::Tokenizer;

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

    if (!tokenizer_api_case()) {
      return false;
    }

    if (!view_api_case()) {
      return false;
    }

    return true;
  }

  bool view_api_case() {

    TEST_EXPECT(V().to_const_char() == nullptr);

    return true;
  }

  bool tokenizer_api_case() {

    {
      T t("0,1,2", T::Construct().set_delimeters(","));
      TEST_EXPECT(t.count() == 3);
      TEST_EXPECT(t.at(0) == "0");
      TEST_EXPECT(t.at(1) == "1");
      TEST_EXPECT(t.at(2) == "2");
    }

    {
      T t(
        "0,'1,2'",
        T::Construct().set_delimeters(",").set_ignore_between("'"));
      TEST_ASSERT(t.count() == 2);
      TEST_EXPECT(t.at(0) == "0");
      TEST_EXPECT(t.at(1) == "'1,2'");
    }

    {
      T t(
        "0,\"1,2\"",
        T::Construct().set_delimeters(",").set_ignore_between("\""));
      TEST_ASSERT(t.count() == 2);
      TEST_EXPECT(t.at(0) == "0");
      TEST_EXPECT(t.at(1) == "\"1,2\"");
    }

    {
      T t(
        "0,1,2,3,4,5",
        T::Construct().set_delimeters(",").set_maximum_delimeter_count(2));
      TEST_EXPECT(t.count() == 3);
      TEST_EXPECT(t.at(0) == "0");
      TEST_EXPECT(t.at(1) == "1");
      TEST_EXPECT(t.at(2) == "2,3,4,5");
    }

    {
      T t(
        "0,'1,2,3',4,5",
        T::Construct()
          .set_delimeters(",")
          .set_maximum_delimeter_count(2)
          .set_ignore_between("'"));
      TEST_EXPECT(t.count() == 3);
      TEST_EXPECT(t.at(0) == "0");
      TEST_EXPECT(t.at(1) == "'1,2,3'");
      TEST_EXPECT(t.at(2) == "4,5");
    }

    {
      T t("0,1,,,2", T::Construct().set_delimeters(","));
      TEST_EXPECT(t.count() == 5);
      TEST_EXPECT(t.at(0) == "0");
      TEST_EXPECT(t.at(1) == "1");
      TEST_EXPECT(t.at(2) == "");
      TEST_EXPECT(t.at(3) == "");
      TEST_EXPECT(t.at(4) == "2");
    }

    {
      T t(
        "0,1,Testing'123'Testing,2",
        T::Construct().set_delimeters(",").set_ignore_between("'"));
      TEST_EXPECT(t.count() == 4);
      TEST_EXPECT(t.at(0) == "0");
      TEST_EXPECT(t.at(1) == "1");
      TEST_EXPECT(t.at(2) == "Testing'123'Testing");
      TEST_EXPECT(t.at(3) == "2");
    }

    {
      T t(
        "'0,1',Testing'123'Testing,2",
        T::Construct().set_delimeters(",").set_ignore_between("'"));
      TEST_EXPECT(t.count() == 3);
      TEST_EXPECT(t.at(0) == "'0,1'");
      TEST_EXPECT(t.at(1) == "Testing'123'Testing");
      TEST_EXPECT(t.at(2) == "2");
    }

    return true;
  }

  bool base64_api_case() {

    bool (*encode_test)(const char *, const char *)
      = [](const char *input, const char *output) {
          return (Base64().encode(SV(input)) == S(output));
        };

    TEST_EXPECT(encode_test("1", "MQ=="));
    TEST_EXPECT(encode_test("ab", "YWI="));
    TEST_EXPECT(encode_test("234", "MjM0"));
    TEST_EXPECT(encode_test("5678", "NTY3OA=="));
    TEST_EXPECT(encode_test("9abcd", "OWFiY2Q="));
    TEST_EXPECT(encode_test("efghij", "ZWZnaGlq"));
    TEST_EXPECT(encode_test("KLMNOPQ", "S0xNTk9QUQ=="));
    TEST_EXPECT(encode_test("rstuvwxy", "cnN0dXZ3eHk="));

    bool (*decode_test)(const char *, const char *)
      = [](const char *output, const char *input) {
          return (V(Base64().decode(input)) == V(output));
        };

    TEST_EXPECT(decode_test("1", "MQ=="));
    TEST_EXPECT(decode_test("ab", "YWI="));
    TEST_EXPECT(decode_test("234", "MjM0"));
    TEST_EXPECT(decode_test("5678", "NTY3OA=="));
    TEST_EXPECT(decode_test("9abcd", "OWFiY2Q="));
    TEST_EXPECT(decode_test("efghij", "ZWZnaGlq"));
    TEST_EXPECT(decode_test("KLMNOPQ", "S0xNTk9QUQ=="));
    TEST_EXPECT(decode_test("rstuvwxy", "cnN0dXZ3eHk="));

    const char test_input[]
      = "In computer science, Base64 is a group of binary-to-text encoding "
        "schemes that represent binary data in an ASCII string format by "
        "translating it into a radix-64 representation. The term Base64 "
        "originates from a specific MIME content transfer encoding. Each "
        "Base64 digit represents exactly 6 bits of data. Three 8-bit bytes "
        "(i.e., a total of 24 bits) can therefore be represented by four 6-bit "
        "Base64 digits. Common to all binary-to-text encoding schemes, Base64 "
        "is designed to carry data stored in binary formats across channels "
        "that only reliably support text content. Base64 is particularly "
        "prevalent on the World Wide Web[1] where its uses include the ability "
        "to embed image files or other binary assets inside textual assets "
        "such as HTML and CSS files.[2] Base64 is also widely used for sending "
        "e-mail attachments. This is required because SMTP in its original "
        "form was designed to transport 7 bit ASCII characters only. This "
        "encoding causes an overhead of 33–36% (33% by the encoding itself, up "
        "to 3% more by the inserted line breaks).";

    const char test_output[]
      = "SW4gY29tcHV0ZXIgc2NpZW5jZSwgQmFzZTY0IGlzIGEgZ3JvdXAgb2YgYmluYXJ5LXRvLX"
        "RleHQgZW5jb2Rpbmcgc2NoZW1lcyB0aGF0IHJlcHJlc2VudCBiaW5hcnkgZGF0YSBpbiBh"
        "biBBU0NJSSBzdHJpbmcgZm9ybWF0IGJ5IHRyYW5zbGF0aW5nIGl0IGludG8gYSByYWRpeC"
        "02NCByZXByZXNlbnRhdGlvbi4gVGhlIHRlcm0gQmFzZTY0IG9yaWdpbmF0ZXMgZnJvbSBh"
        "IHNwZWNpZmljIE1JTUUgY29udGVudCB0cmFuc2ZlciBlbmNvZGluZy4gRWFjaCBCYXNlNj"
        "QgZGlnaXQgcmVwcmVzZW50cyBleGFjdGx5IDYgYml0cyBvZiBkYXRhLiBUaHJlZSA4LWJp"
        "dCBieXRlcyAoaS5lLiwgYSB0b3RhbCBvZiAyNCBiaXRzKSBjYW4gdGhlcmVmb3JlIGJlIH"
        "JlcHJlc2VudGVkIGJ5IGZvdXIgNi1iaXQgQmFzZTY0IGRpZ2l0cy4gQ29tbW9uIHRvIGFs"
        "bCBiaW5hcnktdG8tdGV4dCBlbmNvZGluZyBzY2hlbWVzLCBCYXNlNjQgaXMgZGVzaWduZW"
        "QgdG8gY2FycnkgZGF0YSBzdG9yZWQgaW4gYmluYXJ5IGZvcm1hdHMgYWNyb3NzIGNoYW5u"
        "ZWxzIHRoYXQgb25seSByZWxpYWJseSBzdXBwb3J0IHRleHQgY29udGVudC4gQmFzZTY0IG"
        "lzIHBhcnRpY3VsYXJseSBwcmV2YWxlbnQgb24gdGhlIFdvcmxkIFdpZGUgV2ViWzFdIHdo"
        "ZXJlIGl0cyB1c2VzIGluY2x1ZGUgdGhlIGFiaWxpdHkgdG8gZW1iZWQgaW1hZ2UgZmlsZX"
        "Mgb3Igb3RoZXIgYmluYXJ5IGFzc2V0cyBpbnNpZGUgdGV4dHVhbCBhc3NldHMgc3VjaCBh"
        "cyBIVE1MIGFuZCBDU1MgZmlsZXMuWzJdIEJhc2U2NCBpcyBhbHNvIHdpZGVseSB1c2VkIG"
        "ZvciBzZW5kaW5nIGUtbWFpbCBhdHRhY2htZW50cy4gVGhpcyBpcyByZXF1aXJlZCBiZWNh"
        "dXNlIFNNVFAgaW4gaXRzIG9yaWdpbmFsIGZvcm0gd2FzIGRlc2lnbmVkIHRvIHRyYW5zcG"
        "9ydCA3IGJpdCBBU0NJSSBjaGFyYWN0ZXJzIG9ubHkuIFRoaXMgZW5jb2RpbmcgY2F1c2Vz"
        "IGFuIG92ZXJoZWFkIG9mIDMz4oCTMzYlICgzMyUgYnkgdGhlIGVuY29kaW5nIGl0c2VsZi"
        "wgdXAgdG8gMyUgbW9yZSBieSB0aGUgaW5zZXJ0ZWQgbGluZSBicmVha3MpLg==";

    // TEST_EXPECT(encode_test(test_input, test_output));
    TEST_EXPECT(decode_test(test_input, test_output));

#if RUN_NOT_PASSING
    {
      // the base64 decode needs to have the entire dataset passed in one call
      TEST_EXPECT(
        S(DataFile()
            .write(
              ViewFile(V(test_output)),
              Base64Decoder(),
              DataFile::Write().set_page_size(sizeof(test_output)))
            .data())
        == test_input);
    }
#endif

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
