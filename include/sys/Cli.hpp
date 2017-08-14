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

	enum {
		FORMAT_TEXT /*! Displays messages as plain text */,
		FORMAT_JSON /*! Display messages as JSON formatted strings */,
		FORMAT_TOTAL
	};

	/*! \details Constructs a new object to parse and handle command line interface arguments.
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
	Cli(int argc, char * argv[]);

	/*! \details Handles the --version and -v options to show the version.
	 *
	 * \sa print_version()
	 *
	 */
	void handle_version() const;

	/*! \details Sets the publisher of the program.
	 *
	 * @param publisher A pointer to the publisher's name
	 *
	 */
	void set_publisher(const char * publisher){ m_publisher = publisher; }


	enum {
		PRINT_DEBUG,
		PRINT_INFO,
		PRINT_WARNING,
		PRINT_ERROR,
		PRINT_FATAL,
		PRINT_TOTAL
	};

	/*! \details Prints a message to the user.
	 *
	 * @param value A pointer to the message
	 *
	 * The message will be formatted based on the Cli format's settings.
	 *
	 */
	//void print(char char * type, const char * value);

	/*! \details Prints the progress of an operation.
	 *
	 * @param value The current value of the progress
	 * @param max The maximum value of the progress
	 */
	//void print_progress(int value, int max);

	/*! \details Accesses the program version. */
	const char * version() const { return m_version; }

	/*! \details Accesses the program publisher. */
	const char * publisher() const { return m_publisher; }

	/*! \details Accesses the program name. */
	const char * name() const { return m_name; }

	/*! \details Returns the argument offset by value as a var::String. */
	var::String at(u16 value) const;

	/*! \details Returns the argument offset by value as a mcu_pin_t value. */
	mcu_pin_t pin_at(u16 value) const;

	/*! \details Return the argument offset by value as an int value */
	int value_at(u16 value) const;

	/*! \details Checks to see if the option exists as a command line argument.
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

	/*! \details Gets the argument of an option as a var::String.
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

	/*! \details Gets the argument of an option as a var::String.
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

	/*! \details Gets the argument of an option as a var::String.
	 *
	 * @param option The option to match
	 * @return The value of the argument or 0 if the option wasn't found
	 *
	 * For example, take the given command line
	 *
	 * > program -i 2.1
	 *
	 * `get_option_pio("-i")` will return a mcu_pin_t structure with port = 2 and pin = 1.
	 *
	 *
	 */
	mcu_pin_t get_option_pin(const char * option) const;

	/*! \details Returns the number of arguments. */
	int count() const { return m_argc; }
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
