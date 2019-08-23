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
	/*! \details Constructs an empty string. */
	ConstString();

	/*! \details Constructs a ConstString from a pointer
	  * to a string of zero-terminated characters (c-style string).
	  *
	  * ```
	  * ConstString hello("Hello");
	  * ```
	  *
	  */
	ConstString(
			const char * a /*! C-style, null terminated string */
			);

	/*! \details Points the ConstString to a new
	  * string of zero-terminated characters.
	  *
	  */
	ConstString& operator=(
			const char * a /*! C-style, null terminated string to assign to this object */
			){
		m_string = a; return *this;
	}

	enum npos {
		npos /*! Defines an invalid string length and position */ = (u32)-1
	};

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
	bool operator == (
			const ConstString & a /*! The ConstString to compare this object to */
			) const {
		return compare(arg::StringToCompare(a)) == 0;
	}

	/*! \details Access a byte from the string.
	  *
	  * This operator does not check the bounds of the access.
	  * The at() method checks the bounds.
	  *
	  */
	char operator[](
			u32 position /*! The position of the string to access */
			) const { return cstring()[position]; }

	/*! \details Compares to a c-string (inequality). */
	bool operator!=(const ConstString & a) const { return compare(arg::StringToCompare(a)) != 0; }
	bool operator>(const ConstString & a) const { return compare(arg::StringToCompare(a)) > 0; }
	bool operator<(const ConstString & a) const { return compare(arg::StringToCompare(a)) < 0; }
	bool operator>=(const ConstString & a) const { return compare(arg::StringToCompare(a)) >= 0; }
	bool operator<=(const ConstString & a) const { return compare(arg::StringToCompare(a)) <= 0; }

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
	int to_long(
			const arg::NumberBase base = arg::NumberBase(10)
			){
		return ::strtol(cstring(), 0, base.argument());
	}

	/*! \details Converts the string to a long integer
	 * using the specified number base.
	 *
	 * ```
	 * ConstString number("DEADBEEF");
	 * printf("Number is 0x%X\n",
	 *   number.to_long(
	 *     NumberBase(16)
	 *     )
	 *   );
	 * ```
	 *
	 */
	int to_unsigned_long(
			const arg::NumberBase base = arg::NumberBase(10)
			){
		return ::strtoul(cstring(), 0, base.argument());
	}

	/*! \details Returns character at the specified position.
	 *
	 * \return 0 if pos is past the end of the string.
	 *
	 * ```
	 * ConstString hello("hello");
	 * printf("First character is %c\n", hello.at(0));
	 * printf("Second character is %c\n",
	 *   hello.at(arg::Position(1))
	 *   );
	 * ```
	 *
	 */
	char at(
			const arg::ImplicitPosition position /*! */
			) const;

	/*! \details Returns a c-style string pointer. */
	const char * cstring() const { return m_string; }

	/*! \details Returns the length of the string.
	 *
	 * The string *must* be null-terminated.
	 *
	 * ```
	 * ConstString testing("testing");
	 * printf("length is %d\n", testing.length());
	 * ```
	 *
	 */
	u32 length() const { return strlen(m_string); }

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
	bool is_empty() const {
		return m_string[0] == 0;
	}


	/*! \details Finds a character within this string. */
	u32 find(
			const arg::CharacterToFind a /*! Character to find */,
			const arg::Position pos = arg::Position(0) /*! Start position */
			) const;

	/*! \details Finds the first character that is not as specified. */
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
