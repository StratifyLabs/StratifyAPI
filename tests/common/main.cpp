
#include "UnitTest.hpp"

#define VERSION "0.1"
#include <sys.hpp>

int main(int argc, char *argv[]) {
  Cli cli(argc, argv);

  printer::Printer printer;

  test::Test::initialize(test::Test::Initialize()
                           .set_name(cli.get_name())
                           .set_version(VERSION)
                           .set_git_hash(SOS_GIT_HASH)
                           .set_printer(&printer));

  { UnitTest(cli.get_name()).execute(cli); }

  test::Test::finalize();

  exit(test::Test::final_result() == false);

  return 0;
}
