//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include <cstdio>
#include <cstring>
#include "var/Tokenizer.hpp"
using namespace var;

Tokenizer::Tokenizer(){
	init_members();
	m_is_count_empty_tokens = false;
}

Tokenizer::Tokenizer(const ConstString & src, const ConstString & delim, const ConstString & ignore, bool count_empty, u32 max_delim) : String(src){
	init_members();
	m_is_count_empty_tokens = count_empty;
	parse(delim, ignore, max_delim);
}

bool Tokenizer::belongs_to(const char c, const ConstString & src, unsigned int len){
	unsigned int i;
	const char * s = src.str();
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


void Tokenizer::parse(const ConstString & delim, const ConstString & ignore, u32 max_delim){
	char * p;
	char * end;
	unsigned int len0, len1;
	bool on_token = false;
	char end_match;
	m_num_tokens = 0;
	len0 = delim.length();
	len1 = ignore.length();

	p = cdata();
	m_string_size = String::length();
	end = p + m_string_size;
	if( m_is_count_empty_tokens == true ){
		m_num_tokens++;
	}
	while( p < end ){
		if( len1 > 0 ){
			//this can be used to skip items in quotes "ignore=this" when delim includes = (don't split it)
			while( belongs_to(*p, ignore, len1) ){
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
		if( belongs_to(*p, delim, len0) ){
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
					if( max_delim && (m_num_tokens == max_delim+1) ){
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

const ConstString Tokenizer::at(u32 n) const {
	const char * p;
	unsigned int i;
	bool on_token = false;
	unsigned int token = 0;
	p = str();

	if( n >= size() ){
		return ConstString();
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
	ConstString string_to_copy;
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

	clear();
	memset(used, 0, tmp.size());
	next = cdata();

	for(j=0; j < tmp.size(); j++){
		a = sort_init;
		current = 0;
		for(i=0; i < tmp.size(); i++){
			b = tmp.at(i)[0];
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
		strcpy(next, string_to_copy.str());
		next += string_to_copy.length() + 1;
		used[current] = 1;
	}
}

Tokenizer & Tokenizer::operator=(const Tokenizer & token){
	set_capacity(token.capacity());
	::memcpy(data(), token.data_const(), token.capacity());
	m_num_tokens = token.m_num_tokens;
	m_string_size = token.m_string_size;
	m_is_count_empty_tokens = token.m_is_count_empty_tokens;
	return *this;
}





