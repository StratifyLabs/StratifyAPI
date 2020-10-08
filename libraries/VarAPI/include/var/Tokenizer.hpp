/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef VAR_API_TOKENIZER_HPP_
#define VAR_API_TOKENIZER_HPP_

#include "Data.hpp"
#include "String.hpp"
#include "Vector.hpp"

namespace var {

/*! \brief Tokenize a String
 * \details The Token Class can convert any String into a list of tokens.  The
 * class is similar to STDC strtok().
 *
 */
class Tokenizer : public api::Object {
public:
  class Parse {
    API_ACCESS_COMPOUND(Parse, var::StringView, delimeters);
    API_ACCESS_COMPOUND(Parse, var::StringView, ignore_between);
    API_ACCESS_BOOL(Parse, count_empty_tokens, false);
    API_ACCESS_FUNDAMENTAL(Parse, u32, maximum_delimeter_count, 0);
  };

  Tokenizer();

  /*! \details Sorting Options used with sort() */
  enum class SortBy {
    none /*! Don't sort */,
    ascending /*! Sort from A to Z */,
    descending /*! Sort from Z to A */
  };

  /*! \details Updates the token by parsing a new string.
   *
   * @param delim Delimiter string
   * @param ignore Ignore string
   * @param max_delim The maximum number of delimiters to parse before giving up
   * (0 for no limit)
   *
   * This method will re-parse the current string.  The \a delim value
   * contains all separators.  For example, " \t" will create a new token
   * for each space or tab in the string.  if count_empty() is true, two spaces
   * will mean two tokens.
   *
   * The \a ignore string specifies places not to parse.  For example, "\"" will
   * not parse text between quotes.  This is useful when dealing with standard
   * formats that use quotes to indicate raw data rather than a tokenized
   * format.
   *
   *
   */
  Tokenizer &parse(StringView input, const Parse &options);

  /*! \details Sorts the tokens as specified. */
  Tokenizer &sort(SortBy sort_option);

  /*! \details Returns the total number of tokens. */
  u32 count() const { return m_token_list.count(); }

  /*! \details Returns a pointer to the token specified by offset. */
  const String &at(u32 n) const;

  const var::StringList &list() const { return m_token_list; }

  var::StringList &list() { return m_token_list; }

protected:
private:
  var::StringList m_token_list;
};

typedef Tokenizer Token;

} // namespace var

namespace sys {
class Printer;
Printer &operator<<(Printer &printer, const var::Tokenizer &a);
} // namespace sys

#endif /* VAR_API_TOKENIZER_HPP_ */
