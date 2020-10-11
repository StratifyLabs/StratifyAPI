/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef VAR_API_STRING_HPP_
#define VAR_API_STRING_HPP_

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sos/link.h>
#include <string>

#include "StringView.hpp"
#include "Vector.hpp"

namespace var {

class View;
class Data;

/*! \brief String class
 * \details This is an embedded friendly string class.  It is similar
 * to the C++ string type but is built on var::Data and
 * cstring functions.  The naming convention follows includes
 * many std::string methods.
 *
 *
 *
 * \code
 * #include <sapi/var.hpp>
 *
 * String s1;
 * String s2;
 * s1 = "This is my string";
 * printf("%s", s1.cstring());
 *  //Strings can be compared
 * s2 = "This is another string";
 * if( s1 == s2 ){
 * 	printf("The strings are the same!\n");
 * } else {
 * 	printf("The strings are different\n");
 * }
 * \endcode
 *
 * The above code outputs:
 * \code
 * This is my string 64
 * The strings are different
 * \endcode
 *
 *
 *
 * \note
 *
 * A note about copy initialization:
 *
 * \code
 *
 * String hello("hello"); //OK: const char[6] converted from ConstString
 * String hello = "hello"; //Error: no conversion from const char [6] to String
 * (in copy-initialization)
 *
 * String hello; hello = "hello"; //OK const char[6] converted from ConstString
 * for assignment
 *
 * \endcode
 *
 *
 */
class String : public api::Object {
public:
  constexpr static size_t npos = std::string::npos;

  using Base = StringView::Base;

  using iterator = typename std::string::iterator;
  using const_iterator = typename std::string::const_iterator;
  using reverse_iterator = typename std::string::reverse_iterator;
  using const_reverse_iterator = typename std::string::const_reverse_iterator;

  const_iterator begin() const noexcept { return m_string.begin(); }
  iterator begin() noexcept { return m_string.begin(); }

  const_iterator end() const noexcept { return m_string.end(); }
  iterator end() noexcept { return m_string.end(); }

  const_iterator cbegin() const noexcept { return m_string.cbegin(); }
  const_iterator cend() const noexcept { return m_string.cend(); }

  const_reverse_iterator rbegin() const noexcept { return m_string.rbegin(); }
  reverse_iterator rbegin() noexcept { return m_string.rbegin(); }

  const_reverse_iterator rend() const noexcept { return m_string.rend(); }
  reverse_iterator rend() noexcept { return m_string.rend(); }

  const_reverse_iterator crbegin() const noexcept { return m_string.crbegin(); }
  const_reverse_iterator crend() const noexcept { return m_string.crend(); }

  class Erase {
    API_AF(Erase, size_t, position, 0);
    API_AF(Erase, size_t, length, npos);
  };

  using Append = Erase;
  using Assign = Erase;
  using Find = Erase;
  using GetSubstring = StringView::GetSubstring;

  class Insert {
    API_ACCESS_FUNDAMENTAL(Insert, size_t, position, 0);
    API_ACCESS_FUNDAMENTAL(Insert, size_t, length, 0);
    API_ACCESS_FUNDAMENTAL(Insert, size_t, sub_position, 0);
    API_ACCESS_FUNDAMENTAL(Insert, size_t, sub_length, npos);
  };

  using Compare = Insert;

  /*! \details Constructs an empty string.
   *
   * This constructor does not use any dyncamic memory allocation.
   *
   * The str() method will return a valid zero-length, null-terminated string.
   *
   * If the string is modified using methods herein, the memory
   * will be automatically allocated as needed.
   *
   */
  String();

  explicit String(const char *s) { // cppcheck-suppress[noExplicitConstructor]
    if (s != nullptr) {
      m_string = s;
    }
  }

  explicit String(const Data &data);

  explicit String(StringView s) : m_string(s.string_view()) {}
  String(const String &s) : m_string(s.string()) {}
  String(const char *s, size_t length) : m_string(s, length) {}
  String(size_t length, char c) : m_string(length, c) {}
  String(String &&s) noexcept : m_string(std::move(s.m_string)) {}
  String(
    std::initializer_list<char> il) // cppcheck-suppress[noExplicitConstructor]
    : m_string(il) {}
  String &operator=(const char *s) {
    if (s == nullptr) {
      m_string = std::string();
    } else {
      m_string = s;
    }
    return *this;
  }
  String &operator=(const String &s) {
    m_string = s.string();
    return *this;
  }
  String &operator=(char c) {
    m_string = c;
    return *this;
  }

  explicit String(const std::string &a) : m_string(a) {}
  explicit String(std::string &&s) : m_string(s) {}

  explicit String(const var::View &item);

  String &operator+=(char a) { return append(a); }
  String &operator+=(const String &a) { return append(a); }
  String &operator+=(StringView a) { return append(a); }

  String &operator+=(std::initializer_list<char> il) {
    string() += il;
    return *this;
  }

  String &operator*=(u32 a) {
    reserve(a * length());
    for (u32 i = 0; i < a; i++) {
      append(*this);
    }
    return *this;
  }

  String operator*(u32 a) const {
    String result;
    result.reserve(a * length());
    for (u32 i = 0; i < a; i++) {
      result.append(*this);
    }
    return result;
  }

  String operator+(const String &rhs) const {
    return String(string() + rhs.string());
  }

  String operator+(String &&rhs) const {
    return String(string() + std::move(rhs.string()));
  }
  String operator+(char rhs) const { return String(string() + rhs); }

  ~String() {}

  /*! \details Tests if string is empty.
   *
   * ```
   * ConstString x;
   * if( x.is_empty() ){
   *   //we arrive here
   * }
   * ```
   *
   */
  bool is_empty() const { return m_string.empty(); }

  /*! \details Gets a sub string of the string.
   *
   * @param pos Starting position to look for the sub-string
   * @param len The number of bytes in the String to search
   * @return A new string object containing the sub string specified
   *
   */
  String get_substring(const GetSubstring &options) const {
    if (options.position() >= m_string.length()) {
      return String();
    }
    return String(m_string.substr(options.position(), options.length()));
  }

  String get_substring_at_position(size_t position) const {
    return String(m_string.substr(position));
  }

  String get_substring_with_length(size_t length) const {
    return String(m_string.substr(0, length));
  }

  String &insert(const String &string_to_insert, const Insert &options) {
    if (options.sub_position() == npos) {
      m_string.insert(options.position(), string_to_insert.string());
    } else {
      m_string.insert(
        options.position(),
        string_to_insert.string(),
        options.sub_position(),
        options.sub_length());
    }
    return *this;
  }

  /*! \details Erases a portion of the string starting with the character at
   * \a pos.
   *
   * @param pos The position to start erasing
   * @param len The number of characters to erase
   * @return A reference to this string.
   *
   */
  String &erase(const Erase &options) {
    m_string.erase(options.position(), options.length());
    return *this;
  }

  String &erase(StringView string_to_erase, size_t position = 0);

  class Replace {
    API_AC(Replace, StringView, old_string);
    API_AC(Replace, StringView, new_string);
    API_AF(Replace, size_t, position, 0);
    API_AF(Replace, size_t, count, 0);
  };

  /*! \details Replaces one or more instances of a string with another string
   *
   * @param old_string The string to search for and replace
   * @param new_string The string that will be inserted in place of the old
   * string
   * @return A reference to this string
   *
   */

  String &replace(const Replace &options);

  size_t count(var::StringView to_count) const;
  size_t length() const { return m_string.length(); }
  ssize_t length_signed() const {
    return static_cast<ssize_t>(m_string.length());
  }
  void clear() { m_string.clear(); }

  String &push_back(char a) {
    m_string.push_back(a);
    return *this;
  }

  String &pop_back() {
    m_string.pop_back();
    return *this;
  }

  String &pop_front() { return erase(Erase().set_position(0).set_length(1)); }

  char &at(size_t pos) { return m_string.at(pos); }
  const char &at(size_t pos) const { return m_string.at(pos); }

  char &front() { return m_string.front(); }
  const char &front() const { return m_string.front(); }

  char &back() { return m_string.back(); }
  const char &back() const { return m_string.back(); }

  String &resize(size_t size) {
    m_string.resize(size);
    return *this;
  }

  String &reserve(size_t size) {
    m_string.reserve(size);
    return *this;
  }

  /*! \details Prints a formatted string to this String.
   *
   * @param format Formatted string (same as printf())
   * @return A reference to this String
   *
   * If the formatted string exceeds the length of the string capacity,
   * the string will be resized to accomate the full formatted string.
   *
   * \code
   * #include <sapi/var.hpp>
   * String serial_number;
   * u32 serial_number_value[2] = { 0xAAAABBBB, 0xCCCCEEEE };
   *
   * //prints the formatted serial number to the string
   * serial_number.format("%08lX%08lX",
   *   serial_number_value[1],
   *   serial_number_value[0]);
   *
   * //append to the serial number
   * serial_number << " <- hex dec -> " << String().format("%ld, %ld",
   *   serial_number_value[1],
   *   serial_number_value[0]) << " shown in decimal too."
   * \endcode
   *
   */
  String &format(const char *format, ...);
  String &vformat(const char *fmt, va_list list);

  template <typename T>
  static String number(T value, const char *fmt = nullptr) {
    static_assert(
      std::is_arithmetic<T>::value,
      "Cannot convert non-arithmetic types to string");

    if (fmt == nullptr) {
      return String(std::to_string(value));
    }

    if (std::is_floating_point<T>::value == true) {
      if (sizeof(T) > 4) {
        // double can't fit in 64 characters
        return String().format(fmt, value);
      }
    }

    char buffer[64];
    snprintf(buffer, 63, fmt, value);
    return String(buffer);
  }

  String &assign(const String &string_to_assign) {
    m_string.assign(string_to_assign.string());
    return *this;
  }

  /*! \details Assigns a substring of \a a to this String. */
  String &assign(const String &string_to_assign, const Assign &options) {
    m_string.assign(
      string_to_assign.string(),
      options.position(),
      options.length());
    return *this;
  }

  /*! \details Assigns a maximum of \a n characters of \a a to this String. */
  String &assign(const char *cstring_to_assign, size_t length) {
    if (cstring_to_assign == nullptr) {
      m_string.clear();
    } else {
      m_string.assign(cstring_to_assign, length);
    }
    return *this;
  }

  /*! \details Assigns \a a to this String.  */
  String &assign(const char *cstring_to_assign) {
    if (cstring_to_assign == nullptr) {
      m_string.clear();
    } else {
      m_string.assign(cstring_to_assign);
    }
    return *this;
  }

  String &assign(char character_to_assign, size_t length) {
    m_string.assign(character_to_assign, length);
    return *this;
  }

  String &assign(String &&string_to_move) {
    m_string.assign(std::move(string_to_move.m_string));
    return *this;
  }

  /*! \details Appends \a a to this String.  */
  String &append(const String &string_to_append) {
    m_string.append(string_to_append.string());
    return *this;
  }

  String &append(StringView string_to_append) {
    m_string.append(string_to_append.string_view());
    return *this;
  }

  String &append(const String &string_to_append, const Append &options) {
    m_string.append(
      string_to_append.string(),
      options.position(),
      options.length());
    return *this;
  }

  /*! \details Appends \a c to this String.  */
  String &append(char c) {
    m_string.append(1, c);
    return *this;
  }

  /*! \details Converts the string to upper case.
   *
   * This method operates on the current string (rather
   * than returning a copy.
   *
   * \code
   * #include <sapi/var.hpp>
   *
   * String hello("hello");
   *
   * hello.to_upper(); //hello is now "HELLO"
   * hello.to_lower(); //hellos is now "hello"
   *
   * //making an uppercase copy of a string
   *
   * String hello_to_upper = hello.to_upper(); //both hello and hello_to_upper
   * are "HELLO"
   *
   * String world = String(hello).to_upper(); //world is "HELLO", hello is
   * still "hello"
   *
   * \endcode
   *
   */
  String &to_upper();

  /*! \details Converts the string lower case.
   *
   * This method functions like to_upper() but converts
   * to lower case instead of upper case.
   *
   * See to_upper() for code examples.
   *
   */
  String &to_lower();

  const char *cstring() const { return m_string.c_str(); }

  char *to_char() { return &m_string[0]; }

  int compare(const String &string_to_compare) const {
    return m_string.compare(string_to_compare.string());
  }

  size_t find(StringView a, size_t position = 0) const {
    return m_string.find(a.string_view(), position);
  }

  size_t find_first_of(StringView a, size_t position = 0) const {
    return m_string.find_first_of(a.string_view(), position);
  }

  size_t find_first_not_of(StringView a, size_t position = 0) const {
    return m_string.find_first_not_of(a.string_view(), position);
  }

  size_t reverse_find(StringView a, size_t position = npos) const {
    return m_string.rfind(a.string_view(), position);
  }

  size_t find_last_of(StringView a, size_t position = 0) const {
    return m_string.find_last_of(a.string_view(), position);
  }

  size_t find_last_not_of(StringView a, size_t position = 0) const {
    return m_string.find_last_not_of(a.string_view(), position);
  }

  /*! \details Compares the object to \a str.
   *
   * @param position The position in this object to start the comparison
   * @param length The length of the compared string (this object)
   * @param string_to_compare A reference to the comparing string
   * @param sub_position The position in the comparing string to start
   * comparing
   * @param sub_length The number string characters to compare
   * @return Zero if the strings match
   *
   */
  int compare(const String &string_to_compare, const Compare &options) const {
    return m_string.compare(
      options.position(),
      options.length(),
      string_to_compare.string(),
      options.sub_position(),
      options.sub_length());
  }

  /*! \details Compares to another ConstString.
   *
   *
   * ```
   * ConstString a("hello");
   * ConstString b("world");
   *
   * if( a == b ){
   *   //won't get here
   * }
   * ```
   *
   */
  bool operator==(const String &a) const { return m_string == a.m_string; }
  bool operator==(StringView a) const {
    return m_string == a.string_view();
  }

  bool operator!=(StringView a) const {
    return m_string != a.string_view();
  }

  bool operator==(const char *a) const { return m_string == a; }
  bool operator!=(const char *a) const { return m_string != a; }

  /*! \details Compares to a c-string (inequality). */
  bool operator!=(const String &string_to_compare) const {
    return m_string != string_to_compare.m_string;
  }
  bool operator>(const String &string_to_compare) const {
    return m_string > string_to_compare.m_string;
  }
  bool operator<(const String &string_to_compare) const {
    return m_string < string_to_compare.m_string;
  }
  bool operator>=(const String &string_to_compare) const {
    return m_string >= string_to_compare.m_string;
  }
  bool operator<=(const String &string_to_compare) const {
    return m_string <= string_to_compare.m_string;
  }

  /*! \details Converts the string to an integer.
   *
   * ```
   * ConstString x = "10";
   * printf("X is %d\n", x.to_integer());
   * ```
   *
   */
  int to_integer() const { return ::atoi(cstring()); }

  /*! \details Converts to a float.
   *
   * ```
   * ConstString pi = "3.14";
   * printf("pi is %0.2f\n", pi.to_float());
   * ```
   *
   */
  float to_float() const;

  /*! \details Converts the string to a long integer
   * using the specified number base.
   *
   * ```
   * ConstString number("1000");
   * printf("Number is 0x%X\n",
   *   number.to_long(
   *     NumberBase(10)
   *     )
   *   );
   * ```
   *
   */
  long to_long(Base base = Base::decimal) const {
    return ::strtol(cstring(), nullptr, static_cast<int>(base));
  }

  /*! \details Converts the string to a long integer
   * using the specified number base.
   *
   * ```
   * String number("DEADBEEF");
   * printf("Number is 0x%X\n",
   *   number.to_long(
   *     NumberBase(16)
   *     )
   *   );
   * ```
   *
   */
  unsigned long to_unsigned_long(Base base = Base::decimal) const {
    return ::strtoul(cstring(), nullptr, static_cast<int>(base));
  }

  std::string &string() { return m_string; }
  const std::string &string() const { return m_string; }

  u32 capacity() const { return m_string.capacity(); }

  Vector<String> split(StringView delimiter) const;

  operator StringView() const { return StringView(cstring()); }

  static const String &empty_string() { return m_empty_string; }

private:
  std::string m_string;
  static String m_empty_string;
};

String operator+(StringView lhs, const String &rhs);
String operator+(StringView lhs, StringView rhs);
String operator+(const String &lhs, StringView rhs);
String operator+(StringView lhs, String &&rhs);

using StringList = Vector<String>;

} // namespace var

#endif /* VAR_API_STRING_HPP_ */
