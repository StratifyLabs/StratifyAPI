/* Copyright 2014-2016 Fogo Digital, Inc; All Rights Reserved
 *
 */

#ifndef UI_LISTITEM_HPP_
#define UI_LISTITEM_HPP_

#include "../draw/TextAttr.hpp"
#include "../draw/Icon.hpp"
#include "../sys/Dir.hpp"
#include "ElementLinked.hpp"
#include "List.hpp"


namespace ui {

/*! \brief List Item Class
 * \details A ListItem represents an item in a List.
 *
 */
class ListItem : public ElementLinked {
public:
	/*! \details Construct a new list item */
	ListItem(const char * label, const sg_icon_t * icon = 0, ElementLinked * parent = 0, ElementLinked * child = 0);

	void draw_to_scale(const draw::DrawingScaledAttr & attr);

	/*! \details This methods gives read/write access to the icon attributes */
	draw::IconAttr & icon_attr(){ return m_icon_attr; }

	/*! \details This methods gives read only access to the icon attributes */
	const draw::IconAttr & icon_attr_const() const { return m_icon_attr; }

	/*! \details This methods gives read/write access to the text label attributes */
	draw::TextAttr & label_attr(){ return m_label; }

	/*! \details This methods gives read only access to the text label attributes */
	const draw::TextAttr & label_attr_const() const { return m_label; }

	virtual Element * handle_event(const Event  & event, const draw::DrawingAttr & attr);

private:
	draw::TextAttr m_label;
	draw::IconAttr m_icon_attr;
};

/*! \brief Toggle Item
 * \ingroup list
 * \details The ListItemToggle object is for storing editing a boolean
 * value in the application settings.  The value can be changed directly in the
 * list by selecting it.
 *
 */
class ListItemToggle : public ListItem {
public:
	ListItemToggle(const char * label, ElementLinked * parent = 0);

	/*! \details This method will toggle the value of the item and return the parent list */
	virtual Element * handle_event(const Event  & event, const draw::DrawingAttr & attr);

	/*! \details This methods will set the enabled flag as specified */
	void set_enabled(bool v = true);

	/*! \details This method will set the icon used when the item is enabled */
	void set_toggle_enabled_icon(const sg_icon_t * icon){ m_toggle_enabled_icon = icon; }

	/*! \details This method will set the icon used when the item is enabled */
	void set_toggle_disabled_icon(const sg_icon_t * icon){ m_toggle_disabled_icon = icon; }

private:
	const sg_icon_t * m_toggle_enabled_icon;
	const sg_icon_t * m_toggle_disabled_icon;

};


/*! \brief List Item Check (for use in a checklist)
 * \ingroup list
 */
class ListItemCheck : public ListItem {
public:
	ListItemCheck() : ListItem(0, 0) {}
	ListItemCheck(const char * label, List * parent = 0);
	void set_enabled(bool v = true);
	virtual Element * handle_event(const Event  & event, const draw::DrawingAttr & attr);

};

/*! \brief List Item Callback Class
 * \ingroup list
 * \details The ListItemCallback object allows the application to call a function
 * whenever the item in the list is selected.
 */
class ListItemCallback : public ListItem {
public:
	typedef ElementLinked * (*list_item_callback_t)(ElementLinked * list, ListItemCallback * obj, int event);

	ListItemCallback(list_item_callback_t callback, const char * label, const sg_icon_t * icon = 0, ElementLinked * parent = 0, ElementLinked * child = 0);

	//execute the callback
	virtual Element * handle_event(const Event  & event, const draw::DrawingAttr & attr);

private:
	list_item_callback_t m_callback;
};

/*! \brief List Item Element Class
 * \ingroup list
 * \details The ListItemElement class allows the current element to switch
 * to a child (or target) element.  For example, a List that jumps to another list
 * can use this class as the list item.
 *
 */
class ListItemElement : public ListItem {
public:

	ListItemElement(const char * label, const sg_icon_t * icon = 0, ElementLinked * parent = 0, ElementLinked * child = 0);
};

class ListItemBack : public ListItem {
public:
	ListItemBack(const sg_icon_t * icon = 0, ElementLinked * parent = 0);
	virtual Element * handle_event(const Event  & event, const draw::DrawingAttr & attr);
};

class ListItemExit : public ListItemBack {
public:
	ListItemExit(const sg_icon_t * icon = 0, ElementLinked * parent = 0);
};


template<typename type, size_t n_items> class ListItemsTemplate : public ListTemplate<type, n_items> {
public:
	ListItemsTemplate(const char * const label[], Element * parent = 0) : ListTemplate<type, n_items>(parent) {
		size_t i;
		for(i=0; i < this->size(); i++){
			this->at_item(i)->set_label(label[i]);
			this->at_item(i)->set_parent(this);
		}
	}

	ListItemsTemplate(const char * const label[], const sg_bitmap_hdr_t * const icon[], Element * parent = 0) :
		ListTemplate<type, n_items>(parent) {
		size_t i;
		for(i=0; i < this->size(); i++){
			this->at_item(i)->set_label(label[i]);
			this->at_item(i)->set_app_icon(icon[i]);
			this->at_item(i)->set_parent(this);
		}
	}
};



/*
template<int num_items> class ListItemSettingList : public ListItemsTemplate<ListItemSetting, num_items> {
public:
	ListItemSettingList(const char * const label[], const char * const key[], List * parent = 0) :
		ListItemsTemplate<ListItemSetting, num_items>(label, parent) {
		size_t i;
		for(i=0; i < this->size(); i++){
			this->at_item(i)->set_key(key[i]);
		}
	}
};
 */


class ListDir : public List {
public:
	typedef void (*list_dir_callback_t)(ListDir * list);

	ListDir(const char * path, const sg_icon_t * icon = 0, ElementLinked * parent = 0, ElementLinked * child = 0);
	~ListDir();

	void set_path(const char * path);
	const var::String & path() const { return m_path; }


	virtual ElementLinked * at(list_attr_size_t i);
	inline list_attr_size_t size() const { return m_total; }

	inline void set_suffix_visible(bool v = true){
		set_flag(FLAG_SUFFIX_VISIBLE, v);
	}

	inline bool suffix_visible() const { return flag(FLAG_SUFFIX_VISIBLE); }

	inline void set_callback(list_dir_callback_t callback){ m_callback = callback; }

	inline sys::Dir & dir(){ return m_dir; }
	inline ListItemElement & item(){ return m_item; }

	virtual Element * handle_event(const Event  & event, const draw::DrawingAttr & attr);

protected:

	enum {
		FLAG_SUFFIX_VISIBLE = Element::FLAG_TOTAL
	};

	inline void set_total(size_t total){ m_total = total; }

private:
	sys::Dir m_dir;
	size_t m_total;
	ListItemElement m_item;
	void recount(void);
	list_dir_callback_t m_callback;
	var::String m_path;
};

};





#endif /* UI_LISTITEM_HPP_ */
