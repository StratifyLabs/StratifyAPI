/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef TOKEN_HPP_
#define TOKEN_HPP_

#include "String.hpp"

namespace var {

/*! \brief Tokenize a String
 * \details The Token Class can convert any String into a list of tokens.  The
 * class is similar to STDC strtok().
 *
 */
class Token : public var::String {
public:
	Token();
	Token(char * mem, u32 s, const char * src, const char * delim, const char * ignore = 0, bool count_empty = false);

	/*! \details Constructs and parses a new Token.
	 *
	 * @param src The Source string
	 * @param delim Delimiter string
	 * @param ignore Ignore string
	 * @param count_empty Create empty tokens
	 *
	 * \sa parse()
	 */
	Token(const char * src, const char * delim, const char * ignore = 0, bool count_empty = false);


	/*! \details Sorting Options used with sort() */
	enum sort_options {
		SORT_NONE /*! Don't sort */,
		SORT_AZ /*! Sort from A to Z */,
		SORT_ZA /*! Sort from Z to A */
	};


	/*! \details Updates the token by parsing a new string.
	 *
	 * @param delim Delimiter string
	 * @param ignore Ignore string
	 *
	 * This method will re-parse the current string.  The \a delim value
	 * contains all separators.  For example, " \t" will create a new token
	 * for each space or tab in the string.  if count_empty() is true, two spaces will mean two tokens.
	 *
	 * The \a ignore string specifies places not to parse.  For example, "\"" will not parse text between
	 * quotes.  This is useful when dealing with standard formats that use quotes to indicate raw data
	 * rather than a tokenized format.
	 *
	 *
	 */
	void parse(const char * delim, const char * ignore = 0);


	/*! \details Sorts the tokens as specified. */
	void sort(enum sort_options sort_option = SORT_NONE);


    u32 size() const { return m_num_tokens; }

    /*! \details Returns the total number of tokens. */
    u32 count() const { return m_num_tokens; }

	/*! \details Returns a pointer to the token specified by offset. */
	const char * at(u32 n) const;

	static bool belongs_to(const char c, const char * str, unsigned int len);
	static bool belongs_to(const char c, const char * str){
		return belongs_to(c, str, strlen(str));
	}


	/*! \details Accesses whether or not parse() will count empty tokens. */
	bool is_count_empty_tokens() const { return m_is_count_empty_tokens; }

	/*! \details Sets whether or not parse() should include empty tokens. */
	void set_count_empty_tokens(bool v = true){ m_is_count_empty_tokens = v; }


    Token & operator=(const Token & token);

protected:

private:
	void init_members();
	unsigned int m_num_tokens;
	unsigned int m_string_size;
	bool m_is_count_empty_tokens;


};

}


#endif /* TOKEN_HPP_ */
