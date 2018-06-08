/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DATA_HPP_
#define DATA_HPP_

#include <cstring>
#include <cstdio>
#include "../api/VarObject.hpp"

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
class Data : public api::VarWorkObject {
public:
	/*! \details Constructs a data object with no data */
	Data();

    /*! \details Copy constructor.
     *
     * @param a The data that will be copied into a new object
     *
     * If \a is dynamically allocated, the new object will
     * be a dynamically allocated and a copy of the contents.
     *
     * If \a a is read-only or externally managed, the new object
     * will be externally managed and point to the same memory.
     *
     * \code
     * Data a;
     * char static_buffer[32];
     *
     * a.set(static_buffer, 32);
     *
     * //b will be of size 32 and point to static_buffer
     * Data b = a;
     *
     * a.alloc(64);
     * a.fill(0x55);
     *
     * //c will have it's own 64 byte buffer that is filled with 0x55
     * Data c = a;
     *
     * \endcode
     *
     */
    Data(const Data & a);

    /*! \details Constant assignment operator.
     *
     * @param a The data to assign to the object.
     *
     * This works the same as the copy constructor.
     * Data that is internally managed will be newly allocated
     * in this object and copied. If \a a is externally managed,
     * this object will point to the same data.
     *
     * \sa is_internally_managed()
     *
     */
    Data& operator=(const Data & a);


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
    virtual void fill(unsigned char d);

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
        PRINT_CHAR /*! Print Characters */ = (1<<3),
        PRINT_8 /*! Print as 8 bit values (default) */ = 0,
        PRINT_16 /*! Print as 16 bit values */ = (1<<4),
        PRINT_32 /*! Print as 32 bit values */ = (1<<5),
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

    /*! \details Returns true if the data is internally managed. */
    bool is_internally_managed() const {
        return needs_free();
    }

    /*! \details Returns true if the data object is read only. */
    bool is_read_only() const {
        return m_mem_write == 0;
    }

    /*! \details Sets the object to transfer
     * the ownership when copied.
     *
     * If this method is called, the object will
     * not free dynamically allocated memory when destroyed.
     *
     * Also, if the object is copied, the copying object will take
     * ownership of dyncamilly allocated memory.
     *
     * This method is desiged to be used on objects that are used
     * to return a var::Data object.
     *
     * The following example, shows how this can be used with var::String,
     * which inherits var::Data.
     *
     * \code
     * String get_string(){
     *   String ret;
     *   ret.set_transfer_ownership();
     *   ret.assign("This is a String");
     *   return ret;
     * }
     *
     * String a = get_string(); //a takes ownership of pointer that ret creates
     * \endcode
     */
    void set_transfer_ownership(){
        m_o_flags |= FLAG_IS_TRANSFER_OWNERSHIP;
    }

    Data & transfer_ownership(){
        set_transfer_ownership();
        return *this;
    }

protected:
    void copy(const Data & a);

private:

    void set_needs_free() const { m_o_flags |= FLAG_NEEDS_FREE; }
    void clear_needs_free() const { m_o_flags &= ~FLAG_NEEDS_FREE; }

    bool needs_free() const { return m_o_flags & FLAG_NEEDS_FREE; }
    bool is_transfer_ownership() const { return m_o_flags & FLAG_IS_TRANSFER_OWNERSHIP; }
	void zero();

	static const int m_zero_value;

	const void * m_mem;
	void * m_mem_write;
	u32 m_capacity;

    enum {
        FLAG_NEEDS_FREE = (1<<0),
        FLAG_IS_TRANSFER_OWNERSHIP = (1<<1)
    };
    mutable u32 m_o_flags;

};

}



#endif /* DATA_HPP_ */
