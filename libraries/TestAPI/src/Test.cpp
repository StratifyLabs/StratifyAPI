/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#include <cstdio>

#include "printer/Printer.hpp"
#include "sys.hpp"
#include "test/Case.hpp"
#include "test/Test.hpp"
#include "var/String.hpp"

using namespace var;
using namespace test;
using namespace printer;

printer::Printer *Test::m_printer = nullptr;

bool Test::m_final_result = true;
u32 Test::m_final_duration_microseconds = 0;

Test::Test(var::StringView name) {
  printer().open_object(name);
  m_name = String(name);
  m_test_result = true;
  m_test_duration_microseconds = 0;
}

Test::~Test() {
  // close the JSON object
  PrinterObject pg(printer(), "testResult");
  printer().key("result", m_test_result);
  printer().key(
    "score",
    String::number(get_score(m_test_duration_microseconds)));
  printer().key("microseconds", m_test_duration_microseconds);
  printer().key("memoryLeak", m_test_data_info == var::DataInfo());
  printer().close_object();
  m_final_duration_microseconds += m_test_duration_microseconds;
}

u32 Test::get_score(u32 microseconds) {
  constexpr u32 baseline_microseconds = 1000000000UL;
  return baseline_microseconds / microseconds;
}

void Test::execute(const sys::Cli &cli) {
  ExecuteFlags o_flags = ExecuteFlags::none;

  if (cli.get_option("api") == "true") {
    o_flags |= ExecuteFlags::api;
  }
  if (cli.get_option("stress") == "true") {
    o_flags |= ExecuteFlags::stress;
  }
  if (cli.get_option("performance") == "true") {
    o_flags |= ExecuteFlags::performance;
  }

  if (cli.get_option("all") == "true") {
    o_flags |= ExecuteFlags::all;
  }

  if (o_flags == ExecuteFlags::none) {
    o_flags = ExecuteFlags::api;
  }

  execute(o_flags);
}

void Test::open_case(var::StringView case_name) {
  printer().open_object(case_name);
  m_case_result = true;
  m_case_data_info = DataInfo();
  m_case_timer.restart();
}

void Test::close_case() {
  m_case_timer.stop();
  m_test_duration_microseconds += m_case_timer.microseconds();
  printer().key("result", m_case_result);
  printer().key(
    "score",
    String::number(get_score(m_case_timer.microseconds())));
  printer().key("microseconds", String::number(m_case_timer.microseconds()));
  printer().key("memoryLeak", m_case_data_info == var::DataInfo());
  m_case_result = true;
}

void Test::initialize(const Options &options) {
  API_ASSERT(options.printer() != nullptr);
  m_printer = options.printer();
  m_final_duration_microseconds = 0;

#if !defined __link
  Sys::Info info = Sys().get_info();

  API_ASSERT(info.is_valid());

  printer().object("system", info);

  AppfsInfo appfs_info
    = Appfs().get_info(var::String("/app/flash/") + options.name());
  if (appfs_info.is_valid() == false) {
    appfs_info = Appfs().get_info(var::String("/app/ram/") + options.name());
  }
#endif

  {
    PrinterObject pg(printer(), "test");
    printer().key("name", options.name());
    printer().key("version", options.version());
    printer().key("version", options.version());

#if !defined __link
    if (appfs_info.is_valid()) {
      printer().key("ramSize", String::number(appfs_info.ram_size()));

      printer().key("isFlash", appfs_info.is_flash());

      printer().key("isCodeExternal", appfs_info.is_code_external());

      printer().key("isDataExternal", appfs_info.is_data_external());

      printer().key(
        "isCodeTightlyCoupled",
        appfs_info.is_code_tightly_coupled());

      printer().key(
        "isDataTightlyCoupled",
        appfs_info.is_data_tightly_coupled());

      printer().key(
        "applicationSignature",
        String::number(appfs_info.signature(), "%x"));
    }
#endif
    printer().key("gitHash", options.git_hash());
    printer().key("apiVersion", api::ApiInfo::version());
    printer().key("apiGitHash", api::ApiInfo::git_hash());
  }

  m_final_data_info = var::DataInfo();
}

Test::ExecuteFlags Test::parse_execution_flags(const sys::Cli &cli) {
  ExecuteFlags o_execute_flags = ExecuteFlags::none;

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
    o_execute_flags |= ExecuteFlags::all_types;
  }

  if (cli.get_option("api", "execute api tests") == "true") {
    o_execute_flags |= ExecuteFlags::api;
  }

  if (cli.get_option("performance", "execute performance tests") == "true") {
    o_execute_flags |= ExecuteFlags::performance;
  }

  if (cli.get_option("stress", "execute stress test") == "true") {
    o_execute_flags |= ExecuteFlags::stress;
  }

  if (is_all) {
    if (o_execute_flags == ExecuteFlags::none) {
      o_execute_flags = ExecuteFlags::all;
    } else {
      o_execute_flags |= ~(ExecuteFlags::all_types);
    }
  }

  return o_execute_flags;
}

u32 Test::parse_test(const sys::Cli &cli, var::StringView name, u32 test_flag) {

  if (
    cli.get_option(name, var::String("Execute the ") + name + " test suite")
    == "true") {
    return test_flag;
  }

  return 0;
}

void Test::finalize() {
  PrinterObject pg(printer(), "finalResult");
  printer().key("result", m_final_result);
  printer().key("microseconds", String::number(m_final_duration_microseconds));
  printer().key("memoryLeak", m_final_data_info == var::DataInfo());
  printer().key(
    "score",
    String::number(get_score(m_final_duration_microseconds)));
}

void Test::execute_api_case() {
  Case cg(this, "api");
  execute_class_api_case();
}

void Test::execute_performance_case() {
  Case cg(this, "performance");
  execute_class_performance_case();
}

void Test::execute_stress_case() {
  Case cg(this, "stress");
  execute_class_stress_case();
}

bool Test::execute_class_api_case() {
  printer().key("api", "no test");
  return true;
}

bool Test::execute_class_performance_case() {
  printer().key("performance", "no test");
  return true;
}

bool Test::execute_class_stress_case() {
  printer().key("stress", "no test");
  return true;
}