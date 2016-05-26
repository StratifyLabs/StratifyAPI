//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <var/Token.hpp>
#include <cstdio>

using namespace var;

Token::Token(){
	init_members();
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

Token::Token(const String & src, const char * delim, const char * ignore, bool count_empty) : String(src){
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

		if( belongs_to(*p, delim, len0) ){
			*p = 0;
			if( m_count_empty_tokens == true ){
				m_num_tokens++;
			}
		}

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





