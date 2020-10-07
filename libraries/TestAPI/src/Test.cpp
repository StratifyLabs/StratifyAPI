/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#include <cstdio>

#include "sys.hpp"
#include "test/Case.hpp"
#include "test/Test.hpp"
#include "var/String.hpp"

using namespace test;

bool Test::m_is_initialized = false;
bool Test::m_all_test_result = true;
u32 Test::m_all_test_duration_microseconds = 0;

Test::Test(const var::String &name, Test *parent) {
  // start a JSON object

  m_name = name;

  if (!m_is_initialized) {
    // initialize();
  }

  m_parent = parent;
  if (m_parent) {
    m_indent_count = parent->indent();
  } else {
    m_indent_count = 1;
  }

  m_test_result = true;

  print("\"%s\": {\n", name.cstring());
  increment_indent();

  m_test_duration_microseconds = 0;
}

Test::~Test() {
  // close the JSON object

  if (m_parent) {
    m_parent->m_test_duration_microseconds += m_test_duration_microseconds;
  } else {
    m_all_test_duration_microseconds += m_test_duration_microseconds;
  }

  if (m_test_result == false) {
    print("\"result\": false,\n");
  } else {
    print("\"result\": true,\n");
  }

  print("\"microseconds\": %ld.0\n", m_test_duration_microseconds);
  decrement_indent();
  print("},\n");
}

u32 Test::score() const {
  const u32 baseline_microseconds = 1000000000UL;
  return baseline_microseconds / case_timer().microseconds();
}

void Test::print_case_score() {
  // print_case_message_with_key("score", "%ld", score());
}

void Test::execute(const sys::Cli &cli) {
  u32 o_flags = 0;

  if (cli.get_option("api") == "true") {
    o_flags |= execute_api;
  }
  if (cli.get_option("stress") == "true") {
    o_flags |= execute_stress;
  }
  if (cli.get_option("performance") == "true") {
    o_flags |= execute_performance;
  }
  if (cli.get_option("additional") == "true") {
    o_flags |= execute_additional;
  }
  if (cli.get_option("all") == "true") {
    o_flags |= execute_all;
  }

  if (o_flags == 0) {
    o_flags |= execute_api;
  }

  execute(o_flags);
}

void Test::open_case(var::StringView case_name) {
  print("\"%s\": {\n", case_name.cstring());
  increment_indent();
  m_case_message_number = 0;
  m_case_result = true;
  m_case_timer.restart();
}

void Test::close_case() { close_case(case_result()); }

void Test::close_case(bool result) {
  m_case_timer.stop();
  m_test_duration_microseconds += m_case_timer.microseconds();
  if (result == false) {
    m_all_test_result = false;
    m_test_result = false;
    print("\"result\": false,\n");
  } else {
    print("\"result\": true,\n");
  }

  print("\"microseconds\": %ld.0\n", m_case_timer.microseconds());
  decrement_indent();
  print("},\n");
  m_case_result = true;
}

void Test::initialize(const Options &options) {
  m_is_initialized = true;
  m_all_test_duration_microseconds = 0;

  print_indent(0, "{\n");

#if !defined __link
  Sys::Info info = Sys().get_info();
  print_indent(1, "\"system\": {\n");
  if (info.is_valid() == false) {
    print_indent(2, "\"name\": \"unknown\"\n");
  } else {
    print_indent(2, "\"name\": \"%s\",\n", info.name().cstring());
    print_indent(2, "\"arch\": \"%s\",\n", info.arch().cstring());
    print_indent(
      2,
      "\"bspVersion\": \"%s\",\n",
      info.system_version().cstring());
    print_indent(
      2,
      "\"sosVersion\": \"%s\",\n",
      info.kernel_version().cstring());
    print_indent(2, "\"memorySize\": \"%ld\",\n", 0);
    print_indent(2, "\"projectId\": \"%s\",\n", info.id().cstring());
    print_indent(2, "\"hardwareId\": \"%08X\",\n", info.hardware_id());
    print_indent(
      2,
      "\"serialNumber\": \"%s\",\n",
      info.serial_number().to_string());
    print_indent(2, "\"cpuFrequency\": \"%ld\",\n", info.cpu_frequency());
#if 0
    if (info.mcu & MCU_BOARD_CONFIG_FLAG_ENABLE_CACHE) {
      print_indent(2, "\"cache\": true,\n");
    } else {
      print_indent(2, "\"cache\": false,\n");
    }
#endif
    print_indent(2, "\"bspGitHash\": \"%s\",\n", info.bsp_git_hash().cstring());
    print_indent(2, "\"sosGitHash\": \"%s\",\n", info.sos_git_hash().cstring());
    print_indent(2, "\"mcuGitHash\": \"%s\"\n", info.mcu_git_hash().cstring());
  }
  print_indent(1, "},\n");

  AppfsInfo appfs_info
    = Appfs().get_info(var::String("/app/flash/") + options.name());
  if (appfs_info.is_valid() == false) {
    appfs_info = Appfs().get_info(var::String("/app/ram/") + options.name());
  }
#endif

  print_indent(1, "\"test\": {\n");
  print_indent(2, "\"name\": \"%s\",\n", options.name().cstring());
  // need to add the amount of RAM the program has to output
  print_indent(2, "\"version\": \"%s\",\n", options.version().cstring());
#if !defined __link
  if (appfs_info.is_valid()) {
    print_indent(2, "\"ramSize\": \"%ld\",\n", appfs_info.ram_size());
    if (appfs_info.is_flash()) {
      print_indent(2, "\"isRam\": false,\n");
    } else {
      print_indent(2, "\"isRam\": true,\n");
    }

    if (appfs_info.is_code_external()) {
      print_indent(2, "\"isCodeExternal\": true,\n");
    } else {
      print_indent(2, "\"isCodeExternal\": false,\n");
    }

    if (appfs_info.is_data_external()) {
      print_indent(2, "\"isDataExternal\": true,\n");
    } else {
      print_indent(2, "\"isDataExternal\": false,\n");
    }

    if (appfs_info.is_code_tightly_coupled()) {
      print_indent(2, "\"isCodeTightlyCoupled\": true,\n");
    } else {
      print_indent(2, "\"isCodeTightlyCoupled\": false,\n");
    }

    if (appfs_info.is_data_tightly_coupled()) {
      print_indent(2, "\"isDataTightlyCoupled\": true,\n");
    } else {
      print_indent(2, "\"isDataTightlyCoupled\": false,\n");
    }

    print_indent(
      2,
      "\"applicationSignature\": \"%X\",\n",
      appfs_info.signature());
  }
#endif
  print_indent(2, "\"gitHash\": \"%s\",\n", options.git_hash().cstring());
  print_indent(2, "\"apiVersion\": \"%s\",\n", api::ApiInfo::version());
  print_indent(2, "\"apiGitHash\": \"%s\"\n", api::ApiInfo::git_hash());
  print_indent(1, "},\n");
}

void Test::print(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprint_indent(indent(), fmt, args);
  va_end(args);
}

void Test::print_indent(int indent, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprint_indent(indent, fmt, args);
  va_end(args);
}

void Test::vprint_indent(int indent, const char *fmt, va_list args) {
  int i;
  for (i = 0; i < indent; i++) {
    printf("  ");
  }
  vprintf(fmt, args);
}

u32 Test::parse_execution_flags(const sys::Cli &cli) {
  u32 o_execute_flags = 0;

  bool is_all = false;

  if (
    cli.get_option(
      "all",
      "execute all tests and types (if no type "
      "(api|stress|performance|additional) is specified")
    == "true") {
    is_all = true;
  }

  if (
    cli.get_option(
      "allTypes",
      "execute all test types (api|stress|performance|additional)")
    == "true") {
    o_execute_flags |= Test::execute_all_types;
  }

  if (cli.get_option("api", "execute api tests") == "true") {
    o_execute_flags |= Test::execute_api;
  }

  if (cli.get_option("performance", "execute performance tests") == "true") {
    o_execute_flags |= Test::execute_performance;
  }

  if (cli.get_option("stress", "execute stress test") == "true") {
    o_execute_flags |= Test::execute_stress;
  }

  if (cli.get_option("additional", "execute additional test") == "true") {
    o_execute_flags |= Test::execute_additional;
  }

  if (is_all) {
    if (o_execute_flags == 0) {
      o_execute_flags = Test::execute_all;
    } else {
      o_execute_flags |= ~(Test::execute_all_types);
    }
  }

  return o_execute_flags;
}

u32 Test::parse_test(
  const sys::Cli &cli,
  const var::String &name,
  u32 test_flag) {

  if (
    cli.get_option(name, var::String("Execute the ") + name + " test suite")
    == "true") {
    return test_flag;
  }

  return 0;
}

void Test::finalize() {
  if (m_is_initialized) {
    print_indent(1, "\"result of all tests\": {\n");
    if (m_all_test_result) {
      print_indent(2, "\"result\": true,\n");
    } else {
      print_indent(2, "\"result\": false,\n");
    }
    print_indent(
      2,
      "\"microseconds\": %ld.0\n",
      m_all_test_duration_microseconds);

    // need to add the amount of RAM the program has to output
    print_indent(1, "}");
    m_is_initialized = false;
    print_indent(0, "\n}\n");
  }
}

void Test::execute_api_case() {
  Case cg(this, "api");
  var::DataInfo entry_info;
  bool result = execute_class_api_case();
  if (entry_info == var::DataInfo()) {
    print_case_message("no memory leaks");
  } else {
    print_case_failed("memory leak detected");
    result = false;
  }
}

void Test::execute_performance_case() {
  Case cg(this, "performance");
  var::DataInfo entry_info;
  bool result = execute_class_performance_case();
  if (entry_info == var::DataInfo()) {
    print_case_message("no memory leaks");
  } else {
    print_case_failed("memory leak detected");
    result = false;
  }
}

void Test::execute_stress_case() {
  Case cg(this, "stress");
  var::DataInfo entry_info;
  bool result = execute_class_stress_case();
  if (entry_info == var::DataInfo()) {
    print_case_message("no memory leaks");
  } else {
    print_case_failed("memory leak detected");
    result = false;
  }
}

bool Test::execute_class_api_case() {
  print_case_message("no api case");
  return true;
}

bool Test::execute_class_performance_case() {
  print_case_message("no performance case");
  return true;
}

bool Test::execute_class_stress_case() {
  print_case_message("no stress case");
  return true;
}
