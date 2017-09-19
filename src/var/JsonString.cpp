/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved


#include "var/JsonString.hpp"

namespace var {

JsonString::JsonString(bool is_object) {
	// TODO Auto-generated constructor stub

	m_is_first = true;
	m_is_object = is_object;

	if( is_object ){
		append("\n{");
	} else {
		append("\n[");
	}

}

void JsonString::end(){
	if( m_is_object ){
		append("}\n");
	} else {
		append("]\n");
	}
}

void JsonString::append_separator(){
	if( m_is_first ){
		m_is_first = false;
	} else {
		append(",");
	}
}

void JsonString::append_object(const char * key){
	String str;
	str.sprintf("\"%s\": {", key);
	append_separator();
	append(str);
	m_is_first = true;
}

void JsonString::append_array(const char * key){
	String str;
	str.sprintf("\"%s\": [", key);
	append_separator();
	append(str);
	m_is_first = true;
}

void JsonString::end_object(){
	append("}");
}

void JsonString::end_array(){
	append("]");
}

void JsonString::append_string(const char * key, const char * value){
	String str;
	str.sprintf("\"%s\": \"");
	str.append(value);
	str.append("\"");
	append_separator();
	append(str);
}

void JsonString::append_number(const char * key, int number){
	String str;
	str.sprintf("\"%s\": \"%d\"", key, number);
	append_separator();
	append(str);
}

void JsonString::append_float(const char * key, float number){
	String str;
	str.sprintf("\"%s\": \"%f\"", key, number);
	append_separator();
	append(str);
}


void JsonString::append_true(const char * key){
	String str;
	str.sprintf("\"%s\":true", key);
	append_separator();
	append(str);
}

void JsonString::append_false(const char * key){
	String str;
	str.sprintf("\"%s\":false", key);
	append_separator();
	append(str);
}

void JsonString::append_null(const char * key){
	String str;
	str.sprintf("\"%s\": null", key);
	append_separator();
	append(str);
}

void JsonString::append_object(){
	append_separator();
	append("{");
	m_is_first = true;
}

void JsonString::append_array(){
	append_separator();
	append("[");
	m_is_first = true;
}

void JsonString::append_string(const char * value){
	append_separator();
	append("\"");
	append(value);
	append("\"");
}

void JsonString::append_number(int number){
	String str;
	str.sprintf("\"%d\"", number);
	append_separator();
	append(str);
}

void JsonString::append_float(float number){
	String str;
	str.sprintf("\"%f\"", number);
	append_separator();
	append(str);
}


void JsonString::append_true(){
	append_separator();
	append("true");
}

void JsonString::append_false(){
	append_separator();
	append(", false");

}

void JsonString::append_null(){
	append_separator();
	append(", null");
}


} /* namespace var */
