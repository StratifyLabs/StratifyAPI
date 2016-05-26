//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <Var/String.hpp>
#include <cctype>
#include <cstdio>
#include <cstring>

using namespace Var;

String::String(){
	set_capacity(min_size());
}

String::String(size_t capacity){
	set_capacity(capacity);
}

String::String(const char * s){
	assign(s);
}

String::String(const char * s, size_t len){
	assign(s, len);
}


String::String(char * mem, size_t cap, bool readonly) : Data((void*)mem, cap, readonly){

}

size_t String::capacity() const {
	if( Data::capacity() ){
		return Data::capacity() - 1;
	}
	return 0;
}

//assignment

float String::atoff() const {
#ifndef __link
	return ::atoff(c_str());
#else
	return ::atof(c_str());
#endif
}

char String::at(size_t pos) const {
	const char * s = c_str();
	if (pos < strlen(s)){
		return s[pos];
	}
	return 0;
}

void String::assign(const char * a){
	if( a == 0 ){
		clear();
		return;
	}
	size_t len = strlen(a);
	assign(a, len);
}

void String::assign(const char * a, size_t n){
	if( a != 0 ){

		if( n == (size_t)npos ){
			n = strlen(a);
		}
		set_capacity(n+1); //attempt to resize the object to at least n
		clear();
		strncpy(cdata(), a, n);
	} else {
		clear();
	}
}

void String::append(const char * a){
	if( a == 0 ){
		return;
	}
	size_t len = size();
	size_t alen = strlen(a);
	set_capacity(len + alen); //try to make min capacity
	if( len == 0 ){
		clear(); //previous length was zero -- ensure string is valid
	}
	if( cdata() == 0 ){ return; }
	strncat(cdata(), a, capacity() - len);
}
void String::append(char c){
	size_t len = size();
	size_t alen = 1;
	set_capacity(len + alen + 1); //try to make min capacity
	if( cdata() == 0 ){ return; }
	strncat(cdata(),&c,1);
}

bool String::get_delimited_data(String & dest, int n, char sep, char term){
	dest.clear();
	int element = 0;
	int start = 0;
	int end = 0;
	char c;
	for(size_t i=0; i < size(); i++){
		c = at(i);
		if( (c == sep) || (c == term) ){
			end = i;
			if( n == element ){
				copy(dest.cdata(), end - start, start);
				return true;
			}
			element++;
			start = i+1;
		}
	}
	return false;
}

int String::calc_delimited_data_size(char sep, char term){
	int elements = 1;
	//return the total number of elements
	for(size_t i=0; i < size(); i++){
		if( at(i) == sep ){
			elements++;
		}
		if( at(i) == term ){
			break;
		}
	}
	return elements;
}


String& String::insert(size_t pos, const char * str){
	char * p = cdata();
	if( p == 0 ){ return *this; }
	size_t s;
	char buffer[Data::capacity()];

	if( str == 0 ){
		return *this;
	}

	s = strlen(str);

	//check for room
	if( size() + s <= Data::capacity() ){
		//make room
		strcpy(buffer, p+pos);
		strcpy(p+pos, str);
		strcat(p, buffer);
	}

	return *this;
}

String& String::erase(size_t pos, size_t len){
	char * p = cdata();
	if( p == 0 ){ return *this; }
	if( ((int)len != -1) && (pos + len < size()) ){
		strcpy(p + pos, p + pos + len);
		p[pos+len] = 0;
	} else if (pos < size() ){
		p[pos] = 0;
	}
	return *this;
}


size_t String::copy(char * s, size_t len, size_t pos) const {
	const char * p = c_str();
	size_t siz = size();
	size_t n;
	if( pos < siz ){
		n = siz - pos;
		if( len > n ){
			len = n;
		}
		memcpy(s,p+pos,len);
		return len;
	}
	return 0;
}

String String::substr(size_t pos, size_t len) const {
	if( pos >= size() ){
		return String();
	}

	return String(c_str() + pos, len);
}


void String::toupper(){
	size_t s = size();
	char * p = cdata();
	for(size_t i = 0; i < s; i++){
		p[i] = ::toupper(p[i]);
	}
}

void String::tolower(){
	size_t s = size();
	char * p = cdata();
	for(size_t i = 0; i < s; i++){
		p[i] = ::tolower(p[i]);
	}
}

size_t String::find(const String & str, size_t pos) const{
	return find(str.c_str(), pos, str.size());
}

size_t String::find(const char * str, size_t pos) const {
	if( str == 0 ){
		return npos;
	}
	int len = strlen(str);
	return find(str, pos, len);
}

size_t String::find(const char c, size_t pos) const{
	char str[2];
	str[0] = c;
	str[1] = 0;
	return find(str, pos);
}

size_t String::find(const char * s, size_t pos, size_t n) const {
	//find s (length n) starting at pos
	if( s != 0 ){
		size_t i;
		for(i=pos; i < size(); i++){
			if( strncmp(c_str() + i, s, n) == 0 ){
				return i;
			}
		}
	}
	return npos;
}

size_t String::rfind(const String & str, size_t pos) const{
	return rfind(str.c_str(), pos, str.size());
}

size_t String::rfind(const char * str, size_t pos) const {
	if( str == 0 ){
		return npos;
	}
	int len = strlen(str);
	return rfind(str, pos, len);
}

size_t String::rfind(const char c, size_t pos) const{
	char str[2];
	str[0] = c;
	str[1] = 0;
	return rfind(str, pos);
}

size_t String::rfind(const char * s, size_t pos, size_t n) const {
	//find s (length n) starting at pos
	if( s != 0 ){
		ssize_t i;
		size_t len = strlen(s);
		if( n > len ){
			n = len;
		}
		for(i=size()-n; i >= (ssize_t)pos; i--){
			if( strncmp(c_str() + i, s, n) == 0 ){
				return i;
			}
		}
	}
	return npos;
}

int String::compare(const String & str) const {
	return compare(str.c_str());
}

int String::compare(size_t pos, size_t len, const String & str) const{
	return strncmp(&(c_str()[pos]), str.c_str(), len);
}

int String::compare(size_t pos, size_t len, const String & str, size_t subpos, size_t sublen) const{
	int l_compared;
	int l_comparing;

	const char * compared = &(c_str()[pos]);
	const char * comparing = &(str.c_str()[subpos]);

	l_compared = strnlen(compared, len);
	l_comparing = strnlen(comparing, sublen);

	if( l_compared != l_comparing ){
		return l_comparing - l_compared;
	}

	return strncmp(compared, comparing, l_compared);
}

int String::compare(const char * s) const{
	if( s == 0 ){
		return npos;
	}
	return strcmp(s, c_str());
}

int String::compare(size_t pos, size_t len, const char * s){
	if( s == 0 ){
		return npos;
	}
	return strncmp(&(c_str()[pos]), s, len);
}

int String::compare(size_t pos, size_t len, const char * s, size_t n) const {
	int l;
	const char * str;
	if( s == 0 ){
		return npos;
	}

	str = &(c_str()[pos]);

	l = strlen(str);
	if( l != (int)n ){
		return l - n;
	}

	return strncmp(str, s, n);
}

void StringPath::strip_suffix(){
	size_t dot;
	dot = rfind('.');
	if( dot != npos ){
		cdata()[dot] = 0;
	}
}

const char * StringPath::file_name() const{
	size_t slash;
	slash = rfind('/');
	if( slash != npos ){
		return c_str() + slash + 1;
	}
	return c_str();
}




