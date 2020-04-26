/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#ifndef SAPI_VAR_TOKENIZER_HPP_
#define SAPI_VAR_TOKENIZER_HPP_

#include "../arg/Argument.hpp"
#include "String.hpp"
#include "Vector.hpp"
#include "Data.hpp"

namespace var {

/*! \brief Tokenize a String
 * \details The Token Class can convert any String into a list of tokens.  The
 * class is similar to STDC strtok().
 *
 */
class Tokenizer : public api::WorkObject {
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
			const var::String & input,
			Delimeters delim,
			IgnoreBetween ignore = IgnoreBetween(""),
			MaximumCount maximum_delimeter_count = MaximumCount(0)
			);


	/*! \details Sorting Options used with sort() */
	enum sort_option {
		sort_option_none /*! Don't sort */,
		sort_option_az /*! Sort from A to Z */,
		sort_option_za /*! Sort from Z to A */
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
			const var::String & input,
			Delimeters delim,
			IgnoreBetween ignore = IgnoreBetween(""),
			MaximumCount maximum_count = MaximumCount(0)
			);


	/*! \details Sorts the tokens as specified. */
	void sort(enum sort_option sort_option = sort_option_none);


	/*! \details Returns the total number of tokens. */
	u32 count() const { return m_token_list.count(); }

	/*! \details Returns a pointer to the token specified by offset. */
	const String& at(u32 n) const;


	const var::StringList& list() const {
		return m_token_list;
	}

	var::StringList& list(){
		return m_token_list;
	}

protected:

private:
	var::StringList m_token_list;

};

typedef Tokenizer Token;

}

namespace sys {
class Printer;
Printer& operator << (Printer& printer, const var::Tokenizer & a);
}


#endif /* SAPI_VAR_TOKENIZER_HPP_ */
