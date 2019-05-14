#include "var/Vector.hpp"
#include "var/Tokenizer.hpp"
#include "var/Json.hpp"
#include "sys/Sys.hpp"
#include "sys/requests.h"

#if defined __link
#define atoff atof
#endif


using namespace var;

JsonApi JsonValue::m_api;

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
	if( m_value == 0 ){ return -1; }
	return 0;
}

int JsonValue::assign(const var::ConstString & value){
	if( is_string() ){
		return api()->string_set(m_value, value.cstring());
	} else if( is_real() ){
		return api()->real_set(m_value, ::atoff(value.cstring()));
	} else if( is_integer() ){
		return api()->integer_set(m_value, ::atoi(value.cstring()));
	} else if ( is_true() ){
		if( value == "false" ){

		}
	}

	//can't assign string to object, array, bool, or null

	return -1;
}

int JsonValue::assign(float value){
	return 0;
}

int JsonValue::assign(int value){
	return 0;
}

int JsonValue::assign(bool value){
	return 0;
}


int JsonValue::copy(const JsonValue & value, bool is_deep){
	api()->decref(m_value);
	if( is_deep ){
		m_value = api()->deep_copy(value.m_value);
	} else {
		m_value = api()->copy(value.m_value);
	}
	return 0;
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
		result = "<invalid>";
	}
	return result;
}

float JsonValue::to_float() const {
	if( is_string() ){
		return to_string().to_float();
	}
	return api()->real_value(m_value);
}

int JsonValue::to_integer() const {
	if( is_string() ){
		return to_string().to_integer();
	}
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


int JsonObject::insert(const var::ConstString & key, const JsonValue & value){
	if( create_if_not_valid() < 0 ){
		return -1;
	}

	int result = api()->object_set(m_value, key.cstring(), value.m_value);
	if( result < 0 ){
		//printf("Failed to set JSON key %s to %s %p\n", key.cstring(), value.to_string().cstring(), m_value);
	}
	return result;
}

int JsonObject::update(const JsonValue & value, u8 o_flags){
	if( o_flags & UPDATE_EXISTING ){
		return api()->object_update_existing(m_value, value.m_value);
	}

	if( o_flags & UPDATE_MISSING ){
		return api()->object_update_missing(m_value, value.m_value);
	}

	return api()->object_update(m_value, value.m_value);
}

int JsonObject::remove(const var::ConstString & key){
	return api()->object_del(m_value, key.cstring());
}

u32 JsonObject::count() const {
	return api()->object_size(m_value);
}

int JsonObject::clear(){ return json_object_clear(m_value); }

var::Vector<var::String> JsonObject::keys() const {
	const char *key;
	json_t *value;
	var::Vector<var::String> result;

	for(key = api()->object_iter_key(api()->object_iter(m_value));
		 key && (value = api()->object_iter_value(api()->object_key_to_iter(key)));
		 key = api()->object_iter_key(api()->object_iter_next(m_value, api()->object_key_to_iter(key)))){
		result.push_back(var::String(key));
	}

	return result;
}

JsonValue JsonObject::at(const var::ConstString & key) const {
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

JsonValue JsonArray::at(u32 idx) const {
	return api()->array_get(m_value, idx);
}

int JsonArray::append(const JsonValue & value){
	if( create_if_not_valid() < 0 ){ return -1; }
	return api()->array_append(m_value, value.m_value);
}

int JsonArray::append(const JsonArray & array){
	if( create_if_not_valid() < 0 ){ return -1; }
	return api()->array_extend(m_value, array.m_value);
}

int JsonArray::insert(u32 idx, const JsonValue & value){
	if( create_if_not_valid() < 0 ){ return -1; }
	return api()->array_insert(m_value, idx, value.m_value);
}

int JsonArray::remove(u32 idx){
	return api()->array_remove(m_value, idx);
}

int JsonArray::clear(){
	return api()->array_clear(m_value);
}

JsonString::JsonString(){
	m_value = create();
}


JsonString::JsonString(const var::ConstString & str){
	m_value = api()->create_string(str.cstring());
}

JsonReal::JsonReal(){
	m_value = create();
}

JsonReal::JsonReal(float value){
	m_value = api()->create_real(value);
}

JsonReal & JsonReal::operator=(float a){
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

JsonInteger & JsonInteger::operator=(int a){
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

JsonValue JsonDocument::load_from_file(const var::ConstString & path){
	JsonValue value;
	value.m_value = JsonValue::api()->load_file(path.cstring(), flags(), &m_error.m_value);
	return value;
}

JsonValue JsonDocument::load_from_string(const var::ConstString & json){
	JsonValue value;
	value.m_value = JsonValue::api()->loadb(json.cstring(), json.length(), flags(), &m_error.m_value);
	return value;
}

//only use on Stratify OS
JsonValue JsonDocument::load_from_file(const sys::File & file){
	JsonValue value;
	value.m_value = JsonValue::api()->loadfd(file.fileno(), flags(), &m_error.m_value);
	return value;
}

size_t JsonDocument::load_file_data(void *buffer, size_t buflen, void *data){
	const sys::File * f = (const sys::File *)data;
	return f->read(buffer, buflen);
}

JsonValue JsonDocument::load(const sys::File & file){
	JsonValue value;
	value.m_value = JsonValue::api()->load_callback(load_file_data, (void*)&file, flags(), &m_error.m_value);
	return value;
}

JsonValue JsonDocument::load(json_load_callback_t callback, void * context){
	JsonValue value;
	value.m_value = JsonValue::api()->load_callback(callback, context, flags(), &m_error.m_value);
	return value;
}

int JsonDocument::save_to_file(const JsonValue & value, const var::ConstString & path) const {
	sys::File f;
	int result;
	if( f.create(path) < 0 ){
		set_error_number(f.error_number());
		return -1;
	}
	result = JsonValue::api()->dumpfd(value.m_value, f.fileno(), flags());

	if( f.close() < 0 ){
		set_error_number(f.error_number());
		return -1;
	}

	return result;
}

var::String JsonDocument::stringify(const JsonValue & value) const {
	u32 size = JsonValue::api()->dumpb(value.m_value, 0, 0, flags());
	if( size == 0 ){
		return var::String();
	}
	var::String result;
	if( result.set_capacity(size) < 0 ){
		return var::String();
	}
	if( JsonValue::api()->dumpb(value.m_value, result.to<char>(), result.capacity(), flags()) == 0 ){
		return var::String();
	}
	return result;
}

int JsonDocument::save_to_file(const JsonValue & value, const sys::File & file) const {
	return JsonValue::api()->dumpfd(value.m_value, file.fileno(), flags());
}

int JsonDocument::save(const JsonValue & value, json_dump_callback_t callback, void * context) const {
	return JsonValue::api()->dump_callback(value.m_value, callback, context, flags());
}




