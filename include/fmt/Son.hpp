/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_FMT_SON_HPP_
#define SAPI_FMT_SON_HPP_

#include <cstring>
#include <sapi/son.h>

#if defined __link
#include <sos/link.h>
#endif

#include "../api/FmtObject.hpp"
#include "../var/String.hpp"
#include "../chrono/MicroTime.hpp"
#include "../sys/requests.h"


/*! \cond */

namespace fmt {

typedef api::Api<son_api_t, SON_API_REQUEST> SonApi;

/*! \brief Stratify Object Notation
 *
 * \details The SON (Stratify Object Notation) is an important class to be familiar with.  It is
 * closely related to JSON (and very closely related to BSON).  It allows you to store, access, and modify
 * data in the same way but is designed for use on resource constrained systems.
 *
 * SON data can be easily converted to JSON data using the to_json() method.
 *
 * Important things to note:
 * - Key names are limited to 15 (SON_KEY_NAME_SIZE) characters (longer names are truncated)
 * - The stacksize template value is the depth limit when creating files
 * - You can't modify the length of strings or data objects within a file
 * - You must create root ("") as the first object or array
 *
 * Creation example:
 *
 * \code
 *
 * #include <sapi/fmt.hpp>
 *
 * int main(int argc, char * argv[]){
 * 	Son son<4>; //depth is limited to 4
 *
 *	//tabs are added to help visualize the data in JSON format
 * 	son.create("/home/settings.son");
 * 		son.open_object("");
 * 			son.write("name", "Stratify"); //create a string
 * 			son.write("date", "today"); //create another string
 * 			son.write("value", (u32)100); //write a number);
 * 			son.open_object("time"); //create a new object
 * 				son.write("hour", 12); //add some values
 * 				son.write("min", 00);
 * 				son.write("sec", 59);
 * 			son.close_obj(); //close 'time'
 * 		son.close_obj(); //close root ""
 * 	son.close();
 *
 * 	char buffer[32];
 * 	u32 value;
 * 	son.open_read("/home/settings.son"); //opens read-only
 *
 * 	son.read_str("name", buffer, 32);
 * 	//buffer holds "Stratify"
 *
 * 	value = son.read_unum("hour");
 * 	son.read_str("sec", buffer, 32);
 * 	//buffer holds "59"
 *
 * 	son.to_json("/home.settings.json");
 * 	son.close();
 *
 * 	}
 *  \endcode
 *
 *  The home/settings.json files looks like:
 *
 *      {
 *          "name": "Stratify",
 *          "date": "today",
 *          "value": 100,
 *          "time": {
 *              "hour": 12,
 *              "min": 0,
 *              "sec": 59
 *           }
 *      }
 *
 *  Note that JSON numbers are always in floating point format. So unum and num values
 *  get converted to float value. If the JSON data is converted to SON, all numbers will
 *  be in float format.
 *
 */
class Son : public api::WorkObject {
public:

	/*! \details Constructs a new SON object. */
	Son(u16 max_depth = 8, son_stack_t * stack = 0);

	~Son();

#if defined __link
	void set_driver(void * driver){ son_set_driver(&m_son, driver); }
	//deprecated
	void set_handle(void * handle){ set_driver(handle); }
#endif

	/*! \details Returns file descriptor of the SON file.
	 */
	int fileno() const {
#if defined __link
		if( m_son.phy.driver == 0 ){
			return 0;
		} else {
			return m_son.phy.fd;
		}
#else
		return m_son.phy.fd;
#endif
	}

	/*! \details Creates a new SON file
	 *
	 * @param name The name of the file
	 * @return Zero on success
	 */
	int create(const var::String & name){ return api()->create(&m_son, name.cstring(), m_stack, m_stack_size); }

	/*! \details Creates a memory message SON object.
	 *
	 * @param message The memory message
	 * @param nbyte The number of bytes in the message
	 *
	 * @return Zero on success
	 */
	int create_message(void * message, int nbyte){ return api()->create_message(&m_son, message, nbyte, m_stack, m_stack_size); }

	/*! \details Creates a memory message SON object.
	 *
	 * @param data A reference to the data object used to store the message
	 *
	 * @return Zero on success
	 */
	int create_message(
			var::Data & data
			){
		return api()->create_message(
					&m_son,
					data.to_void(),
					data.capacity(),
					m_stack,
					m_stack_size);
	}


	/*! \details Creates a memory message SON object using dynamic memory allocation.
	 *
	 * @param nbyte The number of bytes in the message
	 *
	 * @return Zero on success
	 */
	int create_message(int nbyte){ return api()->create_message(&m_son, 0, nbyte, m_stack, m_stack_size); }


	/*! \details Sends a message on the specified file descriptor.
	 *
	 * @param fd The file descriptor to write the message to.
	 * @param timeout_msec The max number of milliseconds to wait between bytes before aborting.
	 * @return The number of bytes sent or less then zero for an error
	 */
	int send_message(int fd, int timeout_msec){ return api()->send_message(&m_son, fd, timeout_msec); }


	/*! \details Receives a message on the specified file descriptor.
	 *
	 * @param fd The file descriptor to listen for the message on.
	 * @param timeout_msec The max number of milliseconds to wait between bytes before aborting.
	 * @return The number of bytes received or less than zero for an error
	 */
	int recv_message(int fd, int timeout_msec){ return api()->recv_message(&m_son, fd, timeout_msec); }


	/*! \details Gets the size of the message in bytes.
	 *
	 * @return The number of bytes that will be sent is send_message() is called or less than zero for an error.
	 */
	int get_message_size(){ return api()->get_message_size(&m_son); }


	/*! \details Opens a file for appending data.
	 *
	 * @param name The path/name of the file to open
	 * @return Less than zero for an error
	 */
	int open_append(const var::String & name){ return api()->append(&m_son, name.cstring(), m_stack, m_stack_size); }

	/*! \details Opens a SON file for reading.
	 *
	 * @param name Name of the file
	 * @return Zero on success
	 */
	int open_read(const var::String & name){ return api()->open(&m_son, name.cstring()); }

	/*! \details Opens a SON message for reading.
	 *
	 * @param message The memory message
	 * @param nbyte The number of bytes in the message
	 * @return Zero on success
	 */
	int open_message(void * message, int nbyte){ return api()->open_message(&m_son, message, nbyte); }
	int open_message(var::Data & data){ return api()->open_message(&m_son, data.to_void(), data.capacity()); }

	int open_read_message(void * message, int nbyte){ return open_message(message, nbyte); }
	int open_read_message(var::Data & data){ return open_message(data); }

	/*! \details Opens a SON file for editing.
	 *
	 * @param name Name of the file
	 * @return Zero on success
	 */
	int open_edit(const var::String & name){ return api()->edit(&m_son, name.cstring()); }

	/*! \details Opens a SON message for editing.
	 *
	 * @param message The memory message
	 * @param nbyte The number of bytes in the message
	 * @return Zero on success
	 */
	int open_edit_message(void * message, int nbyte){ return api()->edit_message(&m_son, message, nbyte); }

	/*! \details Closes a SON file. */
	int close(){ return api()->close(&m_son); }

	/*! \details Seeks to the location of the access code and gets the size of the data in bytes.
	 *
	 * @param access The access code to seek to
	 * @param data_size A pointer to write the data size to (null if not needed)
	 * @return Zero on success
	 */
	int seek(const var::String & access, son_size_t & data_size){ return api()->seek(&m_son, access.cstring(), &data_size); }

	/*! \details Seeks the next value in the file.
	 *
	 * @param name A reference to a string where the name will be stored
	 * @param type If non-null, the type of the object will be written to this value.
	 * @return Less than zero for an error or the son_value_t of the current item.
	 *
	 *
	 */
	int seek_next(var::String & name, son_value_t * type = 0){
		name.resize(SON_KEY_NAME_CAPACITY);
		return api()->seek_next(&m_son, name.to_char(), type);
	}

	int seek_next(var::String & name, son_value_t & type){
		name.resize(SON_KEY_NAME_CAPACITY);
		return api()->seek_next(&m_son, name.to_char(), &type);
	}

	/*! \details Converts the data file to JSON.
	 *
	 * @param path The path to a file to create with the JSON data
	 * @return Zero on success
	 */
	int to_json(const var::String & path){
		return api()->to_json(&m_son, path.cstring(), 0, 0);
	}

	/*! \details Converts the data file to JSON.
	 *
	 * @param callback A callback that is used to process the JSON data (c string)
	 * @param context The parameter that is passed to callback
	 * @return Zero on success
	 */
	int to_json(son_to_json_callback_t callback, void * context = 0){
		return api()->to_json(&m_son, 0, callback, context);
	}

	/*! \details Opens a new object while writing or appending.
	 *
	 * @param key The key to use for the new object
	 * @return Zero on success
	 */
	int open_object(const var::String & key){ return api()->open_object(&m_son, key.cstring()); }
	/*! \details Closes an object while writing or appending a file.
	 *
	 */
	int close_object(){ return api()->close_object(&m_son); }

	/*! \details Opens a new array while writing or appending.
	 *
	 * @param key The key to use for the new array
	 * @return Zero on success
	 */
	int open_array(const var::String & key){ return api()->open_array(&m_son, key.cstring()); }

	/*! \details Closes an array while writing or appending.
	 *
	 * @return Zero on success
	 *
	 * To proper operation, this a call to this method must correspond to a
	 * call to open_data().
	 */
	int close_array(){ return api()->close_array(&m_son); }

	/*! \details Opens a data object while writing or appending.
	 *
	 * @param key The key to use for the new data
	 * @return Zero on success
	 */
	int open_data(const var::String & key){ return api()->open_data(&m_son, key.cstring()); }

	/*! \details Closes a data object.
	 *
	 * @return Zero on success
	 *
	 * To proper operation, this a call to this method must correspond to a
	 * call to open_data().
	 *
	 */
	int close_data(){ return api()->close_data(&m_son); }

	/*! \details Writes a key/string pair to the file.
	 *
	 * @param key The value key
	 * @param v A pointer to the string
	 * @return Number of bytes in the value portion to be successfully stored
	 */
	int write(const var::String & key, const var::String & v){
		if( v != "" ){
			return api()->write_str(&m_son, key.cstring(), v.cstring());
		} else {
			return api()->write_null(&m_son, key.cstring());
		}
	}

	/*! \details Writes a key/number pair to the file (s32).
	 *
	 * @param key The value key
	 * @param v The number to write
	 * @return Number of bytes stored (4) if successful
	 */
	int write(const var::String & key, s32 v){ return api()->write_num(&m_son, key.cstring(), v); }

	/*! \details Writes a key/number pair to the file (u32).
	 *
	 * @param key The value key
	 * @param v The number to write
	 * @return Number of bytes stored (4) if successful
	 */
	int write(const var::String & key, u32 v){ return api()->write_unum(&m_son, key.cstring(), v); }

	/*! \details Writes a key/number pair to the file (float).
	 *
	 * @param key The value key
	 * @param v The number to write
	 * @return Number of bytes stored (4) if successful
	 */
	int write(const var::String & key, float v){ return api()->write_float(&m_son, key.cstring(), v); }

	/*! \details Writes a key/bool pair to the file.
	 *
	 * @param key The value key
	 * @param v true or false
	 * @return Number of bytes stored if successful
	 */
	int write(const var::String & key, bool v){
		if( v ){
			return api()->write_true(&m_son, key.cstring());
		} else {
			return api()->write_false(&m_son, key.cstring());
		}
	}

	/*! \details Writes a key/data pair to the file.  The data
	 * is stored in the file in binary format.
	 *
	 * @param key The value key
	 * @param v A pointer to the data to be written
	 * @param size The number of bytes to write
	 * @return Number of bytes stored (4) if successful
	 */
	int write(const var::String & key, const void * v, son_size_t size){ return api()->write_data(&m_son, key.cstring(), v, size); }

	/*! \details Adds data to an open key.
	 *
	 * @param v A pointer to the data to be written
	 * @param size The number of bytes to write
	 * @return Number of bytes stored (4) if successful
	 *
	 * This is used with open_data() and close_data().   These
	 * methods are useful for writing data to the file when the amount of data to be written is unknown
	 * when writing begins.
	 *
	 * \code
	 * #include <sapi/Var.hpp>
	 * Son son<4>;
	 *
	 * son.create("/home/data.son");
	 * son.open_data("datakey");
	 * son.write_open_data(buffer, 32);
	 * son.close_data();
	 * \endcode
	 */
	int write_open_data(const void * v, son_size_t size){ return api()->write_open_data(&m_son, v, size); }

	/*! \details Reads the specified key as a string.  If the original
	 * key was not written as a string, it will be converted to a string.  For example,
	 * a (u32)100 will be converted to "100".  A data object will be converted to base64 encoding.
	 *
	 * @param access Key parameters
	 * @param str var::String reference
	 * @return The number of bytes actually read
	 */
	var::String read_string(const var::String & access){
		//first seek and get the size
		var::String result;
		son_size_t size;
		if( seek(access, size) >= 0 ){
			result.resize(size+1);
			if( api()->read_str(&m_son, access.cstring(), result.to_char(), result.capacity()) < 0 ){
				return var::String();
			}
			return result;
		}
		return var::String();
	}

	/*! \details Reads the specified key as a number (s32).  If the original
	 * key was not written as a s32, it will be converted to one.  A string
	 * of "100" will be converted to (s32)100.  Objects that cannot be converted will return 0.
	 *
	 * @param access Key parameters
	 * @return The number
	 */
	s32 read_num(const var::String & access){ return api()->read_num(&m_son, access.cstring()); }

	/*! \details Reads the specified key as a number (u32).  If the original
	 * key was not written as a s32, it will be converted to one.  A string
	 * of "100" will be converted to (u32)100.  Objects that cannot be converted will return 0.
	 *
	 * @param access Key parameters
	 * @return The number
	 */
	u32 read_unum(const var::String & access){ return api()->read_unum(&m_son, access.cstring()); }

	/*! \details Reads the specified key as a number (float).
	 *
	 * @param access Key parameters
	 * @return The number
	 *
	 *
	 * If the original key was not written as a float, it will be converted to one.  A string
	 * of "100" will be converted to (float)100.  Objects that cannot be converted will return 0.
	 *
	 *
	 */
	float read_float(const var::String & access){ return api()->read_float(&m_son, access.cstring()); }

	/*! \details Reads the specified key as data.  Regardless of the storage
	 * type, the key will be returned as binary data.
	 *
	 * @param access Key parameters
	 * @param data A pointer to the destination data
	 * @param size The number of bytes to read
	 * @return The number of bytes read
	 */
	int read_data(const var::String & access, void * data, son_size_t size){ return api()->read_data(&m_son, access.cstring(), data, size); }

	/*! \details Reads the specified key as a bool.
	 *
	 * @param access Key parameters
	 * @return True if the key is found and is true; false otherwise.
	 */
	bool read_bool(const var::String & access){ return api()->read_bool(&m_son, access.cstring()); }

	/*! \details Edits a float value.
	 *
	 * @param access The access code for the value
	 * @param v The new value to write
	 * @return Zero on success
	 */
	int edit(const var::String & access, float v){ return api()->edit_float(&m_son, access.cstring(), v); }

	/*! \details Edits a data value.
	 *
	 * @param access The access code for the value
	 * @param data A pointer to the source data
	 * @param size The number of bytes to edit
	 * @return Zero on success
	 *
	 * If \a size exceeds the size of the orginal data object, the new data
	 * will be truncated to that size.
	 *
	 */
	int edit(const var::String & access,  const void * data, son_size_t size){ return api()->edit_data(&m_son, access.cstring(), data, size); }

	/*! \details Edits a string value.
	 *
	 * @param access The acces for for the value
	 * @param str The new string
	 * @return Zero on success
	 *
	 * If the length of the new string exceeds the length of the original
	 * string, the new string will be truncated to fit.
	 *
	 */
	int edit(const var::String & access, const var::String & str){ return api()->edit_str(&m_son, access.cstring(), str.cstring()); }

	/*! \details Edits a number value (signed 32-bit).
	 *
	 * @param access The access for for the value
	 * @param v The new value
	 * @return Zero on success
	 */
	int edit(const var::String & access, s32 v){ return api()->edit_num(&m_son, access.cstring(), v); }

	/*! \details Edits a number value (unsigned 32-bit).
	 *
	 * @param access The access for for the value
	 * @param v The new value
	 * @return Zero on success
	 */
	int edit(const var::String & access, u32 v){ return api()->edit_unum(&m_son, access.cstring(), v); }

	/*! \details Edits a boolean value.
	 *
	 * @param access The access for for the value
	 * @param v The new value
	 * @return Zero on success
	 */
	int edit(const var::String & access, bool v){ return api()->edit_bool(&m_son, access.cstring(), v); }

	typedef enum {
		ERR_NONE /*! This value indicates no error has occurred. */ = SON_ERR_NONE,
		ERR_NO_ROOT /*! This error happens when a top level object or array is not created. */ = SON_ERR_NO_ROOT,
		ERR_OPEN_IO /*! This error happens if there is an IO failure to create or open a file (run perror() for more info). */ = SON_ERR_OPEN_IO,
		ERR_READ_IO /*! This error happens when an IO read operation fails (run perror() for more info). */ = SON_ERR_READ_IO,
		ERR_WRITE_IO /*! This error happens when an IO write operation fails (run perror() for more info). */ = SON_ERR_WRITE_IO,
		ERR_CLOSE_IO /*! This error happens when an IO close operation fails (run perror() for more info). */ = SON_ERR_CLOSE_IO,
		ERR_SEEK_IO /*! This error happens when an IO seek operation fails (run perror() for more info). */ = SON_ERR_SEEK_IO,
		ERR_READ_CHECKSUM /*! This error happens when the data in the file has a invalid checksum which indicates a corrupted file or bad file format. */ = SON_ERR_READ_CHECKSUM,
		ERR_CANNOT_APPEND /*! This error happens when an append is attempted on a file that has not been saved for appending */ = SON_ERR_CANNOT_APPEND,
		ERR_CANNOT_WRITE /*! This error happens if a write is attempted on a file that has been opened for reading */ = SON_ERR_CANNOT_WRITE,
		ERR_CANNOT_READ /*! This error happens if a read is attempted on a file that has been opened for writing or appending */ = SON_ERR_CANNOT_READ,
		ERR_INVALID_ROOT /*! This error happens when the root object is not valid (usually a bad file format or corrupted file). */ = SON_ERR_INVALID_ROOT,
		ERR_ARRAY_INDEX_NOT_FOUND /*! This error happens when an array index could not be found */ = SON_ERR_ARRAY_INDEX_NOT_FOUND,
		ERR_ACCESS_TOO_LONG /*! This error happens if the \a access parameter len exceeds \a SON_ACCESS_MAX_USER_SIZE.  */ = SON_ERR_ACCESS_TOO_LONG,
		ERR_KEY_NOT_FOUND /*! This error happens when the key specified by the \a access parameter could not be found. */ = SON_ERR_KEY_NOT_FOUND,
		ERR_STACK_OVERFLOW /*! This error happens if the depth (son_open_array() or son_open_object()) exceeds, the handle's stack size. */ = SON_ERR_STACK_OVERFLOW,
		ERR_INVALID_KEY /*! This happens if an empty key is passed to anything but the root object. */ = SON_ERR_INVALID_KEY,
		ERR_CANNOT_CONVERT /*! This happens if a read is tried by the base data can't be converted */ = SON_ERR_CANNOT_CONVERT,
		ERR_EDIT_TYPE_MISMATCH /*! This happens if a value is edited with a function that doesn't match the base type */ = SON_ERR_EDIT_TYPE_MISMATCH
	} son_err_t;

	/*! \details Returns the current error value. */
	int err() const { return m_son.err; }

	/*! \details Gets the current error value.
	 *
	 * @return The SON error value
	 *
	 * The current error value is returned. This method will clear the
	 * error value such that if it is called twice it will
	 * return the error then it will return SON_ERR_NONE.
	 *
	 */
	int get_error(){ return api()->get_error(&m_son); }

	/*! \details Access the son_t data object. */
	son_t & son(){ return m_son; }

	operator son_t & () { return m_son; }

	static const var::String get_type_description(u8 type){
		switch(type){
			case SON_STRING: return "str";
			case SON_FLOAT: return "float";
			case SON_NUMBER_U32: return "u32";
			case SON_NUMBER_S32: return "s32";
			case SON_FALSE: return "false";
			case SON_NULL: return "null";
			case SON_TRUE: return "true";
			case SON_OBJECT: return "object";
			case SON_ARRAY: return "array";
			default: return "unknown";
		}
	}

	u16 max_depth() const { return m_stack_size; }
	u16 stack_size() const { return m_stack_size; }

	static SonApi & api(){ return m_api; }
private:
	son_t m_son;
	son_stack_t * m_stack;
	u16 m_stack_size;
	bool m_is_stack_needs_free;

	static SonApi m_api;
};

}

/*! \endcond */


#endif /* SAPI_FMT_SON_HPP_ */
