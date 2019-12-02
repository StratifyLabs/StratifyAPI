/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_VAR_TOKENIZER_HPP_
#define SAPI_VAR_TOKENIZER_HPP_

#include "../arg/Argument.hpp"
#include "String.hpp"

namespace var {

/*! \brief Tokenize a String
 * \details The Token Class can convert any String into a list of tokens.  The
 * class is similar to STDC strtok().
 *
 */
class Tokenizer : public var::String {
public:
	Tokenizer();


	using EncodedString = arg::Argument<const var::String &, struct TokenizerEncodedStringTag>;
	using Delimeters = arg::Argument<const var::String &, struct TokenizerDelimetersTag>;
	using IgnoreBetween = arg::Argument<const var::String &, struct TokenizerIgnoreBetweenTag>;

	using IsCountEmpty = arg::Argument<bool, struct TokenizerIsCountEmptyTag>;
	using MaximumCount = arg::Argument<u32, struct TokenizerMaximumCountTag>;

	/*! \details Constructs and parses a new Token.
	  *
	  * @param src The Source string
	  * @param delim Delimiter string
	  * @param ignore Ignore string
	  * @param count_empty Create empty tokens
	  * @param max_delim The maximum number of delimiters to parse before giving up (0 for no limit)
	  *
	  * \sa parse()
	  */
	Tokenizer(
			EncodedString src,
			Delimeters delim,
			IgnoreBetween ignore = IgnoreBetween(""),
			IsCountEmpty is_count_empty = IsCountEmpty(false),
			MaximumCount maximum_delimeter_count = MaximumCount(0)
			);


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
	  * @param max_delim The maximum number of delimiters to parse before giving up (0 for no limit)
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
	void parse(
			Delimeters delim,
			IgnoreBetween ignore = IgnoreBetween(""),
			MaximumCount = MaximumCount(0)
			);


	/*! \details Sorts the tokens as specified. */
	void sort(enum sort_options sort_option = SORT_NONE);


	u32 size() const { return m_num_tokens; }

	/*! \details Returns the total number of tokens. */
	u32 count() const { return m_num_tokens; }

	/*! \details Returns a pointer to the token specified by offset. */
	const String at(u32 n) const;

	static bool belongs_to(const char c, const String & str, unsigned int len);
	static bool belongs_to(const char c, const String & str){
		return belongs_to(
					c,
					str,
					str.length()
					);
	}


	/*! \details Accesses whether or not parse() will count empty tokens. */
	bool is_count_empty_tokens() const { return m_is_count_empty_tokens; }

	/*! \details Sets whether or not parse() should include empty tokens. */
	void set_count_empty_tokens(bool v = true){ m_is_count_empty_tokens = v; }


	Tokenizer & operator=(const Tokenizer & token);

protected:

private:
	void init_members();
	unsigned int m_num_tokens;
	unsigned int m_string_size;
	bool m_is_count_empty_tokens;


};

typedef Tokenizer Token;

}


#endif /* SAPI_VAR_TOKENIZER_HPP_ */
