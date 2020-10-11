/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#include "printer/JsonPrinter.hpp"

using namespace printer;

JsonPrinter::JsonPrinter() {
  container_list().push_back(Container(Level::fatal, ContainerType::array));
  enable_flags(
    PrintFlags::no_progress_newline | PrintFlags::key_quotes
    | PrintFlags::value_quotes);
}

void JsonPrinter::print(
  Level level,
  var::StringView key,
  var::StringView value,
  Newline is_newline) {

  if (level > verbose_level()) {
    return;
  }

  for (auto const &c : container_list()) {
    if (c.verbose_level() > verbose_level()) {
      return;
    }
  }

  insert_comma();

  if (container().type() == ContainerType::array) {
    key = var::StringView().set_null();
  }

  Printer::print(level, key, value, Newline::no);
}

void JsonPrinter::print_open_object(Level level, var::StringView key) {

  if (verbose_level() >= level) {
    insert_comma();
    if (container().type() == ContainerType::object) {
      const var::String string_key = "\"" + key + "\":{";
      interface_print_final(string_key.cstring());
    } else {
      const var::String string_key = "{" + key;
      interface_print_final(string_key.cstring());
    }
  }

  container_list().push_back(Container(level, ContainerType::object));
}

void JsonPrinter::print_open_array(Level level, var::StringView key) {
  if (verbose_level() >= level) {
    insert_comma();
    if (container().type() == ContainerType::object) {
      const var::String string_key = "\"" + key + "\":[";
      interface_print_final(string_key.cstring());
    } else {
      const var::String string_key = "[" + key;
      interface_print_final(string_key.cstring());
    }
  }
  container_list().push_back(Container(level, ContainerType::array));
}

void JsonPrinter::print_close_object() {
  if (container_list().count() > 1) {
    if (verbose_level() >= container().verbose_level()) {
      if (container().type() == ContainerType::array) {
        interface_print_final("]");
      } else if (container().type() == ContainerType::object) {
        interface_print_final("}");
      }
    }
    container_list().pop_back();
  }
}

void JsonPrinter::insert_comma() {
  if (container().count() > 1) {
    interface_print_final(",");
  }
  container().count() = container().count() + 1;
}
