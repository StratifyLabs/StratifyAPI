#include "var/Vector.hpp"
#include "fmt/Json.hpp"

using namespace fmt;

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
        result = "invalid";
    }
    result.set_transfer_ownership();
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
    if( is_true() ){
        return true;
    }
    return false;
}

JsonObject::JsonObject(){
    m_value = json_object();
}

int JsonObject::insert(const var::ConstString & key, const JsonValue & value){
    return json_object_set(m_value, key.str(), value.m_value);
}

int JsonObject::update(const JsonValue & value){
    return json_object_update(m_value, value.m_value);
}

int JsonObject::update_existing(const JsonValue & value){
    return json_object_update_existing(m_value, value.m_value);
}

int JsonObject::update_missing(const JsonValue & value){
    return json_object_update_missing(m_value, value.m_value);
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
        var::String key_string(key);
        key_string.set_transfer_ownership();
        result.push_back(key_string);
    }

    return result;

}

JsonValue JsonObject::at(const var::ConstString & key) const {
    return json_object_get(m_value, key.str());
}

JsonArray::JsonArray(){
    m_value = json_array();
}

u32 JsonArray::count() const {
    return json_array_size(m_value);
}

JsonValue JsonArray::at(u32 idx) const {
    return json_array_get(m_value, idx);
}

int JsonArray::append(const JsonValue & value){
    return json_array_append(m_value, value.m_value);
}

int JsonArray::append(const JsonArray & array){
    return json_array_extend(m_value, array.m_value);
}

int JsonArray::insert(u32 idx, const JsonValue & value){
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
    json_real_set(m_value, a);
    return *this;
}

JsonInteger::JsonInteger(int value){
    m_value = json_integer(value);
}

JsonInteger & JsonInteger::operator=(int a){
    json_integer_set(m_value, a);
    return *this;
}

JsonNull::JsonNull(){
    m_value = json_null();
}

JsonBool::JsonBool(bool value){
    if( value ){
        m_value = json_true();
    } else {
        m_value = json_false();
    }
}

Json::Json(){
    m_flags = 0;
}

int Json::load(const var::ConstString & path){
    m_value = json_load_file(path.str(), flags(), &m_error.m_value);
    return m_value == 0 ? -1 : 0;
}

int Json::load(const var::Data & data){
    m_value = json_loadb(data.cdata_const(), data.size(), flags(), &m_error.m_value);
    return m_value == 0 ? -1 : 0;
}

//only use on Stratify OS
int Json::load(const sys::File & file){
    m_value = json_loadfd(file.fileno(), flags(), &m_error.m_value);
    return m_value == 0 ? -1 : 0;
}

int Json::load(json_load_callback_t callback, void * context){
    m_value = json_load_callback(callback, context, flags(), &m_error.m_value);
    return m_value == 0 ? -1 : 0;
}

