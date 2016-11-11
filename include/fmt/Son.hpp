/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SON_HPP_
#define SON_HPP_

#include <mcu/types.h>
#include <cstring>
#include <unistd.h>
#include <stfy/son.h>

namespace fmt {

/*! \brief Stratify Object Notation
 *
 * \details The SON (Stratify Object Notation) is an important class to be familiar with.  It is
 * closely related to JSON (and very closely related to BSON).  It allows you to store, access, and modify
 * data in the same way you but is designed for use on resource constrained systems.
 *
 * Important things to note:
 * - Key names are limited to 7 characters (longer names are truncated)
 * - The stacksize template value is the depth limit when creating files
 * - You can only append to files that are left with an open ended array
 * - You can't modify the length of strings or data objects within a file
 * - You must create "root" as the first object or array
 *
 * Creation example:
 *
 * \code
 *
 * #include <stfy/Fmt.hpp>
 *
 * int main(int argc, char * argv[]){
 * 	Son son<4>; //depth is limited to 4
 *
 *	//tabs are added to help visualize the parallel to JSON
 * 	son.create("/home/settings.son");
 * 		son.open_obj("root");
 * 			son.write("name", "Stratify"); //create a string
 * 			son.write("date", "today"); //makes sense
 * 			son.write("value", (u32)100); //write a number);
 * 			son.open_obj("stuff");
 * 				son.write("hour", 12);
 * 				son.write("min", 00);
 * 				son.write("sec", 59);
 * 			son.close_obj();
 * 		son.close_obj();
 * 	son.close();
 *
 * 	char buffer[32];
 * 	u32 value;
 * 	son.open("/home/settings.son"); //opens read-only
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
 *  The settings.json files looks like:
 *
 *      {
 *          "name": "Stratify",
 *          "date": "today",
 *          "value": "100",
 *          "stuff": {
 *              "hour": "12",
 *              "min": "0",
 *              "sec": "59"
 *           }
 *      }
 *
 *
 *
 */
template<int stacksize> class Son {
public:
	Son(){ memset(&m_son, 0, sizeof(m_son)); }

#if defined __link
	void set_handle(link_phy_t handle){ son_set_handle(&m_son, handle); }
#else
	void reset(son_t * obj){ lseek(obj->phy.fd, 0, SEEK_SET); }
#endif

	/*! \brief The file descriptor of the SON file.
	 *
	 * @return SON File descriptor
	 */
	int fileno() const {
#if defined __link
		if( m_son.phy.handle == 0 ){
			return 0;
		} else {
			return m_son.phy.fd;
		}
#else
		return m_son.phy.fd;
#endif
	}

	/*! \details Create a new SON file
	 *
	 * @param name The name of the file
	 * @return Zero on success
	 */
	int create(const char * name){ return son_create(&m_son, name, m_stack, stacksize); }
	int append(const char * name){ return son_append(&m_son, name, m_stack, stacksize); }

	/*! \details Open a SON file for reading
	 *
	 * @param name Name of the file
	 * @return Zero on success
	 */
	int open(const char * name){ return son_open(&m_son, name); }


	int close(bool close_all = false){ return son_close(&m_son, close_all); }
	int seek(const char * access, son_size_t * data_size){ return son_seek(&m_son, access, data_size); }
	/*! \brief Convert the data file to JSON */
	int to_json(const char * path){ return son_to_json(&m_son, path); }

	/*! \details Open an object */
	int open_obj(const char * key){ return son_open_obj(&m_son, key); }
	/*! \details Close an object */
	int close_obj(){ return son_close_obj(&m_son); }

	/*! \details Open an object */
	int open_array(const char * key, int fixed_size = 0){ return son_open_array(&m_son, key, fixed_size); }

	/*! \details Close an array */
	int close_array(){ return son_close_array(&m_son); }

	/*! \details Open a data object */
	int open_data(const char * key){ return son_open_data(&m_son, key); }
	/*! \details Close a data object */
	int close_data(){ return son_close_data(&m_son); }

	/*! \details Write a key/string pair to the file
	 *
	 * @param key The value key
	 * @param v A pointer to the string
	 * @return Number of bytes in the value portion to be successfully stored
	 */
	int write(const char * key, const char * v){
		if( v ){
			return son_write_str(&m_son, key, v);
		} else {
			return son_write_null(&m_son, key);
		}
	}

	/*! \details Write a key/number pair to the file (s32)
	 *
	 * @param key The value key
	 * @param v The number to write
	 * @return Number of bytes stored (4) if successful
	 */
	int write(const char * key, s32 v){ return son_write_num(&m_son, key, v); }

	/*! \details Write a key/number pair to the file (u32)
	 *
	 * @param key The value key
	 * @param v The number to write
	 * @return Number of bytes stored (4) if successful
	 */
	int write(const char * key, u32 v){ return son_write_unum(&m_son, key, v); }

	/*! \details Write a key/number pair to the file (float)
	 *
	 * @param key The value key
	 * @param v The number to write
	 * @return Number of bytes stored (4) if successful
	 */
	int write(const char * key, float v){ return son_write_float(&m_son, key, v); }

	/*! \details Write a key/bool pair to the file
	 *
	 * @param key The value key
	 * @param v true or false
	 * @return Number of bytes stored if successful
	 */
	int write(const char * key, bool v){
		if( v ){
			return son_write_true(&m_son, key);
		} else {
			return son_write_false(&m_son, key);
		}
	}

	/*! \details Write a key/data pair to the file.  The data
	 * is stored in the file in binary format
	 *
	 * @param key The value key
	 * @param v A pointer to the data to be written
	 * @param size The number of bytes to write
	 * @return Number of bytes stored (4) if successful
	 */
	int write(const char * key, const void * v, son_size_t size){ return son_write_data(&m_son, key, v, size); }

	/*! \details Add data to an open key.  This is used with open_data() and close_data().   These
	 * methods are useful for writing data to the file when the amount of data to be written is unknown
	 * when writing begins.
	 *
	 * \code
	 * #include <stfy/Var.hpp>
	 * Son son<4>;
	 *
	 * son.create("/home/data.son");
	 * son.open_data("datakey");
	 * son.write_open_data(buffer, 32);
	 * son.close_data();
	 * \endcode
	 *
	 * @param key The value key
	 * @param v A pointer to the data to be written
	 * @param size The number of bytes to write
	 * @return Number of bytes stored (4) if successful
	 */
	int write_open_data(const void * v, son_size_t size){ return son_write_open_data(&m_son, v, size); }

	/*! \details Read the specified key as a string.  If the original
	 * key was not written as a string, it will be converted to a string.  For example,
	 * a (u32)100 will be converted to "100".  A data object will be converted to base64 encoding.
	 *
	 * @param access Key parameters
	 * @param str Pointer to the destination data
	 * @param capacity Size of \a str buffer
	 * @return The number of bytes actually read
	 */
	int read_str(const char * access, char * str, son_size_t capacity){ return son_read_str(&m_son, access, str, capacity); }

	/*! \details Read the specified key as a number (s32).  If the original
	 * key was not written as a s32, it will be converted to one.  A string
	 * of "100" will be converted to (s32)100.  Objects that cannot be converted will return 0.
	 *
	 * @param access Key parameters
	 * @return The number
	 */
	s32 read_num(const char * access){ return son_read_num(&m_son, access); }

	/*! \details Read the specified key as a number (u32).  If the original
	 * key was not written as a s32, it will be converted to one.  A string
	 * of "100" will be converted to (u32)100.  Objects that cannot be converted will return 0.
	 *
	 * @param access Key parameters
	 * @return The number
	 */
	u32 read_unum(const char * access){ return son_read_unum(&m_son, access); }

	/*! \details Read the specified key as a number (float).  If the original
	 * key was not written as a s32, it will be converted to one.  A string
	 * of "100" will be converted to (float)100.  Objects that cannot be converted will return 0.
	 *
	 * @param access Key parameters
	 * @return The number
	 */
	float read_float(const char * access){ return son_read_float(&m_son, access); }

	/*! \details Read the specified key as data.  Regardless of the storage
	 * type, the key will be returned as binary data.
	 *
	 * @param access Key parameters
	 * @param data A pointer to the destination data
	 * @param size The number of bytes to read
	 * @return The number of bytes read
	 */
	int read_data(const char * access, void * data, son_size_t size){ return son_read_data(&m_son, access, data, size); }

	son_t & son(){ return m_son; }

private:
	son_t m_son;
	son_stack_t m_stack[stacksize];
};

};

#endif /* SON_HPP_ */
