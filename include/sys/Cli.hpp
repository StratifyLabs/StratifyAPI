/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SYS_CLI_HPP_
#define SYS_CLI_HPP_

#include "../var/String.hpp"
#include "../var/Token.hpp"

namespace sys {


/*! \brief Command Line Interface class
 * \details This class contains methods to help analyze input from the
 * command line.
 */
class Cli {
public:

	/*! \details Construct a new object to parse and handle command line interface arguments */
	Cli(int argc, char * argv[]);

	/*! \details Set the version of the program */
	void set_version(const char * version){ m_version = version; }

	/*! \details Set the publisher of the program */
	void set_publisher(const char * publisher){ m_publisher = publisher; }

	/*! \details Print the version, name and publisher on the stdout */
	void print_version() const;

	/*! \details Access the program version */
	const char * version() const { return m_version; }

	/*! \details Access the program publisher */
	const char * publisher() const { return m_publisher; }

	/*! \details Access the program name */
	const char * name() const { return m_name; }

	/*! \details Return the argument offset by value as a var::String */
	var::String at(u16 value) const;

	/*! \details Return the argument offset by value as a pio_t value */
	pio_t pio_at(u16 value) const;

	/*! \details Return the argument offset by value as an int value */
	int value_at(u16 value) const;

	/*! \details This method returns true if \a value is one of the options
	 *
	 * @param value A pointer to the option string to search for
	 * @return True if value is any of the options available
	 *
	 * For example,
	 *
	 * > program -v -i text.txt
	 *
	 * `is_option("-v")` will return true.
	 *
	 */
	bool is_option(const char * value) const;

	/*! \details Get the argument of an option as a var::String
	 *
	 * @param option The option to match
	 * @return A String containing the option argument
	 *
	 * For example, take the given command line
	 *
	 * > program -i filename.txt
	 *
	 * `get_option_argument("-i")` will return a String containing "filename.txt"
	 *
	 *
	 */
	var::String get_option_argument(const char * option) const;

	/*! \details Get the argument of an option as a var::String
	 *
	 * @param option The option to match
	 * @return The value of the argument or 0 if the option wasn't found
	 *
	 * For example, take the given command line
	 *
	 * > program -i 10
	 *
	 * `get_option_value("-i")` will return 10.
	 *
	 *
	 */
	int get_option_value(const char * option) const;

	/*! \details Get the argument of an option as a var::String
	 *
	 * @param option The option to match
	 * @return The value of the argument or 0 if the option wasn't found
	 *
	 * For example, take the given command line
	 *
	 * > program -i 2.1
	 *
	 * `get_option_pio("-i")` will return a pio_t structure with port = 2 and pin = 1.
	 *
	 *
	 */
	pio_t get_option_pio(const char * option) const;


	int size() const { return m_argc; }

private:
	u16 m_argc;
	char ** m_argv;
	var::String m_version;
	var::String m_publisher;
	var::String m_name;


};

} /* namespace sys */

#endif /* SYS_CLI_HPP_ */
