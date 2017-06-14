/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DATA_HPP_
#define DATA_HPP_

#include <sys/types.h>

namespace var {

/*! \brief Data storage class
 * \details The Data storage class can be used to manage both statically
 * and dyncamilly allocated data.  The statically allocated data can be
 * either read/write or read-only.
 *
 * \code
 * #include <stfy/var.hpp>
 *
 *
 * const char buffer_const[16];
 * char buffer[16];
 *
 * Data empty; //the size of the data is zero. data_const() will return a pointer to a zero-length null-terminated string
 * Data data_dynamic(16); //this will dynamically allocate 16 bytes (see alloc())
 * Data data_static(buffer, 16); //will use buffer and consider it statically allocated (see set())
 * Data data_readonly(buffer_const, 16, true); //this will consider the data read only -- calls to data() will return 0
 *
 * if( data_readonly.data() == 0 ){
 * 	printf("Data is read only\n"); //this will print
 * }
 *
 * printf("This is empty data %s\n", empty.cdata_const()); //this is OK because empty data will return a valid string pointer
 *
 * \endcode
 *
 *
 */
class Data {
public:
	/*! \details Data object with no data */
	Data();
	/*! \details Data object with non-mutable data (not memory managed) */
	Data(void * mem, size_t s, bool readonly = false); //read/write bitmap
	/*! \details Data oject with dynamically allocated memory with \a s bytes (resizeable) */
	Data(size_t s);

	/*! \details The deconstructor will free any dynamically allocated memory associated with the object */
	virtual ~Data();

	/*! \details The minimum data storage size of any data object */
	static size_t min_size();

	/*! \details Set the storage object of the data. This object
	 * will treat the data as statically allocated and will not free the memory
	 * if free() is called or the object is destroyed.
	 *
	 * If the memory is specified as read-only, data() will return 0 and data_const()
	 * will return a pointer to the memory.
	 *
	 * If this object had previously allocated memory dynamically, the memory will be freed
	 * when this method is called.
	 *
	 * @param mem A pointer to a fixed place in memory to use as storage
	 * @param s The number of bytes allocated
	 * @param readonly true if readonly (used for data stored in flash)
	 */
	void set(void * mem, size_t s, bool readonly = false);

	/*! \details Reallocate the data storage
	 *
	 * If the memory was specified using the set() method, this will return an error.
	 *
	 * @param s The number of bytes to allocate
	 * @param resize if true, the old data is copied to the new data location
	 * @return Zero if the operation was successful and -1 if it failed
	 */
	int alloc(size_t s, bool resize = false);

	/*! \details Resize the data
	 *
	 * This is the same as alloc() with \a resize set to true
	 *
	 * @param s The number of new bytes
	 * @return Zero on success or -1 with errno set
	 */
	int resize(size_t s) { return alloc(s, true); }

	/*! \details Set the minimum capacity of the data storage area.
	 *
	 * If the memory was specified using the set() method, this will return an error.
	 *
	 * If the current capacity is less than \a s, the object will
	 * be resized.
	 *
	 * @param s The number of minimum bytes needed
	 * @return Zero on success
	 */
	int set_min_capacity(size_t s);

	/*! \details Retrieve a pointer to the data.
	 * This will return zero if the data is readonly.
	 *
	 * \sa set()
	 */
	void * data() const { return m_mem_write; }

	/*! \details Retrieve a char pointer to the data.
	 * This will return zero if the data is readonly.
	 *
	 * \sa set()
	 */
	char * cdata() const { return (char *)m_mem_write; }

	/*! \details Retrieve a pointer to const char data.
	 */
	const char * cdata_const() const { return (const char *)m_mem; }

	/*! \details Retrieve a pointer to const data. This will return the
	 * memory pointer whether the memory is read-only or read-write.
	 *
	 */
	const void * data_const() const { return m_mem; }

	/*! \details This method will return the current capcity of the data storage object.
	 *
	 * @return Number of bytes in the data object
	 */
	size_t capacity() const { return m_capacity; }

	/*! \details Free the memory associated with this object.
	 * This will only perform any operations if the memory was
	 * dynamically allocating using this object.
	 *
	 * @return
	 */
	int free();

	/*! \details Write all zeros to the data.
	 * This will not attempt to write read-only data.
	 */
	void clear();

	/*! \details Fill the data with the specified value.
	 * This will not attempt to write read-only data.
	 *
	 * @param d The value to write to the data
	 *
	 */
	void fill(unsigned char d);

	/*! \details For top level data objects this is the
	 * same as capacity().  Other objects may re-implement
	 * to change how much user data is available.  For example,
	 * Var::String will return the size of the string rather
	 * than the capacity of the data object
	 *
	 * @return The number of bytes availabe in a data object
	 *
	 */
	virtual size_t calc_size() const { return m_capacity; }

private:

	void zero();

	static const int m_zero_value;

	const void * m_mem;
	void * m_mem_write;
	size_t m_capacity;
	bool m_needs_free;

};

};



#endif /* DATA_HPP_ */
