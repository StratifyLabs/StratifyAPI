#ifndef CONSTSTRING_HPP
#define CONSTSTRING_HPP

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "../api/VarObject.hpp"

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
	bool operator==(const ConstString & a) const {
		return compare(a) == 0;
	}

	/*! \details Access a byte from the string.
	  *
	  * This operator does not check the bounds of the access.
	  * The at() method checks the bounds.
	  *
	  */
	char operator[](u32 idx) const { return str()[idx]; }

	/*! \details Compares to a c-string (inequality). */
	bool operator!=(const ConstString & a) const { return compare(a) != 0; }
	bool operator>(const ConstString & a) const { return compare(a) > 0; }
	bool operator<(const ConstString & a) const { return compare(a) < 0; }
	bool operator>=(const ConstString & a) const { return compare(a) >= 0; }
	bool operator<=(const ConstString & a) const { return compare(a) <= 0; }

	/*! \details Converts to an integer.
	  *
	  * \code
	  * ConstString x = "10";
	  * printf("X is %d\n", x.atoi());
	  * \endcode
	  *
	  */
	int to_integer() const { return ::atoi(str()); }
	int atoi() const { return to_integer(); }

	/*! \details Converts to a float. */
	float to_float() const;
	float atoff() const { return to_float(); };

	int to_long(int base = 10){ return ::strtol(str(), 0, base); }
	int to_unsigned_long(int base = 10){ return ::strtoul(str(), 0, base); }

	/*! \details Returns character at \a pos. */
	char at(u32 pos) const;

	/*! \details Returns a c-style string pointer. */
	const char * to_char() const { return m_string; }

	virtual const char * str() const { return m_string; }
	//compatible with std::string (but not Stratify API convention)
	const char * c_str() const { return m_string; }

	/*! \details Returns the length of the string. */
	u32 length() const { return strlen(m_string); }
	//compatible with std::string
	u32 len() const { return length(); }

	/*! \details Tests if string is empty. */
	bool is_empty() const { return m_string[0] == 0; }
	//compatible with std::string
	bool empty() const { return is_empty(); }


	/*! \details Finds a character within the object. */
	u32 find(const char a, u32 pos = 0) const;

	/*! \details Finds a var::ConstString within the object.
	  *
	  * @param a The String to find
	  * @param pos The position to start searching (default is beginning)
	  * @return The position of the string or var::String::npos if the String was not found
	  */
	u32 find(const ConstString & a, u32 pos = 0) const;

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
	u32 find(const ConstString & a, u32 pos, u32 n) const;

	/*! \details Finds a string within the string searching from right to left. */
	u32 rfind(const ConstString & a, u32 pos = 0) const;

	/*! \details Finds a character within the string searching from right to left. */
	u32 rfind(const char c, u32 pos = 0) const;

	/*! \details Finds a string within the string searching from right to left. */
	u32 rfind(const ConstString & a, u32 pos, u32 n) const;

	/*! \details Compares the object to \a str.
	  *
	  * @param str A reference comparing string
	  * @returns Zero if the strings are the same
	  *
	  */
	int compare(const ConstString & a) const;

	/*! \details Compares the object to \a str.
	  *
	  * @param pos The position in this object to start the comparison
	  * @param len The number of characters to compare
	  * @param str A reference to the comparing string
	  * @return Zero if the strings match
	  *
	  */
	int compare(u32 pos, u32 len, const ConstString & s) const;

	/*! \details Compares the object to \a str.
	  *
	  * @param pos The position in this object to start the comparison
	  * @param len The length of the compared string (this object)
	  * @param str A reference to the comparing string
	  * @param subpos The position in the comparing string to start comparing
	  * @param n The number os characters to compare
	  * @return Zero if the strings match
	  *
	  */
	int compare(u32 pos, u32 len, const ConstString & str, u32 subpos, u32 n) const;

	int compare(u32 pos, u32 len, const ConstString & s, u32 n) const;

protected:
	void set_string_pointer(const char * s);

private:
	const char * m_string;
};

}

#endif // CONSTSTRING_HPP
