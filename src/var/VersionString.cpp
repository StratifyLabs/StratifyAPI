/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "var/VersionString.hpp"
#include "var/Token.hpp"

using namespace var;

VersionString::VersionString(u8 major, u8 minor, u8 patch){
	from(major,minor,patch);
}

VersionString::VersionString(u16 major_minor){
	from(major_minor);
}

void VersionString::from(u16 major, u8 minor, u8 patch){
	m_version =
			String::number(major) +
			"." +
			String::number(minor) +
			"." +
			String::number(patch);
}

void VersionString::from(u16 major_minor){
	m_version =
			String::number(major_minor >> 8) +
			"." +
			String::number(major_minor & 0xff);
}


VersionString & VersionString::operator << (u16 value){
	from(value);
	return *this;
}

VersionString & VersionString::operator << (u32 value){
	from(value >> 16,
			 (value >> 8) & 0xff,
			 value & 0xff);
	return *this;
}


u32 VersionString::to_bcd() const {
	Tokenizer tokens(
				m_version,
				var::Tokenizer::Delimeters(".")
				);
	u32 result = 0;
	u32 token_max = tokens.count() < 3 ? tokens.count() : 3;
	for(u32 i = 0; i < token_max; i++){
		result |= (String(tokens.at(i)).to_integer() & 0xff) << (8*(2-i));
	}
	return result;
}

int VersionString::compare(const VersionString & a, const VersionString & b){
	Tokenizer a_tokens(
				a.m_version,
				var::Tokenizer::Delimeters(".")
				);
	Tokenizer b_tokens(
				b.m_version,
				var::Tokenizer::Delimeters(".")
				);

	if( a_tokens.count() > b_tokens.count() ){
		return 1;
	}

	if( a_tokens.count() < b_tokens.count() ){
		return -1;
	}

	//count is equal -- check the numbers
	for(u32 i = 0; i < a_tokens.count(); i++){
		if( String(a_tokens.at(i)).to_integer() >
			 String(b_tokens.at(i)).to_integer() ){
			return 1;
		}
		if( String(a_tokens.at(i)).to_integer() <
			 String(b_tokens.at(i)).to_integer() ){
			return -1;
		}
	}

	//versions are equal
	return 0;
}
