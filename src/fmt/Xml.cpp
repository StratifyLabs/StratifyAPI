//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include <errno.h>
#include <limits.h>
#include "var/Token.hpp"
#include "fmt/Xml.hpp"

using namespace fmt;
using namespace var;


//static const char * white_space = "\r\n\t ";

Xml::Xml(const arg::DestinationFilePath & path, const fs::OpenFlags & flags, const fs::Permissions & permissions){
	//init all values to zero
	file_size = 0;
	init(path, flags, permissions);
}

Xml::Xml(){
	file_size = 0;
	indent = 0;
}


int Xml::init(const arg::DestinationFilePath & path, const fs::OpenFlags & flags, const fs::Permissions & permissions){
	indent = 0;

	//close if already open
	if( file_size != 0 ){
		close();
	}

	if( flags.is_write_only() ){
		if( create(
				 path,
				 arg::IsOverwrite(true),
				 permissions
				 ) < 0 ){
			return -1;
		}
	} else {
		if( open(
				 arg::FilePath(path.argument()),
				 flags
				 ) < 0 ){
			return -1;
		}
		file_size = File::size();
		reset_context();
	}

	return 0;
}



//this doesn't modify the context -- just grabs the values
int Xml::get_value(String & dest, const var::ConstString & key) const {
	return set_get_value(dest, key, false);
}

int Xml::set_get_value(String & dest, const var::ConstString & key, bool set) const {
	String s0;
	String s1; //str stripped of attr
	String s2; //attr
	int start_attr, end_attr;
	context_t tmp_content;
	s32 i;

	//strip the attribute from the str -- attr is only valid for this method

	if( key.length() == 0 ){
		return -1; //empty key
	}

	s1 = key;
	s2.clear();

	start_attr = s1.find(arg::CharacterToFind('('));
	if( start_attr > -1 ){
		end_attr = s1.find(
					arg::CharacterToFind(')'),
					arg::Position(start_attr+1)
					);
		if( end_attr > -1 ){
			s2.copy_contents(
						arg::SourceData(s1),
						arg::Position(start_attr+1),
						arg::Size(end_attr-start_attr-1)
						);
			s1.to_char()[start_attr] = 0;
		} else {
			//malformed argument
			return -1;
		}
	}


	//find the descriptor
	if( !s1.is_empty() ){
		if( find_context(s1.cstring(), content, tmp_content) < 0 ){
			return -1;
		}
	} else {
		tmp_content = content;
	}

	if( !s2.is_empty() ){
		//this is an attribute value that needs to be fetched
		dest.clear();
		return find_attribute(s2, dest, tmp_content);
	} else {
		if( set ){
			s0 = dest;
			tmp_content.size = tmp_content.size - tmp_content.start_tag_size - tmp_content.end_tag_size;
			//pad s0 with spaces to the content size

			for(i=s0.size(); i < tmp_content.size; i++){
				s0.append(" ");
			}

			if( write(
					 arg::Location(tmp_content.offset + tmp_content.start_tag_size),
					 arg::SourceBuffer(s0.cstring()),
					 arg::Size(tmp_content.size)
					 ) < 0 ){
				return -1;
			}
		} else {
			read(
						arg::Location(tmp_content.offset),
						arg::DestinationBuffer(s0.to_void()),
						arg::Size(tmp_content.size)
						);
			dest = s0.substr(
						arg::Position(tmp_content.start_tag_size),
						arg::Length(s0.size() - tmp_content.start_tag_size - tmp_content.end_tag_size)
						);
		}
	}

	return 0;
}

int Xml::set_value(const String * src, const var::ConstString & key) const {
	return set_get_value((String&)*src, key, true);
}


//this will define the current context from str
int Xml::find(const var::ConstString & str){
	int ret;
	context_t tmp;

	reset_context(); //reset the global context

	ret = find_context(str, content, tmp);
	if( ret < 0 ){
		return -1;
	}

	content = tmp;
	return 0;
}

int Xml::find_next(const var::ConstString & str){
	int ret;
	context_t tmp;

	content.cursor += content.size;
	content.size = file_size - content.cursor;
	content.end_tag_size = 0;
	content.start_tag_size = 0;

	ret = find_context(str, content, tmp);
	if( ret < 0 ){
		return -1;
	}

	content = tmp;
	return 0;
}

int Xml::count(){
	//counts the number of immediate children -- get the next element in the current context
	String name;
	int num = 0;
	int ret;
	while( (ret = child(name)) == 0 ){
		num++;
	}
	return num;
}

//point the current context to the next sibling -- -1 if none
int Xml::sibling(String & dest, String * value){
	//this needs to load the next content context
	String str;
	int loc;
	int nbytes;
	int end;
	int space;
	context_t target;
	context_t tmp;

	seek(
				arg::Location(content.offset + content.size)
				);
	nbytes = 0;
	do {
		if( read(
				 arg::DestinationBuffer(str.to_char()),
				 arg::Size(str.size())
				 ) < 0 ){
			return -1;
		}
		loc = str.find(arg::CharacterToFind('<'));
		if( loc < 0 ){
			nbytes += str.capacity();
		}
	} while( loc < 0 );

	seek(
				arg::Location(content.offset + content.size + nbytes + loc + 1)
				);

	if( read(
			 arg::DestinationBuffer(str.to_char()),
			 arg::Size(str.size())
			 ) < 0 ){
		return -1;
	}

	end = str.find(arg::CharacterToFind('>'));
	space = str.find(arg::CharacterToFind(' '));

	if( end > space ){
		end = space;
	}

	dest = str.substr(
				arg::Position(0),
				arg::Length(end)
				);

	if( dest.at(0) == '/' ){
		return -1;
	}

	//not find the end of the context
	tmp = content;
	reset_context();

	content.offset = tmp.offset + tmp.size;

	find_target_tag(dest, content, target);
	content = target;

	return 0;
}

//point the current context to the next child -- -1 if none
int Xml::child(String & name, String * value){
	//load the next entry in the current context and update the cursor
	context_t target;
	context_t tmp;

	//remove the start/end tags from the context
	if( content.start_tag_size != 0 ){
		content.size = content.size - content.start_tag_size - content.end_tag_size;
		content.offset += content.start_tag_size;
		content.start_tag_size = 0;
		content.end_tag_size = 0;
	}

	tmp = content;
	name = "";
	if( find_target_tag(name, tmp, target) < 0 ){
		return -1;
	}

	if( value != 0 ){
		this->get_value(*value, name.cstring());
	}

	content.cursor += target.size;
	return 0;
}

//this grabs the next attribute in the top level context
int Xml::attr(String & name, String * value){
	return 0;
}

int Xml::write_start_tag(const var::ConstString & name, const var::ConstString & attrs){
	String str;
	int i;
	for(i=0; i < indent; i++){
		str.append("\t");
	}
	str.append("<"); str.append(name);
	if( attrs != 0 ){
		str.append(" ");
		str.append(attrs);
		str.append(" ");
	}
	str.append(">\n");
	indent++;
	return write(str);
}

int Xml::write_cdata(const var::ConstString & str){
	String s;
	int i;
	for(i=0; i < indent; i++){
		s.append("\t");
	}
	s.append("<![CDATA[");
	s.append(str);
	s.append("]]>");
	return write(s);
}

int Xml::write_end_tag(const var::ConstString & name){
	String str;
	int i;
	if( indent ){
		indent--;
	}
	for(i=0; i < indent; i++){
		str.append("\t");
	}
	str.append("</");
	str.append(name);
	str.append(">\n");
	return write(str);
}
int Xml::write_empty_element_tag(const var::ConstString & name, const var::ConstString & attrs){
	String str;
	int i;
	for(i=0; i < indent; i++){
		str.append("\t");
	}
	str.append("</");
	str.append(name);
	str.append(" ");
	str.append(attrs);
	str.append(">\n");
	return write(str);
}

int Xml::write_element(const var::ConstString & name, const var::ConstString & data, const var::ConstString & attrs){
	String str;
	int i;
	for(i=0; i < indent; i++){
		str.append("\t");
	}
	str.append("<");
	str.append(name);
	if( attrs != 0 ){
		str.append(" ");
		str.append(attrs);
		str.append(" ");
	}
	str.append(">");

	if(  write(str) != (int)str.size() ){
		return -1;
	}

	if(  write(data) < 0 ){
		return -1;
	}

	str.clear();
	str.append("</");
	str.append(name);
	str.append(">\n");

	if(  write(str) != (int)str.size() ){
		return -1;
	}
	return 0;
}

//find str in the current context and define a target context that defines str
int Xml::find_context(const var::ConstString & str, const context_t & current, context_t & target) const{
	Tokenizer tokens(
				arg::TokenEncodedString(str),
				arg::TokenDelimeters(".")
				);
	//String s0;
	String s1;
	seek(arg::Location(current.offset));
	int i, j;
	context_t tmp;

	tmp = current;

	int array_value;

	for(i=0; i < (s32)tokens.size(); i++){


		//check to see if the token defines an array or an attribute ie wpt[1] or wpt(lat)
		array_value = parse_ref_array(s1, tokens.at(i));

		if( array_value < 0 ){
			//malformed reference descriptor
			return -1;
		}


		j = 0;
		while( (int)j <= array_value ){
			if( find_target_tag(s1, tmp, target) < 0 ){
				//failed to find the tag anywhere
				//printf("didn't find %s\n", s1.c_str());
				return -1;
			} else {
				//printf("Found -%s- at %d with size %d (%d < %d)\n", s1.c_str(), target.offset, target.size, j, array_value);
			}
			if( j < array_value ){
				//if going through an array (siblings) skip the whole value
				tmp.cursor += target.size;
				//printf("Target size is %d for %s\n", target.size, s1.c_str());
			} else {
				//if not in an array (child) just skip the start tag
				tmp.cursor += target.start_tag_size;
				//printf("Target start tag size is %d for %s\n", target.start_tag_size, s1.c_str());
			}
			j++;
		}

	}

	return 0;
}

//private workhorse functions
int Xml::parse_ref_array(String & name, const ConstString & str){
	String s0;
	int ret;
	ret = parse_ref(name, s0, str, "[]");
	if( ret == 1 ){
		ret = s0.to_integer();
	}
	return ret;
}

int Xml::parse_ref_attr(String & name, String & attr_name, const ConstString & str){
	String s0;
	int ret;
	ret = parse_ref(name, attr_name, str, "()");
	if( ret < 0 ){
		return -1;
	}
	return 0;
}

int Xml::parse_ref(String & name, String & value, const ConstString & str, const ConstString & enclosing){
	String s0;
	int open_bracket;
	int close_bracket;

	s0 = str;

	open_bracket = s0.find(arg::CharacterToFind(enclosing[0]));
	if( open_bracket > 0 ){
		close_bracket = s0.find(
					arg::CharacterToFind(enclosing[1]),
				arg::Position(open_bracket+1)
				);
		if( close_bracket > 0 ){
			value.copy_contents(
						arg::SourceData(s0),
						arg::Position(open_bracket+1),
						arg::Size(close_bracket-open_bracket-1)
						);
			name.copy_contents(
						arg::SourceData(s0),
						arg::Size(open_bracket)
						);
			return 1;
		} else {
			//malformed statement
			return -1;
		}
	}

	//not an array
	name = s0;
	return 0;
}



/*
 * Start tag is defined: '<' Name (S Attribute)* S? '>' (S is whitespace
 *
 *
 */
int Xml::find_target_tag(String & name, context_t & context, context_t & target) const {
	int start_offset;
	int end_offset;
	String str;

	start_offset = find_tag(name.cstring(), context, "<", target.start_tag_size); //find a normal start tag
	if( start_offset > -1 ){
		target.offset = start_offset;
		//check to see if the start-tag is an empty element tag
		if( is_empty_element_tag(target) ){
			return 0;
		}

		context.cursor = start_offset - context.offset; //add 1 to advance past start tag

		if( name.is_empty() ){
			load_start_tag(str, target);
			Tokenizer t0(
						arg::TokenEncodedString(str),
						arg::TokenDelimeters("\t\r\n <>/"),
						arg::IgnoreTokensBetween("\"'")
						);
			if( t0.size() ){
				name = t0.at(0);
			} else {
				return -1;
			}
			context.cursor++;
		}

		//now find the end tag
		end_offset = find_tag(name.cstring(), context, "</", target.end_tag_size);
		if( end_offset > -1 ){
			target.cursor = 0;
			target.offset = start_offset;
			target.size = end_offset - start_offset + target.end_tag_size;
			return 0;
		}
	}
	return -1;
}

bool Xml::is_empty_element_tag(context_t & target) const{
	char c;
	c = 0;
	read(
				arg::Location(target.offset + target.start_tag_size - 2),
				arg::DestinationBuffer(&c),
				arg::Size(1)
				);
	if( c == '/' ){
		return true;
	}
	return false;
}

int Xml::next_tag_name(
		String & name,
		const context_t & context,
		context_t target){
	int start_loc;
	String s0;

	if( (start_loc = find_tag("", context, "<", target.start_tag_size)) < 0 ){
		return -1;
	}

	target.offset = start_loc;


	if( load_start_tag(s0, target) < 0){
		return -1;
	}

	Tokenizer t0(
				arg::TokenEncodedString(s0),
				arg::TokenDelimeters("\t\r\n <>/"),
				arg::IgnoreTokensBetween("\"'")
				);

	//the name is t0.at(0)
	if( t0.size() ){
		name = t0.at(0);
		return 0;
	}

	return -1;

}

int Xml::check_string_for_open_bracket(String * src, String * cmp) const {
	int loc;
	int cmp_size = cmp->size();
	char term;
	loc = 0;
	do {
		loc = src->find(
					arg::StringToFind(*cmp),
					arg::Position(loc)
					);
		term = src->at(loc + cmp_size);
		if( loc > 0 ){
			if( (term != ' ') && (term != '>') ){
				loc += cmp_size;
				continue;
			} else {
				break;
			}
		}
	} while( loc > 0 );
	return loc;
}


int Xml::find_tag(const var::ConstString & name,
						const context_t & context,
						const var::ConstString & tag_style,
						s32 & tag_size) const {
	int ret;
	int loc;
	int start_loc;
	int end_loc;
	String str;
	String str_cmp;
	int state;
	int bytes_read = 0;
	int page_size;
	int offset;
	int tmp;

	str_cmp.clear();
	str_cmp.append(tag_style);
	str_cmp.append(name);
	state = FIND_STATE_OPENBRACKET;

	start_loc = 0;
	offset = context.offset + context.cursor;

	//read in some data looking for the <name portion
	do {

		page_size = context.size - bytes_read;
		if( page_size > (int)str.capacity() ){
			page_size = str.capacity();
		}

		if( page_size < 0 ){
			return -1;
		}

		ret = read(
					arg::Location(offset),
					arg::DestinationBuffer(str.to_void()),
					arg::Size(page_size)
					);

		loc = 0;
		if( ret > 0 ){
			//we don't want to cut off the starting char so we need to go back to the last >
			tmp = str.rfind(arg::StringToFind(">"));
			if( tmp > 0 ){
				ret = tmp;
				str.to_char()[tmp+1] = 0;
			}

			bytes_read += ret;
			switch(state){
				case FIND_STATE_OPENBRACKET:
					if( (loc = check_string_for_open_bracket(&str, &str_cmp)) > -1 ){


						state = FIND_STATE_CLOSEBRACKET;
						start_loc = offset + loc;
						loc += str_cmp.size();
					} else {
						break;
					}
					//no break;
				case FIND_STATE_CLOSEBRACKET:
					//found part of the start tag -- now find the closing tag -- validate attr's?
					/*
				if( allowed_chars != 0 ){
					//check to see that the chars between the name and close bracket are valid
				}
				 */


					if( (loc = str.find(
							  arg::CharacterToFind('>'),
							  arg::Position(loc)
							  )) > -1 ){
						//found the end
						end_loc = offset + loc;
						tag_size = end_loc - start_loc + 1;
						return start_loc; //location of the start of the tag
					}
			}
			offset += ret;
		}

	} while( (ret > 0) && (bytes_read + context.cursor < context.size) );

	return -1;
}

int Xml::check_chars(const char * src, const char * allowed){
	int i;
	int j;
	int len0, len1;
	len0 = strlen(src);
	len1 = strlen(allowed);
	for(i=0; i < len0; i++){
		for(j=0; j < len1; j++){
			if( src[i] == allowed[j] ){
				break;
			}
		}
		if( j == len1 ){
			return -1;
		}
	}
	//all characters are OK
	return 0;
}

void Xml::show_context(context_t & context){
	String str;
	int page_size;
	int bytes_read;
	int offset;

	bytes_read = 0;

	offset = context.offset;

	do {
		page_size = context.size - bytes_read;
		if( page_size > (int)str.capacity() ){
			page_size = str.capacity();
		}

		memset(str.to_void(), 0, str.size());
		if( read(
				 arg::Location(offset),
				 arg::DestinationBuffer(str.to_void()),
				 arg::Size(page_size)
				 ) > 0 ){
			printf("%s", str.cstring());
		} else {
			return;
		}

		bytes_read += page_size;
		offset += page_size;

	} while( bytes_read < content.size );
}

void Xml::reset_context(){
	memset(&content, 0, sizeof(content));
	content.size = file_size;
}

int Xml::read_context(int offset, String & str, context_t & target) const {
	unsigned int page_size;
	page_size = target.size - offset;
	if( page_size > str.capacity() ){
		page_size = str.capacity();
	}
	return read(
				arg::Location(target.offset + target.cursor + offset),
				arg::DestinationBuffer(str.to_void()),
				arg::Size(str.capacity())
				);
}

int Xml::load_start_tag(String & tag, const context_t & target) const {
	if( tag.capacity() < (unsigned int)target.start_tag_size ){
		return -1;
	}

	return read(
				arg::Location(target.offset),
				arg::DestinationBuffer(tag.to_void()),
				arg::Size(target.start_tag_size)
				);

}

int Xml::find_attribute(String & key, String & value, context_t & target) const{
	String s0;
	unsigned int i;


	if( load_start_tag(s0, target) < 0 ){
		return -1;
	}


	Tokenizer t0(
				arg::TokenEncodedString(s0),
				arg::TokenDelimeters("\t\r\n <>/"),
				arg::IgnoreTokensBetween("\"'")
				);
	for(i=0; i < t0.size(); i++){
		Tokenizer t1(
					arg::TokenEncodedString(t0.at(i)),
					arg::TokenDelimeters("=")
					);
		if( key == t1.at(0) ){
			value = t1.at(1);
			return 0;
		}
	}


	return -1;
}


