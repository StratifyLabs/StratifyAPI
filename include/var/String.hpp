/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef STRING_HPP_
#define STRING_HPP_


#include <sos/link.h>
#include <cstring>
#include <cstdarg>
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
 * This class is for manipulating and modifying strings. The
 * var::ConstString class is for static strings (basically just
 * a `const char *` wrapper). This class inherits var::ConstString
 * which contains methods for getting the length, comparing,
 * finding strings within the string. This class adds the
 * ability to append strings, create sub-strings, add strings, insert,
 * erase, and convert the case.
 *
 *
 * \code
 * #include <sapi/var.hpp>
 *
 * String s1;
 * String s2;
 * s1 = "This is my string";
 * printf("%s", s1.str());
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
class String : public ConstString, public Data {
public:

    enum {
        npos /*! Defines an invalid string length and position */ = (u32)-1
    };

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

    /*! \details Contructs a string as a copy of the string
     * specified.
     *
     * @param a A reference to the string to copy
     *
     */
    String(const String & a) : Data(a){
        set_string_pointer(cdata_const());
    }

    /*! \details Declares a string and initialize to \a s. */
    String(const ConstString & s);

    /*! \details Declares a string and initialize to \a s. */
    String(const ConstString & s, u32 len);


    /*! \details Assigns a to this string.
     *
     * This will do a data copy is a has
     * called Data::set_transfer_ownership().
     *
     */
    String& operator=(const String & a){
        Data::copy(a);
        set_string_pointer(cdata_const());
        return *this;
    }

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
     * String str1("hello");
     * String str2("goodbye");
     * str2 = str1; //both strings are now "hello"
     * \endcode
     *
     */
    String& operator=(const ConstString & a){
        assign(a);
        return *this;
    }

    /*! \details Appends a string to this string. */
    String& operator+=(const ConstString & a){
        append(a);
        return *this;
    }

    /*! \details Appends a character to this string. */
    String& operator+=(char a){
        append(a);
        return *this;
    }

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
     * u32 value = 0xaa55;
     *
     * str1.clear();
     * str1 << "Value is HEX 0x" << String().format("%08lX", value); //with leading zero, uppercase hex
     * str1 << " hex: 0x" << String().format("%lx", value); //no leading zeros, lower-case hex
     * str1 << " or decimal: " << String().format("%ld", value); //unsigned value
     *
     * \endcode
     *
     */
    String& operator<<(const ConstString & a){ append(a); return *this; }

    /*! \details Appends a character to the string. */
    String& operator<<(char c){ append(c); return *this; }


    /*! \details Appends a string to this string and returns a new string. */
    String operator + (const ConstString & a){
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
    int set_size(u32 s);
    int set_capacity(u32 s){ return set_size(s); }


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
    String& insert(u32 pos, const ConstString & s);

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

    //deprecated
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
    int assign(const ConstString & a, u32 subpos, u32 sublen){ return assign(a.str() + subpos, sublen); }
    /*! \details Assigns a maximum of \a n characters of \a a to string. */
    int assign(const ConstString & a, u32 n);
    /*! \details Assigns \a a (zero terminated) to string.  */
    int assign(const ConstString & a);

    /*! \details Appends \a a (zero terminated) to string.  */
    int append(const ConstString & a);
    /*! \details Appends \a c to string.  */
    int append(char c);
    /*! \details Appends \a c to string
     *
     *
     */
    void push_back(char c){ append(c); }

    bool get_delimited_data(String & dest, int n, char sep = ',', char term = '\n');
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

    int vformat(const char * fmt, va_list list);

    int update_capacity(u32 size);

};


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
    NameString(){
        set_capacity(LINK_NAME_MAX);
    }
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
    PathString(){
        set_capacity(LINK_PATH_MAX);
    }

    void strip_suffix();
    const char * file_name() const;
    const char * filename() const { return file_name(); }
};

//deprecated
typedef PathString StringPath;


}

#endif /* STRING_HPP_ */
