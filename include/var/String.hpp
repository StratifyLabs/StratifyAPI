/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#ifndef SAPI_VAR_STRING_HPP_
#define SAPI_VAR_STRING_HPP_


#include <sos/link.h>
#include <cstring>
#include <cstdarg>
#include <cstdlib>
#include <cstdio>
#include <string>
#include "../arg/Argument.hpp"
#include "Vector.hpp"

namespace var {

class Reference;

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
 * String hello = "hello"; //Error: no conversion from const char [6] to String (in copy-initialization)
 *
 * String hello; hello = "hello"; //OK const char[6] converted from ConstString for assignment
 *
 * \endcode
 *
 *
 */
class String : public api::WorkObject {
public:

	enum npos {
		npos /*! Defines an invalid string length and position */ = std::string::npos
	};

	using Source = arg::Argument<const String&, struct StringSourceTag >;
	using Destination = arg::Argument<String&, struct StringDestinationTag >;

	using Position = arg::Argument<size_t, struct StringPositionTag >;
	using SubPosition = arg::Argument<size_t, struct StringSubPositionTag >;
	using SubLength = arg::Argument<size_t, struct StringSubLengthTag >;
	using Length = arg::Argument<size_t, struct StringLengthTag >;
	using MatchLength = arg::Argument<size_t, struct StringMatchLengthTag >;

	using ToInsert = arg::Argument< const var::String &, struct StringToInsertTag >;
	using ToErase = arg::Argument< const var::String &, struct StringToEraseTag >;

	enum number_base {
		base_8 = 8,
		base_10 = 10,
		base_16 = 16,
	};

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
	String(
			const var::String & string_to_assign,
			Position position,
			Length length = Length(npos)
			) : m_string(
					 string_to_assign.string(),
					 position.argument(),
					 length.argument()
					 ){
	}

	String(const char * s){
		if( s != nullptr ){ m_string = s; }
	}
	String(const String & s) : m_string(s.string()){}
	String(const char * s, Length length) : m_string(s,length.argument()){}
	String(Length length, char c) : m_string(length.argument(),c){}
	String(String && s) noexcept : m_string(s.m_string){}
	String (std::initializer_list<char> il) : m_string(il){}
	String& operator=(const char * s){
		if( s == nullptr ){
			m_string.clear();
		} else {
			m_string = s;
		}
		return *this;
	}
	String& operator=(const String & s){ m_string = s.string(); return *this; }
	String& operator=(char c){ m_string = c; return *this; }

	explicit String(const std::string & a) : m_string(a){}
	explicit String(const Reference & reference);

	/*! \details Appends a character to this string. */
	String& operator+=(char a){
		append(a);
		return *this;
	}

	String& operator+=(const String & a){
		append(a);
		return *this;
	}

	String& operator+=(std::initializer_list<char> il){
		string() += il;
		return *this;
	}

	String operator+ (const String& rhs) const { return String(string() + rhs.string()); }
	String operator+ (String&& rhs) const { return String(string() + rhs.string()); }
	String operator+ (const char* rhs) const { return String(string() + rhs); }
	String operator+ (char rhs) const { return String(string() + rhs); }


	/*! \details Appends a c style string go the string.
	  *
	  * The string will be resized to accept the string if needed.
	  *
	  * \code
	  * #include <sapi/var.hpp>
	  * String str0;
	  * String str1;
	  *
	  * str0 = "hello";  //str0 is "hello"
	  * str1 << "hello"; //str1 is "hello"
	  *
	  * //combining = and << can create unwanted effects
	  * str0 = "Hello";
	  * str1 = str0 << "World"; //str1 == str0 == "HelloWorld"
	  * //in the above, << operates on str0 then str1 is assiged to str0
	  *
	  * //to append numbers (or any printf() compatible formatting)
	  * str1.clear();
	  * str1 << "String data is at 0x" << String().format("%p", str1.data());
	  * size_t value = 0xaa55;
	  *
	  * str1.clear();
	  * str1 << "Value is HEX 0x" << String().format("%08lX", value); //with leading zero, uppercase hex
	  * str1 << " hex: 0x" << String().format("%lx", value); //no leading zeros, lower-case hex
	  * str1 << " or decimal: " << String().format("%ld", value); //unsigned value
	  *
	  * \endcode
	  *
	  */
	String& operator<<(const String & a){ append(a); return *this; }

	/*! \details Appends a character to the string. */
	String& operator<<(char c){ append(c); return *this; }


	~String(){}

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
	String create_sub_string(
			Position position = Position(0),
			Length length = Length(npos)
			) const {

		return m_string.substr(
					position.argument(),
					length.argument()
					).c_str();
	}

	/*! \details Inserts \a s (zero terminated) into string at \a pos.
	  *
	  * @param pos Where to insert the string in this object (zero to insert at beginning)
	  * @param s Zero terminated string to insert
	  *
	  * If \a pos is greater than length(), error_number() is set to EINVAL.
	  *
	  */
	String& insert(
			Position position,
			const String & string_to_insert
			){
		m_string.insert(
					position.argument(),
					string_to_insert.string()
					);
		return *this;
	}



	String& insert(
			Position position,
			const String & string_to_insert,
			SubPosition sub_position,
			SubLength sub_length
			){
		m_string.insert(
					position.argument(),
					string_to_insert.string(),
					sub_position.argument(),
					sub_length.argument()
					);
		return *this;
	}

	/*! \details Erases a portion of the string starting with the character at \a pos.
	  *
	  * @param pos The position to start erasing
	  * @param len The number of characters to erase
	  * @return A reference to this string.
	  *
	  */
	String& erase(
			Position position,
			Length length = Length(npos)
			){
		m_string.erase(
					position.argument(),
					length.argument()
					);
		return *this;
	}

	String& erase(const String & string_to_erase,
					  Position position = Position(0),
					  Length length = Length(npos)
			);

	String & reverse_erase(
			Position position,
			Length length
			){
		m_string.erase(
					this->length() - position.argument(),
					length.argument()
					);
		return *this;
	}

	/*! \details Replaces one or more instances of a string with another string
	 *
	 * @param old_string The string to search for and replace
	 * @param new_string The string that will be inserted in place of the old string
	 * @return A reference to this string
	 *
	 */

	String& replace(
			const var::String & old_string,
			ToInsert new_string = ToInsert(""),
			Position position = Position(0),
			Length length = Length(0)
			);

	String& replace(
			ToErase old_string,
			ToInsert new_string = ToInsert(""),
			Position position = Position(0),
			Length length = Length(0)
			){
		return replace(old_string.argument(), new_string, position, length);
	}


	size_t length() const { return m_string.length(); }
	void clear(){ m_string.clear(); }

	String & push_back(char a){
		m_string.push_back(a);
		return *this;
	}

	String & pop_back(){
		m_string.pop_back();
		return *this;
	}

	char & at (size_t pos){ return m_string.at(pos); }
	const char & at (size_t pos) const { return m_string.at(pos); }

	char & front (){ return m_string.front(); }
	const char & front () const { return m_string.front(); }

	char & back (){ return m_string.back(); }
	const char & back () const { return m_string.back(); }


	void resize(size_t size){
		m_string.resize(size);
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
	String& format(const char * format, ...);
	String & vformat(const char * fmt, va_list list);

	//deprecated
	int sprintf(const char * format, ...);


	String & assign(const String & string_to_assign){
		m_string.assign(string_to_assign.string());
		return *this;
	}

	/*! \details Assigns a substring of \a a to this String. */
	String & assign(
			const String & string_to_assign,
			SubPosition sub_position,
			SubLength sub_length
			){
		m_string.assign(
					string_to_assign.string(),
					sub_position.argument(),
					sub_length.argument()
					);
		return *this;
	}


	/*! \details Assigns a maximum of \a n characters of \a a to this String. */
	String & assign(
			const char * cstring_to_assign,
			Length length
			){
		if( cstring_to_assign == nullptr ){
			m_string.clear();
		} else {
			m_string.assign(
						cstring_to_assign,
						length.argument()

						);
		}
		return *this;
	}

	/*! \details Assigns \a a to this String.  */
	String & assign(const char * cstring_to_assign){
		if( cstring_to_assign == nullptr ){
			m_string.clear();
		} else {
			m_string.assign(cstring_to_assign);
		}
		return *this;
	}

	String & assign(
			char character_to_assign,
			Length length
			){
		m_string.assign(
					character_to_assign,
					length.argument()
					);
		return *this;
	}

	String & assign(String && string_to_move){
		m_string.assign(string_to_move.m_string);
		return *this;
	}

	/*! \details Appends \a a to this String.  */
	String & append(const String & string_to_append){
		m_string.append(string_to_append.string());
		return *this;
	}

	String & append(
			const String & string_to_append,
			SubPosition sub_position,
			SubLength sub_length
			){
		m_string.append(
					string_to_append.string(),
					sub_position.argument(),
					sub_length.argument()
					);
		return *this;
	}

	/*! \details Appends \a c to this String.  */
	String & append(char c){
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
	  * String hello_to_upper = hello.to_upper(); //both hello and hello_to_upper are "HELLO"
	  *
	  * String world = String(hello).to_upper(); //world is "HELLO", hello is still "hello"
	  *
	  * \endcode
	  *
	  */
	String & to_upper();


	/*! \details Converts the string lower case.
	  *
	  * This method functions like to_upper() but converts
	  * to lower case instead of upper case.
	  *
	  * See to_upper() for code examples.
	  *
	  */
	String & to_lower();

	const char * cstring() const {
		return m_string.c_str();
	}

	char * to_char(){
		return &m_string[0];
	}

	/*! \details Finds a character within this string. */
	size_t find(
			char character_to_find /*! Character to find */,
			Position position = Position(0) /*! Start position */
			) const {
		return m_string.find(
					character_to_find,
					position.argument()
					);
	}

	/*! \details Finds a var::String within the object.
	  *
	  * @param string_to_find The String to find
	  * @param position The position to start searching (default is beginning)
	  * @return The position of the string or var::String::npos if the String was not found
	  */
	size_t find(
			const String & string_to_find,
			Position position = Position(0)
			) const {
		return m_string.find(
					string_to_find.string(),
					position.argument()
					);
	}

	/*! \details Finds a string within the object.
	  *
	  * @param string_to_find A reference to the string to search for
	  * @param position Where in this string to start looking
	  * @param length The number of bytes to match
	  *
	  * @return The offset in this string where the match occurred
	  * or npos if \a a was not found
	  *
	  */
	size_t find(
			const String & string_to_find,
			Position position,
			Length length
			) const {

		return m_string.find(
					string_to_find.cstring(),
					position.argument(),
					length.argument()
					);
	}

	/*! \details Finds the first character that is not as specified. */
	size_t find_not(
			char character_to_find,
			Position position = Position(0)
			) const {

		return m_string.find_first_not_of(
					character_to_find,
					position.argument()
					);
	}

	/*! \details Finds a string within the string searching from right to left. */
	size_t reverse_find(
			const String & string_to_find,
			Position position = Position(npos)
			) const {
		return m_string.rfind(
					string_to_find.string(),
					position.argument()
					);
	}

	size_t rfind(
			const String & string_to_find,
			Position position = Position(npos)
			) const {
		return m_string.rfind(
					string_to_find.string(),
					position.argument()
					);
	}

	/*! \details Finds a character within the string searching from right to left. */
	size_t reverse_find(
			char c,
			Position position = Position(npos)
			) const {
		return m_string.rfind(
					c,
					position.argument()
					);
	}

	size_t rfind(
			char c,
			Position position = Position(npos)
			) const {
		return m_string.rfind(
					c,
					position.argument()
					);
	}

	/*! \details Finds a string within the string searching from right to left. */
	size_t reverse_find(
			const String & string_to_find,
			Position position,
			Length length
			) const {
		return m_string.rfind(
					string_to_find.cstring(),
					position.argument(),
					length.argument()
					);
	}

	size_t rfind(
			const String & string_to_find,
			Position position,
			Length length
			) const {
		return m_string.rfind(
					string_to_find.cstring(),
					position.argument(),
					length.argument()
					);
	}


	int compare(const String & string_to_compare) const {
		return m_string.compare(string_to_compare.string());
	}

	/*! \details Compares the object to \a str.
	  *
	  * @param position The position in this object to start the comparison
	  * @param length The number of characters to compare
	  * @param string_to_compare A reference to the comparing string
	  * @return Zero if the strings match
	  *
	  */
	int compare(
			Position position,
			Length length,
			const String & string_to_compare
			) const {
		return m_string.compare(
					position.argument(),
					length.argument(),
					string_to_compare.string()
					);
	}

	/*! \details Compares the object to \a str.
	  *
	  * @param position The position in this object to start the comparison
	  * @param length The length of the compared string (this object)
	  * @param string_to_compare A reference to the comparing string
	  * @param sub_position The position in the comparing string to start comparing
	  * @param sub_length The number string characters to compare
	  * @return Zero if the strings match
	  *
	  */
	int compare(
			Position position,
			Length length,
			const String & string_to_compare,
			SubPosition sub_position,
			SubLength sub_length
			) const {
		return m_string.compare(
					position.argument(),
					length.argument(),
					string_to_compare.string(),
					sub_position.argument(),
					sub_length.argument()
					);
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
	bool operator == (
			const String & a /*! The ConstString to compare this object to */
			) const {
		return m_string == a.m_string;
	}

	/*! \details Compares to a c-string (inequality). */
	bool operator!=(const String & string_to_compare) const { return m_string != string_to_compare.m_string; }
	bool operator>(const String & string_to_compare) const { return m_string > string_to_compare.m_string; }
	bool operator<(const String & string_to_compare) const { return m_string < string_to_compare.m_string; }
	bool operator>=(const String & string_to_compare) const { return m_string >= string_to_compare.m_string; }
	bool operator<=(const String & string_to_compare) const { return m_string <= string_to_compare.m_string; }

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
	long to_long(
			enum number_base base = base_10
			){
		return ::strtol(cstring(), nullptr, base);
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
	unsigned long to_unsigned_long(
			enum number_base base = base_10
			){
		return ::strtoul(cstring(), nullptr, base);
	}

	std::string & string(){ return m_string; }
	const std::string & string() const { return m_string; }


	u32 capacity() const { return m_string.capacity(); }


	Vector<String> split(const String & delimiter) const;

private:

	std::string m_string;

};


String operator+ (const char*   lhs, const String& rhs);
String operator+ (const char*   lhs, String&& rhs);

}


#endif /* SAPI_VAR_STRING_HPP_ */
