/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
//Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc


#include <cstdio>
#include <cstring>
#include "var/Reference.hpp"
#include "var/Tokenizer.hpp"
using namespace var;

Tokenizer::Tokenizer(){
	init_members();
	m_is_count_empty_tokens = false;
}

Tokenizer::Tokenizer(
		const var::String & input,
		Delimeters delim,
		IgnoreBetween ignore,
		IsCountEmpty is_count_empty,
		MaximumCount maximum_delimeter_count){


	init_members();
	m_is_count_empty_tokens = is_count_empty.argument();
	parse(input,
			delim,
			ignore,
			maximum_delimeter_count);
}

bool Tokenizer::belongs_to(
		const char c,
		const String & src,
		unsigned int len
		){
	unsigned int i;
	const char * s = src.cstring();
	for(i=0; i < len; i++){
		if( c == *s++ ){
			return true;
		}
	}

	return false;
}

void Tokenizer::init_members(){
	m_num_tokens = 0; m_string_size = 0;
}


void Tokenizer::parse(
		const var::String & input,
		Delimeters delim,
		IgnoreBetween ignore,
		MaximumCount max_delim){
	char * p;
	char * end;
	unsigned int len0, len1;
	bool on_token = false;
	char end_match;

	m_data.copy_contents(input);
	char c = 0;
	m_data.append(c);

	m_num_tokens = 0;
	len0 = delim.argument().length();
	len1 = ignore.argument().length();

	p = m_data.to_char();
	m_string_size = m_data.size();
	end = p + m_string_size;
	if( m_is_count_empty_tokens == true ){
		m_num_tokens++;
	}
	while( p < end ){
		if( len1 > 0 ){
			//this can be used to skip items in quotes "ignore=this" when delim includes = (don't split it)
			while( belongs_to(*p, ignore.argument(), len1) ){
				end_match = *p;
				//fast forward to next member of ignore that matches the first
				p++;
				while( (*p != end_match) && (*p != 0) ){
					p++;
				}
				if( *p == 0 ){
					if( (m_is_count_empty_tokens == false) && (on_token == false) ){
						m_num_tokens++;
					}
					return;
				}
				p++; //move past the last token
				if( *p == 0 ){
					if( (m_is_count_empty_tokens == false) && (on_token == false) ){
						m_num_tokens++;
					}
					return;
				}

			}
		}

		//check to see if the current character is part of the delimiter string
		if( belongs_to(*p, delim.argument(), len0) ){
			*p = 0; //set the character to zero
			if( m_is_count_empty_tokens == true ){
				m_num_tokens++;
			}
		}

		//check for an empty token
		if( m_is_count_empty_tokens == false ){
			if( *p != 0 ){
				if( on_token == false ){
					m_num_tokens++;
					on_token = true;
					if( max_delim.argument() &&
						 (m_num_tokens == max_delim.argument()+1) ){
						return;
					}
				}
			} else {
				on_token = false;
			}
		}
		p++;
	}
}

const String Tokenizer::at(u32 n) const {
	const char * p;
	unsigned int i;
	bool on_token = false;
	unsigned int token = 0;
	p = m_data.to_const_char();

	if( n >= size() ){
		return String();
	}

	if( m_is_count_empty_tokens ){
		while( token != n ){
			if( *p == 0 ){
				token++;
			}
			p++;
		}
	} else {
		i = 0;
		while( (i < m_string_size) && (*p == 0) ){
			i++;
			p++;
		}
		on_token = true;
		while( token != n ){
			p++;
			if( *p != 0 ){
				if( on_token == false ){
					token++;
					on_token = true;
				}
			} else {
				on_token = false;
			}
		}
	}

	return p;
}

void Tokenizer::sort(enum sort_options sort_option){

	u32 i;
	u32 j;
	Tokenizer tmp;
	int current;
	String string_to_copy;
	char * next;
	u8 sort_init;
	char a;
	char b;

	switch(sort_option){
		case SORT_AZ:
			sort_init = 255;
			break;
		case SORT_ZA:
			sort_init = 0;
			break;
		default:
			return;
	}


	tmp = *this; //make a copy of the current object
	u8 used[tmp.size()];

	m_data.clear();
	memset(used, 0, tmp.size());
	next = m_data.to_char();

	for(j=0; j < tmp.size(); j++){
		a = sort_init;
		current = 0;
		for(i=0; i < tmp.size(); i++){
			b = tmp.at(i).at(0);
			if( used[i] == 0 ){
				switch(sort_option){
					default: break;
					case SORT_AZ:
						if(b < a){
							current = i;
							a = b;
						}
						break;
					case SORT_ZA:
						if(b > a){
							current = i;
							a = b;
						}
						break;
				}
			}
		}
		string_to_copy = tmp.at(current);
		strcpy(next, string_to_copy.cstring());
		next += string_to_copy.length() + 1;
		used[current] = 1;
	}
}

var::Vector<var::String> Tokenizer::to_list() const {
	var::Vector<var::String> result;
	for(u32 i=0; i < count(); i++){
		result << at(i);
	}
	return result;
}





