/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef STRING_HPP_
#define STRING_HPP_


#include <sos/link.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>

#include "Data.hpp"
#include "StringUtil.hpp"
#include "ConstString.hpp"

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
class String : public Data, public ConstString {
public:

    enum {
        npos /*! Defines an invalid string length and position */ = (u32)-1
    };

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
     * @param str A reference to the string to copy.
     *
     * if \a s is_internally_managed(), this object will
     * allocate memory internally and copy the contents of str.
     *
     * If str !is_internally_managed(), this object will refer
     * to the same memory as str.
     *
     */
    String(const String & a) : Data(a){
        set_string_pointer(cdata_const());
    }

    /*! \details Declares a string and initialize to \a s. */
    String(const char * s);
    String(const ConstString & s);

    /*! \details Declares a string and initialize to \a s. */
    String(const char * s, u32 len);

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
        assign(a.str());
        return *this;
    }

    /*! \details Appends a string to this string. */
    String& operator+=(const String & a){
        append(a);
        return *this;
    }

    /*! \details Appends a character to this string. */
    String& operator+=(char a){
        append(a);
        return *this;
    }


    /*! \details Appends a string to this string and returns a new string. */
    String operator + (const String & a){
        String ret = *this;
        ret.append(a);
        ret.set_transfer_ownership();
        return ret;
    }

    ~String(){}


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
    int set_capacity(u32 s);

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

    /*! \details Appends a var::String to this String.
     *
     * The string will be resized to accept the string if needed.
     *
     */
    String& operator<<(const String & a){ append(a.str()); return *this; }

	/*! \details Appends a character to the string. */
	String& operator<<(char c){ append(c); return *this; }


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



	/*! \details Prints a formatted string to this String.
	 *
	 * @param format Formatted string
	 * @return Total number of characters written to the string
	 *
	 * If the formatted string exceeds the length of the string capacity,
     * the string will be resized to accomate the full formatted string.
	 *
	 */
	int sprintf(const char * format, ...);



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

	/*! \details Assigns a substring of \a a to string. */
    int assign(const char * a, u32 subpos, u32 sublen){ return assign(a + subpos, sublen); }
	/*! \details Assigns a maximum of \a n characters of \a a to string. */
    int assign(const char * a, u32 n);
	/*! \details Assigns \a a (zero terminated) to string.  */
    int assign(const char * a);
    /*! \details Assigns \a a to this String.  */
    int assign(const String & a){ return assign(a.str()); }

	/*! \details Appends \a a (zero terminated) to string.  */
    int append(const char * a);
    /*! \details Appends a String to this string. */
    int append(const String & a){ return append(a.c_str()); }
	/*! \details Appends \a c to string.  */
    int append(char c);
	/*! \details Appends \a c to string.  */
    void push_back(char c){ append(c); }

	/*! \details Copies the \a nth element (separated by \a sep) of the string to to \a dest. */
	bool get_delimited_data(String & dest, int n, char sep = ',', char term = '\n');
	/*! \details Returns the number of elements in the String. */
	int calc_delimited_data_size(char sep = ',', char term = '\n');

	/*! \details Copies a portion of this string to \a s. */
    u32 copy(String & s, u32 n, u32 pos = 0) const;

	/*! \details Converts to upper case. */
    void to_upper();

    //compatible with C++ string
    void toupper(){ to_upper(); }

	/*! \details Converts to lower case. */
    void to_lower();

    //compatible with C++ string
    void tolower(){ to_lower(); }

    using ConstString::operator ==;
    using ConstString::operator !=;

private:

    int update_capacity(u32 size);

};

/*! \brief String using static memory allocation
 * \details Static String Template Class
 *
 * This template is used for declaring fixed memory size strings.
 *
 */
template <unsigned int arraysize>
class StaticString : public String {
public:
    StaticString() : String(m_str, arraysize+1, false){ clear(); }
    StaticString(const char * str) : String(m_str, arraysize+1, false){ assign(str); }
    StaticString(const String & str) : String(m_str, arraysize+1, false){ assign(str); }

private:
    char m_str[arraysize+1];
};


/*!
 * \brief String for file files
 * \details String for file names
 */
class NameString : public String {
public:
    NameString() : String(LINK_NAME_MAX){}
};

//deprecated
typedef NameString StringName;

/*! \brief String for file paths
 * \details String for path names
 *
 *
 */
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
