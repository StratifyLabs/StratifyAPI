
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

    if (!path_api_case()) {
      return false;
    }

    if (!file_api_case()) {
      return false;
    }

    if (!file_system_api_case()) {
      return false;
    }

    return true;
  }

  bool path_api_case() {
    using P = fs::Path;
    printer::PrinterObject po(printer(), "path");

    TEST_ASSERT(P("data/test.json").path() == "data/test.json");
    TEST_ASSERT(P("flat").name() == "flat");
    TEST_ASSERT(P("flat.json").name() == "flat.json");
    TEST_ASSERT(P("flat.json").suffix() == "json");
    TEST_ASSERT(P("data/test.json").suffix() == "json");
    TEST_ASSERT(P("data/test.json").name() == "test.json");
    TEST_ASSERT(P("data/test.json").base_name() == "test");
    TEST_ASSERT(P("data/test.json").no_suffix() == "data/test");
    TEST_ASSERT(P("data/test.json").parent_directory() == "data");
    TEST_ASSERT(P("/Users/data/test.json").parent_directory() == "/Users/data");
    TEST_ASSERT(P("/Users/data/test.json").no_suffix() == "/Users/data/test");
    TEST_ASSERT(P("data/.test.json").is_hidden());
    TEST_ASSERT(P("data/test.json").is_hidden() == false);

    return true;
  }

  bool file_system_api_case() {
    using FS = fs::FileSystem;
    using F = fs::File;
    using DF = fs::DataFile;

    printer::PrinterObject po(printer(), "file_system");

    {
      printer::PrinterObject po(printer(), "create/remove directories");

      constexpr const char *file_name = "filessytem.txt";
      const StringView dir_name = "tmpdir";
      const StringView dir_name_recursive = "tmpdir/tmp/dir";
      const StringView file_name2 = "filessytem2.txt";

      // cleanup
      reset_error_context();
      FS().remove_directory("tmpdir/tmp/dir", FS::IsRecursive::yes);
      reset_error_context();
      FS().remove_directory("tmpdir/tmp", FS::IsRecursive::yes);
      reset_error_context();
      FS().remove_directory("tmpdir", FS::IsRecursive::yes);
      reset_error_context();

      TEST_ASSERT(is_success());

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

      TEST_ASSERT(FS().create_directory(dir_name).is_success());
      TEST_ASSERT(FS().remove_directory(dir_name).is_success());

      TEST_EXPECT(FS().create_directory(dir_name_recursive).is_error());

      TEST_EXPECT(dir_name_recursive == error_context().message());
      TEST_EXPECT(error_context().message() == dir_name_recursive);

      reset_error_context();

      TEST_EXPECT(FS()
                    .create_directory(dir_name_recursive, FS::IsRecursive::yes)
                    .is_success());

      TEST_EXPECT(F(dir_name_recursive + "/tmp.txt", F::IsCreateOverwrite::yes)
                    .write("Hello")
                    .is_success);

      TEST_ASSERT(
        DF()
          .write(F(dir_name_recursive + "/tmp.txt", OpenMode::read_only()))
          .get_string()
        == "Hello");

      TEST_EXPECT(F(Path(dir_name_recursive).parent_directory() + "/tmp.txt",
                    F::IsCreateOverwrite::yes)
                    .write("Hello2")
                    .is_success);

      TEST_ASSERT(
        DF()
          .write(
            F(Path(dir_name_recursive).parent_directory() + "/tmp.txt",
              OpenMode::read_only()))
          .get_string()
        == "Hello2");

      TEST_EXPECT(
        F(Path(Path(dir_name_recursive).parent_directory()).parent_directory()
            + "/tmp.txt",
          F::IsCreateOverwrite::yes)
          .write("Hello3")
          .is_success);

      TEST_ASSERT(
        DF()
          .write(F(
            Path(Path(dir_name_recursive).parent_directory()).parent_directory()
              + "/tmp.txt",
            OpenMode::read_only()))
          .get_string()
        == "Hello3");

      TEST_EXPECT(FS().exists(dir_name_recursive) == true);

      TEST_EXPECT(
        FS().exists(Path(dir_name_recursive).parent_directory()) == true);

      TEST_EXPECT(
        FS().exists(
          Path(Path(dir_name_recursive).parent_directory()).parent_directory())
        == true);

      TEST_EXPECT(
        FS().remove_directory(dir_name, FS::IsRecursive::yes).is_success());

      TEST_EXPECT(
        FS().remove_directory(dir_name, FS::IsRecursive::yes).is_error());

      TEST_EXPECT(error_context().message() == dir_name);

      reset_error_context();
    }

    {
      printer::PrinterObject po(printer(), "directory permissions");
      Permissions permissions = FS().get_info(".").permissions();
      const StringView dir_name = "permdir";

      TEST_ASSERT(FS().create_directory(dir_name).is_success());
      TEST_ASSERT(FS().get_info(dir_name).permissions() == permissions);
    }

    return true;
  }

  bool file_api_case() {
    using F = fs::File;
    using DF = fs::DataFile;
    using FS = fs::FileSystem;
    printer::PrinterObject po(printer(), "file");

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
