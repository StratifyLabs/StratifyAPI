/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
//Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc


#include <cstdio>
#include <cstring>
#include "var/Reference.hpp"
#include "var/Tokenizer.hpp"
#include "sys/Printer.hpp"

using namespace var;

sys::Printer& operator << (sys::Printer& printer, const var::Tokenizer & a){
	printer.print_open_object(printer.verbose_level(), "tokens");
	for(u32 i=0; i < a.count(); i++){
		printer.print(printer.verbose_level(),
									var::String().format("[%04d]", i).cstring(),
									a.at(i).cstring()
									);
	}
	printer.print_close_object();
	return printer;
}

Tokenizer::Tokenizer(){}

Tokenizer::Tokenizer(
		const var::String & input,
		Delimeters delim,
		IgnoreBetween ignore,
		MaximumCount maximum_delimeter_count){
	parse(input,
				delim,
				ignore,
				maximum_delimeter_count);
}

Tokenizer::Tokenizer(
		const var::String & input,
		const ParseOptions& options
		){
	parse(input, options);
}


void Tokenizer::parse(
		const var::String & input,
		const ParseOptions & options
		){
	parse(
				input,
				Delimeters(options.delimeters()),
				IgnoreBetween(options.ignore_between()),
				MaximumCount(options.maximum_token_count())
				);
}

void Tokenizer::parse(
		const var::String & input,
		Delimeters delim,
		IgnoreBetween ignore,
		MaximumCount maximum_count
		){

	const u32 max_delimeter_count = maximum_count.argument();
	const u32 length = input.length();
	m_token_list = StringList();
	u32 cursor = 0;
	u32 sub_position = 0;

	while( cursor < length ){
		if( delim.argument().find( input.at(cursor) ) != String::npos ){
			m_token_list.push_back(
						input.create_sub_string(
							String::Position(sub_position),
							String::Length(cursor - sub_position)
							)
						);
			sub_position = cursor + 1;
			if( max_delimeter_count && (m_token_list.count() == max_delimeter_count) ){
				cursor = length-1;
			}
		} else if( ignore.argument().find( input.at(cursor) ) != String::npos ){
			//skip the space between specific characters
			char end = input.at(cursor);
			cursor++;
			while( (cursor < length) && (input.at(cursor) != end) ){
				cursor++;
			}
		}

		cursor++;
	}

	//push the last token
	m_token_list.push_back(
				input.create_sub_string(
					String::Position(sub_position),
					String::Length(cursor - sub_position)
					)
				);
}

const String& Tokenizer::at(u32 n) const {
	if( n < m_token_list.count() ){
		return m_token_list.at(n);
	} else {
		return String::empty_string();
	}
}

void Tokenizer::sort(var::Tokenizer::sort_option sort_option){
	switch(sort_option){
		case sort_option_az:
			m_token_list.sort(StringList::ascending);
			break;
		case sort_option_za:
			m_token_list.sort(StringList::descending);
			break;
		default:
			return;
	}
}




