/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#include "sys/Cli.hpp"
#include "printer/Printer.hpp"

printer::Printer &
printer::operator<<(printer::Printer &printer, const sys::Cli &a) {
#if 0
  printer.print_open_object(printer.verbose_level(), a.name().cstring());
  {
    printer.key("publisher", a.publisher());
    printer.print_open_object(printer.verbose_level(), "arguments");
    {
      for (u32 i = 0; i < a.count(); i++) {
        printer.key(0, "%s", a.at(i).cstring());
      }
      printer.print_close_object();
    }
    printer.print_close_object();
  }
#endif
  return printer;
}

using namespace var;
using namespace sys;

Cli::Cli(int argc, char *argv[]) {
  if (argc < 0) {
    argc = 0;
  }
  m_argc = argc;
  m_argv = argv;
}

void Cli::handle_version(const HandleVersion &options) const {
#if !defined __link
  if (get_option("--version").is_empty() == false) {
    printf(
      "%s version: %s by %s \n",
      get_name().cstring(),
      options.version().cstring(),
      options.publisher().cstring());
    exit(0);
  }
#endif
}

var::String Cli::to_string() const {
  String result;
  for (u32 i = 1; i < count(); i++) {
    result += at(i);
    if (i < count() - 1) {
      result += " ";
    }
  }
  return result;
}

String Cli::at(u16 value) const {
  String arg;
  if (value < m_argc) {
    arg.assign(m_argv[value]);
  }
  return arg;
}

bool Cli::is_option_equivalent_to_argument(
  StringView option,
  StringView argument) const {
  String option_string = String(option);
  String argument_string = String(argument);
  if (is_case_senstive() == false) {
    option_string.to_upper();
    argument_string.to_upper();
  }

  return compare_with_prefix(option_string, argument_string);
}

bool Cli::compare_with_prefix(var::StringView option, var::StringView argument)
  const {
  if (argument.at(0) != '-') {
    return false;
  }
  if (option == argument) {
    return true;
  }
  const String with_prefix = "--" + option;
  if (with_prefix == argument) {
    return true;
  }
  if (
    with_prefix.get_substring(String::GetSubstring().set_position(1))
    == argument) {
    return true;
  }
  return false;
}

bool Cli::is_option_equivalent_to_argument_with_equality(
  StringView option,
  StringView argument,
  StringView value) const {

  if (argument.at(0) != '-') {
    return false;
  }

  Tokenizer tokens = Tokenizer(
    argument,
    Tokenizer::Construct()
      .set_delimeters("=")
      .set_ignore_between("")
      .set_maximum_delimeter_count(1));

  if (tokens.count() == 2) {
    String a = String(option);
    String b = tokens.at(0);
    if (is_case_senstive() == false) {
      a.to_upper();
      b.to_upper();
    }

    if (compare_with_prefix(a, b)) {
      value = tokens.at(1);
      return true;
    }
  }
  return false;
}

var::String Cli::get_option(StringView name, StringView help) const {
  var::String result;
  u32 args;

  if (help.is_empty() == false) {
    m_help_list.push_back(String(name) + ": " + help);
  }

  for (args = 1; args < count(); args++) {
    if (is_option_equivalent_to_argument(name, at(args))) {
      if (count() > args + 1) {
        String value = at(args + 1);
        if (value.at(0) == '-') {
          result = "true";
        } else {
          result = at(args + 1);
        }
        return result;
      } else {
        return String("true");
      }
    } else if (is_option_equivalent_to_argument_with_equality(
                 name,
                 at(args),
                 result)) {
      return result;
    }
  }
  return String();
}

var::StringView Cli::get_name() const {
  if (m_argc > 0) {
    StringView result = m_argv[0];
    size_t slash_position = result.reverse_find('/');
    if (slash_position != StringView::npos) {
      result.pop_front(slash_position + 1);
    }

    return result;
  }
  return var::StringView();
}

var::StringView Cli::get_path() const {
  if (m_argc > 0) {
    return m_argv[0];
  }
  return var::StringView();
}

Cli &Cli::show_help(const ShowHelp &options) {
  printf("%s options:\n", get_name().cstring());
  for (u32 i = 0; i < m_help_list.count(); i++) {
    printf("- %s\n", m_help_list.at(i).cstring());
  }
  return *this;
}
