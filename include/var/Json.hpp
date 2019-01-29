#ifndef JSON_HPP
#define JSON_HPP

#include <jansson/jansson_api.h>
#include "../api/VarObject.hpp"
#include "../var/Vector.hpp"
#include "../var/String.hpp"
#include "../sys/File.hpp"
#include "../sys/requests.h"

namespace var {

#undef TRUE
#undef FALSE

class JsonDocument;

class JsonError : public api::VarInfoObject {
public:
	JsonError(){
		memset(&m_value, 0, sizeof(m_value));
	}
	int line() const { return m_value.line; }
	int column() const { return m_value.column; }
	int position() const { return m_value.position; }
	var::ConstString source() const { return m_value.source; }
	var::ConstString text() const { return m_value.text; }

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

class JsonValue : public api::VarWorkObject {
public:
	JsonValue();

	JsonValue(json_t * value);
	JsonValue(const JsonValue & value);
	JsonValue & operator=(const JsonValue & value);
	~JsonValue();

	JsonValue(JsonValue && a);
	JsonValue& operator=(JsonValue && a);

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
	bool is_valid() const{ return m_value != 0; }

	enum type {
		INVALID = -1,
		OBJECT = JSON_OBJECT,
		ARRAY = JSON_ARRAY,
		STRING = JSON_STRING,
		REAL = JSON_REAL,
		INTEGER = JSON_INTEGER,
		TRUE = JSON_TRUE,
		FALSE = JSON_FALSE,
		ZERO = JSON_NULL
	};

	enum type type() const {
		if( m_value ){
			return (enum type)json_typeof(m_value);
		}
		return INVALID;
	}

	bool is_object() const { return type() == OBJECT; }
	bool is_array() const { return type() == ARRAY; }
	bool is_string() const { return type() == STRING; }
	bool is_real() const { return type() == REAL; }
	bool is_integer() const { return type() == INTEGER; }
	bool is_true() const { return type() == TRUE; }
	bool is_false() const { return type() == FALSE; }
	bool is_null() const { return type() == ZERO; }
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
	 * - INVALID: "<invalid>"
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
	float to_real() const;

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
	int assign(const var::ConstString & value);


	int assign(float value);
	int assign(int value);
	int assign(bool value);

	int copy(const JsonValue & value, bool is_deep = true);


	static JsonApi & api(){ return m_api; }

protected:
	int create_if_not_valid();
	virtual json_t * create(){
		printf("create JSON Value -- 0\n");
		return 0;
	}




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
	json_t * m_value;

	void add_reference(json_t * value);

	static JsonApi m_api;


};

class JsonObject : public JsonValue {
public:

	JsonObject();

	JsonObject(const JsonObject & value);
	JsonObject & operator=(const JsonObject & value);

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
	int insert(const var::ConstString & key, const JsonValue & value);

	enum {
		UPDATE_EXISTING = 0x01,
		UPDATE_MISSING = 0x02
	};

	int update(const JsonValue & value, u8 o_flags = 0);

	/*!
	  * \details Removes the specified key from the object.
	  * \param key The key to remove
	  * \return Zero on success (-1 is key was not found)
	  *
	  */
	int remove(const var::ConstString & key);

	/*!
	  * \details Returns the number of key/value pairs in the object
	  * \return The number of pairs
	  */
	u32 count() const;

	/*!
	  * \details Removes all key/value pairs from the object.
	  * \return Zero on success
	  */
	int clear();

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
	JsonValue at(const var::ConstString & key) const;

	var::Vector<var::String> keys() const;

private:
	json_t * create();

};

class JsonArray : public JsonValue {
public:
	JsonArray();
	JsonArray(const JsonArray & value);
	JsonArray & operator=(const JsonArray & value);

	u32 count() const;

	/*! \details Returns a JsonValue as a reference at
	  * the specified index.
	  *
	  * @param idx The index of the value to access
	  * @return A JsonValue as a reference (see JsonValue::is_observer())
	  *
	  */
	JsonValue at(u32 idx) const;

	int append(const JsonValue & value);
	int append(const JsonArray & array);
	int insert(u32 idx, const JsonValue & value);
	int remove(u32 idx);
	int clear();
private:

	json_t * create();
};

class JsonString : public JsonValue {
public:
	JsonString();
	JsonString(const var::ConstString & str);
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
class JsonDocument : public api::VarWorkObject {
public:

	/*! \details Constructs a Json object from a JsonValue.
	 *
	 * @param value JsonValue in memory that this class will refer to.
	 *
	 */
	JsonDocument(u32 o_flags = INDENT_3){
		set_flags(o_flags);
	}

	void set_flags(u32 flags){ m_flags = flags; }
	u32 flags() const { return m_flags; }

	//load a JSON object or array from a file?
	/*!
	  * \defails Loads a JSON value from a file
	  * \param path The path to the file
	  * \return Zero on success
	  */
	JsonValue load_from_file(const var::ConstString & path);

	/*!
	  * \details Loads a JSON value from a data object
	  * \param data A reference to the data object containing the JSON
	  * \return
	  */
	JsonValue load_from_string(const var::ConstString & json);

	/*!
	  * \details Loads a JSON value from an already open file
	  * \param file A reference to the file containing JSON
	  * \return Zero on success
	  */
	JsonValue load_from_file(const sys::File & file);

	/*!
	  * \details Loads a JSON value from streaming data
	  * \param callback The function to call when more data is available
	  * \param context This is passed to \a callback but not used internally
	  * \return Zero on success
	  */
	JsonValue load(json_load_callback_t callback, void * context);

	int save_to_file(const JsonValue & value, const var::ConstString & path) const;
	var::String stringify(const JsonValue & value) const;
	int save_to_file(const JsonValue & value, const sys::File & file) const;
	int save(const JsonValue & value, json_dump_callback_t callback, void * context) const;

	enum {
		REJECT_DUPLICATES = JSON_REJECT_DUPLICATES,
		DISABLE_EOF_CHECK = JSON_DISABLE_EOF_CHECK,
		DECODE_ANY = JSON_DECODE_ANY,
		DECODE_INT_AS_REAL = JSON_DECODE_INT_AS_REAL,
		ALLOW_NULL = JSON_ALLOW_NUL,
		INDENT_1 = JSON_INDENT(1),
		INDENT_2 = JSON_INDENT(2),
		INDENT_3 = JSON_INDENT(3),
		INDENT_4 = JSON_INDENT(4),
		INDENT_5 = JSON_INDENT(5),
		INDENT_6 = JSON_INDENT(6),
		INDENT_7 = JSON_INDENT(7),
		INDENT_8 = JSON_INDENT(8),
		COMPACT = JSON_COMPACT,
		ENSURE_ASCII = JSON_ENSURE_ASCII,
		ENCODE_ANY = JSON_ENCODE_ANY,
		PRESERVE_ORDER = JSON_PRESERVE_ORDER,
		ESCAPE_SLASH = JSON_ESCAPE_SLASH,
		EMBED = JSON_EMBED
	};

	const JsonError & error() const { return m_error; }

private:
	u32 m_flags;
	JsonError m_error;
};

}

#endif // JSON_HPP
