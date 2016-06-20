/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef ITEM_HPP_
#define ITEM_HPP_

#include <mcu/types.h>
#include <cstring>

namespace var {

class ItemObject {
public:
	int save(const char * path, size_t offset = 0) const;
	int load(const char * path, size_t offset = 0);
	void clear();

protected:
	virtual void * data_ptr() = 0;
	virtual const void * cdata_ptr() const = 0;
	virtual size_t size() const = 0;

};


template<typename data_type> class Item: public ItemObject {
public:
	inline Item(){ memset(&m_item, 0, sizeof(m_item)); }
	inline Item(const data_type & item){ set(item); }
	inline Item(const data_type * item){ set(item); }

	operator data_type() const { return m_item; }

	inline const data_type & item() const { return m_item; }
	inline const data_type & value() const { return m_item; }
	inline size_t size() const { return sizeof(data_type); }
	inline const data_type * cdata() const { return &m_item; }

protected:
	inline void set(const data_type * item){ memcpy(&m_item, item, sizeof(m_item)); }
	inline void set(const data_type & item){ set(&item); }
	inline data_type * data() { return &m_item; }

	inline void * data_ptr(){ return (void*)&m_item; }
	inline const void * cdata_ptr() const { return (const void*)&m_item; }

private:
	data_type m_item;
};

};

#endif /* ITEM_HPP_ */
