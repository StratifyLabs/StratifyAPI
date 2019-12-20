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
			enum verbose_level level = level_fatal){
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
			enum verbose_level level = level_fatal){
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
			enum verbose_level level = level_info
			);
	MarkdownPrinter & close_header();


	MarkdownPrinter & open_paragraph(
			enum verbose_level level = level_info
			);

	MarkdownPrinter & close_paragraph();

	//increase list level -- can be nested
	MarkdownPrinter & open_list(
			enum list_type type = list_unordered,
			enum verbose_level level = level_info
			);

	MarkdownPrinter & close_list();

	//cannot be nested
	MarkdownPrinter & open_code(
			const var::String & language = "",
			//unique id value
			enum verbose_level level = level_info
			);
	MarkdownPrinter & close_code();

	MarkdownPrinter & open_blockquote(
			enum verbose_level level = level_info
			);

	MarkdownPrinter & close_blockquote();

	MarkdownPrinter & open_table(
			const var::Vector<var::String> & header,
			enum verbose_level level = level_info
			);

	MarkdownPrinter & append_table_row(const var::Vector<var::String> & row);
	MarkdownPrinter & close_table();

	MarkdownPrinter & open_pretty_table(
			const var::Vector<var::String> & header
			);

	MarkdownPrinter & append_pretty_table_row(const var::Vector<var::String> & row);
	MarkdownPrinter & close_pretty_table(enum verbose_level level = level_info);

	bool is_pretty_table_valid() const {
		return m_pretty_table.count() && m_pretty_table.front().count();
	}

	const var::Vector<var::Vector<var::String>> & pretty_table() const {
		return m_pretty_table;
	}

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
	void print_open_object(enum verbose_level level, const char * key);
	void print_close_object();
	void print(
			enum verbose_level level,
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

}

#endif // SAPI_SYS_MARKDOWNPRINTER_HPP
