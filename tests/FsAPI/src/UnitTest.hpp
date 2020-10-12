
#include <cstdio>
#include <mcu/types.h>

#include "api/api.hpp"
#include "chrono.hpp"
#include "fs.hpp"
#include "sys.hpp"
#include "test/Test.hpp"
#include "var.hpp"

class UnitTest : public test::Test {
public:
  UnitTest(var::StringView name) : test::Test(name) {}

  bool execute_class_api_case() {

    TEST_EXPECT(true);

    if (!file_api_case()) {
      return false;
    }

    if (!file_system_api_case()) {
      return false;
    }

    return true;
  }

  bool file_system_api_case() {
    using FS = fs::FileSystem;
    using F = fs::File;

    constexpr const char *file_name = "filessytem.txt";
    StringView dir_name = "tmpdir";
    StringView file_name2 = "filessytem2.txt";

    TEST_ASSERT(F(file_name, F::IsCreateOverwrite::yes)
                  .write("Filesystem file")
                  .status()
                  .is_success());

    TEST_EXPECT(FS().exists(file_name));

    // not exists should not affect the error context
    TEST_EXPECT(!FS().exists(file_name2) && status().is_success());

    TEST_EXPECT(FS().remove(file_name).status().is_success());
    TEST_EXPECT(!FS().exists(file_name) && status().is_success());

    TEST_ASSERT(F(file_name, F::IsCreateOverwrite::yes)
                  .write(file_name2)
                  .status()
                  .is_success());

    TEST_EXPECT(FS().size(file_name) == file_name2.length());

    TEST_EXPECT(FS().create_directory(dir_name).is_success());
    TEST_EXPECT(FS().remove_directory(dir_name).is_success());

    return true;
  }

  bool file_api_case() {
    using F = fs::File;
    using DF = fs::DataFile;
    using FS = fs::FileSystem;

    constexpr const char *file_name = "tmp.txt";

    const std::array<const char *, 5> test_strings = {
      "Testing String 0\n",
      "Testing String 1\n",
      "Testing String 2\n",
      "Testing String 3\n",
      "Testing String 4\n"};

    reset_error_context();

    TEST_ASSERT(F(file_name, F::IsCreateOverwrite::yes)
                  .write(test_strings.at(0))
                  .is_success());

    TEST_EXPECT(F(file_name, F::IsCreateOverwrite::no)
                  .write(test_strings.at(0))
                  .is_error());

    TEST_EXPECT(var::StringView(error_context().message()) == file_name);

    reset_error_context();

    TEST_EXPECT(
      String(DF().write(F(file_name, OpenMode::read_only())).data())
      == test_strings.at(0));

    TEST_EXPECT(return_value() == StringView(test_strings.at(0)).length());

    TEST_ASSERT(F(file_name, F::IsCreateOverwrite::yes)
                  .write(test_strings.at(0))
                  .write(test_strings.at(1))
                  .write(test_strings.at(2))
                  .write(test_strings.at(3))
                  .write(test_strings.at(4))
                  .status()
                  .is_success());

    TEST_EXPECT(F("tmp1.txt", OpenMode::read_only()).status().is_error());
    TEST_EXPECT(F(file_name, OpenMode::read_only()).status().is_error());

    reset_error_context();
    TEST_EXPECT(F(file_name, OpenMode::read_only()).status().is_success());

    {
      F tmp(file_name, OpenMode::read_write());

      TEST_EXPECT(FS().get_info(tmp).is_file());

      TEST_EXPECT(
        DataFile()
          .reserve(256)
          .write(tmp, F::Write().set_terminator('\n'))
          .get_string()
        == test_strings.at(0));

      TEST_EXPECT(
        DataFile()
          .reserve(256)
          .write(tmp, F::Write().set_terminator('\n'))
          .get_string()
        == test_strings.at(1));

      TEST_EXPECT(
        DataFile()
          .reserve(256)
          .write(tmp, F::Write().set_terminator('\n'))
          .get_string()
        == test_strings.at(2));

      TEST_EXPECT(
        DataFile()
          .reserve(256)
          .write(tmp, F::Write().set_terminator('\n'))
          .get_string()
        == test_strings.at(3));

      TEST_EXPECT(
        DataFile()
          .reserve(256)
          .write(tmp, F::Write().set_terminator('\n'))
          .get_string()
        == test_strings.at(4));

      TEST_EXPECT(
        DataFile()
          .reserve(256)
          .write(
            tmp.seek(var::StringView(test_strings.at(0)).length()),
            F::Write().set_terminator('\n'))
          .get_string()
        == test_strings.at(1));
    }

    return true;
  }

private:
};
