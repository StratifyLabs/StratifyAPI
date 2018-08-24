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


String::String(const ConstString & s, u32 len){
    assign(s, len);
}


String::String(char * mem, u32 capacity, bool readonly) : Data((void*)mem, capacity, readonly){
    if( !readonly ){
        clear();
    }
}

u32 String::capacity() const {
    if( Data::capacity() ){
        return Data::capacity() - 1;
    }
    return 0;
}

String& String::format(const char * format, ...){
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

int String::vformat(const char * fmt, va_list list){
    int result;
    if( capacity() == 0 ){
        set_size(minimum_size());
    }
    result = vsnprintf(cdata(), capacity(), fmt, list);
    if( result > (int)capacity() ){ //if the data did not fit, make the buffer bigger
        if( set_capacity(result+1) >= 0 ){
            vsnprintf(cdata(), capacity(), fmt, list);
        }
    }
    return result;
}

int String::set_size(u32 s){
    int result = Data::set_size(s+1);
    set_string_pointer(cdata_const());
    return result;
}

int String::assign(const ConstString & a){
    return assign(a, a.length());
}

int String::assign(const ConstString & a, u32 n){
    //check for null
    if( a.str() != str() ){ //check for assignment to self - no action needed
        if( n == (u32)npos ){ n = a.length(); }
        if( set_capacity(n) < 0 ){ return -1; }
        clear();
        strncpy(cdata(), a.str(), n);
    }
    return 0;
}

int String::append(const ConstString & a){
    if( a == 0 ){ return 0; }
    u32 len = length();
    u32 alen = a.length();
    set_capacity(len + alen); //try to make min capacity
    if( len == 0 ){
        clear(); //previous length was zero -- ensure string is valid
    }
    if( cdata() == 0 ){ return -1; }
    strncat(cdata(), a.str(), capacity() - len);
    return 0;
}

int String::append(char c){
    u32 len = length();
    u32 alen = 1;
    set_capacity(len + alen + 1); //try to make min capacity
    if( cdata() == 0 ){ return -1; }
    strncat(cdata(),&c,1);
    return 0;
}

bool String::get_delimited_data(String & dest, int n, char sep, char term){
    dest.clear();
    int element = 0;
    int start = 0;
    int end = 0;
    char c;
    for(u32 i=0; i < length(); i++){
        c = at(i);
        if( (c == sep) || (c == term) ){
            end = i;
            if( n == element ){
                copy(dest, end - start, start);
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
    for(u32 i=0; i < length(); i++){
        if( at(i) == sep ){
            elements++;
        }
        if( at(i) == term ){
            break;
        }
    }
    return elements;
}


String& String::insert(u32 pos, const ConstString & str){

    if( cdata() == 0 ){
        assign(str);
        return *this;
    }

    u32 s;
    u32 len = length();

    if( pos > len ){
        set_error_number(EINVAL);
        return *this;
    } else if( pos == len ){
        append(str);
    } else {

        //this needs a limit
        s = str.length();

        char buffer[len+1];

        if( set_capacity( len + s ) < 0 ){
            exit_fatal("failed to alloc for insert");
            return *this;
        }

        strncpy(buffer, cdata()+pos, len+1); //copy the existing string to buffer
        strncpy(cdata()+pos, str.str(), capacity() - pos);
        strncat(cdata(), buffer, capacity());
    }

    return *this;
}

String& String::erase(u32 pos, u32 len){
    char * p = cdata();
    u32 s = length();
    if( p == 0 ){ return *this; }
    if( (len != npos) && (pos + len < s) ){
        int remaining;
        remaining = s - pos - len;
        if( remaining > 0 ){
            ::memcpy(p + pos, p + pos + len, remaining);
            p[pos+remaining] = 0;
        } else {
            p[pos] = 0;
        }
    } else if (pos < s ){
        p[pos] = 0;
    }
    return *this;
}


u32 String::copy(String & s, u32 len, u32 pos) const {
    const char * p = c_str();
    u32 siz = length();
    u32 n;
    if( pos < siz ){
        n = siz - pos;
        if( len > n ){
            len = n;
        }
        ::memcpy(s.data(),p+pos,len);
        return len;
    }
    return 0;
}

String String::substr(u32 pos, u32 len) const {
    if( pos >= length() ){
        return String();
    }

    String ret;
    ret.assign(str() + pos, len);
    ret.set_transfer_ownership();
    return ret;
}


String & String::to_upper(){
    u32 s = length();
    char * p = cdata();
    for(u32 i = 0; i < s; i++){
        p[i] = ::toupper(p[i]);
    }
    return *this;
}

String & String::to_lower(){
    u32 s = length();
    char * p = cdata();
    for(u32 i = 0; i < s; i++){
        p[i] = ::tolower(p[i]);
    }
    return *this;
}

void PathString::strip_suffix(){
    u32 dot;
    dot = rfind('.');
    if( dot != npos ){
        cdata()[dot] = 0;
    }
}

const char * PathString::file_name() const{
    u32 slash;
    slash = rfind('/');
    if( slash != npos ){
        return c_str() + slash + 1;
    }
    return c_str();
}




