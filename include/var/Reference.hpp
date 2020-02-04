/*! \file */ //Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#ifndef SAPI_VAR_REFERENCE_HPP_
#define SAPI_VAR_REFERENCE_HPP_

#include <type_traits>
#include <cstring>
#include <cstdio>
#include "../api/VarObject.hpp"
#include "../arg/Argument.hpp"

#include "String.hpp"
#include "Vector.hpp"
#include "Array.hpp"

#if !defined __link
#include <malloc.h>
#endif


namespace var {


class Data;

/*! \brief Reference Class
 * \details The Reference class
 * is for referring to data. The data has a
 * pointer to a buffer and a size.
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
class Reference : public virtual api::WorkObject {
public:

	using ReadOnlyBuffer = arg::Argument<const void*, struct ReferenceReadOnlyBufferTag >;
	using ReadWriteBuffer = arg::Argument<void*, struct ReferenceReadWriteBufferTag >;

	using SourceBuffer = arg::Argument<const void*, struct ReferenceSourceBufferTag >;
	using DestinationBuffer = arg::Argument<void*, struct ReferenceDestinationBufferTag >;

	using Count = arg::Argument<size_t, struct ReferenceCountTag >;
	using Size = arg::Argument<size_t, struct ReferenceSizeTag >;
	using PageSize = arg::Argument<size_t, struct PageSizeTag >;

	using Source = arg::Argument<const Reference&, struct ReferenceSourceTag >;
	using Destination = arg::Argument<Reference&, struct ReferenceDestinationTag >;

	using Position = arg::Argument<size_t, struct ReferencePositionTag >;
	using IsOverwrite = arg::Argument<bool, struct ReferenceIsOverwritenTag >;


	/*! \details Constructs an empty
	 * data reference.
	 *
	 * is_valid() will return false until
	 * refer_to() is called.
	 *
	 *
	 */
	Reference();

	Reference(const char * cstring){
		set_reference(
					ReadOnlyBuffer(cstring),
					ReadWriteBuffer(nullptr),
					Size(var::String(cstring).length())
					);
	}

	Reference(const Data & data);
	Reference(Data & data);

	Reference(const String & str){
		set_reference(
					ReadOnlyBuffer(str.cstring()),
					ReadWriteBuffer(nullptr),
					Size(str.length())
					);
	}

	Reference(String & str){
		set_reference(
					ReadOnlyBuffer(str.cstring()),
					ReadWriteBuffer(str.to_char()),
					Size(str.length())
					);
	}

	var::String to_string() const;


	template<typename T> Reference(const Vector<T> & vector){
		set_reference(
					ReadOnlyBuffer(vector.to_const_void()),
					ReadWriteBuffer(nullptr),
					Size(vector.count()*sizeof(T))
					);
	}

	template<typename T> Reference(Vector<T> & vector){
		set_reference(
					ReadOnlyBuffer(vector.to_const_void()),
					ReadWriteBuffer(vector.to_void()),
					Size(vector.count()*sizeof(T))
					);
	}

	template<typename T, size_t size_value> Reference(const Array<T, size_value> & array){
		set_reference(
					ReadOnlyBuffer(array.to_const_void()),
					ReadWriteBuffer(nullptr),
					Size(size_value * sizeof(T))
					);
	}

	template<typename T, size_t size_value> Reference(Array<T, size_value> & array){
		set_reference(
					ReadOnlyBuffer(array.to_const_void()),
					ReadWriteBuffer(array.to_void()),
					Size(size_value * sizeof(T))
					);
	}


	template<typename T> Reference(T & item){
		//catch all
		refer_to(item);
	}



	/*! \details Constructs a read-only
	 * data reference to buffer with
	 * the specified size.
	 *
	 * ```
	 * //md2code:main
	 * const char buffer[16] = {0};
	 * Reference read_only_data =
	 *   Reference(
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
	Reference(
			ReadOnlyBuffer buffer,
			Size size
			);

	/*! \details Constructs a read-write
	 * data reference to a buffer with
	 * the specified size.
	 *
	 * ```
	 * //md2code:main
	 *	char buffer[16] = {0};
	 * Reference read_write_data =
	 *   Reference(
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
	Reference(
			ReadWriteBuffer buffer,
			Size size
			);

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
	 * Reference data_structure;
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


	Reference & refer_to(const Reference & reference){
		m_data_write = nullptr;
		m_data_read = reference.m_data_read;
		m_size = reference.m_size;
		return *this;
	}

	Reference & refer_to(Reference & reference){
		m_data_write = reference.m_data_write;
		m_data_read = reference.m_data_read;
		m_size = reference.m_size;
		return *this;
	}

	/*! \details Refers to an item.
	 *
	 * ```
	 * //md2code:main
	 * pio_attr_t pio_attributes;
	 *
	 * Reference data_structure;
	 * data_structure.refer_to(pio_attributes);
	 * data_structure.fill<u8>(0);
	 *
	 * if( data_structure.to_void() == (void*)&pio_attributes ){
	 *   printf("this will print\n");
	 * }
	 * ```
	 *
	 */
	template<typename T> Reference & refer_to(T & item){

		static_assert(
					std::is_trivial<T>::value &&
					std::is_standard_layout<T>::value,
					"Cannot construct reference from non-trivial non-standard-layout types"
					);

		if( std::is_const<T>::value == false ){
			set_reference(
						ReadOnlyBuffer(&item),
						ReadWriteBuffer((void*)&item),
						Size(sizeof(T))
						);
		} else {
			set_reference(
						ReadOnlyBuffer(&item),
						ReadWriteBuffer(nullptr), //read only data
						Size(sizeof(T))
						);
		}

		return *this;

	}


	/*! \details Refers to a readonly buffer
	 * with the specified size.
	 *
	 * ```
	 * //md2code:main
	 * char buffer[16];
	 *
	 * Reference data_reference =
	 *   Reference(
	 *     Reference::ReadOnlyBuffer(buffer),
	 *     Reference::Size(16)
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
	Reference & refer_to(
			ReadOnlyBuffer read_only_data,
			Size size
			);

	/*! \details Refers to a read-write buffer
	 * with the specified size.
	 *
	 * ```
	 * //md2code:main
	 * char buffer[16];
	 *
	 * Reference data_reference =
	 *   Reference(
	 *     Reference::ReadWriteBuffer(buffer),
	 *     Reference::Size(16)
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
	Reference & refer_to(
			ReadWriteBuffer data,
			Size size
			);

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
	template<typename T> Reference& fill(
			const T & value,
			Count count = Count(0)
			){
		if( count.argument() == 0 ){
			count.argument() = this->count<T>();
		}
		for(u32 i=0; i < count.argument(); i++){
			to<T>()[i] = value;
		}
		return *this;
	}

	template<typename T> Reference & populate(
			T (*calculate_value)(
				Position position,
				Count count
				),
			Reference::Count count = Reference::Count(0)
			){
		if( count.argument() == 0 ){
			count.argument() = this->count<T>();
		}
		for(u32 i=0; i < count.argument(); i++){
			to<T>()[i] = calculate_value(Position(i), count);
		}
		return *this;
	}

	template<typename T> size_t count() const {
		return size() / sizeof(T);
	}

	/*! \details Fill the data with zeros.
	 *
	 */
	virtual void clear(){
		if( write_data() ){
			memory_set(
						write_data(),
						0,
						Size(size())
						);
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
	 * Reference data_reference(buffer);
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
	void swap_byte_order(size_t size = 4);

	/*! \details Returns true if the data object is read only.
	 *
	 */
	bool is_read_only() const {
		return m_data_write == 0;
	}

	/*! \details Returns true if the contents
	 * of both Reference objects are the same.
	 *
	 */
	bool operator == (const Reference & a) const {
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
	bool operator != (const Reference & a) const {
		return !(*this == a);
	}

	/*!
	 * \details Returns the effective size of the data.
	 *
	 *
	 */
	size_t size() const { return m_size; }


	static void memory_copy(
			SourceBuffer read_data,
			DestinationBuffer write_data,
			Size size
			){
		::memcpy(write_data.argument(),
					read_data.argument(),
					size.argument());
	}

	static void memory_set(
			void * dest,
			char c,
			Size size){
		::memset(
					dest,
					c,
					size.argument()
					);
	}

	/*! \details Returns a pointer to the data (read/write)
	 * This will return zero if the data is readonly.
	 *
	 * ```
	 * //md2code:main
	 * char buffer[64];
	 * Reference a(buffer); //allocate 64 bytes of data
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
		return static_cast<T*>(write_data());
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

	/*! \details Saves the data that is reference
	 * to a file specified by *file_path*.
	 *
	 */
	int save(
			const var::String file_path,
			IsOverwrite is_overwrite = IsOverwrite(false)
			) const;

protected:

	/*! \cond */

	const void * read_data() const {
		return m_data_read;
	}

	void * write_data() const {
		return m_data_write;
	}

	virtual void set_reference(
			ReadOnlyBuffer read_data,
			ReadWriteBuffer write_data,
			Size size
			);

	void set_reference_internally(
			ReadOnlyBuffer read_data,
			ReadWriteBuffer write_data,
			Size size
			);

	void set_size_internally(u32 size);


private:
	const void * m_data_read;
	void * m_data_write;
	size_t m_size;


	//friend in order to assign null string to zero-length
	static const int m_zero_value;

	/*! \endcond */

};

}




#endif /* SAPI_VAR_DATA_HPP_ */
