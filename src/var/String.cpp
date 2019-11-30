//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include <errno.h>
#include <cctype>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include "var/Data.hpp"
#include "var/String.hpp"
#include "sys.hpp"

using namespace var;

String::String(){
	//creates an empty string -- Data class and ConstString class will point to a zero value variable
}


String & String::format(const char * format, ...){
	va_list args;
	va_start(args, format);
	vformat(format, args);
	va_end (args);
	return *this;
}


int String::sprintf(const char * format, ...){
	int result;
	va_list args;
	va_start(args, format);
	result = vformat(format, args);
	va_end (args);
	return result;
}

int String::vformat(
		const char * fmt,
		va_list list
		){
	int result;
	if( capacity() == 0 ){
		resize(var::Data::minimum_capacity());
	}
	result = vsnprintf(to_char(), capacity()-1, fmt, list);
	if( result >= (int)capacity()-1 ){ //if the data did not fit, make the buffer bigger
		resize(result+1);
		vsnprintf(to_char(), capacity()-1, fmt, list);
	}
	return result;
}



String& String::erase(
		const String & string_to_erase,
		Position position,
		Length length
		){
	size_t erase_pos;
	size_t count_value = 0;
	size_t len = string_to_erase.length();
	while( (count_value++ < length.argument()) &&
			 ((erase_pos = find(string_to_erase, position, Length(len))) != npos) ){
		erase(Position(erase_pos), Length(len));
	}
	return *this;
}

String& String::replace(
		ToErase old_string,
		ToInsert new_string,
		Position position,
		Length length
		){
	size_t pos = position.argument();
	size_t old_length = old_string.argument().length();
	size_t new_length = new_string.argument().length();
	size_t replaced_count = 0;
	while( ((pos = find(old_string.argument(), Position(pos))) != String::npos) &&
			 (length.argument() ? replaced_count < length.argument() : 1) ){
		erase(Position(pos), Length(old_length));
		insert(Position(pos), new_string);
		pos += new_length;
		replaced_count++;
	}
	return *this;
}


String & String::to_upper(){
	for(auto & c: m_string){
		c = std::toupper(c);
	}
	return *this;
}

String & String::to_lower(){
	for(auto & c: m_string){
		c = std::tolower(c);
	}
	return *this;
}


#if 0
String & String::operator << (
		const var::Reference & reference
		){

	for(
		 u32 i=0;
		 i < reference.size();
		 i++){

		append(String().format(
					 "%02x", reference.to_const_u8()[i]
					 )
				 );
	}

	return *this;
}
#endif

float String::to_float() const {
#ifndef __link
	return ::atoff(cstring());
#else
	return ::atof(cstring());
#endif
}




