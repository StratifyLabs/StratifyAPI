/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved


#ifndef SAPI_SYS_JSON_PRINTER_HPP_
#define SAPI_SYS_JSON_PRINTER_HPP_

#include "../var/String.hpp"

namespace sys {

class JsonPrinter : public var::String {
public:
	JsonPrinter(bool is_object = true);


	void end();
	void end_object();
	void end_array();

	void append_object(const var::ConstString & key);
	void append_array(const var::ConstString & key);


	void append_string(const var::ConstString & key, const var::ConstString & value);
	void append_number(const var::ConstString & key, int number);
	void append_float(const var::ConstString & key, float number);
	void append_true(const var::ConstString & key);
	void append_false(const var::ConstString & key);
	void append_null(const var::ConstString & key);


	//appending in arrays
	void append_object();
	void append_array();

	void append_string(const var::ConstString & value);
	void append_number(int number);
	void append_float(float number);
	void append_true();
	void append_false();
	void append_null();

private:

	void append_separator();
	bool m_is_first;
	bool m_is_object;

};

} /* namespace var */

#endif /* SAPI_SYS_JSON_PRINTER_HPP_ */
