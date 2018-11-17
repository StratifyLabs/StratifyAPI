/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_VAR_DATA_HPP_
#define SAPI_VAR_DATA_HPP_

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
 *
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
	Data(Data && a);

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
	Data& operator=(Data && a);

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
	Data(void * mem, u32 size);

	/*! \details Constructs a data object with non-mutable data pointers to constant memory.
	 *
	 * @param mem A pointer to constant data
	 * @param size The number of bytes for mem
	 *
	 *
	 */
	Data(const void * mem, u32 size);

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

	/*! \details Returns a pointer to the data (read/write)
	 * This will return zero if the data is readonly.
	 *
	 * \code
	 * Data a(64); //allocate 64 bytes of data
	 * u32 * value = a.to<u32>(); //casts data as u32*
	 * \endcode
	 *
	 * Many common types are implemented as a non-template
	 * function.
	 *
	 * See to_u8(), to_u16(), to_s32(), etc
	 *
	 */
	template<typename T> T * to(){ return (T*)m_mem_write; }

	/*! \details Casts data as a read-only pointer.
	 *
	 */
	template<typename T> const T * to() const { return (const T*)m_mem; }

	/*! \details Accesses a value in the data.
	 *
	 * @param T template typename
	 * @param idx index of the value to access
	 *
	 * If the index exceeds the size of the data, the index is set to 0.
	 *
	 * \code
	 * Data a(64); //a is 64 bytes
	 * a.at<char>(0) = 'a'; //assign 'a' to the first char location
	 * a.at<u32>(4) = 0xAAAA5555; //assigns a u32 value assuming a is a u32 array
	 * u32 value = a.at<u32>(4); //reads a value as if a is a u32 array
	 * \endcode
	 *
	 *
	 */
	template<typename T> T & at(u32 idx){
		if( idx*sizeof(T) >= size() ){
			//set the error number to overflow
			idx = 0;
		}
		return to<T>()[idx];
	}

	template<typename T> const T & at(u32 idx) const {
		if( idx*sizeof(T) >= size() ){
			//set the error number to overflow
			idx = 0;
		}
		return to<T>()[idx];
	}

	/*! \details Returns a pointer to the data as a `void*`. */
	void * to_void() { return to<void>(); }
	/*! \details Returns a pointer to the data as a `char*`. */
	char * to_char() { return to<char>(); }
	/*! \details Returns a pointer to the data as a `u8*`. */
	u8 * to_u8() { return to<u8>(); }
	/*! \details Returns a pointer to the data as a `s8*`. */
	s8 * to_s8() { return to<s8>(); }
	/*! \details Returns a pointer to the data as a `u16*`. */
	u16 * to_u16() { return to<u16>(); }
	/*! \details Returns a pointer to the data as a `s16*`. */
	s16 * to_s16() { return to<s16>(); }
	/*! \details Returns a pointer to the data as a `u32*`. */
	u32 * to_u32() { return to<u32>(); }
	/*! \details Returns a pointer to the data as a `s32*`. */
	s32 * to_s32() { return to<s32>(); }
	/*! \details Returns a pointer to the data as a `u64*`. */
	u64 * to_u64() { return to<u64>(); }
	/*! \details Returns a pointer to the data as a `s64*`. */
	s64 * to_s64() { return to<s64>(); }
	/*! \details Returns a pointer to the data as a `float*`. */
	float * to_float(){ return to<float>(); }

	/*! \details Returns a pointer to the data as a `const void*`. */
	const void * to_void() const { return to<void>(); }
	/*! \details Returns a pointer to the data as a `const char*`. */
	const char * to_char() const { return to<char>(); }
	/*! \details Returns a pointer to the data as a `const u8*`. */
	const u8 * to_u8() const { return to<u8>(); }
	/*! \details Returns a pointer to the data as a `const s8*`. */
	const s8 * to_s8() const { return to<s8>(); }
	/*! \details Returns a pointer to the data as a `const u16*`. */
	const u16 * to_u16() const { return to<u16>(); }
	/*! \details Returns a pointer to the data as a `const s16*`. */
	const s16 * to_s16() const { return to<s16>(); }
	/*! \details Returns a pointer to the data as a `const u32*`. */
	const u32 * to_u32() const { return to<u32>(); }
	/*! \details Returns a pointer to the data as a `const s32*`. */
	const s32 * to_s32() const { return to<s32>(); }
	/*! \details Returns a pointer to the data as a `const u64*`. */
	const u64 * to_u64() const { return to<u64>(); }
	/*! \details Returns a pointer to the data as a `const s64*`. */
	const s64 * to_s64() const { return to<s64>(); }
	/*! \details Returns a pointer to the data as a `const float*`. */
	const float * to_float() const { return to<float>(); }

	/*! \details Accesses a referece to a value in the data as a u8 array. */
	u8 & at_u8(u32 idx) { return at<u8>(idx); }
	/*! \details Accesses a referece to a value in the data as a s8 array. */
	s8 & at_s8(u32 idx) { return at<s8>(idx); }
	/*! \details Accesses a referece to a value in the data as a u16 array. */
	u16 & at_u16(u32 idx) { return at<u16>(idx); }
	/*! \details Accesses a referece to a value in the data as a s16 array. */
	s16 & at_s16(u32 idx) { return at<s16>(idx); }
	/*! \details Accesses a referece to a value in the data as a u32 array. */
	u32 & at_u32_at(u32 idx) { return at<u32>(idx); }
	/*! \details Accesses a referece to a value in the data as a s32 array. */
	s32 & at_s32(u32 idx) { return at<s32>(idx); }
	/*! \details Accesses a referece to a value in the data as a u64 array. */
	u64 & at_u64(u32 idx) { return at<u64>(idx); }
	/*! \details Accesses a referece to a value in the data as a s64 array. */
	s64 & at_s64(u32 idx) { return at<s64>(idx); }
	/*! \details Accesses a referece to a value in the data as a float array. */
	float & at_float(u32 idx) { return at<float>(idx); }

	/*! \details Accesses a referece to a value in the data as a u8 array. */
	const u8 & at_u8(u32 idx) const { return at<u8>(idx); }
	/*! \details Accesses a referece to a value in the data as a s8 array. */
	const s8 & at_s8(u32 idx) const { return at<s8>(idx); }
	/*! \details Accesses a referece to a value in the data as a u16 array. */
	const u16 & at_u16(u32 idx) const { return at<u16>(idx); }
	/*! \details Accesses a referece to a value in the data as a s16 array. */
	const s16 & at_s16(u32 idx) const { return at<s16>(idx); }
	/*! \details Accesses a referece to a value in the data as a u32 array. */
	const u32 & at_u32(u32 idx) const { return at<u32>(idx); }
	/*! \details Accesses a referece to a value in the data as a s32 array. */
	const s32 & at_s32(u32 idx) const { return at<s32>(idx); }
	/*! \details Accesses a referece to a value in the data as a u64 array. */
	const u64 & at_u64(u32 idx) const { return at<u64>(idx); }
	/*! \details Accesses a referece to a value in the data as a s64 array. */
	const s64 & at_s64(u32 idx) const { return at<s64>(idx); }
	/*! \details Accesses a referece to a value in the data as a float array. */
	const float & at_float(u32 idx) const { return at<float>(idx); }

	//deprecated access
	char * cdata() const { return (char *)m_mem_write; }
	const char * cdata_const() const { return (const char *)m_mem; }
	const void * data_const() const { return m_mem; }
	void * data(){ return m_mem_write; }
	const void * data() const { return m_mem; }

	/*! \details Free the memory associated with this object.
	 * This will only perform any operations if the memory was
	 * dynamically allocating using this object.
	 *
	 * @return Zero on success
	 */
	int free();

	//deprecated -- use fill(0);
	void clear();

	/*! \details Fill the data with the specified value.
	 * This will not attempt to write read-only data.
	 *
	 * @param d The value to write to the data
	 *
	 */
	void fill(unsigned char d);

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

	int copy_data(const void * buffer, u32 size);
	int copy_cstring(const char * str);


	/*! \details Copies the contents of a into the memory of
	 * this object.
	 *
	 * @param a Data to copy contents of
	 * @param size The number of bytes to copy
	 *
	 * The capacity() of this object must be greater
	 * than or equal to the size() of a.
	 *
	 * On successful copy, the size() of this object will
	 * be set to the \a size parameter.
	 *
	 *
	 */
	int copy_contents(const Data & a, u32 size);


	/*!
	 * \details Copies the contents of another data object.
	 * \param a The object whose contents will be copied
	 * \return Zero on success or less than zero if memory could not be allocated
	 *
	 */
	int copy_contents(const Data & a);

	/*!
	 * \details Copies the contents of another data object to this object.
	 * \param a The data object whose contents will be copied
	 * \param destination_position The offset in this object for the copy destination
	 * \param size The number of bytes to copy
	 * \return Zero on success or less than zero if memory could not be allocated
	 */
	int copy_contents(const Data & a, u32 destination_position, u32 size);

	/*! \details Append data to this object.
	 *
	 * @param a A reference to the data that will be appended.
	 * @return Zero on success or -1 if an error occurred (unable to allocate memory)
	 *
	 */
	int append(const Data & a){
		return copy_contents(a, size(), a.size());
	}

	int append(u8 value){ return append(Data((const void*)&value, sizeof(value))); }
	int append(s8 value){ return append(Data((const void*)&value, sizeof(value))); }
	int append(u16 value){ return append(Data((const void*)&value, sizeof(value))); }
	int append(s16 value){ return append(Data((const void*)&value, sizeof(value))); }
	int append(u32 value){ return append(Data((const void*)&value, sizeof(value))); }
	int append(s32 value){ return append(Data((const void*)&value, sizeof(value))); }
	int append(u64 value){ return append(Data((const void*)&value, sizeof(value))); }
	int append(s64 value){ return append(Data((const void*)&value, sizeof(value))); }
	int append(float value){ return append(Data(&value, sizeof(value))); }

	Data & operator << (const Data & a){ append(a); return *this; }
	Data & operator << (u8 a){ append(a); return *this; }
	Data & operator << (s8 a){ append(a); return *this; }
	Data & operator << (u16 a){ append(a); return *this; }
	Data & operator << (s16 a){ append(a); return *this; }
	Data & operator << (u32 a){ append(a); return *this; }
	Data & operator << (s32 a){ append(a); return *this; }
	Data & operator << (u64 a){ append(a); return *this; }
	Data & operator << (s64 a){ append(a); return *this; }

	void print(unsigned int value) const;

	static void reclaim_heap_space(){
#if !defined __link
		::free((void*)1);
#endif
	}

protected:
	void copy_object(const Data & a);
	void move_object(Data & a);

private:

	friend class ConstString;

	void set_needs_free() const { m_o_flags |= FLAG_NEEDS_FREE; }
	void clear_needs_free() const { m_o_flags &= ~FLAG_NEEDS_FREE; }

	bool needs_free() const { return m_o_flags & FLAG_NEEDS_FREE; }
	void zero();

	static const int m_zero_value;

	const void * m_mem;
	void * m_mem_write;
	u32 m_capacity;
	u32 m_size;

	enum {
		FLAG_NEEDS_FREE = (1<<0)
	};
	mutable u32 m_o_flags;

};

/*! \brief Structured Data Class
 * \details The Structured Data class is used for encapsulating
 * data structures in the Data class.
 *
 * This makes it much easier to compare, manipulate, and integrate the
 * data.  Here are some examples:
 *
 * \code
 *
 * typedef struct {
 *   u32 number;
 *   const char * name;
 *   void * next;
 *   u32 checksum;
 * } my_struct_t;
 *
 * StructuredData<my_struct_t> my_struct;
 *
 * my_struct.clear(); //set to all zeros
 *
 * my_struct->number = 47; //use -> to access members of my_struct_t
 *
 * File f;
 * f.write(my_struct); //write to a file
 *
 *
 * \endcode
 *
 *
 */
template <class T> class StructuredData : public Data {
public:

	/*! \details Constructs an empty data structure (fills with zero). */
	StructuredData() : Data(&m_item, sizeof(m_item)){
		clear();
	}

	/*! \details Constructs a data structure based on the value passed. */
	StructuredData(const T & item) : Data(&m_item, sizeof(m_item)){
		m_item = item;
	}

	/*! \details Accesses a member of the struct (read/write). */
	T * operator -> () { return (T*)data(); }
	/*! \details Accesses a member of the struct (read only). */
	const T * operator -> () const { return (T*)data(); }

	//operator const T & () const { return m_item; }
	//operator T & () { return m_item; }

private:
	T m_item;
};

}




#endif /* SAPI_VAR_DATA_HPP_ */
