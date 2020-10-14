/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef VAR_API_DATA_HPP_
#define VAR_API_DATA_HPP_

#include <cstdio>
#include <cstring>
#include <type_traits>
#include <vector>

#include "View.hpp"

#if !defined __link
#include <malloc.h>
#endif

namespace var {

class DataInfo {
public:
  DataInfo() { refresh(); }

#if !defined __link
  void refresh() { m_info = mallinfo(); }
  u32 arena() const { return m_info.arena; }
  u32 free_block_count() const { return m_info.ordblks; }
  u32 free_size() const { return m_info.fordblks; }
  u32 used_size() const { return m_info.uordblks; }
#else
  void refresh() {}
  u32 arena() const { return 0; }
  u32 free_block_count() const { return 0; }
  u32 free_size() const { return 0; }
  u32 used_size() const { return 0; }
#endif

  bool operator==(const DataInfo &a) { return used_size() == a.used_size(); }

private:
#if !defined __link
  struct mallinfo m_info;
#endif
};

/*! \brief Data storage class
 * \details The Data class
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
class Data : public api::ExecutionContext {
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
  Data() = default;

  explicit Data(std::initializer_list<u8> il) : m_data(il) {}

  /*! \details Constructs data with dynamically allocated memory with \a size
   * bytes (resizeable)
   *
   *  @param size The number of bytes to allocate
   *
   */
  explicit Data(size_t size);

  String to_string() const { return View(*this).to_string(); }
  static Data from_string(var::StringView value);

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

  /*! \details Resizes the data (equivalent to allocate()).
   *
   * @param size The number of new bytes
   * @return Zero on success or -1 with errno set
   *
   * This is the same as allocate() with \a resize set to true
   *
   */
  Data &resize(size_t size);

  /*! \details Free the memory associated with this object.
   * This will only perform any operations if the memory was
   * dynamically allocatißng using this object.
   *
   * @return Zero on success
   */
  Data &free() {
    m_data = std::vector<u8>();
    return *this;
  }

  class Copy {
    API_AF(Copy, size_t, destination_position, 0);
    API_AF(Copy, size_t, size, -1);

  public:
    Copy() : m_destination_position(0), m_size(static_cast<size_t>(-1)) {}
  };
  /*!
   * \details Copies the contents of another data object to this object.
   * \param a The data object whose contents will be copied
   * \param destination The offset in this object for the copy destination
   * \param size The number of bytes to copy
   * \return Zero on success or less than zero if memory could not be allocated
   */
  Data &copy(const View a, const Copy &options = Copy());

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
  Data &append(const View reference);

  class Erase {
    API_AF(Erase, size_t, position, 0);
    API_AF(Erase, size_t, size, 0);
  };

  Data &erase(const Erase &options) {
    m_data.erase(
      m_data.begin() + options.position(),
      m_data.begin() + options.size());
    return *this;
  }

#if !defined __link
  /*! \details Releases heap space back to the stack.
   *
   * This method uses a special function available
   * only on Stratify OS that tells the malloc/free
   * system to decrease the heap space if the is free'd
   * memory on the top of the heap.
   *
   */
  static void reclaim_heap_space() { ::free((void *)1); }
#else
  static void reclaim_heap_space() {}
#endif

  Data &reserve(size_t size) {
    m_data.reserve(size);
    return *this;
  }

  bool operator==(const var::Data &data) const { return data.m_data == m_data; }
  bool operator!=(const var::Data &data) const { return data.m_data != m_data; }
  bool operator>(const var::Data &data) const { return data.m_data > m_data; }
  bool operator<(const var::Data &data) const { return data.m_data < m_data; }

  void *data() { return static_cast<void *>(m_data.data()); }
  const void *data() const { return static_cast<const void *>(m_data.data()); }
  u8 *data_u8() { return (m_data.data()); }
  const u8 *data_u8() const { return (m_data.data()); }

  size_t size() const { return m_data.size(); }
  ssize_t size_signed() const { return static_cast<ssize_t>(m_data.size()); }

protected:
  void copy_object(const Data &a);
  void move_object(Data &a);

private:
  std::vector<u8> m_data;
};

} // namespace var

#if USE_PRINTER
namespace sys {
class Printer;
#if !defined __link
Printer &operator<<(sys::Printer &printer, const var::DataInfo &a);
#endif
} // namespace sys
#endif

#endif /* VAR_API_DATA_HPP_ */
