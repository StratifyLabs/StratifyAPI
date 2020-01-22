#ifndef SAPI_SYS_JSONPRINTER_HPP
#define SAPI_SYS_JSONPRINTER_HPP


#include "Printer.hpp"

namespace sys {

class JsonPrinter : public Printer {
public:
	JsonPrinter();

	JsonPrinter & open_object(
			const var::String & key,
			enum verbose_level level = level_fatal);

	JsonPrinter & close_object();

	JsonPrinter & open_array(const var::String & key, enum verbose_level level = level_fatal);
	JsonPrinter & close_array(){
		return close_object();
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
	void print(enum verbose_level level, const char * key, const char * value, bool is_newline = true);


	Container & container(){
		return m_container_list.back();
	}

	const Container & container() const {
		return m_container_list.back();
	}

	void insert_comma();

};

}

#endif // SAPI_SYS_JSONPRINTER_HPP
