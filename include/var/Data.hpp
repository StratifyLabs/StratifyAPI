/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_VAR_DATA_HPP_
#define SAPI_VAR_DATA_HPP_

#include <type_traits>
#include <cstring>
#include <cstdio>
#include "../api/VarObject.hpp"
#include "../arg/Argument.hpp"

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


private:
	struct mallinfo m_info;
};
#endif

//there needs to be a superclass here that defines all the interfaces

class DataReference : public virtual api::WorkObject {
public:

	DataReference();

	DataReference(
			arg::ReadOnlyBuffer buffer,
			arg::Size size
			);

	DataReference(
			arg::ReadWriteBuffer buffer,
			arg::Size size
			);

	template<typename T> DataReference(T & item){
		refer_to(item);
	}

	bool is_valid() const {
		return size() > 0;
	}

	bool is_null() const { return size() == 0; }

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
	template<typename T> void refer_to(T & item){

		if( std::is_const<T>::value == false ){
			set_reference(
						arg::ReadOnlyBuffer(&item),
						arg::ReadWriteBuffer((void*)&item),
						arg::Size(sizeof(T))
						);
		} else {
			set_reference(
						arg::ReadOnlyBuffer(&item),
						arg::ReadWriteBuffer(0), //read only data
						arg::Size(sizeof(T))
						);
		}
	}

	void refer_to(
			const arg::ReadOnlyBuffer read_only_data,
			arg::Size size
			);

	void refer_to(
			arg::ReadWriteBuffer data,
			arg::Size size
			);

	/*! \details Fill the data with zeros.
	 *
	 */
	virtual void clear(){ fill<u8>(0); }

	/*! \details Fill the data with the specified value.
	 * This will not attempt to write read-only data.
	 *
	 * @param d The value to write to the data
	 *
	 */
	template<typename T> void fill(
			const T & value,
			arg::Count count = arg::Count(0)
			){
		if( count.argument() == 0 ){
			count.argument() = size()/ sizeof(T);
		}
		for(u32 i=0; i < count.argument(); i++){
			at<T>(i) = value;
		}
	}

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
	void swap_byte_order(const arg::ImplicitSize size = 4);

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
		return m_data_write == 0;
	}

	/*! \details Returns true if the contents
	 * of both Data objects are the same.
	 *
	 */
	bool operator == (const DataReference & a) const {
		if( a.size() == size() ){
			return memcmp(
						read_data(),
						a.read_data(),
						size()
						) == 0;
		}
		return false;
	}

	/*! \details Returns true if the contents of the
	 * data objects are not the same.
	 *
	 */
	bool operator != (const DataReference & a) const {
		return !(*this == a);
	}

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
	template<typename T> T * to() const {
		if( std::is_const<T>::value ){
			return (T*)read_data();
		}
		return (T*)write_data();
	}

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
	template<typename T> T & at(arg::ImplicitPosition position) const {
		u32 count = size() / sizeof(T);
		position.argument() =
				position.argument() % count;
		if( (std::is_const<T>::value == false) &&
			 (write_data() == 0)
			 ){
			exit_fatal("DataReference::at() read only");
		}
		return to<T>()[position.argument()];
	}

	const char * to_const_char() const { return to<const char>(); }
	char * to_char() const { return to<char>(); }

	const void * to_const_void() const { return to<const void>(); }
	void * to_void() const { return to<void>(); }

	const void * to_const_u8() const { return to<const u8>(); }
	u8 * to_u8() const { return to<u8>(); }

	const u16 * to_const_u16() const { return to<const u16>(); }
	u16 * to_u16() const { return to<u16>(); }

	const u32 * to_const_u32() const { return to<const u32>(); }
	u32 * to_u32() const { return to<u32>(); }

	const u64 * to_const_u64() const { return to<const u64>(); }
	u64 * to_u64() const { return to<u64>(); }

	const s8 * to_const_s8() const { return to<const s8>(); }
	s8 * to_s8() const { return to<s8>(); }

	const s16 * to_const_s16() const { return to<const s16>(); }
	s16 * to_s16() const { return to<s16>(); }

	const s32 * to_const_s32() const { return to<const s32>(); }
	s32 * to_s32() const { return to<s32>(); }

	const s64 * to_const_s64() const { return to<const s64>(); }
	s64 * to_s64() const { return to<s64>(); }

	const float * to_const_float() const { return to<const float>(); }
	float * to_float() const { return to<float>(); }


	const char at_const_char(arg::ImplicitPosition position) const { return at<const char>(position); }
	char & at_char(arg::ImplicitPosition position) const { return at<char>(position); }

	u8 at_const_u8(arg::ImplicitPosition position) const { return at<const u8>(position); }
	u8 & at_u8(arg::ImplicitPosition position) const { return at<u8>(position); }

	u16 at_const_u16(arg::ImplicitPosition position) const { return at<const u16>(position); }
	u16 & at_u16(arg::ImplicitPosition position) const { return at<u16>(position); }

	u32 at_const_u32(arg::ImplicitPosition position) const { return at<const u32>(position); }
	u32 & at_u32(arg::ImplicitPosition position) const { return at<u32>(position); }

	u64 at_const_u64(arg::ImplicitPosition position) const { return at<const u64>(position); }
	u64 & at_u64(arg::ImplicitPosition position) const { return at<u64>(position); }

	s8 at_const_s8(arg::ImplicitPosition position) const { return at<const s8>(position); }
	s8 & at_s8(arg::ImplicitPosition position) const { return at<s8>(position); }

	s16 at_const_s16(arg::ImplicitPosition position) const { return at<const s16>(position); }
	s16 & at_s16(arg::ImplicitPosition position) const { return at<s16>(position); }

	s32 at_const_s32(arg::ImplicitPosition position) const { return at<const s32>(position); }
	s32 & at_s32(arg::ImplicitPosition position) const { return at<s32>(position); }

	s64 at_const_s64(arg::ImplicitPosition position) const { return at<const s64>(position); }
	s64 & at_s64(arg::ImplicitPosition position) const { return at<s64>(position); }

	float at_const_float(arg::ImplicitPosition position) const { return at<const float>(position); }
	float & at_float(arg::ImplicitPosition position) const { return at<float>(position); }

	static void memory_copy(
			const arg::SourceBuffer read_data,
			arg::DestinationBuffer write_data,
			arg::Size size
			){
		::memcpy(write_data.argument(),
					read_data.argument(),
					size.argument());
	}

	static void memory_set(
			arg::DestinationBuffer dest,
			arg::CharacterToAssign c,
			arg::Size size){
		::memset(
					dest.argument(),
					c.argument(),
					size.argument()
					);
	}

protected:

	/*! \cond */
	const void * read_data() const {
		return m_data_read;
	}

	void * write_data() const {
		return m_data_write;
	}

	virtual void set_reference(
			const arg::ReadOnlyBuffer read_data,
			arg::ReadWriteBuffer write_data,
			arg::Size size
			);

	void set_reference_internally(
			const arg::ReadOnlyBuffer read_data,
			arg::ReadWriteBuffer write_data,
			arg::Size size
			);

	void set_size_internally(u32 size);

private:




	const void * m_data_read;
	void * m_data_write;
	u32 m_size;


	//friend in order to assign null string to zero-length
	friend class ConstString;
	static const int m_zero_value;

	/*! \endcond */

};

/*! \brief Data storage class
 * \details The Data storage class can be used to manage both statically
 * and dyncamilly allocated data.  The statically allocated data can be
 * either read/write or read-only.
 *
 * ```
 * #include <sapi/var.hpp>
 *
 * Data block = Data( arg::Size(64) );
 *
 * ```
 *
 *
 *
 *
 *
 */
class Data : public DataReference {
public:
	/*! \details Constructs a data object with no data. */
	Data();

	/*! \details Constructs a new data object as a copy of another object.
	 *
	 * @param a The data that will be copied into a new object
	 *
	 *
	 * ```
	 * #include <sapi/var.hpp>
	 *
	 * Data a = Data( arg::Size(128) );
	 * a.fill(0);
	 * Data b(a); //b has has a copy of a
	 *
	 * ```
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
	 *
	 */
	Data& operator=(const Data & a);
	Data& operator=(Data && a);


	/*! \details Constructs data with dynamically allocated memory with \a size bytes (resizeable)
	 *
	 *  @param size The number of bytes to allocate
	 *
	 */
	Data(const arg::Size size);

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

	/*! \details Returns the current capacity of the data storage object.
	 *
	 * @return Number of bytes in the data object
	 *
	 * The capacity of the object will always be greater than
	 * or equal to size().
	 *
	 *
	 */
	u32 capacity() const { return m_capacity; }

	/*! \details Allocates (or reallocates) memory for the Data object.
	 *
	 * @param size The number of bytes to allocate
	 * @param resize If true, the old data is copied to the new data location
	 * @return Zero If the operation was successful and -1 if it failed
	 *
	 * If the memory was specified using the set() method or constructed as
	 * statically allocated memory, this will return an error.
	 */
	int allocate(const arg::Size size,
					 const arg::IsResize is_resize = arg::IsResize(false)
			);

	/*! \details Resizes the data (equivalent to allocate()).
	 *
	 * @param size The number of new bytes
	 * @return Zero on success or -1 with errno set
	 *
	 * This is the same as allocate() with \a resize set to true
	 *
	 */
	int resize(const arg::ImplicitSize size){
		return allocate(
					arg::Size(size.argument()),
					arg::IsResize(true)
					);
	}


	/*! \details Free the memory associated with this object.
	 * This will only perform any operations if the memory was
	 * dynamically allocating using this object.
	 *
	 * @return Zero on success
	 */
	int free();



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
	int copy_contents(
			const arg::SourceData a,
			const arg::Size size
			);

	int copy_data(const void * buffer, u32 size);

	int copy_cstring(const char * str);

	/*!
	 * \details Copies the contents of another data object.
	 * \param a The object whose contents will be copied
	 * \return Zero on success or less than zero if memory could not be allocated
	 *
	 */
	int copy_contents(const arg::SourceData a);

	/*!
	 * \details Copies the contents of another data object to this object.
	 * \param a The data object whose contents will be copied
	 * \param destination_position The offset in this object for the copy destination
	 * \param size The number of bytes to copy
	 * \return Zero on success or less than zero if memory could not be allocated
	 */
	int copy_contents(
			const arg::SourceData a,
			const arg::Location destination,
			const arg::Size size
			);

	/*! \details Append data to this object.
	 *
	 * @param a A reference to the data that will be appended.
	 * @return Zero on success or -1 if an error occurred (unable to allocate memory)
	 *
	 */
	int append(const arg::SourceData & a){
		return copy_contents(
					a,
					arg::Location(size()),
					arg::Size(a.argument().size())
					);
	}

	template <typename T> int append(const T & value){
		return append(
					arg::SourceData( DataReference(value) )
					);
	}

	Data & operator << (const DataReference & a){ append(a); return *this; }
	Data & operator << (u8 a){ append(a); return *this; }
	Data & operator << (s8 a){ append(a); return *this; }
	Data & operator << (u16 a){ append(a); return *this; }
	Data & operator << (s16 a){ append(a); return *this; }
	Data & operator << (u32 a){ append(a); return *this; }
	Data & operator << (s32 a){ append(a); return *this; }
	Data & operator << (u64 a){ append(a); return *this; }
	Data & operator << (s64 a){ append(a); return *this; }

#if !defined __link
	static void reclaim_heap_space(){
		::free((void*)1);
	}
#endif

	bool is_reference() const;

protected:
	void copy_object(const Data & a);
	void move_object(Data & a);

	void set_reference(
			const arg::ReadOnlyBuffer read_data,
			arg::ReadWriteBuffer write_data,
			arg::Size size
			);

private:

	u32 m_capacity;
	bool m_is_free_needed;

	void set_free_needed(bool value = true){
		m_is_free_needed = value;
	}

	bool is_free_needed() const {
		return m_is_free_needed;
	}

	void zero();
};


}




#endif /* SAPI_VAR_DATA_HPP_ */
