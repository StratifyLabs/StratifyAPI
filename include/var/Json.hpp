/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef SAPI_VAR_JSON_HPP_
#define SAPI_VAR_JSON_HPP_

#include <jansson/jansson_api.h>
#include "../api/VarObject.hpp"
#include "../var/Vector.hpp"
#include "../var/String.hpp"
#include "../fs/File.hpp"
#include "../sys/requests.h"

namespace var {

#undef TRUE
#undef FALSE

class Container;
class JsonDocument;

class JsonError {
public:
	JsonError(){
		memset(&m_value, 0, sizeof(m_value));
	}
	int line() const { return m_value.line; }
	int column() const { return m_value.column; }
	int position() const { return m_value.position; }
	var::String source() const { return m_value.source; }
	var::String text() const { return m_value.text; }

private:
	friend class JsonDocument;
	json_error_t m_value;
};

class JsonObject;
class JsonArray;
class JsonReal;
class JsonNull;
class JsonInteger;
class JsonString;

typedef api::Api<jansson_api_t, JANSSON_API_REQUEST> JsonApi;

class JsonValue : public api::WorkObject {
public:

	using IsDeep = arg::Argument<bool, struct JsonValueIsDeepTag>;

	JsonValue();

	JsonValue(json_t * value);
	JsonValue(const JsonValue & value);
	JsonValue(fs::File::Path path);

	JsonValue& operator=(const JsonValue & value);
	~JsonValue();

	JsonValue(JsonValue && a);
	JsonValue& operator=(JsonValue && a);

	operator const JsonObject& () const {
		return to_object();
	}

	operator JsonObject&()	{
		return to_object();
	}

	operator const JsonArray& () const {
		return to_array();
	}

	operator JsonArray&(){
		return to_array();
	}

	/*! \details Returns true if the value is valid.
		*
		* \code
		* JsonObject object;
		* object.is_valid(); //true - a valid (though empty) object
		* object.insert("foo", "bar");
		* object.at("foo").is_valid(); //true
		* object.at("bar").is_valid(); //false
		*
		* JsonValue value;
		* value.is_valid(); //false
		*
		* JsonValue object_observer(object);
		*
		* object_observer.is_valid(); //true
		*
		* \endcode
		*
		*/
	bool is_valid() const{ return m_value != nullptr; }

	enum types {
		type_invalid = -1,
		type_object = JSON_OBJECT,
		type_array = JSON_ARRAY,
		type_string = JSON_STRING,
		type_real = JSON_REAL,
		type_integer = JSON_INTEGER,
		type_true = JSON_TRUE,
		type_false = JSON_FALSE,
		type_null = JSON_NULL,
		type_zero = JSON_NULL
	};

	enum types type() const {
		if( m_value ){
			return (enum types)json_typeof(m_value);
		}
		return type_invalid;
	}

	bool is_object() const { return type() == type_object; }
	bool is_array() const { return type() == type_array; }
	bool is_string() const { return type() == type_string; }
	bool is_real() const { return type() == type_real; }
	bool is_integer() const { return type() == type_integer; }
	bool is_true() const { return type() == type_true; }
	bool is_false() const { return type() == type_false; }
	bool is_null() const { return type() == type_zero; }
	bool is_zero() const { return is_null(); }

	JsonValue & to_value(){ return *this; }
	const JsonValue & to_value() const { return *this; }
	const JsonObject & to_object() const;
	JsonObject & to_object();
	const JsonArray & to_array() const;
	JsonArray & to_array();

	/*! \details Converts the JSON value to string.
	 *
	 * @return A string representing the value
	 *
	 * The following types are converting to a string
	 * - STRING: as a string
	 * - INTEGER: as a formatted string
	 * - FLOAT: as a formatted string
	 * - TRUE: "true"
	 * - FALSE: "false"
	 * - NULL: "null"
	 * - OBJECT: "{object}"
	 * - ARRAY: "[array]"
	 * - INVALID: ""
	 *
	 * To convert a json object or array to a
	 * proper string using Json::stringify().
	 *
	 * \code
	 * JsonValue json_value; //can be any JSON type
	 * Json json = json_value;
	 * var::String as_string = json.stringify();
	 * \endcode
	 *
	 */
	var::String to_string() const;

	/*! \details Returns a float value of the JSON value.
	 *
	 * If the JSON value is of type float, integer, or string,
	 * it will be converted to a real number.
	 *
	 * Otherwise, 0.0f is returned.
	 *
	 */
	float to_float() const;

	/*! \details Returns an integer value of the JSON value.
	 *
	 * If the JSON value is of type float, integer, or string,
	 * it will be converted to an integer.
	 *
	 * Otherwise, 0 is returned.
	 *
	 */
	int to_integer() const;

	/*! \details Returns a bool value of the JSON value.
	 *
	 * If the value is a string, it will return true
	 * if the string is "true" (not case sensitive).
	 *
	 * - INTEGER: true if non-zero
	 * - FLOAT: true if non-zero
	 * - TRUE: true
	 * - FALSE: false
	 * - NULL: false
	 * - OBJECT: true
	 * - ARRAY: true
	 * - INVALID: false
	 *
	 */
	bool to_bool() const;

	//how to handle object creation -- explicitly or implicity so memory isn't leaked
	/*
		* For object creation, create objects on demand. When
		*
		* JsonObject object; //not valid yet
		*
		* object.insert("foo", "bar"); // object is created upon insert
		*
		* object.load("/path/to/json"); //object is now valid
		*
		* object.load("/path/to/other/json"); //previous object is freed before new one loaded
		*
		* //here other_object is not created but is just observing - if it was previously created - it is decremented
		* JsonObject other_object = object.at("object").to_object();
		*
		*
		*/

	/*! \details Assigns a string to the current JSON value.
		*
		* @param value The string to assign
		*
		* This method will not change the underlying JSON type. If a
		* string value is assigned to a real type, the string
		* will be converted to a real. Consider the following
		* code snippet.
		*
		* \code
		* JsonReal real_value(100.0f);
		* real_value.assign("200.0"); //"200.0" is converted to 200.0f
		* real_value.assign("Hello World"); //"Hello World" is converted to 0.0f
		*
		* JsonInteger integer_value(1);
		*
		* bool_value.assign("2"); //assigns 2
		* bool_value.assign("3"); /assign 3
		* bool_value.assign("Hello World"); //assigns 0
		* \endcode
		*
		*
		*/
	JsonValue& assign(const var::String & value);


	JsonValue& assign(float value);
	JsonValue& assign(int value);
	JsonValue& assign(bool value);

	JsonValue& copy(
			const JsonValue & value,
			IsDeep is_deep = IsDeep(true)
			);


	static JsonApi & api(){ return m_api; }

protected:
	int create_if_not_valid();
	virtual json_t * create(){
		return 0;
	}

	int set_translated_error_number_if_error(int e) const;

private:
	friend class JsonDocument;
	friend class JsonObject;
	friend class JsonArray;
	friend class JsonTrue;
	friend class JsonFalse;
	friend class JsonReal;
	friend class JsonInteger;
	friend class JsonString;
	friend class JsonNull;
	friend class JsonKeyValue;
	json_t * m_value;

	void add_reference(json_t * value);

	static JsonApi m_api;


};

class JsonKeyValue : public JsonValue {
public:
	JsonKeyValue(const var::String& key, const JsonValue& value) : JsonValue(value) {
		m_key = key;
	}

	JsonKeyValue& set_value(const JsonValue& a){
		add_reference(a.m_value);
		return *this;
	}
	const JsonValue& value() const { return *this; }
	JsonValue get_value() const { return *this; }


private:
	API_READ_ACCESS_COMPOUND(JsonKeyValue,var::String,key);
};

template<class Derived> class JsonKeyValueList : public var::Vector<Derived> {
public:
	Derived at(const var::String& key){
		for(const auto & item: *this){
			if( item.key() == key ){
				return item;
			}
		}
		return Derived("", JsonValue());
	}
};

class JsonObject : public JsonValue {
public:

	JsonObject();

	JsonObject(const JsonObject & value);
	JsonObject & operator=(const JsonObject & value);

	template<class T> JsonKeyValueList<T> construct_key_list(){
		var::StringList list = key_list();
		JsonKeyValueList<T> result;
		for(const auto& key: list){
			result.push_back(T(key, at(key)));
		}
		return result;
	}

	template<class T> JsonKeyValueList<T> construct_key_list_copy(){
		var::StringList list = key_list();
		JsonKeyValueList<T> result;
		for(const auto& key: list){
			result.push_back(T(key, JsonValue().copy(at(key))));
		}
		return result;
	}

	template<class T> explicit JsonObject(const JsonKeyValueList<T>& list){
		m_value = create();
		for(const auto & item: list){
			insert(item.key(), item.value());
		}
	}

	/*! \details Returns true if the object is empty. */
	bool is_empty() const { return count() == 0; }

	/*!
		* \details Inserts the key value pair into the object.
		*
		* \param key The UTF-8 key
		* \param value The value of the key
		* \return Zero on success
		*
		* If \a key already exists in the object, it is
		* updated to the new value. If \a key does not
		* exist in the object, it is created.
		*
		*/
	JsonObject& insert(
			const var::String & key,
			const JsonValue & value
			);

	JsonObject& insert(const var::JsonKeyValue& key_value){
		return insert(key_value.key(), key_value.value());
	}

	JsonObject& insert(
			const var::String & key,
			bool value
			);

	enum updates {
		update_none = 0x00,
		update_existing = 0x01,
		update_missing = 0x02,
		update_missing_and_existing = 0x03
	};

	JsonObject& update(
			const JsonValue & value,
			enum updates o_flags = update_none
			);

	/*!
		* \details Removes the specified key from the object.
		* \param key The key to remove
		* \return Zero on success (-1 is key was not found)
		*
		*/
	JsonObject& remove(const var::String & key);

	/*!
		* \details Returns the number of key/value pairs in the object
		* \return The number of pairs
		*/
	u32 count() const;

	/*!
		* \details Removes all key/value pairs from the object.
		* \return Zero on success
		*/
	JsonObject& clear();

	/*!
		* \details Returns a JsonValue (as a reference) to the specified key.
		* \param key The key to access
		* \return A JsonValue (referenced to the key)
		*
		* If the key does not exist, an invalid JsonValue is returned.
		*
		* \code
		* JsonObject json_object;
		* json_object.insert("name", "john");
		* json_object.at("name").is_valid(); //this is true
		* json_object.at("john").is_valid(); //this is false
		* \endcode
		*
		* Becaues the JsonValue is referenced (see JsonValue::is_observer())
		* the life of the object is not affected when the returned
		* JsonValue is destroyed.
		*
		*/
	JsonValue at(const var::String & key) const;


	var::StringList key_list() const;
	var::StringList keys() const {
		return key_list();
	}

private:
	json_t * create();

};

class JsonArray : public JsonValue {
public:
	JsonArray();
	JsonArray(const JsonArray & value);
	JsonArray & operator=(const JsonArray & value);

	explicit JsonArray(const var::StringList& list);
	explicit JsonArray(const var::Vector<float>& list);
	explicit JsonArray(const var::Vector<u32>& list);
	explicit JsonArray(const var::Vector<s32>& list);

	template<class T> explicit JsonArray(const var::Vector<T>& list){
		m_value = create();
		for(const auto & item: list){
			append(item.to_object());
		}
	}

	template<class T> var::Vector<T> construct_list() const {
		var::Vector<T> result;
		result.reserve(count());
		for(u32 i=0; i < count(); i++){
			result.push_back(T(at(i).to_object()));
		}
		return result;
	}

	template<class T> var::Vector<T> construct_list_copy() const {
		var::Vector<T> result;
		result.reserve(count());
		for(u32 i=0; i < count(); i++){
			result.push_back(T(JsonValue().copy(at(i))));
		}
		return result;
	}

	/*! \details Returns true if the object is empty. */
	bool is_empty() const { return count() == 0; }

	u32 count() const;

	/*! \details Returns a JsonValue as a reference at
		* the specified index.
		*
		* @param idx The index of the value to access
		* @return A JsonValue as a reference (see JsonValue::is_observer())
		*
		*/
	JsonValue at(size_t position) const;

	JsonArray& append(const JsonValue & value);

	JsonArray& append(const JsonArray & array);

	JsonArray& insert(
			size_t position,
			const JsonValue & value
			);

	JsonArray& remove(size_t position);
	JsonArray& clear();

	var::StringList string_list();
	var::Vector<s32> integer_list();
	var::Vector<float> float_list();
	var::Vector<bool> bool_list();

private:

	json_t * create();
};

class JsonString : public JsonValue {
public:
	JsonString();
	JsonString(const var::String & str);
private:
	json_t * create();
};

class JsonReal : public JsonValue {
public:
	JsonReal();
	JsonReal(float value);
	JsonReal & operator=(float a);
private:
	json_t * create();
};

class JsonInteger : public JsonValue {
public:
	JsonInteger();
	JsonInteger(int value);
	JsonInteger & operator=(int a);
private:
	json_t * create();
};

class JsonNull : public JsonValue {
public:
	JsonNull();
private:
	json_t * create();
};


class JsonTrue : public JsonValue {
public:
	JsonTrue();
private:
	json_t * create();
};

class JsonFalse : public JsonValue {
public:
	JsonFalse();
private:
	json_t * create();
};

/*! \brief Json Class
 * \details The Json class is a JsonValue
 * that can be loaded and saved from
 * either memory or the filesystem.
 *
 */
class JsonDocument : public api::WorkObject {
public:

	enum options {
		option_reject_duplicates = JSON_REJECT_DUPLICATES,
		option_disable_eof_check = JSON_DISABLE_EOF_CHECK,
		option_decode_any = JSON_DECODE_ANY,
		option_decode_int_as_real = JSON_DECODE_INT_AS_REAL,
		option_allow_null = JSON_ALLOW_NUL,
		option_indent1 = JSON_INDENT(1),
		option_indent2 = JSON_INDENT(2),
		option_indent3 = JSON_INDENT(3),
		option_indent4 = JSON_INDENT(4),
		option_indent5 = JSON_INDENT(5),
		option_indent6 = JSON_INDENT(6),
		option_indent7 = JSON_INDENT(7),
		option_indent8 = JSON_INDENT(8),
		option_compact = JSON_COMPACT,
		option_ensure_ascii = JSON_ENSURE_ASCII,
		option_encode_any = JSON_ENCODE_ANY,
		option_preserve_order = JSON_PRESERVE_ORDER,
		option_escape_slash = JSON_ESCAPE_SLASH,
		option_embed = JSON_EMBED
	};

	using FilePath = fs::File::Path;

	/*! \details Constructs a Json object from a JsonValue.
	 *
	 * @param value JsonValue in memory that this class will refer to.
	 *
	 */
	JsonDocument(u32 o_flags = option_indent3){
		set_flags(o_flags);
	}

	JsonDocument & set_options(u32 o_option){ m_flags = o_option; return *this; }
	u32 options() const { return m_flags; }

	JsonDocument & set_flags(u32 flags){ m_flags = flags; return *this; }
	u32 flags() const { return m_flags; }

	//load a JSON object or array from a file?
	/*!
		* \details Loads a JSON value from a file
		* \param path The path to the file
		* \return Zero on success
		*/
	JsonValue load(fs::File& file);

	/*!
		* \details Loads a JSON value from a data object
		* \param data A reference to the data object containing the JSON
		* \return
		*/
	JsonValue load(
			const String & json
			);

#if defined __link
	using XmlString = arg::Argument<const String &, struct JsonDocumentXmlStringTag>;
	using XmlFilePath = arg::Argument<const String &, struct JsonDocumentXmlFilePathTag>;

	/*!
		* \details Loads a JSON value from a data object
		* \param data A reference to the data object containing the JSON
		* \return
		*/
	JsonValue load(
			XmlString xml
			);

	JsonValue load(
			XmlFilePath path
			);
#endif


	/*!
		* \details Loads a JSON value from an already open file
		* \param file A reference to the file containing JSON
		* \return Zero on success
		*/
	JsonValue load(
			FilePath file
			);

	/*!
		* \details Loads a JSON value from streaming data
		* \param callback The function to call when more data is available
		* \param context This is passed to \a callback but not used internally
		* \return Zero on success
		*/
	JsonValue load(
			json_load_callback_t callback,
			void * context
			);

	int save(const var::JsonValue & value,
					 fs::File::Path path
					 ) const;

	var::String to_string(
			const JsonValue & value
			) const;

	var::String stringify(
			const JsonValue & value
			) const {
		return to_string(value);
	}

	int save(
			const JsonValue & value,
			fs::File & file
			) const;

	int save(
			const JsonValue & value,
			json_dump_callback_t callback,
			void * context
			) const;

	const JsonError & error() const { return m_error; }

private:

	u32 m_flags;
	JsonError m_error;

	static size_t load_file_data(void * buffer, size_t buflen, void *data);

};

}


namespace sys {
class Printer;
Printer& operator << (Printer& printer, const var::JsonValue & a);
Printer& operator << (Printer& printer, const var::JsonError & a);
Printer& print_value(Printer& printer, const var::JsonValue & a, const var::String& key);
}

//full copy, no reference to original
#define JSON_ACCESS_STRING_WITH_KEY(c, k, v) \
	var::String get_##v() const { return to_object().at(MCU_STRINGIFY(k)).to_string(); } \
	c& set_##v(const var::String& value){ to_object().insert(MCU_STRINGIFY(k), var::JsonString(value)); return *this; } \
	c& remove_##v(){ to_object().remove(MCU_STRINGIFY(k)); return *this; } \
	void json_access_string_with_key_never_used_##v()

#define JSON_ACCESS_STRING(c, v) JSON_ACCESS_STRING_WITH_KEY(c,v,v)

//full copy, no reference to original
#define JSON_ACCESS_BOOL_WITH_KEY(c, k, v) \
	bool is_##v() const { return to_object().at(MCU_STRINGIFY(k)).to_bool(); } \
	c& set_##v(bool value = true){ to_object().insert(MCU_STRINGIFY(k), value); return *this; } \
	c& remove_##v(){ to_object().remove(MCU_STRINGIFY(k)); return *this; } \
	void json_access_bool_with_key_never_used_##v()

#define JSON_ACCESS_BOOL(c, v) JSON_ACCESS_BOOL_WITH_KEY(c,v,v)


//full copy, no reference to original
#define JSON_ACCESS_INTEGER_WITH_KEY(c, k, v) \
	s32 get_##v() const { return to_object().at(MCU_STRINGIFY(k)).to_integer(); } \
	c& set_##v(s32 value){ to_object().insert(MCU_STRINGIFY(k), var::JsonInteger(value)); return *this; } \
	c& remove_##v(){ to_object().remove(MCU_STRINGIFY(k)); return *this; } \
	void json_access_integer_with_key_never_used_##v()
#define JSON_ACCESS_INTEGER(c, v) JSON_ACCESS_INTEGER_WITH_KEY(c, v, v)

//gets a copy that refers to the original JSON values
#define JSON_ACCESS_OBJECT_WITH_KEY(c, T, k, v) \
	T v() const { return T(to_object().at(MCU_STRINGIFY(k))); } \
	T get_##v() const { return T(var::JsonObject().copy(to_object().at(MCU_STRINGIFY(k)))); } \
	c& set_##v(const T& a){ to_object().at(MCU_STRINGIFY(k)) = a.to_object(); return *this; } \
	c& remove_##v(){ to_object().remove(MCU_STRINGIFY(k)); return *this; } \
	void json_access_object_with_key_never_used_##v()

#define JSON_ACCESS_OBJECT(c, T, v) JSON_ACCESS_OBJECT_WITH_KEY(c,T,v,v)

//constructs a copy of a list that refers to original values (or full copy with get) T should have JsonKeyValue as a base
#define JSON_ACCESS_OBJECT_LIST_WITH_KEY(c, T, k, v) \
	var::JsonObject v##_to_object() const { return to_object().at(MCU_STRINGIFY(k)); } \
	var::JsonKeyValueList<T> get_##v() const { return to_object().at(MCU_STRINGIFY(k)).to_object().construct_key_list_copy<T>(); } \
	var::JsonKeyValueList<T> v() const { return to_object().at(MCU_STRINGIFY(k)).to_object().construct_key_list<T>(); } \
	c& set_##v(const var::JsonKeyValueList<T>& a){ to_object().insert(MCU_STRINGIFY(k), var::JsonObject(a)); return *this; } \
	c& remove_##v(){ to_object().remove(MCU_STRINGIFY(k)); return *this; } \
	void json_access_object_list_never_used_##v()

#define JSON_ACCESS_OBJECT_LIST(c, T, v) JSON_ACCESS_OBJECT_LIST_WITH_KEY(c, T, v, v)


//constructs a copy of a list that refers to original values (or full copy with get)
#define JSON_ACCESS_ARRAY_WITH_KEY(c, T, k, v) \
	var::JsonArray v##_to_array() const { return to_object().at(MCU_STRINGIFY(k)); } \
	var::Vector<T> get_##v() const { return to_object().at(MCU_STRINGIFY(k)).to_array().construct_list_copy<T>(); } \
	var::Vector<T> v() const { return to_object().at(MCU_STRINGIFY(k)).to_array().construct_list<T>(); } \
	c& set_##v(const var::Vector<T>& a){ to_object().insert(MCU_STRINGIFY(k), var::JsonArray(a)); return *this; } \
	c& remove_##v(){ to_object().remove(MCU_STRINGIFY(k)); return *this; } \
	void json_access_array_with_key_never_used_##v()

#define JSON_ACCESS_ARRAY(c, T, v) JSON_ACCESS_ARRAY_WITH_KEY(c,T,v,v)

//gets a copy that refers to the original JSON values
#define JSON_ACCESS_VALUE_WITH_KEY(c, k, v) \
	const var::JsonValue& v() const { return to_object().at(MCU_STRINGIFY(k)); } \
	var::JsonValue get_##v() const { return var::JsonValue().copy(to_object().at(MCU_STRINGIFY(k))); } \
	c& set_##v(const var::JsonValue& a){ to_object().insert(MCU_STRINGIFY(k), a); return *this; } \
	void json_access_value_with_key_never_used_##v()

#define JSON_ACCESS_VALUE(c, v) JSON_ACCESS_VALUE_WITH_KEY(c,v,v)

//full copy, no reference to original
#define JSON_ACCESS_STRING_ARRAY_WITH_KEY(c, k, v) \
	var::StringList get_##v() const { return to_object().at(MCU_STRINGIFY(k)).to_array().string_list(); } \
	c& set_##v(const var::StringList& a){ to_object().insert(MCU_STRINGIFY(k), var::JsonArray(a)); return *this; } \
	c& remove_##v(){ to_object().remove(MCU_STRINGIFY(k)); return *this; } \
	void json_access_array_with_key_never_used_##v()

#define JSON_ACCESS_STRING_ARRAY(c, v) JSON_ACCESS_STRING_ARRAY_WITH_KEY(c,v,v)


#define JSON_ACCESS_CONSTRUCT_OBJECT(c) \
	c() : var::JsonValue(var::JsonObject()){} \
	c(const var::JsonObject object) : var::JsonValue(object){} \
	void json_access_construct_object_never_used_##v()

//access functions for working with JsonKeyValue objects
#define JSON_ACCESS_KEY_VALUE_PAIR_STRING(c,k,v) \
	const var::String& k() const { return key(); } \
	var::String get_##v() const { return to_string(); } \
	c& set_##v(const var::String& a){ set_value( var::JsonString(a) ); return *this; } \
	void json_access_key_value_pair_string_never_used_##v()

//access functions for working with JsonKeyValue objects
#define JSON_ACCESS_KEY_VALUE_PAIR_REAL(c,k,v) \
	const var::String& k() const { return key(); } \
	float v() const { return to_float(); } \
	float get_##v() const { return to_float(); } \
	c& set_##v(float a){ set_value( var::JsonReal(a) ); return *this; } \
	void json_access_key_value_pair_real_never_used_##v()

//access functions for working with JsonKeyValue objects
#define JSON_ACCESS_KEY_VALUE_PAIR_BOOL(c,k,v) \
	const var::String& k() const { return key(); } \
	bool is_##v() const { return to_bool(); } \
	c& set_##v(bool a = true){ a ? set_value( var::JsonTrue() ) : set_value( var::JsonFalse() ); return *this; } \
	void json_access_key_value_pair_bool_never_used_##v()

//access functions for working with JsonKeyValue objects
#define JSON_ACCESS_KEY_VALUE_PAIR_ARRAY(c,T,k,v) \
	const var::String& k() const { return key(); } \
	var::Vector<T> v() const { return to_array().construct_list<T>(); } \
	var::Vector<T> get_##v() const { return to_array().construct_list_copy<T>(); } \
	c& set_##v(const var::Vector<T>& a){ set_value( var::JsonArray(a) ); return *this; } \
	void json_access_key_value_pair_array_never_used_##v()

//access functions for working with JsonKeyValue objects
#define JSON_ACCESS_KEY_VALUE_PAIR_STRING_ARRAY(c,T,k,v) \
	const var::String& k() const { return key(); } \
	var::StringList v() const { return to_array().construct_list<T>(); } \
	var::StringList get_##v() const { return to_array().construct_list_copy<T>(); } \
	c& set_##v(const var::StringList& a){ set_value( var::JsonArray(a) ); return *this; } \
	void json_access_key_value_pair_string_array_never_used_##v()

//access functions for working with JsonKeyValue objects
#define JSON_ACCESS_KEY_VALUE_PAIR_INTEGER_ARRAY(c,T,k,v) \
	const var::String& k() const { return key(); } \
	var::Vector<s32> get_##v() const { return to_array().construct_list<T>(); } \
	c& set_##v(const var::Vector<s32>& a){ set_value( var::JsonArray(a) ); return *this; } \
	void json_access_key_value_pair_integer_array_never_used_##v()

//access functions for working with JsonKeyValue objects
#define JSON_ACCESS_KEY_VALUE_PAIR_REAL_ARRAY(c,T,k,v) \
	const var::String& k() const { return key(); } \
	var::Vector<float> get_##v() const { return to_array().construct_list<T>(); } \
	c& set_##v(const var::Vector<float>& a){ set_value( var::JsonArray(a) ); return *this; } \
	void json_access_key_value_pair_integer_array_never_used_##v()


#endif // SAPI_VAR_JSON_HPP_
