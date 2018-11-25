#ifndef SAPI_VAR_VERSIONSTRING_HPP
#define SAPI_VAR_VERSIONSTRING_HPP

#include "String.hpp"

namespace var {

class VersionString : public api::VarInfoObject {
public:
	VersionString(u8 major = 0, u8 minor = 0, u8 patch = 0);

	u32 to_bcd() const;

	u16 to_bcd16() const{
		return to_bcd() >> 8;
	}

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

	bool operator >= (const VersionString & a) const{
		return compare(*this, a) >= 0;
	}

	String & string(){ return m_version; }
	const String & string() const { return m_version; }

private:

	static int compare(const VersionString & a, const VersionString & b);
	String m_version;

};

}

#endif // SAPI_VAR_VERSIONSTRING_HPP
