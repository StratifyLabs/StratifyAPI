/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef SAPI_SYS_JSONPRINTER_HPP
#define SAPI_SYS_JSONPRINTER_HPP


#include "Printer.hpp"

namespace sys {

class JsonPrinter : public Printer {
public:
	JsonPrinter();



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
	void print_open_object(enum levels level, const char * key);
	void print_close_object();
	void print_open_array(
			enum levels level,
			const char * key
			);
	void print_close_array(){
		return print_close_object();
	}
	void print(enum levels level, const char * key, const char * value, bool is_newline = true);


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
