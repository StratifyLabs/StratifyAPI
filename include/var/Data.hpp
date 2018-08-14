/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef DATA_HPP_
#define DATA_HPP_

#include <cstring>
#include <cstdio>
#include "../api/VarObject.hpp"

#if !defined __link
#include <malloc.h>
#endif


namespace var {

#if !defined __link
class DataInfo : public api::InfoObject {
public:
    DataInfo(){ refresh(); }

    void refresh(){ m_info = mallinfo(); }

    u32 arena() const { return m_info.arena; }
    u32 free_block_count() const { return m_info.ordblks; }
    u32 free_size() const { return m_info.fordblks; }
    u32 used_size() const { return m_info.uordblks; }

#if 0
    mi.arena = (total_chunks) * MALLOC_CHUNK_SIZE + (sizeof(malloc_chunk_t) - MALLOC_DATA_SIZE);
    mi.ordblks = total_free_chunks;
    mi.fordblks = total_free_chunks * MALLOC_CHUNK_SIZE;
    mi.uordblks = total_used_memory;
#endif

private:
    struct mallinfo m_info;
};
#endif

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
 * Data data_dynamic(16); //this will dynamically allocate at least 16 bytes (see alloc())
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
 * Data objects that are dynamically allocated and returned from a method
 * can use the set_transfer_ownership() method to pass the interal pointer
 * to a new object rather that making a copy and freeing the temporary object.
 *
 * \code
 *
 * #include <sapi/var.hpp>
 *
 * Data my_data_function(){
 *   Data result;
 *   result.set_capacity(128);
 *   result.fill(0xaa);
 *   result.set_transfer_ownership();
 *   return result;
 * }
 *
 * //here the result will not be freed but the pointer will be taken over by my_data
 *
 * Data my_data = my_data_function();
 *
 * my_data_function(); //this the ownership transfer is not complete, the result data will be freed right away
 *
 * \endcode
 *
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

    bool operator == (const Data & a ) const {
        if( a.size() == size() ){
            return memcmp(cdata_const(), a.cdata_const(), size()) == 0;
        }
        return false;
    }

    bool operator != (const Data & a ) const {
        return !(*this == a);
    }


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
    static u32 block_size();

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
     * be resized and possibly copied to a new location.
     *
     * If the capacity is more than \a s, the effective size
     * will be updated to \a s.
	 *
	 */
    int set_size(u32 s);

    //doesn't really make sense to set the capacity because it will always jump to the next largest block
    int set_capacity(u32 s){ return set_size(s); }


    /*!
     * \details Returns the effective size of the data.
     *
     * \return The effective size of the data object
     *
     * For dynamically allocated objects, the capacity will
     * be greater than or equal to the size.
     *
     * Inherited objects can override this value with additional
     * context. For example, var::String implements size()
     * to return the length of the string.  var::Vector returns
     * the number of items times the size of each item.
     *
     *
     */
    virtual u32 size() const { return m_size; }

    /*! \details Returns the current capcity of the data storage object.
     *
     * @return Number of bytes in the data object
     *
     * The capacity of the object will always be greater than
     * or equal to size().
     *
     *
     */
    u32 capacity() const { return m_capacity; }

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
     * @param size 4 to swap as 32-bit words, otherwise swap 16-bit words (default is 4)
     *
     * If the data is read-only, no change is made
     * and error_number() is set to EINVAL.
     *
     * On Cortex-M chips this method makes use of the built-in byte
     * swapping instructions (so it is fast).
     *
     * \code
     * #include <sapi/var.hpp>
     * #include <sapi/hal.hpp>
     *
     * Spi spi(0);
     * Data buffer(16);
     * spi.init();
     * spi.read(buffer);
     *
     * //assume the spi outputs big endian data -- swaps 32-bit words
     * buffer.swap_byte_order();
     * buffer.swap_byte_order(4); //this is the same as calling swap_byte_order()
     *
     * //or if the spi is big endian but uses 16-bit words
     * buffer.swap_byte_order(2);
     * \endcode
     *
     *
     */
    void swap_byte_order(int size = 4);

    /*! \details Returns true if the data is internally managed.
     *
     * The data is internally managed if it has been dynamically allocated.
     *
     * \code
     *
     * u8 my_buffer[16];
     *
     * Data my_data_buffer(my_buffer, 16); //
     * my_data_buffer.is_internally_managed(); //false because my buffer was provided externally
     * Data my_data(16);
     * my_data.is_internally_managed(); //true because 16 bytes were allocated internally
     *
     * \endcode
     *
     */
    bool is_internally_managed() const {
        return needs_free();
    }

    /*! \details Returns true if the data object is read only.
     *
     * Data objects are only read-only if they are set that way.
     * Only externally managed data can be read-only.
     *
     * \code
     * u8 my_buffer[16];
     * Data my_readonly_buffer(my_buffer, 16, true); //marked as read-only
     * Data my_readwrite_buffer0(my_buffer, 16, false); //marked as read-write
     * Data my_readwrite_buffer1(my_buffer, 16); //same as line above
     * Data my_interally_managaed_data(64);
     *
     * my_readonly_buffer.is_read_only(); //true
     * my_readwrite_buffer0.is_read_only(); //false
     * my_readwrite_buffer1.is_read_only(); //false
     * my_interally_managaed_data.is_read_only(); //false
     *
     * \endcode
     *
     * Calling read/write operations (fill(), clear(), swap_byte_order()) on read-only data will
     * have no effect.
     *
     *
     */
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


protected:
    void copy(const Data & a);

private:

    friend class ConstString;

    void set_needs_free() const { m_o_flags |= FLAG_NEEDS_FREE; }
    void clear_needs_free() const { m_o_flags &= ~FLAG_NEEDS_FREE; }

    bool needs_free() const { return m_o_flags & FLAG_NEEDS_FREE; }
    bool is_transfer_ownership() const { return m_o_flags & FLAG_IS_TRANSFER_OWNERSHIP; }
	void zero();

	static const int m_zero_value;

	const void * m_mem;
	void * m_mem_write;
	u32 m_capacity;
    u32 m_size;

    enum {
        FLAG_NEEDS_FREE = (1<<0),
        FLAG_IS_TRANSFER_OWNERSHIP = (1<<1)
    };
    mutable u32 m_o_flags;

};

}



#endif /* DATA_HPP_ */
