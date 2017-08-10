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
 * to the C++ string type but is built on Var::Data and
 * cstring functions.  The naming convetion follows that of std::string
 * rather than the typical Stratify Library naming convention.
 *
 *
 * \code
 * #include <sapi/Var.hpp>
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
	/*! \details Declare an empty string */
	String();
	/*! \details Declare an emtpy string of a specified capacity */
	String(u32 capacity);
	/*! \details Declare a string and initialize to \a s */
	String(const char * s);
	/*! \details Declare a string and initialize to \a s */
	String(const char * s, u32 len);

	operator const char * () const { return c_str(); }


	enum {
		npos = (u32)-1
	};

	String(char * mem, u32 cap, bool readonly = false);


	int set_capacity(u32 s){ return Data::set_capacity(s+1); }

	/*! \details Assign a c-string */
	String& operator=(const char * a){ assign(a); return *this; }

	/*! \details Assigns the value of a String to another String.
	 *
	 * @param a The new String to assign
	 * @return A reference to the destination String
	 *
	 * This method will make a copy of the string in a new memory
	 * location rather than just using the data location
	 * of the source string.
	 */
	String& operator=(String & a){ assign(a.c_str()); return *this; }

	/*! \details Appends a c style string go the string. The string will be resized to accept the character if needed. */
	String& operator<<(const char * a){ append(a); return *this; }

	/*! \details Appends a character to the string. */
	String& operator<<(char c){ append(c); return *this; }


	/*! \details Compare to a c-string */
	bool operator==(const char * cmp) const { return (strncmp(this->c_str(), cmp, this->capacity()) == 0); }
	/*! \details Compare to a c-string */
	bool operator!=(const char * cmp) const { return (strncmp(this->c_str(), cmp, this->capacity()) != 0); }

	/*! \details Convert to an integer */
	int atoi() const { return ::atoi(c_str()); }

	/*! \details Convert to a float */
	float atoff() const;

	/*! \details Get a sub string of the string */
	String substr(u32 pos = 0, u32 len = npos) const;

	/*! \details Insert \a s (zero terminated) into string at \a pos */
	String& insert(u32 pos, const char * s);

	/*! \details Erase a portion of the string starting with the character at \a pos */
	String& erase(u32 pos, u32 len = -1);

	/*! \details Return character at \a pos */
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
	 * @return The total number of characters written to the stdout
	 */
	int printf(){ return ::printf("%s", c_str()); }



	u32 capacity() const;

	const char * c_str() const { return cdata_const(); }

	/*! \details Return the length of the string */
	u32 size() const { return strlen(c_str()); }
	/*! \details Return the length of the string */
	inline u32 lengtheight() const { return size(); }
	inline u32 len() const { return size(); }

	/*! \details Test if string is empty */
	bool empty() const { return size() == 0; }

	/*! \details Assign a substring of \a a to string */
	void assign(const char * a, u32 subpos, u32 sublen){ assign(a + subpos, sublen); }
	/*! \details Assign a maximum of \a n characters of \a a to string */
	void assign(const char * a, u32 n);
	/*! \details Assign \a a (zero terminated) to string  */
	void assign(const char * a);
	/*! \details Append \a a (zero terminated) to string  */
	void append(const char * a);
	/*! \details Append \a c to string  */
	void append(char c);
	/*! \details Append \a c to string  */
	inline void push_back(char c) { append(c); }

	/*! \details Copy the \a nth element (separated by \a sep) of the string to to \a dest */
	bool get_delimited_data(String & dest, int n, char sep = ',', char term = '\n');
	/*! \details Return the number of elements in the String */
	int calc_delimited_data_size(char sep = ',', char term = '\n');

	/*! \details Copy a portion of the string to \a s */
	u32 copy(char * s, u32 len, u32 pos = 0) const;
	inline u32 copy(String & s, u32 n, u32 pos = 0) const {
		return copy(s.cdata(), n, pos);
	}

	/*! \details Convert to upper case */
	void toupper();

	/*! \details Convert to lower case */
	void tolower();

	/*! \details Find a var::String within the object
	 *
	 * @param str The String to find
	 * @param pos The position to start searching
	 * @return The position of the string or var::String::npos if the String was not found
	 */
	u32 find(const String & str, u32 pos = 0) const;

	/*! \details Find a c string within the object */
	u32 find(const char * str, u32 pos = 0) const;
	/*! \details Find a character within the object */
	u32 find(const char c, u32 pos = 0) const;
	u32 find(const char * s, u32 pos, u32 n) const;

	u32 rfind(const String & str, u32 pos = 0) const;
	u32 rfind(const char * str, u32 pos = 0) const;
	u32 rfind(const char c, u32 pos = 0) const;
	u32 rfind(const char * s, u32 pos, u32 n) const;


	int compare(const String & str) const;
	int compare(u32 pos, u32 len, const String & str) const;
	int compare(u32 pos, u32 len, const String & str, u32 subpos, u32 sublen) const;
	int compare(const char * s) const;
	int compare(u32 pos, u32 len, const char * s);
	int compare(u32 pos, u32 len, const char * s, u32 n) const;

private:
};

/*! \details Static String Template Class
 * \details This template is used for declaring fixed length strings.
 */
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
class StringName : public String {
public:
	StringName() : String(LINK_NAME_MAX){}
};

/*! \details String for path names */
class StringPath : public String {
public:
	StringPath() : String(LINK_PATH_MAX){}

	void strip_suffix();
	const char * file_name() const;
	const char * filename() const { return file_name(); }
};


};

#endif /* STRING_HPP_ */
