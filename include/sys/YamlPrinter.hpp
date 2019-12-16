#ifndef SAPI_SYS_YAMLPRINTER_HPP
#define SAPI_SYS_YAMLPRINTER_HPP

#include "Printer.hpp"

namespace sys {

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
	 * p.open_object("keys", Printer::DEBUG);
	 * p.key("key0", "value0");
	 * p.key("key1", "value1");
	 * p.key("key2", "value2");
	 * p.close_object();
	 * ```
	 *
	 */
	YamlPrinter & open_object(
			const var::String & key,
			enum verbose_level level = level_fatal);

	YamlPrinter & close_object();


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
	 * p.open_array("keys", Printer::MESSAGE);
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
	YamlPrinter & open_array(const var::String & key, enum verbose_level level = level_fatal);
	void close_array(){
		close_object();
	}

	/*! \details Returns a printer termination object for closing
	 * an array or object with the `<<` operator.
	 *
	 * ```
	 * //md2code:main
	 * Printer p;
	 * p.open_object("empty object") << p.close();
	 * p.open_array("messages", Printer::MESSAGE) << "hello" << p.close();
	 * ```
	 *
	 */
	static PrinterTermination close(){
		return PrinterTermination();
	}

	using Printer::operator <<;

	YamlPrinter & operator << (const PrinterTermination & printer_termination){
		close_object();
		return *this;
	}

	YamlPrinter& set_top_verbose_level(enum verbose_level level){
		container_list().front().set_verbose_level(level);
		return *this;
	}

private:

	enum container_type {
		container_array,
		container_object
	};

	using Container = PrinterContainer<enum container_type>;
	var::Vector<Container> m_container_list;
	var::Vector<Container> & container_list(){ return m_container_list; }
	const var::Vector<Container> & container_list() const { return m_container_list; }

	//re-implemented virtual functions from Printer
	void print_open_object(enum verbose_level level, const char * key);
	void print_close_object();
	void print(enum verbose_level level, const char * key, const char * value);


	Container & container(){
		return m_container_list.back();
	}

	const Container & container() const {
		return m_container_list.back();
	}


};

}

#endif // SAPI_SYS_YAMLPRINTER_HPP
