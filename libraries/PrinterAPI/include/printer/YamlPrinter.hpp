/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#ifndef PRINTER_API_PRINTER_YAMLPRINTER_HPP_
#define PRINTER_API_PRINTER_YAMLPRINTER_HPP_

#include "Printer.hpp"
#include "var/Vector.hpp"

namespace printer {

class YamlPrinter : public Printer {
public:
  YamlPrinter();

  /*! \details Opens a YAML object.
   *
   * @param key The key to be used for the object
   * @param level The filter level for child keys
   *
   * YAML object values are not preceded by '-' while arrays are.
   * Objects should have unique keys in order to have properly
   * formatted YAML. The debug(), info(), warning(), error(),
   * and fatal() methods can be used at most once inside of an object
   * without having a duplicate key. The key() method can be used to assign
   * unique keys to each object.
   *
   * ```
   * //md2code:main
   * Printer p;
   * //whole object only prints with debug verbose level
   * p.open_object("keys", Printer::level_debug);
   * p.key("key0", "value0");
   * p.key("key1", "value1");
   * p.key("key2", "value2");
   * p.close_object();
   * ```
   *
   */

  /*! \details Opens a YAML array.
   *
   * @param key The key to be used for the object
   * @param level The filter level for child keys
   *
   * YAML object values are not preceded by '-' while arrays are.
   * Objects should have unique keys in order to have properly
   * formatted YAML. The debug(), info(), warning(), error(),
   * and fatal() methods can be used multple times inside of
   * an array and are indexed when parsed.
   *
   *
   *
   * ```
   * //md2code:main
   * Printer p;
   * //whole object only prints with debug verbose level
   * p.open_array("keys", Printer::level_message);
   * p.info("Hello");
   * p.info("Hello");
   * p.info("Hello");
   * p.warning("Hello");
   * p.message("Hello");
   * p.message("Hello");
   * p.warning("Hello");
   * p.close_array(); //close_array() and close_object() are interchangeable
   * ```
   *
   *
   * By default the Printer starts with an open array.  The following
   * code can change the top level type to an object.
   *
   * ```
   * //md2code:main
   * Printer p; //printer is now an array
   * p.close_array();
   * p.open_object(""); //top level type is an object
   * ```
   *
   */

  /*! \details Returns a printer termination object for closing
   * an array or object with the `<<` operator.
   *
   * ```
   * //md2code:main
   * Printer p;
   * p.open_object("empty object") << p.close();
   * p.open_array("messages", Printer::level_message) << "hello" << p.close();
   * ```
   *
   */
  static PrinterTermination close() { return PrinterTermination(); }

  using Printer::operator<<;

  YamlPrinter &operator<<(const PrinterTermination &printer_termination) {
    close_object();
    return *this;
  }

  YamlPrinter &set_top_verbose_level(Level level) {
    container_list().front().set_verbose_level(level);
    return *this;
  }

private:
  enum class ContainerType { array, object };

  using Container = PrinterContainer<ContainerType>;
  var::Vector<Container> m_container_list;
  var::Vector<Container> &container_list() { return m_container_list; }
  const var::Vector<Container> &container_list() const {
    return m_container_list;
  }

  // re-implemented virtual functions from Printer
  void print_open_object(Level level, var::StringView key);
  void print_close_object();
  void print_open_array(Level level, var::StringView key);
  void print_close_array() { return print_close_object(); }
  void print(
    Level level,
    var::StringView key,
    var::StringView value,
    Newline is_newline = Newline::yes);

  Container &container() { return m_container_list.back(); }

  const Container &container() const { return m_container_list.back(); }
};

} // namespace printer

#endif // PRINTER_API_PRINTER_YAMLPRINTER_HPP_
