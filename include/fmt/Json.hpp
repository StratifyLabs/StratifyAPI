#ifndef JSON_HPP
#define JSON_HPP

#include <jansson/jansson.h>
#include "../api/FmtObject.hpp"
#include "../var/Vector.hpp"
#include "../var/String.hpp"

namespace fmt {

class Json;

class JsonError : public api::FmtInfoObject {
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
    friend class Json;
    json_error_t m_value;
};

class JsonObject;
class JsonArray;
class JsonBool;
class JsonReal;
class JsonNull;
class JsonInteger;
class JsonString;

class JsonValue : public api::FmtInfoObject {
public:
    JsonValue(){
        m_value = 0;
        m_is_observer = true;
    }

    JsonValue(json_t * value){
        m_value = value;
        m_is_observer = true;
    }

    ~JsonValue(){
        //only decref if object was create (not just a reference)
        if( !m_is_observer ){ json_decref(m_value); }
        m_value = 0;
    }

    /*! \details Returns true if the JsonValue is a reference
     * to an object that is owned and managed by another instance.
     *
     * When a JsonValue class object is created directly,
     * it is simply used to reference and access other objects.
     * When a specific type of JsonValue is created (like JsonObject),
     * memory is dynamically allocated for that object and managed
     * within JsonValue.
     *
     * \code
     * JsonArray array; //allocate memory for a new array
     * JsonObject object; //allocate memory for an object
     * array.append(JsonReal(0.0f)); //allocate memory for an real and reference it to array
     * object.insert(array); //reference array to object
     * \endcode
     *
     * In the above snippet, array is allocated and referenced by
     * object. The memory allocated for array will not be destroyed
     * until array and all references to it are destroyed. This will
     * happen automatically when the object's deconstructors are called.
     *
     * In the code below, we observe values without affecting
     * how their memory is managed.
     *
     * \code
     * JsonArray array; //allocate memory for a new array
     * JsonObject object; //allocate memory for an object
     * array.append(JsonReal(0.0f)); //allocate memory for an real and reference it to array
     * object.insert("array", array); //reference array to object
     *
     * //value_observer observes array without affecting how it is freed
     * JsonValue value_observer = object.at("array");
     *
     * \endcode
     *
     * In the above code, if `value_observer` is accessed after array has
     * been freed, there will be problems. However, `object` will
     * always be valid because it references `array` so that array is not freed
     * until `object` is done with it.
     *
     */
    bool is_observer() const { return m_is_observer; }

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

    const JsonObject & to_object() const;
    JsonObject & to_object();
    const JsonArray & to_array() const;
    JsonArray & to_array();
    var::String to_string() const;
    float to_real() const;
    int to_integer() const;
    bool to_bool() const;

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
     * JsonBool bool_value(true);
     *
     * bool_value.assign("false"); //assigns false
     * bool_value.assign("true"); /assign true
     * bool_value.assign("Hello World"); assigns false
     * \endcode
     *
     *
     */
    int assign(const var::ConstString & value);


    int assign(float value);
    int assign(int value);
    int assign(bool value);

private:
    friend class Json;
    friend class JsonObject;
    friend class JsonArray;
    friend class JsonBool;
    friend class JsonTrue;
    friend class JsonFalse;
    friend class JsonReal;
    friend class JsonInteger;
    friend class JsonString;
    friend class JsonNull;
    json_t * m_value;
    bool m_is_observer;

};

class JsonObject : public JsonValue {
public:

    JsonObject();

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

};

class JsonArray : public JsonValue {
public:
    JsonArray();
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

};

class JsonString : public JsonValue {
public:
    JsonString(const var::ConstString & str);
};

class JsonReal : public JsonValue {
public:
    JsonReal(float value);
    JsonReal & operator=(float a);

};

class JsonInteger : public JsonValue {
public:
    JsonInteger(int value);
    JsonInteger & operator=(int a);
};

class JsonNull : public JsonValue {
public:
    JsonNull();
};

class JsonBool : public JsonValue {
public:
    JsonBool(bool value);
};

class JsonTrue : public JsonBool {
public:
    JsonTrue() : JsonBool(true){}
};

class JsonFalse : public JsonBool {
public:
    JsonFalse() : JsonBool(false){}
};

class Json : public JsonValue {
public:
    Json();

    Json(const JsonValue & value) : JsonValue(value.m_value){}

    void set_flags(u32 flags){ m_flags = flags; }

    u32 flags() const { return m_flags; }

    //load a JSON object or array from a file?
    /*!
     * \defails Loads a JSON value from a file
     * \param path The path to the file
     * \return Zero on success
     */
    int load(const var::ConstString & path);

    /*!
     * \details Loads a JSON value from a data object
     * \param data A reference to the data object containing the JSON
     * \return
     */
    int load(const var::Data & data);

    /*!
     * \details Loads a JSON value from an already open file
     * \param file A reference to the file containing JSON
     * \return Zero on success
     */
    int load(const sys::File & file);

    /*!
     * \details Loads a JSON value from streaming data
     * \param callback The function to call when more data is available
     * \param context This is passed to \a callback but not used internally
     * \return Zero on success
     */
    int load(json_load_callback_t callback, void * context);

    int save(const var::ConstString & path);
    int save(var::Data & data);
    int save(const sys::File & file);
    int save(json_dump_callback_t callback, void * context);


    enum {
        REJECT_DUPLICATES = JSON_REJECT_DUPLICATES,
        DISABLE_EOF_CHECK = JSON_DISABLE_EOF_CHECK,
        DECODE_ANY = JSON_DECODE_ANY,
        DECODE_INT_AS_REAL = JSON_DECODE_INT_AS_REAL,
        ALLOW_NULL = JSON_ALLOW_NUL
    };

    const JsonError & error() const { return m_error; }

private:
    u32 m_flags;
    JsonError m_error;
};

}

#endif // JSON_HPP
