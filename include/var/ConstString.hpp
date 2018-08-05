#ifndef CONSTSTRING_HPP
#define CONSTSTRING_HPP

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "../api/VarObject.hpp"

namespace var {



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

    /*! \details Converts to an integer.
     *
     * \code
     * ConstString x = "10";
     * printf("X is %d\n", x.atoi());
     * \endcode
     *
     */
    int atoi() const { return ::atoi(str()); }

    /*! \details Converts to a float. */
    float atoff() const;

    /*! \details Returns character at \a pos. */
    char at(u32 pos) const;

    /*! \details Returns a c-style string pointer. */
    virtual const char * str() const { return m_string; }
    //compatible with std::string (but not Stratify API convention)
    const char * c_str() const { return str(); }

    /*! \details Returns the length of the string. */
    u32 length() const { return strlen(str()); }
    //compatible with std::string
    u32 len() const { return length(); }

    /*! \details Tests if string is empty. */
    bool is_empty() const { return length() == 0; }
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
