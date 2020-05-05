/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#include <type_traits>

#include "var/Vector.hpp"
#include "var/Tokenizer.hpp"
#include "var/Json.hpp"
#include "sys/Sys.hpp"
#include "sys/Printer.hpp"
#include "sys/requests.h"

#if defined __link
#include "xml2json.hpp"
#define atoff atof
#endif

sys::Printer& sys::operator << (sys::Printer& printer, const var::JsonValue & a){
	return print_value(printer, a, "");
}

sys::Printer& sys::operator << (Printer& printer, const var::JsonError & a){
	printer.key("text", a.text());
	printer.key("line", var::String::number(a.line()));
	printer.key("column", var::String::number(a.column()));
	printer.key("position", var::String::number(a.position()));
	printer.key("source", a.source());
	return printer;
}

sys::Printer& sys::print_value(Printer& printer, const var::JsonValue & a, const var::String& key){
	if(a.is_object()){
		var::StringList key_list = a.to_object().keys();
		if( !key.is_empty() ){
			printer.print_open_object(printer.verbose_level(), key.cstring());
		}
		for(const auto & subkey: key_list){
			const var::JsonValue& entry = a.to_object().at(subkey);
			print_value(printer, entry, subkey);
		}
		if( !key.is_empty() ){
			printer.print_close_object();
		}
	} else if( a.is_array() ){
		if( !key.is_empty() ){
			printer.print_open_array(printer.verbose_level(), key.cstring());
		}
		for(u32 i=0; i < a.to_array().count(); i++){
			const var::JsonValue& entry = a.to_array().at(i);
			var::String subkey;
			subkey.format("[%04d]", i);
			print_value(printer, entry, subkey);
		}
		if( !key.is_empty() ){
			printer.print_close_array();
		}
	} else {
		if( key.is_empty() ){
			printer.key(nullptr, a.to_string().cstring());
		} else {
			printer.key(key, a.to_string());
		}

	}
	return printer;
}

using namespace var;


JsonApi JsonValue::m_api;

int JsonValue::set_translated_error_number_if_error(int e) const {
	int error_code = 0;
	switch(e){
		case json_error_unknown: error_code = api::error_code_var_json_unknown; break;
		case json_error_out_of_memory: error_code = api::error_code_var_json_out_of_memory; break;
		case json_error_stack_overflow: error_code = api::error_code_var_json_stack_overflow; break;
		case json_error_cannot_open_file: error_code = api::error_code_var_json_cannot_open_file; break;
		case json_error_invalid_argument: error_code = api::error_code_var_json_invalid_argument; break;
		case json_error_invalid_utf8: error_code = api::error_code_var_json_invalid_utf8; break;
		case json_error_premature_end_of_input: error_code = api::error_code_var_json_premature_end_of_input; break;
		case json_error_end_of_input_expected: error_code = api::error_code_var_json_end_of_input_expected; break;
		case json_error_invalid_syntax: error_code = api::error_code_var_json_invalid_syntax; break;
		case json_error_invalid_format: error_code = api::error_code_var_json_invalid_format; break;
		case json_error_wrong_type: error_code = api::error_code_var_json_wrong_type; break;
		case json_error_null_character: error_code = api::error_code_var_json_null_character; break;
		case json_error_null_value: error_code = api::error_code_var_json_null_value; break;
		case json_error_null_byte_in_key: error_code = api::error_code_var_json_null_byte_in_key; break;
		case json_error_duplicate_key: error_code = api::error_code_var_json_duplicate_key; break;
		case json_error_numeric_overflow: error_code = api::error_code_var_json_numeric_overflow; break;
		case json_error_item_not_found: error_code = api::error_code_var_json_item_not_found; break;
		case json_error_index_out_of_range: error_code = api::error_code_var_json_index_out_of_range; break;
	}
	return set_error_number_if_error( error_code );
}


JsonValue::JsonValue(){
	if( api().is_valid() == false ){ exit_fatal("json api missing"); }
	m_value = 0; //create() method from children are not available in the constructor
}

JsonValue::JsonValue(json_t * value){
	if( api().is_valid() == false ){ exit_fatal("json api missing"); }
	add_reference(value);
}

JsonValue::JsonValue(const JsonValue & value){
	if( api().is_valid() == false ){ exit_fatal("json api missing"); }
	add_reference(value.m_value);
}

JsonValue & JsonValue::operator=(const JsonValue & value){
	api()->decref(m_value);
	add_reference(value.m_value);
	return *this;
}

void JsonValue::add_reference(json_t * value){
	m_value = value;
	api()->incref(value);
}

JsonValue::JsonValue(JsonValue && a){
	if( this != &a ){
		m_value = a.m_value;
		a.m_value = 0;
	}
}

JsonValue& JsonValue::operator=(JsonValue && a){
	if( this != &a ){
		m_value = a.m_value;
		a.m_value = 0;
	}
	return *this;
}

JsonValue::~JsonValue(){
	//only decref if object was create (not just a reference)
	api()->decref(m_value);
	m_value = 0;
}


const JsonObject & JsonValue::to_object() const {
	return (const JsonObject &)*this;
}

JsonObject & JsonValue::to_object(){
	return (JsonObject&)*this;
}

const JsonArray & JsonValue::to_array() const {
	return (const JsonArray &)*this;
}

JsonArray & JsonValue::to_array(){
	return (JsonArray &)*this;
}

int JsonValue::create_if_not_valid(){
	if( is_valid() ){ return 0; }
	m_value = create();
	if( m_value == 0 ){
		set_translated_error_number_if_error( json_error_out_of_memory );
		return -1;
	}
	return 0;
}

JsonValue& JsonValue::assign(const var::String & value){
	if( is_string() ){
		set_translated_error_number_if_error( api()->string_set(m_value, value.cstring()) );
	} else if( is_real() ){
		set_translated_error_number_if_error( api()->real_set(m_value, ::atoff(value.cstring())) );
	} else if( is_integer() ){
		set_translated_error_number_if_error( api()->integer_set(m_value, ::atoi(value.cstring())) );
	} else if ( is_true() || is_false() ){
		if( value == "true" ){
			*this = JsonTrue();
		} else {
			*this = JsonFalse();
		}
	}

	return *this;
}

JsonValue& JsonValue::assign(float value){
	return *this;
}

JsonValue& JsonValue::assign(int value){
	return *this;
}

JsonValue& JsonValue::assign(bool value){
	return *this;
}


JsonValue& JsonValue::copy(const JsonValue& value, IsDeep is_deep){
	api()->decref(m_value);
	if( is_deep.argument() ){
		m_value = api()->deep_copy(value.m_value);
	} else {
		m_value = api()->copy(value.m_value);
	}
	return *this;
}

var::String JsonValue::to_string() const {
	var::String result;
	if( is_string() ){
		result = api()->string_value(m_value);
	} else if( is_real() ){
		result.format("%f", api()->real_value(m_value));
	} else if (is_integer() ){
		result.format("%ld", api()->integer_value(m_value));
	} else if( is_true() ){
		result = "true";
	} else if( is_false() ){
		result = "false";
	} else if( is_null() ){
		result = "null";
	} else if( is_object() ){
		result = "{object}";
	} else if ( is_array() ){
		result = "[array]";
	} else {
		result = "";
	}
	return result;
}

float JsonValue::to_float() const {
	if( is_string() ){
		return to_string().to_float();
	} else if( is_integer() ){
		return to_integer() * 1.0f;
	}
	return api()->real_value(m_value);
}

int JsonValue::to_integer() const {
	if( is_string() ){
		return to_string().to_integer();
	} else if( is_real() ){
		return to_float();
	}

	if( is_true() ){ return 1; }
	if( is_false() ){ return 0; }
	if( is_null() ){ return 0; }

	return api()->integer_value(m_value);
}

bool JsonValue::to_bool() const {
	if( is_true() ){ return true; }
	if( is_false() ){ return false; }

	if( is_string() ){
		var::String s = to_string();
		s.to_upper();
		if( s == "TRUE" ){ return true; }
		return false;
	}

	if( is_integer() ){ return to_integer() != 0; }
	if( is_real() ){ return to_float() != 0.0f; }
	if( is_object() ){ return true; }
	if( is_array() ){ return true; }
	return false;
}

JsonObject::JsonObject(){
	m_value = create();
}

JsonObject::JsonObject(const JsonObject & value){
	add_reference(value.m_value);
}

JsonObject & JsonObject::operator=(const JsonObject & value){
	api()->decref(m_value);
	add_reference(value.m_value);
	return *this;
}

json_t * JsonObject::create(){
	return api()->create_object();
}

json_t * JsonArray::create(){
	return api()->create_array();
}

json_t * JsonReal::create(){
	return api()->create_real(0.0f);
}

json_t * JsonInteger::create(){
	return api()->create_integer(0);
}

json_t * JsonTrue::create(){
	return api()->create_true();
}

json_t * JsonFalse::create(){
	return api()->create_false();
}

json_t * JsonString::create(){
	return api()->create_string("");
}

json_t * JsonNull::create(){
	return api()->create_null();
}

JsonObject& JsonObject::insert(
		const var::String & key,
		bool value
		){
	if( value ){
		return insert(key, JsonTrue());
	}

	return insert(key, JsonFalse());
}

JsonObject& JsonObject::insert(
		const var::String & key,
		const JsonValue & value
		){
	if( create_if_not_valid() < 0 ){
		return *this;
	}

	set_translated_error_number_if_error(
				api()->object_set(
					m_value,
					key.cstring(),
					value.m_value
					)
				);
	return *this;
}

JsonObject& JsonObject::update(
		const JsonValue & value,
		enum updates o_flags
		){
	if( o_flags & update_existing ){
		set_translated_error_number_if_error(
					api()->object_update_existing(
						m_value,
						value.m_value
						)
					);
		return *this;
	}

	if( o_flags & update_missing ){
		set_translated_error_number_if_error(
					api()->object_update_missing(
						m_value,
						value.m_value
						)
					);
		return *this;
	}

	set_translated_error_number_if_error(
				api()->object_update(
					m_value,
					value.m_value
					)
				);
	return *this;
}

JsonObject& JsonObject::remove(const var::String & key){
	set_translated_error_number_if_error( api()->object_del(m_value, key.cstring()) );
	return *this;
}

u32 JsonObject::count() const {
	return api()->object_size(m_value);
}

JsonObject& JsonObject::clear(){
	set_translated_error_number_if_error( api()->object_clear(m_value) );
	return *this;
}

var::StringList JsonObject::key_list() const {
	const char *key;
	json_t *value;
	var::StringList result;

	for(key = api()->object_iter_key(api()->object_iter(m_value));
			key && (value = api()->object_iter_value(api()->object_key_to_iter(key)));
			key = api()->object_iter_key(api()->object_iter_next(m_value, api()->object_key_to_iter(key)))){
		result.push_back(var::String(key));
	}

	return result;
}

JsonValue JsonObject::at(const var::String & key) const {
	return api()->object_get(m_value, key.cstring());
}

JsonArray::JsonArray(){
	m_value = create();
}

JsonArray::JsonArray(const JsonArray & value){
	add_reference(value.m_value);
}

JsonArray & JsonArray::operator=(const JsonArray & value){
	api()->decref(m_value);
	add_reference(value.m_value);
	return *this;
}

u32 JsonArray::count() const {
	return api()->array_size(m_value);
}

JsonValue JsonArray::at(size_t position) const {
	return api()->array_get(
				m_value,
				position
				);
}

JsonArray::JsonArray(const var::Vector<var::String>& list){
	m_value = create();
	for(const auto & entry: list){
		append(JsonString(entry));
	}
}

JsonArray::JsonArray(const var::Vector<float>& list){
	m_value = create();
	for(const auto & entry: list){
		append(JsonReal(entry));
	}
}

JsonArray::JsonArray(const var::Vector<u32>& list){
	m_value = create();
	for(const auto & entry: list){
		append(JsonInteger(entry));
	}
}

JsonArray::JsonArray(const var::Vector<s32>& list){
	m_value = create();
	for(const auto & entry: list){
		append(JsonInteger(entry));
	}
}

JsonArray& JsonArray::append(const JsonValue & value){
	if( create_if_not_valid() < 0 ){ return *this; }
	set_translated_error_number_if_error(
				api()->array_append(
					m_value,
					value.m_value
					)
				);
	return *this;
}

JsonArray& JsonArray::append(const JsonArray & array){
	if( create_if_not_valid() < 0 ){ return *this; }
	set_translated_error_number_if_error(
				api()->array_extend(m_value, array.m_value)
				);
	return *this;
}

JsonArray& JsonArray::insert(
		size_t position,
		const JsonValue & value
		){
	if( create_if_not_valid() < 0 ){ return *this; }
	set_translated_error_number_if_error(
				api()->array_insert(
					m_value,
					position,
					value.m_value
					)
				);
	return *this;
}

JsonArray& JsonArray::remove(
		size_t position
		){
	set_translated_error_number_if_error(
				api()->array_remove(
					m_value,
					position
					)
				);
	return *this;
}

JsonArray& JsonArray::clear(){
	set_translated_error_number_if_error(
				api()->array_clear(m_value)
				);
	return *this;
}

var::Vector<var::String> JsonArray::string_list(){
	var::Vector<var::String> result;
	for(u32 i=0; i < count(); i++){
		result.push_back( at(i).to_string() );
	}
	return result;
}

var::Vector<s32> JsonArray::integer_list(){
	var::Vector<s32> result;
	for(u32 i=0; i < count(); i++){
		result.push_back( at(i).to_integer() );
	}
	return result;
}

var::Vector<float> JsonArray::float_list(){
	var::Vector<float> result;
	for(u32 i=0; i < count(); i++){
		result.push_back( at(i).to_float() );
	}
	return result;
}

var::Vector<bool> JsonArray::bool_list(){
	var::Vector<bool> result;
	for(u32 i=0; i < count(); i++){
		result.push_back( at(i).to_bool() );
	}
	return result;
}

JsonString::JsonString(){
	m_value = create();
}

JsonString::JsonString(const var::String & str){
	m_value = api()->create_string(str.cstring());
}

JsonReal::JsonReal(){
	m_value = create();
}

JsonReal::JsonReal(float value){
	m_value = api()->create_real(value);
}

JsonReal& JsonReal::operator=(float a){
	if( create_if_not_valid() == 0 ){
		api()->real_set(m_value, a);
	}
	return *this;
}

JsonInteger::JsonInteger(){
	m_value = create();
}

JsonInteger::JsonInteger(int value){
	m_value = api()->create_integer(value);
}

JsonInteger& JsonInteger::operator=(int a){
	if( create_if_not_valid() == 0 ){
		api()->integer_set(m_value, a);
	}
	return *this;
}

JsonNull::JsonNull(){
	m_value = create();
}

JsonTrue::JsonTrue(){
	m_value = create();
}

JsonFalse::JsonFalse(){
	m_value = create();
}

JsonValue JsonDocument::load(
		FilePath path
		){
	JsonValue value;
	value.m_value = JsonValue::api()->load_file(
				path.argument().cstring(),
				flags(),
				&m_error.m_value
				);
	return value;
}

#if defined __link
JsonValue JsonDocument::load(
		XmlString xml
		){
	std::string json_string
			= xml2json(
				xml.argument().cstring()
				);
	return load(String(json_string.c_str()));
}

JsonValue JsonDocument::load(
		XmlFilePath path
		){
	fs::DataFile data_file(
				fs::File::Path(path.argument())
				);
	String xml_string = String( data_file.data() );
	return load(XmlString(xml_string));
}
#endif

JsonValue JsonDocument::load(
		const var::String & json
		){
	JsonValue value;
	value.m_value = JsonValue::api()->loadb(
				json.cstring(),
				json.length(),
				flags(),
				&m_error.m_value);
	return value;
}

size_t JsonDocument::load_file_data(
		void *buffer,
		size_t buflen,
		void *data
		){
	const fs::File * f = (const fs::File *)data;
	return f->read(
				buffer,
				fs::File::Size(buflen)
				);
}

JsonValue JsonDocument::load(
		fs::File& file
		){
	JsonValue value;
	value.m_value = JsonValue::api()->load_callback(
				load_file_data,
				static_cast<void*>(&file),
				flags(),
				&m_error.m_value
				);
	return value;
}

JsonValue JsonDocument::load(
		json_load_callback_t callback,
		void * context
		){
	JsonValue value;
	value.m_value = JsonValue::api()->load_callback(callback, context, flags(), &m_error.m_value);
	return value;
}

int JsonDocument::save(
		const JsonValue & value,
		fs::File::Path path
		) const {
	fs::File f;
	int result;

#if defined __win32
	result = JsonValue::api()->dump_file(
				value.m_value,
				path.argument().cstring(),
				flags()
				);
#else
	if( f.create(
				path.argument(),
				fs::File::IsOverwrite(true)
				) < 0 ){
		set_error_number(f.error_number());
		return -1;
	}

	result = JsonValue::api()->dumpfd(
				value.m_value,
				f.fileno(),
				flags());


	if( f.close() < 0 ){
		set_error_number(f.error_number());
		return -1;
	}
#endif

	return result;
}

var::String JsonDocument::to_string(
		const var::JsonValue& value
		) const {
	u32 size = JsonValue::api()->dumpb(
				value.m_value,
				0,
				0,
				flags()
				);
	if( size == 0 ){
		return var::String();
	}
	var::String result;
	result.resize(size);

	if( JsonValue::api()->dumpb(
				value.m_value,
				result.to_char(),
				result.capacity(),
				flags()
				) == 0 ){
		return var::String();
	}
	return result;
}

int JsonDocument::save(
		const JsonValue & value,
		fs::File & file
		) const {
	return JsonValue::api()->dumpfd(
				value.m_value,
				file.fileno(),
				flags());
}

int JsonDocument::save(
		const JsonValue & value,
		json_dump_callback_t callback,
		void * context
		) const {
	return JsonValue::api()->dump_callback(value.m_value, callback, context, flags());
}
