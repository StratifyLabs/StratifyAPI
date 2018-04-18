/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef STRING_HPP_
#define STRING_HPP_


#include <sos/link.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>

#include "Data.hpp"
#include "StringUtil.hpp"

namespace var {

/*! \brief String class
 * \details This is an embedded friendly string class.  It is similar
 * to the C++ string type but is built on var::Data and
 * cstring functions.  The naming convention follows includes
 * many std::string methods.
 *
 *
 * \code
 * #include <sapi/var.hpp>
 *
 * String s1;
 * String s2;
 * s1 = "This is my string";
 * s1 << " " << s1.capacity() << "\n";
 * printf("%s", s1.c_str());
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
 */
class String : public Data {
public:
    /*! \details Constructs an empty string and allocates the minimum capacity.
     *
     * To construct a String without using any memory allocation us String(0).
     *
     */
	String();

    /*! \details Declares an empty string of a specified capacity.
     *
     * @param capacity The minimum capacity of the string.
     *
     * If \a capacity is zero, no memory will be allocated to the string.
     *
     */
	String(u32 capacity);

    /*! \details Contructs a string as a copy of the string
     * specified.
     *
     * @param s The string to copy.
     *
     * if \a s is_internally_managed(), this object will
     * allocate memory internally and copy the contents of \a s.
     *
     * If \s !is_internally_managed(), this object will refer
     * to the same memory as \a s.
     *
     */
    String(const String & s) : Data(s){}

    /*! \details Assigns the value of a String to another String.
     *
     * @param a The new String to assign
     * @return A reference to the destination String
     *
     * This method will make a copy of the string in a new memory
     * location rather than just using the data location
     * of the source string.
     *
     * \code
     * String str1 = "hello";
     * String str2 = "goodbye";
     * str2 = str1; //both strings are now "hello"
     * \endcode
     *
     */
    String& operator=(const String & a){
        assign(a.c_str());
        return *this;
    }

    ~String(){}

	/*! \details Declares a string and initialize to \a s. */
	String(const char * s);

	/*! \details Declares a string and initialize to \a s. */
	String(const char * s, u32 len);

	/*! \details Returns a c-string.
	 *
	 * This operator causes a String object
	 * to be converted to a c-style string if
	 * the compiler expects a c-style string.
	 *
	 * Here is an example:
	 *
	 * \code
	 * String str;
	 * char buffer[16];
	 * str.assign("Hello");
	 * strcpy(buffer, str); //strcpy() expects a const char *, str is converted to one
	 * \endcode
	 *
	 */
	operator const char * () const { return c_str(); }


	enum {
        npos /*! Defines an invalid string length and position */ = (u32)-1
    };

	/*! \details Constructs a string using statically allocated memory.
	 *
	 * @param mem A pointer to memory to use
	 * @param capacity The number of bytes available
	 * @param readonly True if the mem is in readonly memory
	 */
	String(char * mem, u32 capacity, bool readonly = false);


	/*! \details Sets the capacity of the string.
	 *
	 * @param s The number of bytes to reserve to string capacity.
	 * @return Less than zero on an error
	 *
	 * If a String uses dynamic memory allocation, this method
	 * will increase the capacity of the String. If \a s
	 * is smaller than capacity(), this function return
	 * without changing the capacity.
	 *
	 */
    int set_capacity(u32 s){ return Data::set_capacity(s+1); }

    /*! \details Assigns a c-string to a String. */
    String& operator=(const char * a){
        assign(a); return *this;
    }

	/*! \details Appends a c style string go the string.
	 *
	 * The string will be resized to accept the string if needed.
	 *
	 */
	String& operator<<(const char * a){ append(a); return *this; }

	/*! \details Appends a character to the string. */
	String& operator<<(char c){ append(c); return *this; }


	/*! \details Compares to a c-string. */
	bool operator==(const char * cmp) const { return (strncmp(this->c_str(), cmp, this->capacity()) == 0); }

	/*! \details Compares to a c-string (inequality). */
	bool operator!=(const char * cmp) const { return (strncmp(this->c_str(), cmp, this->capacity()) != 0); }

	/*! \details Converts to an integer.
	 *
	 * \code
	 * String x = "10";
	 * printf("X is %d\n", x.atoi());
	 * \endcode
	 *
	 */
	int atoi() const { return ::atoi(c_str()); }

	/*! \details Converts to a float. */
	float atoff() const;

	/*! \details Gets a sub string of the string.
	 *
	 * @param pos Starting position to look for the sub-string
	 * @param len The number of bytes in the String to search
	 *
	 */
	String substr(u32 pos = 0, u32 len = npos) const;

    /*! \details Inserts \a s (zero terminated) into string at \a pos.
     *
     * @param pos Where to insert the string in this object (zero to insert at beginning)
     * @param s Zero terminated string to insert
     *
     * If \a pos is greater than length(), error_number() is set to EINVAL.
     *
     */
	String& insert(u32 pos, const char * s);

    /*! \details Erases a portion of the string starting with the character at \a pos.
     *
     * @param pos The position to start erasing
     * @param len The number of characters to erase
     * @return A reference to this string.
     *
     */
	String& erase(u32 pos, u32 len = -1);

	/*! \details Returns character at \a pos. */
	char at(u32 pos) const;

	/*! \details Prints a formatted string to this String.
	 *
	 * @param format Formatted string
	 * @return Total number of characters written to the string
	 *
	 * If the formatted string exceeds the length of the string capacity,
	 * the formatted string will be truncated to fit.
	 *
	 */
	int sprintf(const char * format, ...);


	/*! \details Prints the string on the standard output.
	 *
     * @param o_flags This is ignored but is used to override Data::print
	 * @return The total number of characters written to the stdout
     *
	 */
    int print(u32 o_flags = PRINT_HEX) const {
        (void)o_flags; //unused
        return ::printf("%s", str());
    }
    //deprecated
#if !defined __link
    [[deprecated("Replaced by String::print()")]]
#endif
    int printf() const { return ::printf("%s", str()); }


	/*! \details Returns the capacity of the string.
	 *
	 * The capacity is the current number of bytes allocated
	 * in memory for the string. set_capacity() will
	 * increase the capacity of the string. The append()
	 * method will also increase the capacity of the
	 * String if the appended string needs the extra space.
	 *
	 */
	u32 capacity() const;

	/*! \details Returns a c-style string pointer. */
    const char * str() const { return cdata_const(); }
    //compatible with std::string (but not Stratify API convertion)
    const char * c_str() const { return cdata_const(); }

	/*! \details Returns the length of the string. */
    u32 size() const { return strnlen(c_str(), capacity()); }

	/*! \details Returns the length of the string. */
	u32 length() const { return size(); }
    //compatible with std::string
	u32 len() const { return size(); }

	/*! \details Tests if string is empty. */
	bool is_empty() const { return size() == 0; }
    //compatible with std::string
    bool empty() const { return size() == 0; }

	/*! \details Assigns a substring of \a a to string. */
	void assign(const char * a, u32 subpos, u32 sublen){ assign(a + subpos, sublen); }
	/*! \details Assigns a maximum of \a n characters of \a a to string. */
	void assign(const char * a, u32 n);
	/*! \details Assigns \a a (zero terminated) to string.  */
	void assign(const char * a);
	/*! \details Appends \a a (zero terminated) to string.  */
	void append(const char * a);
	/*! \details Appends \a c to string.  */
	void append(char c);
	/*! \details Appends \a c to string.  */
    void push_back(char c) { append(c); }

	/*! \details Copies the \a nth element (separated by \a sep) of the string to to \a dest. */
	bool get_delimited_data(String & dest, int n, char sep = ',', char term = '\n');
	/*! \details Returns the number of elements in the String. */
	int calc_delimited_data_size(char sep = ',', char term = '\n');

	/*! \details Copies a portion of the string to \a s. */
	u32 copy(char * s, u32 len, u32 pos = 0) const;

	/*! \details Copies a portion of this string to \a s. */
	u32 copy(String & s, u32 n, u32 pos = 0) const {
		return copy(s.cdata(), n, pos);
	}

	/*! \details Converts to upper case. */
	void toupper();

	/*! \details Converts to lower case. */
	void tolower();

	/*! \details Finds a var::String within the object.
	 *
	 * @param str The String to find
	 * @param pos The position to start searching
	 * @return The position of the string or var::String::npos if the String was not found
	 */
	u32 find(const String & str, u32 pos = 0) const;

	/*! \details Finds a c string within the object. */
	u32 find(const char * str, u32 pos = 0) const;
	/*! \details Finds a character within the object. */
	u32 find(const char c, u32 pos = 0) const;
	/*! \details Finds a string within the object. */
	u32 find(const char * s, u32 pos, u32 n) const;

	/*! \details Finds a string within the string searching from right to left. */
	u32 rfind(const String & str, u32 pos = 0) const;
	/*! \details Finds a string within the string searching from right to left. */
	u32 rfind(const char * str, u32 pos = 0) const;
	/*! \details Finds a character within the string searching from right to left. */
	u32 rfind(const char c, u32 pos = 0) const;
	/*! \details Finds a string within the string searching from right to left. */
	u32 rfind(const char * s, u32 pos, u32 n) const;

    /*! \details Compares the object to \a str.
     *
     * @param str A reference comparing string
     * @returns Zero if the strings are the same
     *
     */
	int compare(const String & str) const;

    /*! \details Compares the object to \a str.
     *
     * @param pos The position in this object to start the comparison
     * @param len The number of characters to compare
     * @param str A reference to the comparing string
     * @return Zero if the strings match
     *
     */
	int compare(u32 pos, u32 len, const String & str) const;


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
    int compare(u32 pos, u32 len, const String & str, u32 subpos, u32 n) const;

    /*! \details Compares the object to \a str.
     *
     * @param str A reference comparing string
     * @returns Zero if the strings are the same
     *
     */
    int compare(const char * s) const;

    /*! \details Compares the object to \a str.
     *
     * @param pos The position in this object to start the comparison
     * @param len The length of the compared string (this object)
     * @param s The characters to compare
     * @return Zero if the strings match
     *
     */
	int compare(u32 pos, u32 len, const char * s);

    /*! \details Compares the object to \a str.
     *
     * @param pos The position in this object to start the comparison
     * @param len The length of the compared string (this object)
     * @param s The characters to compare
     * @param n The number os characters to compare
     * @return Zero if the strings match
     *
     */
	int compare(u32 pos, u32 len, const char * s, u32 n) const;

private:
};

/*! \details Static String Template Class
 * \details This template is used for declaring fixed length strings.
 */
template <unsigned int arraysize>
class StaticString : public String {
public:
    StaticString() : String(s, arraysize+1, false){ clear(); }
    StaticString(const char * str) : String(s, arraysize+1, false){ assign(str); }
    StaticString(const String & str) : String(s, arraysize+1, false){ assign(str); }

private:
	char s[arraysize+1];
};

//deprecated
template <unsigned int arraysize>
class StringStatic : public String {
public:
    StringStatic() : String(s, arraysize+1, false){ clear(); }
    StringStatic(const char * str) : String(s, arraysize+1, false){ assign(str); }
    StringStatic(const String & str) : String(s, arraysize+1, false){ assign(str); }

private:
    char s[arraysize+1];
};

/*! \details String for file names */
class NameString : public String {
public:
    NameString() : String(LINK_NAME_MAX){}
};

//deprecated
typedef NameString StringName;

/*! \details String for path names */
class PathString : public String {
public:
    PathString() : String(LINK_PATH_MAX){}

	void strip_suffix();
	const char * file_name() const;
	const char * filename() const { return file_name(); }
};

//deprecated
typedef PathString StringPath;


}

#endif /* STRING_HPP_ */
