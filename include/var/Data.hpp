/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DATA_HPP_
#define DATA_HPP_

#include "../api/SObject.hpp"

namespace var {

/*! \brief Data storage class
 * \details The Data storage class can be used to manage both statically
 * and dyncamilly allocated data.  The statically allocated data can be
 * either read/write or read-only.
 *
 * \code
 * #include <sapi/var.hpp>
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
class Data : public api::SObject {
public:
	/*! \details Constructs a data object with no data */
	Data();

	/*! \details Constructs a data object with non-mutable data pointers (not memory managed)
	 *
	 * @param mem A pointer to the data
	 * @param size The number of bytes available
	 * @param readonly If true, the data will be accessible as read-only
	 *
	 * The function does not manage the memory provided. The alloc() and free()
	 * methods will have no effect.
	 *
	 */
    Data(void * mem, u32 size, bool readonly = false);

    /*! \details Constructs data with dynamically allocated memory with \a size bytes (resizeable)
	 *
	 *  @param size The number of bytes to allocate
	 *
	 */
	Data(u32 size);

	/*! \details Deconstructs a Data object.
	 *
     * This method will free any data that was allocated dynamically by this object.
     *
	 *
	 */
	virtual ~Data();

	/*! \details Returns the minimum data storage size of any Data object. */
	static u32 minimum_size();

	/*! \details Sets the storage object of the data. This object
	 * will treat the data as statically allocated and will not free the memory
	 * if free() is called or the object is destroyed.
	 *
	 * @param mem A pointer to a fixed place in memory to use as storage
	 * @param size The number of bytes allocated
	 * @param readonly true if readonly (used for data stored in flash)
	 *
	 * If the memory is specified as read-only, data() will return 0 and data_const()
	 * will return a pointer to the memory.
	 *
	 * If this object had previously allocated memory dynamically, the memory will be freed
	 * when this method is called.
	 *
	 */
	void set(void * mem, u32 size, bool readonly = false);

	/*! \details Allocates (or reallocates) memory for the Data object.
	 *
	 * @param size The number of bytes to allocate
	 * @param resize If true, the old data is copied to the new data location
	 * @return Zero If the operation was successful and -1 if it failed
	 *
	 * If the memory was specified using the set() method or constructed as
	 * statically allocated memory, this will return an error.
	 */
	int alloc(u32 size, bool resize = false);

	/*! \details Resizes the data (equivalent to alloc()).
	 *
	 * @param size The number of new bytes
	 * @return Zero on success or -1 with errno set
	 *
	 * This is the same as alloc() with \a resize set to true
	 *
	 */
	int resize(u32 size) { return alloc(size, true); }

    /*! \details Dynamically allocates memory for the data object.
	 *
     * @param s The number of minimum bytes to allocate
	 * @return Zero on success
	 *
     * If the memory was specified using the set() method,
     * this will return an error.
	 *
	 * If the current capacity is less than \a s, the object will
	 * be resized. Otherwise, the data will
	 *
	 */
    int set_capacity(u32 s);

    /*! \details Returns a pointer to the data.
	 * This will return zero if the data is readonly.
	 *
	 * \sa set()
	 */
	void * data() const { return m_mem_write; }

    /*! \details Returns a char pointer to the data.
	 * This will return zero if the data is readonly.
	 *
	 * \sa set()
	 */
	char * cdata() const { return (char *)m_mem_write; }

    /*! \details Returns a pointer to const char data.
	 */
	const char * cdata_const() const { return (const char *)m_mem; }

    /*! \details Returns a pointer to const data. This will return the
	 * memory pointer whether the memory is read-only or read-write.
	 *
	 */
	const void * data_const() const { return m_mem; }

    /*! \details Returns the current capcity of the data storage object.
	 *
	 * @return Number of bytes in the data object
	 */
	u32 capacity() const { return m_capacity; }

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
     * var::String will return the size of the string rather
     * than the capacity of the data object.
	 *
	 * @return The number of bytes availabe in a data object
	 *
	 */
	virtual u32 calc_size() const { return m_capacity; }

    enum {
        PRINT_HEX /*! Print hex data */ = (1<<0),
        PRINT_UNSIGNED /*! Print unsigned integers */ = (1<<1),
        PRINT_SIGNED /*! Printd signed integers */ = (1<<2),
        PRINT_CHAR /*! Print Characters */ = (1<<3)
    };

    /*! \details Prints the data to the standard output. */
    virtual int print(u32 o_flags = PRINT_HEX) const;


    /*! \details Swaps the byte order of the data.
     *
     * @param size 4 to swap as 32-bit words, otherwise swap 16-bit words
     *
     * If the data is read-only, no change is made
     * and error_number() is set to EINVAL.
     *
     */
    void swap_byte_order(int size = 4);

private:

	void zero();

	static const int m_zero_value;

	const void * m_mem;
	void * m_mem_write;
	u32 m_capacity;
	bool m_needs_free;

};

}



#endif /* DATA_HPP_ */
