#include "var/Vector.hpp"
#include "fmt/Json.hpp"

using namespace fmt;

JsonValue::JsonValue(json_t * value){
	add_reference(value);
}

JsonValue::JsonValue(const JsonValue & value){
	add_reference(value.m_value);
}

JsonValue & JsonValue::operator=(const JsonValue & value){
	json_decref(m_value);
	add_reference(value.m_value);
	return *this;
}

void JsonValue::add_reference(json_t * value){
	m_value = value;
	json_incref(value);
}

JsonValue::JsonValue(JsonValue && a){
	m_value = a.m_value;
	a.m_value = 0;
}

JsonValue& JsonValue::operator=(JsonValue && a){
	m_value = a.m_value;
	a.m_value = 0;
	return *this;
}

JsonValue::~JsonValue(){
	//only decref if object was create (not just a reference)
	json_decref(m_value);
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
		return json_string_set(m_value, value.str());
	} else if( is_real() ){
		return json_real_set(m_value, ::atof(value.str()));
	} else if( is_integer() ){
		return json_integer_set(m_value, ::atoi(value.str()));
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


var::String JsonValue::to_string() const {
	var::String result;
	if( is_string() ){
		result = json_string_value(m_value);
	} else if( is_real() ){
		result.format("%f", json_real_value(m_value));
	} else if (is_integer() ){
		result.format("%ld", json_integer_value(m_value));
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

float JsonValue::to_real() const {
	if( is_string() ){
		return ::atof(json_string_value(m_value));
	}
	return json_real_value(m_value);
}

int JsonValue::to_integer() const {
	if( is_string() ){
		return ::atol(json_string_value(m_value));
	}
	return json_integer_value(m_value);
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
	if( is_real() ){ return to_real() != 0.0f; }
	if( is_object() ){ return true; }
	if( is_array() ){ return true; }
	return false;
}

JsonObject::JsonObject(){}

JsonObject::JsonObject(const JsonObject & value){
	add_reference(value.m_value);
}

JsonObject & JsonObject::operator=(const JsonObject & value){
	json_decref(m_value);
	add_reference(value.m_value);
	return *this;
}

json_t * JsonObject::create(){
	return json_object();
}

json_t * JsonArray::create(){
	return json_array();
}

json_t * JsonReal::create(){
	return json_real(0.0f);
}

json_t * JsonInteger::create(){
	return json_integer(0);
}

json_t * JsonTrue::create(){
	return json_true();
}

json_t * JsonFalse::create(){
	return json_false();
}

json_t * JsonString::create(){
	return json_string("");
}

json_t * Json::create(){
	return 0;
}

int JsonObject::insert(const var::ConstString & key, const JsonValue & value){
	if( create_if_not_valid() < 0 ){
		printf("Could not create -- still invalid\n");
		return -1;
	}
	int result = json_object_set(m_value, key.str(), value.m_value);
	if( result < 0 ){
		printf("Failed to set JSON key %s to %s %p\n", key.str(), value.to_string().str(), m_value);
	}
	return result;
}

int JsonObject::update(const JsonValue & value, u8 o_flags){
	if( o_flags & UPDATE_EXISTING ){
		return json_object_update_existing(m_value, value.m_value);
	}

	if( o_flags & UPDATE_MISSING ){
		return json_object_update_missing(m_value, value.m_value);
	}

	return json_object_update(m_value, value.m_value);
}

int JsonObject::remove(const var::ConstString & key){
	return json_object_del(m_value, key.str());
}

u32 JsonObject::count() const {
	return json_object_size(m_value);
}

int JsonObject::clear(){ return json_object_clear(m_value); }

var::Vector<var::String> JsonObject::keys() const {
	const char *key;
	json_t *value;
	var::Vector<var::String> result;

	json_object_foreach(m_value, key, value) {
		result.push_back(var::String(key));
	}

	return result;
}

JsonValue JsonObject::at(const var::ConstString & key) const {
	return json_object_get(m_value, key.str());
}

JsonArray::JsonArray(){
	m_value = json_array();
}

JsonArray::JsonArray(const JsonArray & value){
	add_reference(value.m_value);
}

JsonArray & JsonArray::operator=(const JsonArray & value){
	json_decref(m_value);
	add_reference(value.m_value);
	return *this;
}

u32 JsonArray::count() const {
	return json_array_size(m_value);
}

JsonValue JsonArray::at(u32 idx) const {
	return json_array_get(m_value, idx);
}

int JsonArray::append(const JsonValue & value){
	if( create_if_not_valid() < 0 ){ return -1; }
	return json_array_append(m_value, value.m_value);
}

int JsonArray::append(const JsonArray & array){
	if( create_if_not_valid() < 0 ){ return -1; }
	return json_array_extend(m_value, array.m_value);
}

int JsonArray::insert(u32 idx, const JsonValue & value){
	if( create_if_not_valid() < 0 ){ return -1; }
	return json_array_insert(m_value, idx, value.m_value);
}

int JsonArray::remove(u32 idx){
	return json_array_remove(m_value, idx);
}

int JsonArray::clear(){
	return json_array_clear(m_value);
}

JsonString::JsonString(const var::ConstString & str){
	m_value = json_string(str.str());
}

JsonReal::JsonReal(float value){
	m_value = json_real(value);
}

JsonReal & JsonReal::operator=(float a){
	if( create_if_not_valid() == 0 ){
		json_real_set(m_value, a);
	}
	return *this;
}

JsonInteger::JsonInteger(int value){
	m_value = json_integer(value);
}

JsonInteger & JsonInteger::operator=(int a){
	if( create_if_not_valid() == 0 ){
		json_integer_set(m_value, a);
	}
	return *this;
}

Json::Json(){
	m_flags = 0;
}

int Json::load_from_file(const var::ConstString & path){
	json_decref(m_value);
	m_value = json_load_file(path.str(), flags(), &m_error.m_value);
	if( m_value ){
		return 0;
	}
	return -1;
}

int Json::load_from_string(const var::ConstString & json){
	json_decref(m_value);
	m_value = json_loadb(json.str(), json.length(), flags(), &m_error.m_value);
	if( m_value ){
		return 0;
	}
	return -1;
}

//only use on Stratify OS
int Json::load_from_file(const sys::File & file){
	json_decref(m_value);
	m_value = json_loadfd(file.fileno(), flags(), &m_error.m_value);
	return m_value == 0 ? -1 : 0;
}

int Json::load(json_load_callback_t callback, void * context){
	json_decref(m_value);
	m_value = json_load_callback(callback, context, flags(), &m_error.m_value);
	if( m_value ){
		return 0;
	}
	return -1;
}

int Json::save_to_file(const var::ConstString & path) const {
	return json_dump_file(m_value, path.str(), flags());
}

var::String Json::stringify() const {
	u32 size = json_dumpb(m_value, 0, 0, flags());
	if( size == 0 ){
		return var::String();
	}
	var::String result;
	if( result.set_capacity(size) < 0 ){
		return var::String();
	}
	if( json_dumpb(m_value, result.to<char>(), result.capacity(), flags()) == 0 ){
		return var::String();
	}
	return result;
}

int Json::save_to_file(const sys::File & file) const {
	return json_dumpfd(m_value, file.fileno(), flags());
}

int Json::save(json_dump_callback_t callback, void * context) const {
	return json_dump_callback(m_value, callback, context, flags());
}

