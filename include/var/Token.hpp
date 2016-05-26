/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef TOKEN_HPP_
#define TOKEN_HPP_

#include "String.hpp"

namespace var {

/*! \brief Abstract Token Class
 * \details The Token Class can convert any String into a list of Token.  The
 * class is similar to STDC strtok().
 *
 */
class Token : public String {
public:
	Token();
	Token(char * mem, size_t s, const char * src, const char * delim, const char * ignore = 0, bool count_empty = false);

	/*! \details Construct and parse a new Token.
	 *
	 * @param src The Source string
	 * @param delim Delimiter string
	 * @param ignore Ignore string
	 * @param count_empty Create empty tokens
	 *
	 * \sa parse()
	 */
	Token(const char * src, const char * delim, const char * ignore = 0, bool count_empty = false);
	Token(const String & src, const char * delim, const char * ignore = 0, bool count_empty = false);

	/*! \details Construct and parse a new Token.
	 *
	 * This method will re-parse the current string.  The \a delim value
	 * contains all separators.  For example, " \t" will create a new token
	 * for each space or tab in the string.  if count_empty() is true, two spaces will mean two tokens.
	 *
	 * The \a ignore string specifies places not to parse.  For example, "\"" will not parse text between
	 * quotes.  This is useful when dealing with standard formats that use quotes to indicate raw data
	 * rather than a tokenized format.
	 *
	 * @param delim Delimiter string
	 * @param ignore Ignore string
	 *
	 * \sa parse()
	 */
	void parse(const char * delim, const char * ignore = 0);


	/*! \details Return the total number of tokens */
	inline size_t size() const { return m_num_tokens; }

	/*! \details Return a pointer to the token specified by offset */
	const char * at(size_t n) const;

	static bool belongs_to(const char c, const char * str, unsigned int len);
	static bool belongs_to(const char c, const char * str){
		return belongs_to(c, str, strlen(str));
	}

	bool count_empty_tokens() const { return m_count_empty_tokens; }
	void set_count_empty_tokens(bool v = true){ m_count_empty_tokens = v; }

protected:

private:
	void init_members();
	unsigned int m_num_tokens;
	unsigned int m_string_size;
	bool m_count_empty_tokens;


};

};


#endif /* TOKEN_HPP_ */
