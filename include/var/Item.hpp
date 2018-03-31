/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved

#ifndef ITEM_HPP_
#define ITEM_HPP_

#include <cstring>
#include "../api/SObject.hpp"

namespace var {

/*! \brief Abstract Item for storing binary data */
class ItemObject {
public:
	/*! \details Saves the item to a file.
	 *
	 * @param path The path to the file
	 * @return Zero on success
	 */
	virtual int save(const char * path) const;

	/*! \details Loads the item from a file.
	 *
	 * @param path The path to the file
	 * @return Zero on success
	 */
	virtual int load(const char * path);

	/*! \details Clears the data in the item. */
	virtual void clear();

	/*! \details Returns the size of the item */
	virtual u32 size() const = 0;

protected:
	virtual void * data_void() = 0;
	virtual const void * data_void_const() const = 0;

};

/*! \brief Item Class for storing and accessing binary data */
template<typename data_type> class Item: public ItemObject {
public:

	/*! \details Create an empty item (all zeros) */
	Item(){ clear(); }
	/*! \details Create an Item by making a copy of \a item */
	Item(const data_type & item){ set(item); }
	/*! \details Create an Item by making a copy of \a item */
	Item(const data_type * item){ set(item); }

	/*! \details This operator will return a reference of underlying binary data type. */
	operator const data_type & () const { return m_item; }

	operator const data_type * () const { return &m_item; }

	inline const data_type & item() const { return m_item; }
	/*! \details This gives read-only access to the item and items members */
	inline const data_type & value() const { return m_item; }
	/*! \details This method returns the size of the item */
	inline u32 size() const { return sizeof(data_type); }
	/*! \details This returns a read-only pointer to the item */
	inline const data_type * data_const() const { return &m_item; }

protected:
	inline void set(const data_type * item){ memcpy(&m_item, item, size()); }
	inline void set(const data_type & item){ set(&item); }
	inline data_type * data() { return &m_item; }

	inline void * data_void(){ return (void*)&m_item; }
	inline const void * data_void_const() const { return (const void*)&m_item; }

private:
	data_type m_item;
};

}

#endif /* ITEM_HPP_ */
