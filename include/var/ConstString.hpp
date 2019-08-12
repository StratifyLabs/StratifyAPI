#ifndef SAPI_VAR_CONSTSTRING_HPP
#define SAPI_VAR_CONSTSTRING_HPP

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "../api/VarObject.hpp"
#include "../arg/Argument.hpp"

namespace var {


/*! \brief Constant String Class
 * \details The ConstString class is basically
 * just a pointer to a string literal with methods added
 * that make immuatable strings easier to work with (as
 * compared to C).
 *
 *
 * Here are a few examples
 *
 * \code
 *
 * //comparing strings
 *
 * const char * c_str0 = "hello";
 * const char * c_str1 = "world";
 * ConstString const_string0 = "hello";
 * ConstString const_string1 = "world";
 *
 * if (strcmp(c_str0, c_str1) == 0 ){
 *   //they are the same
 * }
 *
 * if( const_string0 == const_string1 ){
 *  //they are the same
 * }
 *
 * //finding strings
 *
 * if( const_string1.find("worl") == 0 ){
 *  //const_string1 starts with "worl"
 * }
 *
 * //converting to numbers
 * ConstString number_string = "123";
 *
 * printf("Number string is %d\n", number_string.atoi());
 *
 *
 * \endcode
 *
 * The class var::String inherits ConstString and
 * adds the ability to dyncamically allocate and
 * modify strings.
 *
 *
 */
class ConstString : public api::VarWorkObject
{
public:
	ConstString();

	/*! \details Constructs a ConstString from a pointer
	  * to a string of zero-terminated characters.
	  */
	ConstString(const char * a);

	/*! \details Points the ConstString to a new
	  * string of zero-terminated characters.
	  *
	  */
	ConstString& operator=(const char * a){
		m_string = a; return *this;
	}

	enum {
		npos /*! Defines an invalid string length and position */ = (u32)-1
	};

	/*! \details Compares to a c-string. */
	bool operator == (const ConstString & a) const {
		return compare(arg::StringToCompare(a)) == 0;
	}

	/*! \details Access a byte from the string.
	  *
	  * This operator does not check the bounds of the access.
	  * The at() method checks the bounds.
	  *
	  */
	char operator[](u32 idx) const { return cstring()[idx]; }

	/*! \details Compares to a c-string (inequality). */
	bool operator!=(const ConstString & a) const { return compare(arg::StringToCompare(a)) != 0; }
	bool operator>(const ConstString & a) const { return compare(arg::StringToCompare(a)) > 0; }
	bool operator<(const ConstString & a) const { return compare(arg::StringToCompare(a)) < 0; }
	bool operator>=(const ConstString & a) const { return compare(arg::StringToCompare(a)) >= 0; }
	bool operator<=(const ConstString & a) const { return compare(arg::StringToCompare(a)) <= 0; }

	/*! \details Converts to an integer.
	  *
	  * \code
	  * ConstString x = "10";
	  * printf("X is %d\n", x.atoi());
	  * \endcode
	  *
	  */
	int to_integer() const { return ::atoi(cstring()); }
	int atoi() const { return to_integer(); }

	/*! \details Converts to a float. */
	float to_float() const;
	float atoff() const { return to_float(); }

	int to_long(
			const arg::NumberBase base = arg::NumberBase(10)
			){
		return ::strtol(cstring(), 0, base.argument());
	}

	int to_unsigned_long(
			const arg::NumberBase base = arg::NumberBase(10)
			){
		return ::strtoul(cstring(), 0, base.argument());
	}

	/*! \details Returns character at \a pos.
	 *
	 * Returns 0 if pos is past the end of the string.
	 *
	 */
	char at(const arg::ImplicitPosition position) const;

	/*! \details Returns a c-style string pointer. */
	const char * cstring() const { return m_string; }

	/*! \details Returns the length of the string. */
	u32 length() const { return strlen(m_string); }

	/*! \details Tests if string is empty. */
	bool is_empty() const { return m_string[0] == 0; }


	/*! \details Finds a character within the object. */
	u32 find(
			const arg::CharacterToFind a,
			const arg::Position pos = arg::Position(0)
			) const;

	/*! \details Finds a character within the object. */
	u32 find_not(
			const arg::CharacterToFind a,
			const arg::Position pos = arg::Position(0)
			) const;

	/*! \details Finds a var::ConstString within the object.
	  *
	  * @param a The String to find
	  * @param pos The position to start searching (default is beginning)
	  * @return The position of the string or var::String::npos if the String was not found
	  */
	u32 find(
			const arg::StringToFind a,
			const arg::Position pos = arg::Position(0)
			) const;

	/*! \details Finds a string within the object.
	  *
	  * @param a A reference to the string to search for
	  * @param pos Where in this string to start looking
	  * @param n The number of bytes to match
	  *
	  * @return The offset in this string where the match occurred
	  * or npos if \a a was not found
	  *
	  */
	u32 find(
			const arg::StringToFind a,
			arg::Position pos,
			const arg::Length length
			) const;

	/*! \details Finds a string within the string searching from right to left. */
	u32 rfind(
			const arg::StringToFind a,
			const arg::Position pos = arg::Position(0)
			) const;

	/*! \details Finds a character within the string searching from right to left. */
	u32 rfind(
			const arg::CharacterToFind c,
			const arg::Position position = arg::Position(0)
			) const;

	/*! \details Finds a string within the string searching from right to left. */
	u32 rfind(
			const arg::StringToFind a,
			const arg::Position position,
			arg::Length length
			) const;

	/*! \details Compares the object to \a str.
	  *
	  * @param str A reference comparing string
	  * @returns Zero if the strings are the same
	  *
	  */
	int compare(const arg::StringToCompare a) const;

	/*! \details Compares the object to \a str.
	  *
	  * @param pos The position in this object to start the comparison
	  * @param len The number of characters to compare
	  * @param str A reference to the comparing string
	  * @return Zero if the strings match
	  *
	  */
	int compare(
			const arg::Position position,
			const arg::Length	length,
			const arg::StringToCompare s) const;

	/*! \details Compares the object to \a str.
	  *
	  * @param pos The position in this object to start the comparison
	  * @param len The length of the compared string (this object)
	  * @param str A reference to the comparing string
	  * @param subpos The position in the comparing string to start comparing
	  * @param n The number string characters to compare
	  * @return Zero if the strings match
	  *
	  */
	int compare(
			const arg::Position position,
			const arg::Length length,
			const arg::StringToCompare string_to_compare,
			const arg::SubPosition sub_position,
			const arg::MatchLength match_length
			) const;

	int compare(
			const arg::Position position,
			const arg::Length length,
			const arg::StringToCompare string_to_compare,
			const arg::MatchLength match_length) const;

protected:
	void set_string_pointer(const char * s);

private:
	const char * m_string;
};

}

#endif // SAPI_VAR_CONSTSTRING_HPP
