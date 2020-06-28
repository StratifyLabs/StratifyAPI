/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef SAPI_SYS_MARKDOWNPRINTER_HPP
#define SAPI_SYS_MARKDOWNPRINTER_HPP

#include "Printer.hpp"

namespace sys {

class MarkdownPrinter : public Printer{
public:
   MarkdownPrinter();

	enum directive {
		no_directive,
		emphasis_bold,
		emphasis_italic,
		emphasis_underline,
		emphasis_strikethrough,
		suppress_newline,
		insert_newline
	};

	enum list_type {
		list_unordered,
		list_ordered
	};

   //emoji: https://www.webfx.com/tools/emoji-cheat-sheet/

   using Printer::operator <<;
   MarkdownPrinter &  operator << (enum directive directive);

   MarkdownPrinter & horizontal_line();

	MarkdownPrinter & open_object(
			const var::String & key,
			enum levels level = level_fatal){
		open_header(key, level);
		return open_list(list_unordered, level);
	}

	MarkdownPrinter & close_object(){
		close_list();
		return close_header();
	}

	MarkdownPrinter & close(){
		close_list();
		return close_header();
	}

	MarkdownPrinter & open_array(
			const var::String & key,
			enum levels level = level_fatal){
		open_header(key, level);
		return open_list(list_ordered, level);
	}

	MarkdownPrinter & close_array(){
		close_list();
		return close_header();
	}

	//increase header level -- can be nested
	MarkdownPrinter & open_header(
			const var::String & key,
			enum levels level = level_info
			);
	MarkdownPrinter & close_header();


	MarkdownPrinter & open_paragraph(
			enum levels level = level_info
			);

	MarkdownPrinter & close_paragraph();

	//increase list level -- can be nested
	MarkdownPrinter & open_list(
			enum list_type type = list_unordered,
			enum levels level = level_info
			);

	MarkdownPrinter & close_list();

	//cannot be nested
	MarkdownPrinter & open_code(
			const var::String & language = "",
			//unique id value
			enum levels level = level_info
			);
	MarkdownPrinter & close_code();

	MarkdownPrinter & open_blockquote(
			enum levels level = level_info
			);

	MarkdownPrinter & close_blockquote();

	MarkdownPrinter & open_table(
			const var::StringList & header,
			enum levels level = level_info
			);

	MarkdownPrinter & append_table_row(const var::StringList& row);
	MarkdownPrinter & close_table();

	MarkdownPrinter & open_pretty_table(
			const var::StringList & header
			);

	MarkdownPrinter & append_pretty_table_row(const var::StringList & row);
	MarkdownPrinter & close_pretty_table(enum levels level = level_info);

	bool is_pretty_table_valid() const {
		return m_pretty_table.count() && m_pretty_table.front().count();
	}

	const var::Vector<var::Vector<var::String>> & pretty_table() const {
		return m_pretty_table;
	}

	MarkdownPrinter & hyperlink(
			const var::String & text,
			const var::String & link);

	MarkdownPrinter & image(
			const var::String & text,
			const var::String & link);

#if 0
	using Printer::debug;
	using Printer::message;
	using Printer::info;
	using Printer::warning;
	using Printer::error;
	using Printer::fatal;

	Printer & debug(const char * fmt, ...);
	Printer & message(const char * fmt, ...);
	Printer & info(const char * fmt, ...);
	Printer & warning(const char * fmt, ...);
	Printer & error(const char * fmt, ...);
	Printer & fatal(const char * fmt, ...);
#endif

private:
	enum container_type {
		container_top,
		container_code,
		container_header,
		container_paragraph,
		container_ordered_list,
		container_unordered_list,
		container_blockquote,
	};

	using Container = PrinterContainer<enum container_type>;
	var::Vector<Container> m_container_list;
	bool m_is_last_close;
	enum directive m_directive;
	var::Vector<var::Vector<var::String>> m_pretty_table;


	var::Vector<Container> & container_list(){ return m_container_list; }
	const var::Vector<Container> & container_list() const { return m_container_list; }

	//re-implemented virtual functions from Printer
	void print_open_object(enum levels level, const char * key);
	void print_close_object();
	void print(
			enum levels level,
			const char * key,
			const char * value,
			bool is_newline = true
			);

	Container & container(){
		return m_container_list.back();
	}

	const Container & container() const {
		return m_container_list.back();
	}

	bool pop_container(enum container_type type){
		bool has_type = false;
		for(const auto & c: m_container_list){
			if( c.type() == type ){
				has_type = true;
			}
		}

		if( !has_type ){ return false; }

		while( container().type() != type ){
			m_container_list.pop_back();
		}

		m_container_list.pop_back();
		return true;
	}

	u16 calculate_nesting() const {
		u16 nesting = 0;
		enum container_type type = container().type();
		for(auto const & c: m_container_list){
			if( c.type() == type ){
				nesting++;
			}
		}
		return nesting;
	}


	bool close_type(enum container_type type);


};

class MarkdownHeader {
public:
	MarkdownHeader(
			MarkdownPrinter & printer,
			const var::String & header,
			enum MarkdownPrinter::levels level = MarkdownPrinter::level_info) : m_printer(printer){
		printer.open_header(header, level);
	}

	~MarkdownHeader(){
		m_printer.close_header();
	}

private:
	MarkdownPrinter & m_printer;
};

class MarkdownCode {
public:
	MarkdownCode(
			MarkdownPrinter & printer,
			const var::String & language,
			enum MarkdownPrinter::levels level = MarkdownPrinter::level_info)
		: m_printer(printer){
		printer.open_code(language, level);
	}

	~MarkdownCode(){
		m_printer.close_code();
	}

private:
	MarkdownPrinter & m_printer;
};

class MarkdownBlockQuote {
public:
	explicit MarkdownBlockQuote(
			MarkdownPrinter & printer,
			enum MarkdownPrinter::levels level = MarkdownPrinter::level_info)
		: m_printer(printer){
		m_printer.open_blockquote(level);
	}

	~MarkdownBlockQuote(){
		m_printer.close_blockquote();
	}

private:
	MarkdownPrinter & m_printer;
};

class MarkdownParagraph {
public:
	explicit MarkdownParagraph(
			MarkdownPrinter & printer,
			enum MarkdownPrinter::levels level = MarkdownPrinter::level_info)
		: m_printer(printer){
		m_printer.open_paragraph(level);
	}

	~MarkdownParagraph(){
		m_printer.close_paragraph();
	}

private:
	MarkdownPrinter & m_printer;
};

class MarkdownList {
public:
	MarkdownList(
			MarkdownPrinter & printer,
			enum MarkdownPrinter::list_type type,
			enum MarkdownPrinter::levels level = MarkdownPrinter::level_info)
		: m_printer(printer){
		printer.open_list(type, level);
	}

	~MarkdownList(){
		m_printer.close_list();
	}

private:
	MarkdownPrinter & m_printer;
};

class MarkdownPrettyTable {
public:
	MarkdownPrettyTable(
			MarkdownPrinter & printer,
			const var::Vector<var::String> & header,
			enum MarkdownPrinter::levels level = MarkdownPrinter::level_info)
		: m_printer(printer){
		printer.open_pretty_table(header);
	}

	~MarkdownPrettyTable(){
		m_printer.close_pretty_table();
	}

private:
	MarkdownPrinter & m_printer;
};

}

#endif // SAPI_SYS_MARKDOWNPRINTER_HPP
