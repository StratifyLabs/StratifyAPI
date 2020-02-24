/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef SAPI_VAR_VERSIONSTRING_HPP_
#define SAPI_VAR_VERSIONSTRING_HPP_

#include "String.hpp"

namespace var {

class VersionString : public api::InfoObject {
public:
	VersionString(u8 major = 0, u8 minor = 0, u8 patch = 0);
	VersionString(u16 major_minor);

	u32 to_bcd() const;

	u16 to_bcd16() const{
		return to_bcd() >> 8;
	}

	VersionString & operator << (u16 value);
	VersionString & operator << (u32 value);

	bool operator == (const VersionString & a) const{
		return compare(*this, a) == 0;
	}

	bool operator > (const VersionString & a) const{
		return compare(*this, a) > 0;
	}

	bool operator < (const VersionString & a) const{
		return compare(*this, a) < 0;
	}

	bool operator <= (const VersionString & a) const{
		return compare(*this, a) <= 0;
	}

	/*! \details Returns true if this version string
	 * is greater than or equal to a.
	 *
	 * @param a version compared against.
	 *
	 * See compare() for more details.
	 *
	 */
	bool operator >= (const VersionString & a) const{
		return compare(*this, a) >= 0;
	}

	String & string(){ return m_version; }
	const String & string() const { return m_version; }

	/*! \details Compares two version strings.
	 *
	 * @param a first string
	 * @param b second string
	 * @return 1 if a > b, 0 if a == b or -1 if a < b
	 *
	 * If a is formatted as x.y.z and b is formatted
	 * as x.y then a is greater than b and 1 will be returned. If b
	 * uses x.y.z and a uses x.y then -1 will be returned.
	 *
	 * Generally, a project uses one format or the other
	 * and the two formats should not need to be compared.
	 *
	 * Also, an empty version is always less than a version that
	 * is populated.
	 *
	 */
	static int compare(const VersionString & a, const VersionString & b);

private:

	String m_version;

	void from(u16 major, u8 minor, u8 patch);
	void from(u16 major_minor);

};

}

#endif // SAPI_VAR_VERSIONSTRING_HPP_
