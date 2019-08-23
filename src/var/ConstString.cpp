#include "var/ConstString.hpp"
#include "var/Data.hpp"

using namespace var;

ConstString::ConstString(){
	//point to a valid empty string
	set_string_pointer(0);
}

ConstString::ConstString(const char * a){
	set_string_pointer(a);
}

void ConstString::set_string_pointer(const char * s){
	if( s != 0 ){
		m_string = s;
	} else {
		m_string = (const char*)&DataReference::m_zero_value;
	}
}

float ConstString::to_float() const {
#ifndef __link
	return ::atoff(cstring());
#else
	return ::atof(cstring());
#endif
}

char ConstString::at(
		const arg::ImplicitPosition position
		) const {
	if (position.argument() < length()){
		return cstring()[position.argument()];
	}
	return 0;
}


u32 ConstString::find(const arg::StringToFind str,
							 const arg::Position pos
							 ) const {
	return find(
				str,
				pos,
				arg::Length(str.argument().length())
				);
}

u32 ConstString::find(
		const arg::CharacterToFind c,
		const arg::Position pos
		) const{
	char str[2];
	str[0] = c.argument();
	str[1] = 0;
	return find(arg::StringToFind(str), pos);
}

u32 ConstString::find_not(
		const arg::CharacterToFind a,
		const arg::Position pos
		) const {
	u32 l = length();
	u32 pos_value = pos.argument();
	while( (at(pos_value) != a.argument()) && (pos_value < l) ){
		pos_value++;
	}
	return pos_value;
}


u32 ConstString::find(const arg::StringToFind s,
							 arg::Position pos,
							 const arg::Length length) const {
	//find s (length n) starting at pos

	if( s.argument().is_empty() == false ){
		u32 len = this->length();
		if( len < length.argument() ){
			return npos;
		}
		for(u32 i=pos.argument(); i < len - length.argument() + 1; i++){
			if( strncmp(cstring() + i, s.argument().cstring(), length.argument()) == 0 ){
				return i;
			}
		}
	}
	return npos;
}

u32 ConstString::rfind(
		const arg::StringToFind str,
		const arg::Position pos
		) const {
	return rfind(
				str,
				pos,
				arg::Length(str.argument().length())
				);
}

u32 ConstString::rfind(
		const arg::CharacterToFind c,
		const arg::Position position
		) const{
	char str[2];
	str[0] = c.argument();
	str[1] = 0;
	return rfind(arg::StringToFind(str), position);
}

u32 ConstString::rfind(
		const arg::StringToFind s,
		const arg::Position position,
		arg::Length length
		) const {
	//find s (length n) starting at pos
	u32 n_value = length.argument();
	if( s.argument().length() != 0 ){
		u32 len = s.argument().length();
		if( n_value > len ){
			n_value = len;
		}
		u32 this_len = this->length();
		for(s32 i=this_len-n_value; i >= (s32)position.argument(); i--){
			if( strncmp(
					 cstring() + i,
					 s.argument().cstring(),
					 n_value) == 0 ){
				return i;
			}
		}
	}
	return npos;
}

int ConstString::compare(
		const arg::Position position,
		const arg::Length length,
		const arg::StringToCompare s
		) const {
	return strncmp(
				cstring() + position.argument(),
				s.argument().cstring(),
				length.argument());
}

int ConstString::compare(
		const arg::Position position,
		const arg::Length length,
		const arg::StringToCompare string_to_compare,
		const arg::SubPosition sub_position,
		const arg::MatchLength match_length
		) const{
	int l_compared;
	int l_comparing;

	const char * compared = cstring() + position.argument();
	const char * comparing = &(string_to_compare.argument().cstring()[sub_position.argument()]);

	l_compared = strnlen(compared, length.argument());
	l_comparing = strnlen(comparing, match_length.argument());

	if( l_compared != l_comparing ){
		return l_comparing - l_compared;
	}

	return strncmp(compared, comparing, l_compared);
}

int ConstString::compare(const arg::StringToCompare s) const {
	return strcmp(cstring(), s.argument().cstring());
}

int ConstString::compare(
		const arg::Position position,
		const arg::Length length,
		const arg::StringToCompare string_to_compare,
		const arg::MatchLength match_length
		) const {
	u32 l;
	const char * str_at_position;

	str_at_position = cstring() + position.argument();

	l = strnlen(str_at_position, match_length.argument());
	if( l != match_length.argument() ){
		return l - match_length.argument();
	}

	return strncmp(
				str_at_position,
				string_to_compare.argument().cstring(),
				match_length.argument()
				);
}

//ConstString ConstString::substr (u32 pos, u32 len) const {
//    u32 current_length = length();
//    if(pos<0||pos>current_length) {
//        return NULL;
//    }
//    if(pos+len>current_length||len==npos) {
//        len=current_length-pos;
//    }
//    ConstString sub_string;
//    int length=pos+len;
//    const char* new_string=new const char[len];
//    int string_index=0;
//    for(int index=pos;index<length;index++) {
//        new_string[string_index++]=at(pos);
//    }
//    sub_string.set_string_pointer(new_string);
//    return sub_string;
//}
