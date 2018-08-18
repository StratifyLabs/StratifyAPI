#ifndef JSON_HPP
#define JSON_HPP

#include <jansson/jansson.h>
#include "../api/FmtObject.hpp"
#include "../var/ConstString.hpp"

namespace fmt {

class JsonValue : public api::FmtInfoObject {
public:
    JsonValue(){ m_value = 0; }

    enum type {
        OBJECT = JSON_OBJECT,
        ARRAY = JSON_ARRAY,
        STRING = JSON_STRING,
        REAL = JSON_REAL,
        INTEGER = JSON_INTEGER,
        TRUE = JSON_TRUE,
        FALSE = JSON_FALSE,
        ZERO = JSON_NULL
    };

    enum type type() const { return (enum type)json_type(m_value); }

    bool is_object() const { return type() == OBJECT; }
    bool is_array() const { return type() == ARRAY; }
    bool is_string() const { return type() == STRING; }
    bool is_real() const { return type() == REAL; }
    bool is_integer() const { return type() == INTEGER; }
    bool is_true() const { return type() == TRUE; }
    bool is_false() const { return type() == FALSE; }
    bool is_null() const { return type() == ZERO; }
    bool is_zero() const { return is_null(); }

protected:
    json_t * m_value;
};

class JsonObject : public JsonValue {
public:
    JsonObject(){
        m_value = json_object();
    }

    int insert(const var::ConstString & key, const JsonValue & value){
        return json_object_set(m_value, key.str(), value.m_value);
    }

    int remove(const var::ConstString & key){
        return json_object_del(m_value, key.str());
    }

    u32 count() const {
        return json_object_size(m_value);
    }

};

class JsonArray : public JsonValue {
public:
    JsonArray(){
        m_value = json_array();
    }

    u32 count() const { return json_array_size(m_value); }
};

class JsonString : public JsonValue {
public:
    JsonString(const var::ConstString & str){
        m_value = json_string(str.str());
    }
};

class JsonReal : public JsonValue {
public:
    JsonReal(float value){
        m_value = json_real(value);
    }

    JsonReal & operator=(float a){
        json_real_set(&m_value, a);
    }

};

class JsonInteger : public JsonValue {
public:
    JsonInteger(){}
};

class JsonNull : public JsonValue {
public:
    JsonNull(){}
};

class JsonBool : public JsonValue {
public:
    JsonBool(){}
};

class JsonTrue : public JsonBool {
public:
    JsonTrue(){}
};

class JsonFalse : public JsonBool {
public:
    JsonFalse(){}
};

class Json : public api::FmtWorkObject {
public:
    Json();
};

}

#endif // JSON_HPP
