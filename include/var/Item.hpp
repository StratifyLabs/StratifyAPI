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
	inline Item(){ memset(&_item, 0, sizeof(_item)); }
	inline Item(const data_type & item){ set(item); }
	inline Item(const data_type * item){ set(item); }

	inline const data_type & item() const { return _item; }
	inline size_t size() const { return sizeof(data_type); }
	inline const data_type * cdata() const { return &_item; }

protected:
	inline void set(const data_type * item){ memcpy(&_item, item, sizeof(_item)); }
	inline void set(const data_type & item){ set(&item); }
	inline data_type * data() { return &_item; }

	inline void * data_ptr(){ return (void*)&_item; }
	inline const void * cdata_ptr() const { return (const void*)&_item; }

private:
	data_type _item;
};

};

#endif /* ITEM_HPP_ */
