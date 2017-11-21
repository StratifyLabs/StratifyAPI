/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SON_HPP_
#define SON_HPP_

#include <mcu/types.h>
#include <cstring>
#include <unistd.h>
#include <sapi/son.h>

#if defined __link
#include <sos/link.h>
#else
#include "../var/String.hpp"
#endif

namespace fmt {

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
 * 		son.open_obj("");
 * 			son.write("name", "Stratify"); //create a string
 * 			son.write("date", "today"); //create another string
 * 			son.write("value", (u32)100); //write a number);
 * 			son.open_obj("time"); //create a new object
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
template<int stacksize> class Son {
public:

	/*! \details Constructs a new SON object. */
	Son(){
		memset(&m_son, 0, sizeof(m_son));
#if defined __link
		set_driver(0);
#endif
	}

#if defined __link
	void set_driver(void * driver){ son_set_driver(&m_son, driver); }
	//deprecated
	void set_handle(void * handle){ set_driver(handle); }
#else
	static void reset(son_t * son){ lseek(son->phy.fd, 0, SEEK_SET); }
	void reset(){ reset(&m_son); }
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
	int create(const char * name){ return son_api()->create(&m_son, name, m_stack, stacksize); }

	/*! \details Creates a memory image SON object.
	 *
	 * @param image The memory image
	 * @param nbyte The number of bytes in the image
	 *
	 * @return Zero on success
	 */
	int create_image(void * image, int nbyte){ return son_api()->create_image(&m_son, image, nbyte, m_stack, stacksize); }

	/*! \details Creates a memory image SON object using dynamic memory allocation.
	 *
	 * @param nbyte The number of bytes in the image
	 *
	 * @return Zero on success
	 */
	int create_image(int nbyte){ return son_api()->create(&m_son, 0, nbyte); }


	/*! \details Opens a file for appending data.
	 *
	 * @param name The path/name of the file to open
	 * @return Less than zero for an error
	 */
	int open_append(const char * name){ return son_api()->append(&m_son, name, m_stack, stacksize); }

	/*! \details Opens a SON file for reading.
	 *
	 * @param name Name of the file
	 * @return Zero on success
	 */
	int open_read(const char * name){ return son_api()->open(&m_son, name); }

	/*! \details Opens a SON image for reading.
	 *
	 * @param image The memory image
	 * @param nbyte The number of bytes in the image
	 * @return Zero on success
	 */
	int open_read_image(void * image, int nbyte){ return son_api()->open_image(&m_son, image, nbyte); }

	/*! \details Opens a SON file for editing.
	 *
	 * @param name Name of the file
	 * @return Zero on success
	 */
	int open_edit(const char * name){ return son_api()->edit(&m_son, name); }

	/*! \details Opens a SON image for editing.
	 *
	 * @param image The memory image
	 * @param nbyte The number of bytes in the image
	 * @return Zero on success
	 */
	int open_edit_image(void * image, int nbyte){ return son_api()->edit_image(&m_son, image, nbyte); }

	/*! \details Closes a SON file. */
	int close(){ return son_api()->close(&m_son); }

	/*! \details Seeks to the location of the access code and gets the size of the data in bytes.
	 *
	 * @param access The access code to seek to
	 * @param data_size A pointer to write the data size to (null if not needed)
	 * @return Zero on success
	 */
	int seek(const char * access, son_size_t * data_size){ return son_api()->seek(&m_son, access, data_size); }
	/*! \details Converts the data file to JSON
	 *
	 * @param path The path to a file to create with the JSON data
	 * @return Zero on success
	 */
	int to_json(const char * path){ return son_api()->to_json(&m_son, path); }

	/*! \details Opens a new object while writing or appending.
	 *
	 * @param key The key to use for the new object
	 * @return Zero on success
	 */
	int open_obj(const char * key){ return son_api()->open_obj(&m_son, key); }
	/*! \details Closes an object while writing or appending a file.
	 *
	 */
	int close_obj(){ return son_api()->close_obj(&m_son); }

	/*! \details Opens a new array while writing or appending.
	 *
	 * @param key The key to use for the new array
	 * @return Zero on success
	 */
	int open_array(const char * key){ return son_api()->open_array(&m_son, key); }

	/*! \details Closes an array while writing or appending.
	 *
	 * @return Zero on success
	 *
	 * To proper operation, this a call to this method must correspond to a
	 * call to open_data().
	 */
	int close_array(){ return son_api()->close_array(&m_son); }

	/*! \details Opens a data object while writing or appending.
	 *
	 * @param key The key to use for the new data
	 * @return Zero on success
	 */
	int open_data(const char * key){ return son_api()->open_data(&m_son, key); }

	/*! \details Closes a data object.
	 *
	 * @return Zero on success
	 *
	 * To proper operation, this a call to this method must correspond to a
	 * call to open_data().
	 *
	 */
	int close_data(){ return son_api()->close_data(&m_son); }

	/*! \details Writes a key/string pair to the file.
	 *
	 * @param key The value key
	 * @param v A pointer to the string
	 * @return Number of bytes in the value portion to be successfully stored
	 */
	int write(const char * key, const char * v){
		if( v ){
			return son_api()->write_str(&m_son, key, v);
		} else {
			return son_api()->write_null(&m_son, key);
		}
	}

#if !defined __link
	/*! \details Writes a key/string pair to the file.
	 *
	 * @param key The value key
	 * @param v A var::String reference
	 * @return Number of bytes in the value portion to be successfully stored
	 */
	int write(const char * key, const var::String & v){
		return son_api()->write_str(&m_son, key, v.c_str());
	}
#endif

	/*! \details Writes a key/number pair to the file (s32).
	 *
	 * @param key The value key
	 * @param v The number to write
	 * @return Number of bytes stored (4) if successful
	 */
	int write(const char * key, s32 v){ return son_api()->write_num(&m_son, key, v); }

	/*! \details Writes a key/number pair to the file (u32).
	 *
	 * @param key The value key
	 * @param v The number to write
	 * @return Number of bytes stored (4) if successful
	 */
	int write(const char * key, u32 v){ return son_api()->write_unum(&m_son, key, v); }

	/*! \details Writes a key/number pair to the file (float).
	 *
	 * @param key The value key
	 * @param v The number to write
	 * @return Number of bytes stored (4) if successful
	 */
	int write(const char * key, float v){ return son_api()->write_float(&m_son, key, v); }

	/*! \details Writes a key/bool pair to the file.
	 *
	 * @param key The value key
	 * @param v true or false
	 * @return Number of bytes stored if successful
	 */
	int write(const char * key, bool v){
		if( v ){
			return son_api()->write_true(&m_son, key);
		} else {
			return son_api()->write_false(&m_son, key);
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
	int write(const char * key, const void * v, son_size_t size){ return son_api()->write_data(&m_son, key, v, size); }

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
	int write_open_data(const void * v, son_size_t size){ return son_api()->write_open_data(&m_son, v, size); }

	/*! \details Reads the specified key as a string.  If the original
	 * key was not written as a string, it will be converted to a string.  For example,
	 * a (u32)100 will be converted to "100".  A data object will be converted to base64 encoding.
	 *
	 * @param access Key parameters
	 * @param str Pointer to the destination data
	 * @param capacity Size of \a str buffer
	 * @return The number of bytes actually read
	 */
	int read_str(const char * access, char * str, son_size_t capacity){ return son_api()->read_str(&m_son, access, str, capacity); }

#if !defined __link
	/*! \details Reads the specified key as a string.  If the original
	 * key was not written as a string, it will be converted to a string.  For example,
	 * a (u32)100 will be converted to "100".  A data object will be converted to base64 encoding.
	 *
	 * @param access Key parameters
	 * @param str var::String reference
	 * @return The number of bytes actually read
	 */
	int read_str(const char * access, var::String & str){
		return son_api()->read_str(&m_son, access, str.cdata(), str.capacity());
	}
#endif

	/*! \details Reads the specified key as a number (s32).  If the original
	 * key was not written as a s32, it will be converted to one.  A string
	 * of "100" will be converted to (s32)100.  Objects that cannot be converted will return 0.
	 *
	 * @param access Key parameters
	 * @return The number
	 */
	s32 read_num(const char * access){ return son_api()->read_num(&m_son, access); }

	/*! \details Reads the specified key as a number (u32).  If the original
	 * key was not written as a s32, it will be converted to one.  A string
	 * of "100" will be converted to (u32)100.  Objects that cannot be converted will return 0.
	 *
	 * @param access Key parameters
	 * @return The number
	 */
	u32 read_unum(const char * access){ return son_api()->read_unum(&m_son, access); }

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
	float read_float(const char * access){ return son_api()->read_float(&m_son, access); }

	/*! \details Reads the specified key as data.  Regardless of the storage
	 * type, the key will be returned as binary data.
	 *
	 * @param access Key parameters
	 * @param data A pointer to the destination data
	 * @param size The number of bytes to read
	 * @return The number of bytes read
	 */
	int read_data(const char * access, void * data, son_size_t size){ return son_api()->read_data(&m_son, access, data, size); }

	/*! \details Reads the specified key as a bool.
	 *
	 * @param access Key parameters
	 * @return True if the key is found and is true; false otherwise.
	 */
	bool read_bool(const char * access){ return son_api()->read_bool(&m_son, access); }

	/*! \details Edits a float value.
	 *
	 * @param access The access code for the value
	 * @param v The new value to write
	 * @return Zero on success
	 */
	int edit(const char * access, float v){ return son_api()->edit_float(&m_son, access, v); }

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
	int edit(const char * access,  const void * data, son_size_t size){ return son_api()->edit_data(&m_son, access, data, size); }

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
	int edit(const char * access, const char * str){ return son_api()->edit_str(&m_son, access, str); }

	/*! \details Edits a string value.
	 *
	 * @param access The acces for for the value
	 * @param v The new string
	 * @return Zero on success
	 *
	 * If the length of the new string exceeds the length of the original
	 * string, the new string will be truncated to fit.
	 *
	 */
	int edit(const char * access, const var::String & v){ return son_api()->edit_str(&m_son, access, v.c_str()); }

	/*! \details Edits a number value (signed 32-bit).
	 *
	 * @param access The access for for the value
	 * @param v The new value
	 * @return Zero on success
	 */
	int edit(const char * access, s32 v){ return son_api()->edit_num(&m_son, access, v); }

	/*! \details Edits a number value (unsigned 32-bit).
	 *
	 * @param access The access for for the value
	 * @param v The new value
	 * @return Zero on success
	 */
	int edit(const char * access, u32 v){ return son_api()->edit_unum(&m_son, access, v); }

	/*! \details Edits a boolean value.
	 *
	 * @param access The access for for the value
	 * @param v The new value
	 * @return Zero on success
	 */
	int edit(const char * access, bool v){ return son_api()->edit_bool(&m_son, access, v); }

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
		ERR_STACK_OVERFLOW /*! This error happens if the depth (son_open_array() or son_open_obj()) exceeds, the handle's stack size. */ = SON_ERR_STACK_OVERFLOW,
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
	int get_error(){ return son_api()->get_error(&m_son); }

	/*! \details Access the son_t data object. */
	son_t & son(){ return m_son; }

private:
	son_t m_son;
	son_stack_t m_stack[stacksize];
};

};

#endif /* SON_HPP_ */
