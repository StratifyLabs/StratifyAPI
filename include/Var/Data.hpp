/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DATA_HPP_
#define DATA_HPP_

#include <sys/types.h>

namespace Var {

/*! \brief Data storage class
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

#ifndef __MCU_ONLY__
	virtual ~Data();
#endif

	/*! \details The minimum data storage size of any data object */
	static size_t min_size();

	/*! \details Set the storage object of the data
	 *
	 * @param mem A pointer to a fixed place in memory to use as storage
	 * @param s The number of bytes allocated
	 * @param readonly true if readonly (used for data stored in flash)
	 */
	void set(void * mem, size_t s, bool readonly = false);

	/*! \details Reallocate the data storage
	 *
	 * @param s The number of bytes to allocate
	 * @param resize if true, the old data is copied to the new data location
	 * @return
	 */
	int alloc(size_t s, bool resize = false);

	/*! \details Resize the data
	 *
	 * This is the same as alloc() with \a resize set to true
	 *
	 * @param s The number of new bytes
	 * @return Zero on success or -1 with errno set
	 */
	inline int resize(size_t s) { return alloc(s, true); }

	/*! \details Set the minimum capacity of the data storage area
	 *
	 * If the current capacity is less than \a s, the object will
	 * be resized.
	 *
	 * @param s The number of minimum bytes needed
	 * @return Zero on success
	 */
	int set_capacity(size_t s);
	inline int mincapacity(size_t s){ return set_capacity(s); }

	/*! \details Retrieve a pointer to the data.
	 * This will return zero if the data is readonly.
	 */
	inline void * data() const __attribute__((always_inline)) { return mem_write; }
	/*! \details Retrieve a char pointer to the data.
	 * This will return zero if the data is readonly.
	 */
	inline char * cdata() const __attribute__((always_inline)) { return (char *)mem_write; }
	/*! \details Retrieve a pointer to const char data.
	 */
	inline const char * cdata_const() const { return (const char *)mem; }
	/*! \details Retrieve a pointer to const data.
	 */
	inline const void * data_const() const { return mem; }

	/*! \details This method will return the current capcity of the data storage object.
	 *
	 * @return Number of bytes in the data object
	 */
	inline size_t capacity() const { return _capacity; }
	int free();

	/*! \details Write all zeros to the data.
	 * This will not attempt to write read-only data.
	 */
	void clear(void);

	/*! \details Fill the data with the specified value.
	 * This will not attempt to write read-only data.
	 *
	 * @param The value to write to the data
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
	virtual size_t size() const { return _capacity; }

private:

	void zero(void);

	static const int zero_value;

	const void * mem;
	void * mem_write;
	size_t _capacity;
	bool needs_free;

};

};



#endif /* DATA_HPP_ */
