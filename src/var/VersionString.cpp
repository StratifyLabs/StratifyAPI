#include "var/VersionString.hpp"
#include "var/Token.hpp"

using namespace var;

VersionString::VersionString(u8 major, u8 minor, u8 patch){
	m_version.format("%d.%d.%d", major, minor, patch);
}


u32 VersionString::to_bcd() const {
	Tokenizer tokens(m_version, ".");
	u32 result = 0;
	u32 token_max = tokens.count() < 3 ? tokens.count() : 3;
	for(u32 i = 0; i < token_max; i++){
		result |= (tokens.at(i).atoi() & 0xff) << (8*(2-i));
	}
	return result;
}

int VersionString::compare(const VersionString & a, const VersionString & b){
	Tokenizer a_tokens(a.m_version, ".");
	Tokenizer b_tokens(b.m_version, ".");

	for(u32 i = 0; i < a_tokens.count(); i++){
		if( b_tokens.count() > i ){
			if( a_tokens.at(i).to_integer() > b_tokens.at(i).to_integer() ){
				return 1;
			}
			if( a_tokens.at(i).to_integer() < b_tokens.at(i).to_integer() ){
				return -1;
			}
		} else {
			return 1;
		}
	}

	return 0;

}
