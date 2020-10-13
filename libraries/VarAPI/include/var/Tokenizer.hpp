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
  class Construct {
    API_ACCESS_COMPOUND(Construct, var::StringView, delimeters);
    API_ACCESS_COMPOUND(Construct, var::StringView, ignore_between);
    API_ACCESS_FUNDAMENTAL(Construct, u32, maximum_delimeter_count, 0);
  };

  Tokenizer(StringView input, const Construct &options);

  /*! \details Sorting Options used with sort() */
  enum class SortBy {
    none /*! Don't sort */,
    ascending /*! Sort from A to Z */,
    descending /*! Sort from Z to A */
  };

  /*! \details Sorts the tokens as specified. */
  Tokenizer &sort(SortBy sort_option);

  /*! \details Returns the total number of tokens. */
  u32 count() const { return m_token_list.count(); }

  /*! \details Returns a pointer to the token specified by offset. */
  const String &at(u32 n) const;

  const var::StringList &list() const { return m_token_list; }

  var::StringList &list() { return m_token_list; }

  var::String join(StringView delimeter) const;

protected:
private:
  var::StringList m_token_list;

  void parse(StringView input, const Construct &options);
};

typedef Tokenizer Token;

} // namespace var

#endif /* VAR_API_TOKENIZER_HPP_ */
