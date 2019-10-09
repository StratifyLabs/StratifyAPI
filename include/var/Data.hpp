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

/*! \brief Data Reference Class
 * \details The Data Reference class
 * is for referring to data (if that wasn't obvious
 * from the name).
 *
 * ```
 * //md2code:include
 * #include <sapi/var.hpp>
 * #include <sapi/fs.hpp>
 * ```
 *
 * The reference includes a pointer to some data
 * and the size of the data.
 *
 * This class allows passing the data reference as
 * an argument without passing the size separately.
 * Here is a quick example of how this is useful (and good).
 *
 * ```
 * //this is good because you can't mess up the size
 * int write(const DataReference & data);
 *
 * //this is not good because you can mess up the size
 * int write(const void * data, int bytes_in_data);
 * ```
 *
 * Now let's see this in practice.
 *
 * ```
 * //md2code:main
 *
 * File f;
 * f.open(
 *   arg::FilePath("/home/test.txt"),
 *   OpenFlags::append()
 *   );
 *
 * u32 data[4];
 *
 * f.write(
 *   arg::SourceData(DataReference(data))
 *   ); //writes 4 * sizeof(u32) bytes
 * //or
 * f.write(
 *   arg::SourceBuffer(data),
 *   //above cases needn't worry about size
 *   arg::Size(sizeof(data))
 *   );
 * ```
 *
 *
 *
 */
class DataReference : public virtual api::WorkObject {
public:

	/*! \details Constructs an empty
	 * data reference.
	 *
	 * is_valid() will return false until
	 * refer_to() is called.
	 *
	 *
	 */
	DataReference();


	/*! \details Constructs a read-only
	 * data reference to buffer with
	 * the specified size.
	 *
	 * ```
	 * //md2code:main
	 * const char buffer[16] = {0};
	 * DataReference read_only_data =
	 *   DataReference(
	 *     arg::ReadOnlyBuffer(buffer),
	 *     arg::Size(16)
	 *   );
	 *
	 * if( read_only_data.to_char() == nullptr ){
	 *   printf("this will print (data is read-only)\n");
	 * }
	 *
	 * if( read_only_data.to_const_char() == nullptr ){
	 *   printf("this won't print\n");
	 * }
	 * ```
	 *
	 *
	 */
	DataReference(
			arg::ReadOnlyBuffer buffer,
			arg::Size size
			);

	/*! \details Constructs a read-write
	 * data reference to a buffer with
	 * the specified size.
	 *
	 * ```
	 * //md2code:main
	 *	char buffer[16] = {0};
	 * DataReference read_write_data =
	 *   DataReference(
	 *     arg::ReadWriteBuffer(buffer),
	 *     arg::Size(16)
	 *   );
	 *
	 * if( read_write_data.to_char() == nullptr ){
	 *   printf("this won't print\n");
	 * }
	 *
	 * if( read_write_data.to_const_char() == nullptr ){
	 *   printf("this won't print\n");
	 * }
	 * ```
	 *
	 *
	 */
	DataReference(
			arg::ReadWriteBuffer buffer,
			arg::Size size
			);

	/*! \details Constructs a read-write
	 * data reference to some other item.
	 * The target item can be of any type. The size
	 * will be figured out.
	 *
	 * If the item is a `const` item, the
	 * data reference will be read only.
	 *
	 *
	 * ```
	 * //md2code:include
	 * #include <sos/dev/pio.h>
	 * ```
	 *
	 * ```
	 * //md2code:main
	 * pio_attr_t pio_attributes;
	 *
	 * DataReference data_structure(pio_attributes);
	 * data_structure.fill<u8>(0);
	 *
	 * if( data_structure.to_void() == (void*)&pio_attributes ){
	 *   printf("this will print\n");
	 * }
	 * ```
	 *
	 */
	template<typename T> DataReference(T & item){
		refer_to(item);
	}

	/*! \details Returns true if the data reference
	 * is valid.
	 *
	 * If the read and write pointers are both `nullptr`,
	 * this will return false.
	 *
	 * ```
	 * //md2code:main
	 * pio_attr_t pio_attributes;
	 *
	 * DataReference data_structure;
	 * if( data_structure.is_valid() ){
	 *   printf("this won't print\n");
	 * }
	 *
	 * data_structure.refer_to(pio_attributes);
	 *
	 * if( data_structure.is_valid() ){
	 *   printf("this will print\n");
	 * }
	 * ```
	 *
	 */
	bool is_valid() const {
		return size() > 0;
	}

	/*! \details Returns true if the data object
	 * is not valid.
	 *
	 */
	bool is_null() const { return size() == 0; }

	/*! \details Refers to an item.
	 *
	 * ```
	 * //md2code:main
	 * pio_attr_t pio_attributes;
	 *
	 * DataReference data_structure;
	 * data_structure.refer_to(pio_attributes);
	 * data_structure.fill<u8>(0);
	 *
	 * if( data_structure.to_void() == (void*)&pio_attributes ){
	 *   printf("this will print\n");
	 * }
	 * ```
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


	/*! \details Refers to a readonly buffer
	 * with the specified size.
	 *
	 * ```
	 * //md2code:main
	 * char buffer[16];
	 *
	 * DataReference data_reference =
	 *   DataReference(
	 *     arg::ReadOnlyBuffer(buffer),
	 *     arg::Size(16)
	 *   );
	 *
	 * if( data_reference.to_void() == nullptr ){
	 *   printf("this will print\n");
	 * }
	 *
	 * if( data_reference.to_const_void() == nullptr ){
	 *   printf("this won't print\n");
	 * }
	 * ```
	 *
	 */
	void refer_to(
			const arg::ReadOnlyBuffer read_only_data,
			arg::Size size
			);

	/*! \details Refers to a read-write buffer
	 * with the specified size.
	 *
	 * ```
	 * //md2code:main
	 * char buffer[16];
	 *
	 * DataReference data_reference =
	 *   DataReference(
	 *     arg::ReadWriteBuffer(buffer),
	 *     arg::Size(16)
	 *   );
	 *
	 * if( data_reference.to_void() == nullptr ){
	 *   printf("this won't print\n");
	 * }
	 *
	 * if( data_reference.to_const_void() == nullptr ){
	 *   printf("this won't print\n");
	 * }
	 * ```
	 *
	 */
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
	 *
	 * ```
	 * //md2code:main
	 * char buffer[16];
	 *
	 * DataReference data_reference(buffer);
	 *
	 * data_reference.fill<u8>(0xaa);
	 * data_reference.fill<u32>(0xaabbccdd);
	 * data_reference.fill((u16)0xaa55);
	 * ```
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

	template<typename T> void populate(
			T (*calculate_value)(arg::Position position),
			arg::Count count = arg::Count(0)
			){
		if( count.argument() == 0 ){
			count.argument() = this->count<T>();
		}
		for(u32 i=0; i < count.argument(); i++){
			at<T>(i) = calculate_value(arg::Position(i));
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
	 * ```
	 * //md2code:include
	 * #include <sapi/var.hpp>
	 * #include <sapi/hal.hpp>
	 * ```
	 *
	 * ```
	 * //md2code:main
	 * char buffer[16];
	 * DataReference data_reference(buffer);
	 *
	 * //assume the spi outputs big endian data -- swaps 32-bit words
	 * data_reference.swap_byte_order();
	 * data_reference.swap_byte_order(4); //this is the same as calling swap_byte_order()
	 *
	 * //or for swapping bytes in 16-bit words
	 * data_reference.swap_byte_order(2);
	 * ```
	 *
	 *
	 */
	void swap_byte_order(const arg::ImplicitSize size = 4);

	/*! \details Returns true if the data object is read only.
	 *
	 */
	bool is_read_only() const {
		return m_data_write == 0;
	}

	/*! \details Returns true if the contents
	 * of both DataReference objects are the same.
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
	 *
	 */
	virtual u32 size() const { return m_size; }


	/*! \details Returns a pointer to the data (read/write)
	 * This will return zero if the data is readonly.
	 *
	 * ```
	 * //md2code:main
	 * char buffer[64];
	 * DataReference a(buffer); //allocate 64 bytes of data
	 * u32 * value = a.to<u32>(); //casts data as u32*
	 * const u32 * const_value = a.to<const u32>(); //works with read only data
	 * if( value == const_value ){
	 *   printf("prints for read-write objects but not read-only\n");
	 * }
	 * ```
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
	 *
	 * If the index exceeds the size of the data, the index is set to 0.
	 *
	 * ```
	 * //md2code:main
	 * char buffer[64];
	 * DataReference a(buffer); //a is 64 bytes
	 * a.at<char>(arg::Position(0)) = 'a'; //assign 'a' to the first char location
	 * a.at<u32>(arg::Position(4)) = 0xAAAA5555; //assigns a u32 value assuming a is a u32 array
	 * u32 value = a.at<u32>(arg::Position(4)); //reads a value as if a is a u32 array
	 * printf("value is 0x%lx\n", value);
	 * ```
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

	/*! \details Returns the number of items that
	 * fit into the data.
	 *
	 * ```
	 * //md2code:main
	 * Data items( arg::Size(64) );
	 *
	 * printf(
	 *   "%ld int's fit in items\n",
	 *   items.count<int>();
	 * );
	 * ```
	 */
	template<typename T> u32 count() const {
		return size() / sizeof(T);
	}


	const char * to_const_char() const { return to<const char>(); }
	char * to_char() const { return to<char>(); }

	const void * to_const_void() const { return to<const void>(); }
	void * to_void() const { return to<void>(); }

	const u8 * to_const_u8() const { return to<const u8>(); }
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
 * \details The Data class inherits
 * DataReference and adds dynamic memory
 * allocation so both the reference
 * and the target data are managed by the same class.
 *
 *
 * ```
 * //md2code:include
 * #include <sapi/var.hpp>
 * ```
 *
 * ```
 * //md2code:main
 *
 * //allocation 64 bytes
 * Data block = Data( arg::Size(64) );
 *
 * //use DataReference inherited methods
 * block.fill<u32>(0xaabbccdd);
 * printf("First Byte: 0x%x\n",
 *   block.at_const_char(
 *     arg::Position(0)
 *     )
 *   );
 *
 * printf("Second Word: 0x%lx\n",
 *   block.at_u32(
 *     arg::Position(1)
 *     )
 *   );
 *
 * //once ~Data() is called the memory is freed
 * ```
 *
 * Data objects can also act just like references
 * without managing the memory internally.
 *
 *
 * ```
 * //md2code:include
 * #include <sos/dev/pio.h>
 * ```
 *
 * ```
 * //md2code:main
 * pio_attr_t pio_attributes;
 *
 * Data data;
 * data.refer_to(pio_attributes);
 *
 * if( data.is_reference() == true ){
 *   printf("this will print\n");
 * }
 *
 * data.allocate( arg::Size(64) );
 *
 * if( data.is_reference() == true ){
 *   printf("this won't print\n");
 * }
 * ```
 *
 *
 */
class Data : public DataReference {
public:
	/*! \details Constructs a data object with no data.
	 *
	 * The new object has zero size and is not valid.
	 *
	 * ```
	 * //md2code:main
	 * Data a;
	 * if( a.size() == 0 ){
	 *   printf("yep!\n");
	 * }
	 *
	 * if( a.is_valid() ){
	 *   printf("nope!\n");
	 * }
	 * ```
	 *
	 *
	 */
	Data();

	/*! \details Constructs a new data object as a copy of another object.
	 *
	 * @param a The data that will be copied into a new object
	 *
	 *
	 * ```
	 * //md2code:main
	 * Data a = Data( arg::Size(128) );
	 * a.fill(0);
	 * Data b(a); //b has has a copy of a
	 * ```
	 *
	 */
	Data(const Data & a);

	/*! \details Constructs a data object by moving
	 * the target's data to this object.
	 *
	 * C++ uses this constructor to move data and
	 * avoid a malloc/copy/free operation sequence.
	 *
	 * This code uses the move operator:
	 *
	 * ```
	 * //md2code:main
	 * //data is moved from unnamed object to moved_to_object
	 * Data moved_to_object = Data( arg::Size(64) );
	 * ```
	 *
	 */
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
	static u32 minimum_capacity();
	/*! \cond */
	static u32 block_size();
	/*! \endcond */

	/*! \details Returns the current capacity of the data storage object.
	 *
	 *
	 * The capacity of the object will always be greater than
	 * or equal to size(). When data is allocated, it is
	 * allocated in minimum_capacity() blocks. The capacity
	 * represents the actual amount of allocatd data
	 * in bytes. The size() dictates how many of those
	 * allocated bytes are effective.
	 *
	 * ```
	 * //md2code:main
	 * Data small_block(arg::Size(1));
	 * printf(
	 *   "Size is %ld, Capacity is %ld\n",
	 *   small_block.size(),
	 *   small_block.capacity()
	 *   );
	 *
	 * //size will be one, capacity will be minimum_capacity()
	 *
	 * //this will just reassign size without using malloc/free
	 * small_block.allocate(arg::Size(2));
	 * ```
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



	/*! \details Copies the contents of another data object
	 *  into the memory of this object.
	 *
	 * This object will be resized if there
	 * is not enough room to fit the contents.
	 *
	 * This object's size won't necessarily be
	 * equal to the copied object's size.
	 *
	 *
	 *
	 */
	int copy_contents(
			const arg::SourceData a,
			const arg::Size size
			);

	/*! \cond */
	int copy_data(const void * buffer, u32 size);
	int copy_cstring(const char * str);
	/*! \endcond */

	/*!
	 * \details Copies the contents of another data object.
	 *
	 */
	int copy_contents(const arg::SourceData a);

	/*!
	 * \details Copies the contents of another data object to this object.
	 * \param a The data object whose contents will be copied
	 * \param destination The offset in this object for the copy destination
	 * \param size The number of bytes to copy
	 * \return Zero on success or less than zero if memory could not be allocated
	 */
	int copy_contents(
			const arg::SourceData a,
			const arg::Position destination,
			const arg::Size size
			);

	/*! \details Appends the contents of another
	 * data object to this object.
	 *
	 * ```
	 * //md2code:main
	 * Data source_data(arg::Size(64));
	 * Data destination_data(arg::Size(64));
	 * source_data.fill<u8>(0x0a);
	 * destination_data.fill<u8>(0x0b);
	 * destination_data.append(
	 *   arg::SourceData(source_data)
	 *   );
	 * ```
	 *
	 */
	int append(const arg::SourceData & a){
		return copy_contents(
					a,
					arg::Position(size()),
					arg::Size(a.argument().size())
					);
	}

	/*! \details Appends an arbitrary type to
	 * this data object.
	 *
	 * ```
	 * //md2code:main
	 * pio_attr_t pio_attributes;
	 * Data destination_data(arg::Size(64));
	 * destination_data.fill<u8>(0x0b);
	 * destination_data.append(pio_attributes);
	 * ```
	 *
	 */
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
	/*! \details Releases heap space back to the stack.
	 *
	 * This method uses a special function available
	 * only on Stratify OS that tells the malloc/free
	 * system to decrease the heap space if the is free'd
	 * memory on the top of the heap.
	 *
	 */
	static void reclaim_heap_space(){
		::free((void*)1);
	}
#endif

	/*! \details Returns true if the data refers to another
	 * item rather than managing memory dynamically.
	 *
	 * ```
	 * //md2code:main
	 *
	 * Data data(arg::Size(64));
	 *
	 * if( data.is_reference() ){
	 *   printf("this won't print\n");
	 * }
	 *
	 * u32 some_value = 64;
	 * data.refer_to(some_value); //64 bytes freed
	 *
	 *	if( data.is_reference() ){
	 *   printf("this will print: %ld\n",
	 *     data.at_u32(0)
	 *   );
	 * }
	 * ```
	 *
	 */
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
