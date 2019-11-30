/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved


#include "sys/JsonPrinter.hpp"

using namespace var;

namespace sys {

JsonPrinter::JsonPrinter(bool is_object) {
	// TODO Auto-generated constructor stub

	m_is_first = true;
	m_is_object = is_object;

	if( is_object ){
		append("\n{");
	} else {
		append("\n[");
	}

}

void JsonPrinter::end(){
	if( m_is_object ){
		append("}\n");
	} else {
		append("]\n");
	}
}

void JsonPrinter::append_separator(){
	if( m_is_first ){
		m_is_first = false;
	} else {
		append(",");
	}
}

void JsonPrinter::append_object(const String & key){
	String str;
	str.format("\"%s\": {", key.cstring());
	append_separator();
	append(str.cstring());
	m_is_first = true;
}

void JsonPrinter::append_array(const String & key){
	String str;
	str.format("\"%s\": [", key.cstring());
	append_separator();
	append(str.cstring());
	m_is_first = true;
}

void JsonPrinter::end_object(){
	append("}");
}

void JsonPrinter::end_array(){
	append("]");
}

void JsonPrinter::append_string(const String & key, const String & value){
	String str;
	str.sprintf("\"%s\": \"%s\"", key.cstring(), value.cstring());
	append_separator();
	append(str.cstring());
}

void JsonPrinter::append_number(const String & key, int number){
	String str;
	str.sprintf("\"%s\": \"%d\"", key.cstring(), number);
	append_separator();
	append(str.cstring());
}

void JsonPrinter::append_float(const String & key, float number){
	String str;
	str.sprintf("\"%s\": \"%f\"", key.cstring(), number);
	append_separator();
	append(str.cstring());
}


void JsonPrinter::append_true(const String & key){
	String str;
	str.sprintf("\"%s\":true", key.cstring());
	append_separator();
	append(str.cstring());
}

void JsonPrinter::append_false(const String & key){
	String str;
	str.sprintf("\"%s\":false", key.cstring());
	append_separator();
	append(str.cstring());
}

void JsonPrinter::append_null(const String & key){
	String str;
	str.sprintf("\"%s\": null", key.cstring());
	append_separator();
	append(str.cstring());
}

void JsonPrinter::append_object(){
	append_separator();
	append("{");
	m_is_first = true;
}

void JsonPrinter::append_array(){
	append_separator();
	append("[");
	m_is_first = true;
}

void JsonPrinter::append_string(const String & value){
	append_separator();
	append("\"");
	append(value);
	append("\"");
}

void JsonPrinter::append_number(int number){
	String str;
	str.sprintf("\"%d\"", number);
	append_separator();
	append(str.cstring());
}

void JsonPrinter::append_float(float number){
	String str;
	str.sprintf("\"%f\"", number);
	append_separator();
	append(str.cstring());
}


void JsonPrinter::append_true(){
	append_separator();
	append("true");
}

void JsonPrinter::append_false(){
	append_separator();
	append(", false");

}

void JsonPrinter::append_null(){
	append_separator();
	append(", null");
}


} /* namespace var */
