/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef SAPI_SYS_CLI_HPP_
#define SAPI_SYS_CLI_HPP_

#include "var/String.hpp"
#include "var/StringView.hpp"
#include "var/Tokenizer.hpp"
#include "var/Vector.hpp"

namespace sys {

/*! \brief Command Line Interface class
 * \details This class contains methods to help analyze input from the
 * command line.
 */
class Cli : public api::ExecutionContext {
public:


  /*! \details Constructs a new object to parse and handle command line
   * interface arguments.
   *
   * @param argc The number of arguments
   * @param argv A pointer to the arguments
   *
   * The Cli is construted as shown in the following example.
   * \code
   * int main(int argc, char * argv[]){
   * 	Cli cli(argc, argv);
   * }
   * \endcode
   *
   */
  Cli(int argc, char *argv[]);

  class HandleVersion {
    API_AC(HandleVersion, var::StringView, version);
    API_AC(HandleVersion, var::StringView, publisher);
    API_AC(HandleVersion, var::StringView, githash);
  };

  /*! \details Handles the --version and -v options to show the version.
   *
   * \sa print_version()
   *
   */
  void handle_version(const HandleVersion &options) const;

  /*! \details Sets whether the arguments are case sensitive. */
  Cli &set_case_sensitive(bool value = true) {
    m_is_case_sensitive = value;
    return *this;
  }

  /*!
   * \details Returns true if parsing is case sensitive.
   *
   * By default, parsing is case senstive unless set_case_sensitive(false)
   * is called.
   *
   */
  bool is_case_senstive() const { return m_is_case_sensitive; }

  /*! \details Accesses the path to the program (including the name). */
  const char *app_git_hash() const { return m_app_git_hash; }

  var::String to_string() const;

  var::StringView at(u16 value) const;

  /*! \details Checks to see if the option exists and returns its value
   *  as a String.
   *
   * @param name The option name
   *
   * This method will work with the following notations
   *
   * ```
   * -<name>
   * -<name> <value>
   * -<name>=<value>
   * --<name>
   * --<name> <value>
   * --<name>=<value>
   * ```
   *
   * If `-<name>` or `--<name>` is given, the return value is set to "true".
   *
   */
  var::StringView get_option(
    var::StringView name,
    var::StringView help = var::StringView()) const;

  /*! \details Returns the number of arguments. */
  u32 count() const { return m_argc; }
  u32 size() const { return m_argc; }

  var::StringView get_name() const;
  var::StringView get_path() const;

  class ShowHelp {
    API_AF(ShowHelp, var::StringView, publisher, "");
    API_AF(ShowHelp, var::StringView, version, "");
  };

  Cli &show_help(const ShowHelp &options);
  Cli &show_version(const ShowHelp &options);

private:
  u16 m_argc;
  char **m_argv;
  bool m_is_case_sensitive;
  const char *m_app_git_hash;
  mutable var::StringList m_help_list;
};


} /* namespace sys */

namespace printer {
class Printer;
Printer &operator<<(Printer &printer, const sys::Cli &a);
} // namespace printer

#endif /* SAPI_SYS_CLI_HPP_ */
