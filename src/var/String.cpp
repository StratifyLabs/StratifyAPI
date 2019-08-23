//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include <errno.h>
#include <cctype>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include "var/String.hpp"
#include "sys.hpp"

using namespace var;

String::String(){
	//creates an empty string -- Data class and ConstString class will point to a zero value variable
}

String::String(const ConstString & s){
	assign(s);
}


String::String(const ConstString & s, const arg::Length length){
	assign(s, length);
}

u32 String::capacity() const {
	if( Data::capacity() ){
		return Data::capacity() - 1;
	}
	return 0;
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
		resize(minimum_size());
	}
	result = vsnprintf(to_char(), capacity()-1, fmt, list);
	if( result >= (int)capacity()-1 ){ //if the data did not fit, make the buffer bigger
		if( resize(result+1) >= 0 ){
			vsnprintf(to_char(), capacity()-1, fmt, list);
		}
	}
	return result;
}

int String::resize(
		const arg::ImplicitSize size
		){
	bool is_zero_size = (this->size() == 0);
	int result = Data::resize(size.argument()+1);
	set_string_pointer( to_const_char() );

	//need to zero out the buffer if it wasn't a valid string
	if( is_zero_size ){
		fill((u8)0);
	}
	return result;
}

int String::assign(const ConstString & a){
	return assign(a, arg::Length(a.length()));
}

int String::assign(const ConstString & a,
						 const arg::Length length
						 ){
	//check for null
	u32 length_value = length.argument();
	if( a.cstring() != this->to_const_char() ){ //check for assignment to self - no action needed
		if( length_value == (u32)npos ){ length_value = a.length(); }

		if( resize(length_value) < 0 ){
			return -1;
		}

		clear();
		strncpy(to_char(), a.cstring(), length_value);
	}
	return 0;
}

int String::append(const ConstString & a){
	if( a == 0 ){ return 0; }
	u32 len = length();
	u32 alen = a.length();
	if( resize(len + alen) < 0 ){
		return -1;
	}
	if( len == 0 ){
		clear(); //previous length was zero -- ensure string is valid
	}
	strncat(to_char(), a.cstring(), capacity() - len);
	return 0;
}

int String::append(char c){
	u32 len = length();
	u32 alen = 1;
	if( resize(len + alen + 1) < 0 ){
		return -1;
	}
	strncat(to_char(), &c, 1);
	return 0;
}


String& String::insert(
		const arg::Position position,
		const arg::StringToInsert string_to_insert
		){

	if( to_const_char() == 0 ){
		assign(string_to_insert.argument());
		return *this;
	}

	u32 s;
	u32 len = length();

	if( position.argument() > len ){
		set_error_number(EINVAL);
		return *this;
	} else if( position.argument() == len ){
		append(string_to_insert.argument());
	} else {

		//this needs a limit
		s = string_to_insert.argument().length();

		char buffer[len+1];

		if( resize( len + s ) < 0 ){
			exit_fatal("failed to alloc for insert");
			return *this;
		}

		strncpy(buffer, to_const_char()+position.argument(), len+1); //copy the existing string to buffer
		strncpy(to_char()+position.argument(), string_to_insert.argument().cstring(), capacity() - position.argument());
		strncat(to_char(), buffer, capacity());
	}

	return *this;
}

String& String::erase(const arg::Position position,
		const arg::Length length
		){
	char * p = to<char>();
	u32 s = this->length();
	if( p == 0 ){ return *this; }
	if( (length.argument() != npos) && (position.argument() != npos) && (position.argument() + length.argument() < s) ){
		int remaining;
		remaining = s - position.argument() - length.argument();
		if( remaining > 0 ){
			::memcpy(p + position.argument(), p + position.argument() + length.argument(), remaining);
			p[position.argument()+remaining] = 0;
		} else {
			p[position.argument()] = 0;
		}
	} else if (position.argument() < s ){
		p[position.argument()] = 0;
	}
	return *this;
}

String& String::erase(
		const arg::StringToErase string_to_erase,
		const arg::Position position,
		const arg::Length length
		){
	u32 erase_pos;
	u32 count_value = 0;
	u32 len = string_to_erase.argument().length();
	while( (count_value++ < length.argument()) &&
			 ((erase_pos = find(arg::StringToFind(string_to_erase.argument()), position, arg::Length(len))) != npos) ){
		erase(arg::Position(erase_pos), arg::Length(len));
	}
	return *this;
}

String& String::replace(
		const arg::StringToErase old_string,
		const arg::StringToInsert new_string,
		const arg::Position position,
		const arg::Length length
		){
	u32 pos = position.argument();
	u32 old_length = old_string.argument().length();
	u32 new_length = new_string.argument().length();
	u32 replaced_count = 0;
	while( ((pos = find(arg::StringToFind(old_string.argument()), arg::Position(pos))) != String::npos) &&
			 (length.argument() ? replaced_count < length.argument() : 1) ){
		erase(arg::Position(pos), arg::Length(old_length));
		insert(arg::Position(pos), new_string);
		pos += new_length;
		replaced_count++;
	}
	return *this;
}

String String::substr(const arg::Position position,
		const arg::Length length
		) const {
	if( position.argument() >= this->length() ){
		return String();
	}
	String ret;
	ret.assign(cstring() + position.argument(), length);
	return ret;
}


String & String::to_upper(){
	u32 s = length();
	char * p = to_char();
	for(u32 i = 0; i < s; i++){
		p[i] = ::toupper(p[i]);
	}
	return *this;
}

String & String::to_lower(){
	u32 s = length();
	char * p = to_char();
	for(u32 i = 0; i < s; i++){
		p[i] = ::tolower(p[i]);
	}
	return *this;
}

void PathString::strip_suffix(){
	u32 dot;
	dot = rfind(arg::CharacterToFind('.'));
	if( dot != npos ){
		to_char()[dot] = 0;
	}
}

const char * PathString::file_name() const{
	u32 slash;
	slash = rfind(arg::CharacterToFind('/'));
	if( slash != npos ){
		return cstring() + slash + 1;
	}
	return cstring();
}




