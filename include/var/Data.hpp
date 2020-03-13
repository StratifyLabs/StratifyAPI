/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#ifndef SAPI_VAR_DATA_HPP_
#define SAPI_VAR_DATA_HPP_

#include <type_traits>
#include <cstring>
#include <cstdio>
#include <vector>
#include "../api/VarObject.hpp"
#include "../arg/Argument.hpp"
#include "Reference.hpp"


#if !defined __link
#include <malloc.h>
#endif


namespace var {

class DataInfo : public api::InfoObject {
public:

	DataInfo(){ refresh(); }

#if !defined __link
	void refresh(){
		m_info = mallinfo();
	}
	u32 arena() const { return m_info.arena; }
	u32 free_block_count() const { return m_info.ordblks; }
	u32 free_size() const { return m_info.fordblks; }
	u32 used_size() const { return m_info.uordblks; }
#else
	void refresh(){}
	u32 arena() const { return 0; }
	u32 free_block_count() const { return 0; }
	u32 free_size() const { return 0; }
	u32 used_size() const { return 0; }
#endif

	bool operator == (const DataInfo & a){
		return used_size() == a.used_size();
	}

private:
#if !defined __link
	struct mallinfo m_info;
#endif
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
class Data : public Reference {
public:

	using IsResize = arg::Argument<bool, struct DataIsResizeTag>;

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

	Data(const Data & a) : m_data(a.m_data){
		update_reference();
	}

	Data(Data && a){
		m_data = std::move(a.m_data);
		a.update_reference();
		update_reference();
	}


	Data(std::initializer_list<u8> il) : m_data(il){
		update_reference();
	}

	Data & operator=(const Data & a){
		m_data = a.m_data;
		update_reference();
		return *this;
	}

	Data & operator=(Data && a){
		m_data = std::move(a.m_data);
		a.update_reference();
		update_reference();
		return *this;
	}

	Data & operator= (std::initializer_list<u8> il){
		m_data = il;
		update_reference();
		return *this;
	}

	/*! \details Constructs data with dynamically allocated memory with \a size bytes (resizeable)
	 *
	 *  @param size The number of bytes to allocate
	 *
	 */
	Data(size_t size);


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
	u32 capacity() const { return m_data.capacity(); }

	/*! \details Allocates (or reallocates) memory for the Data object.
	 *
	 * @param size The number of bytes to allocate
	 * @param resize If true, the old data is copied to the new data location
	 * @return Zero If the operation was successful and -1 if it failed
	 *
	 * If the memory was specified using the set() method or constructed as
	 * statically allocated memory, this will return an error.
	 */
	int allocate(
			size_t size,
			IsResize is_resize = IsResize(false)
			);

	/*! \details Resizes the data (equivalent to allocate()).
	 *
	 * @param size The number of new bytes
	 * @return Zero on success or -1 with errno set
	 *
	 * This is the same as allocate() with \a resize set to true
	 *
	 */
	int resize(
			size_t size
			){
		return allocate(size, IsResize(true));
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
			const Reference & a,
			Size size
			);

	/*! \cond */
	int copy_data(const void * buffer, u32 size);
	/*! \endcond */

	/*!
	 * \details Copies the contents of another data object.
	 *
	 */
	int copy_contents(
			const Reference & reference
			);

	/*!
	 * \details Copies the contents of another data object to this object.
	 * \param a The data object whose contents will be copied
	 * \param destination The offset in this object for the copy destination
	 * \param size The number of bytes to copy
	 * \return Zero on success or less than zero if memory could not be allocated
	 */
	int copy_contents(
			const Reference & a,
			Position destination_position,
			Size source_size
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
	Data & append(const Reference & reference);

	Data & operator << (const Reference & reference);
	Data & operator << (u8 a);
	Data & operator << (s8 a);
	Data & operator << (u16 a);
	Data & operator << (s16 a);
	Data & operator << (u32 a);
	Data & operator << (s32 a);
	Data & operator << (u64 a);
	Data & operator << (s64 a);


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
	template<typename T> T & at(size_t position){
		u32 count = size() / sizeof(T);
		position = position % count;
		return to<T>()[position];
	}

	template<typename T> const T & at(size_t position) const {
		u32 count = size() / sizeof(T);
		position = position % count;
		return to<T>()[position];
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

	void reserve(size_t size){
		m_data.reserve(size);
	}


	const char at_const_char(size_t position) const { return at<const char>(position); }
	char & at_char(size_t position) { return at<char>(position); }

	u8 at_const_u8(size_t position) const { return at<const u8>(position); }
	u8 & at_u8(size_t position) { return at<u8>(position); }

	u16 at_const_u16(size_t position) const { return at<const u16>(position); }
	u16 & at_u16(size_t position) { return at<u16>(position); }

	u32 at_const_u32(size_t position) const { return at<const u32>(position); }
	u32 & at_u32(size_t position) { return at<u32>(position); }

	u64 at_const_u64(size_t position) const { return at<const u64>(position); }
	u64 & at_u64(size_t position) { return at<u64>(position); }

	s8 at_const_s8(size_t position) const { return at<const s8>(position); }
	s8 & at_s8(size_t position) { return at<s8>(position); }

	s16 at_const_s16(size_t position) const { return at<const s16>(position); }
	s16 & at_s16(size_t position) { return at<s16>(position); }

	s32 at_const_s32(size_t position) const { return at<const s32>(position); }
	s32 & at_s32(size_t position) { return at<s32>(position); }

	s64 at_const_s64(size_t position) const { return at<const s64>(position); }
	s64 & at_s64(size_t position) { return at<s64>(position); }

	float at_const_float(size_t position) const { return at<const float>(position); }
	float & at_float(size_t position) { return at<float>(position); }

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
#else
	static void reclaim_heap_space(){}
#endif

	void * data(){
		return static_cast<void*>(m_data.data());
	}

	const void * data() const {
		return static_cast<const void*>(m_data.data());
	}

	bool is_reference() const;

protected:
	void copy_object(const Data & a);
	void move_object(Data & a);


private:

	void update_reference();

	std::vector<u8> m_data;

};


}




#endif /* SAPI_VAR_DATA_HPP_ */
