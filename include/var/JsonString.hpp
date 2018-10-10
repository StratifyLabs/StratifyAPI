/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved


#ifndef VAR_JSONSTRING_HPP_
#define VAR_JSONSTRING_HPP_

#include "String.hpp"
#include "ConstString.hpp"

namespace var {

class JsonString : public String {
public:
	JsonString(bool is_object = true);


	void end();
	void end_object();
	void end_array();

	void append_object(const ConstString & key);
	void append_array(const ConstString & key);


	void append_string(const ConstString & key, const ConstString & value);
	void append_number(const ConstString & key, int number);
	void append_float(const ConstString & key, float number);
	void append_true(const ConstString & key);
	void append_false(const ConstString & key);
	void append_null(const ConstString & key);


	//appending in arrays
	void append_object();
	void append_array();

	void append_string(const ConstString & value);
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

#endif /* VAR_JSONSTRING_HPP_ */
