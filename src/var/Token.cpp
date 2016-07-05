//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <cstdio>
#include <cstring>
#include "var/Token.hpp"
using namespace var;

Token::Token(){
	init_members();
	m_count_empty_tokens = false;
}

Token::Token(char * mem, size_t s, const char * src, const char * delim, const char * ignore, bool count_empty) : String(mem, s, false){
	init_members();
	m_count_empty_tokens = count_empty;
	clear(); assign(src); parse(delim, ignore);
}

Token::Token(const char * src, const char * delim, const char * ignore, bool count_empty) : String(src){
	init_members();
	m_count_empty_tokens = count_empty;
	parse(delim, ignore);
}

bool Token::belongs_to(const char c, const char * str, unsigned int len){
	unsigned int i;
	len = strlen(str);
	for(i=0; i < len; i++){
		if( c == *str++ ){
			return true;
		}
	}

	return false;
}

void Token::init_members(){
	m_num_tokens = 0; m_string_size = 0;
}


void Token::parse(const char * delim, const char * ignore){
	char * p;
	char * end;
	unsigned int len0, len1;
	bool on_token = false;
	char end_match;
	m_num_tokens = 0;
	len0 = strlen(delim);
	if( ignore ){
		len1 = strlen(ignore);
	}

	p = cdata();
	m_string_size = String::size();
	end = p + m_string_size;
	if( m_count_empty_tokens == true ){
		m_num_tokens++;
	}
	while( p < end ){
		if( ignore != 0 ){
			//this can be used to skip items in quotes "ignore=this"
			if( belongs_to(*p, ignore, len1) ){
				end_match = *p;
				//fast forward to next member of ignore
				p++;
				while( *p != end_match ){
					p++;
				}
				p++;
			}
		}

		//check to see if the current character is part of the delimiter string
		if( belongs_to(*p, delim, len0) ){
			*p = 0; //set the character to zero
			if( m_count_empty_tokens == true ){
				m_num_tokens++;
			}
		}

		//check for an empty token
		if( m_count_empty_tokens == false ){
			if( *p != 0 ){
				if( on_token == false ){
					m_num_tokens++;
					on_token = true;
				}
			} else {
				on_token = false;
			}
		}
		p++;
	}
}

const char * Token::at(size_t n) const {
	const char * p;
	unsigned int i;
	bool on_token = false;
	unsigned int token = 0;
	p = c_str();

	if( m_num_tokens <= n ){
		return 0;
	}

	if( m_count_empty_tokens ){
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

void Token::sort(enum sort_options sort_option){

	u32 i;
	u32 j;
	Token tmp;
	int current;
	const char * string_to_copy;
	char * next;
	char sort_init;
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
			if( (used[i] == 0) ){
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
		strcpy(next, string_to_copy);
		next += strlen(string_to_copy) + 1;
		used[current] = 1;
	}
}

Token & Token::operator=(const Token & token){
	set_capacity(token.capacity());
	memcpy(data(), token.data_const(), token.capacity());
	m_num_tokens = token.m_num_tokens;
	m_string_size = token.m_string_size;
	m_count_empty_tokens = token.m_count_empty_tokens;
	return *this;
}





