/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
//Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc


#include <errno.h>
#include <cctype>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include "var/Data.hpp"
#include "var/String.hpp"
#include "var/Tokenizer.hpp"
#include "sys.hpp"

using namespace var;

String String::m_empty_string;

String var::operator+ (const char*   lhs, const String& rhs){
	return String(lhs) + rhs;
}

String var::operator+ (const char*   lhs, String&& rhs){
	return String(lhs) + rhs;
}

String::String(){}


String::String(const Reference & reference){
	m_string.assign(
				reference.to_const_char(),
				reference.size()
				);
}

String & String::format(const char * format, ...){
	va_list args;
	va_start(args, format);
	vformat(format, args);
	va_end (args);
	return *this;
}

String& String::vformat(
		const char * fmt,
		va_list list
		){

	if( fmt == nullptr ){ return *this; }

	va_list list_copy;
	va_copy(list_copy, list);
	const int buffer_size = 64;
	char buffer[buffer_size];
	int size = vsnprintf(
				buffer,
				buffer_size-1,
				fmt,
				list
				);
	if( size <= buffer_size-1 ){
		*this = String(buffer);
	} else {
		Data tmp(size+1);
		size = vsnprintf(
					tmp.to_char(),
					tmp.size(),
					fmt,
					list_copy
					);

		if( size > 0 ){
			*this = String(tmp.to_const_char());
		} else {
			clear();
		}
	}
	va_end(list_copy);

	return *this;
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
		const String & old_string,
		ToInsert new_string,
		Position position,
		Length length
		){
	size_t pos = position.argument();
	size_t old_length = old_string.length();
	size_t new_length = new_string.argument().length();
	size_t replaced_count = 0;
	while( ((pos = find(old_string, Position(pos))) != String::npos) &&
				 (length.argument() ? replaced_count < length.argument() : 1) ){
		erase(Position(pos), Length(old_length));
		insert(Position(pos), new_string.argument());
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


Vector<String> String::split(const String & delimiter) const {
	Tokenizer tokens(
				*this,
				Tokenizer::Delimeters(delimiter)
				);
	return tokens.list();
}



