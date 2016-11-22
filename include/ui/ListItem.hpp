/* Copyright 2014-2016 Tyler Gilbert, Inc; All Rights Reserved
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
	draw::IconAttr & icon(){ return m_icon_attr; }
	draw::IconAttr & icon_attr(){ return m_icon_attr; }

	/*! \details This methods gives read only access to the icon attributes */
	const draw::IconAttr & icon_const() const { return m_icon_attr; }
	const draw::IconAttr & icon_attr_const() const { return m_icon_attr; }

	/*! \details This methods gives read/write access to the text label attributes */
	draw::TextAttr & text(){ return m_text_attr; }
	draw::TextAttr & text_attr(){ return m_text_attr; }

	const draw::TextAttr & text_const() const { return m_text_attr; }


	virtual Element * handle_event(const Event  & event, const draw::DrawingAttr & attr);

private:
	draw::TextAttr m_text_attr;
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


/*! \brief List Item Back Entry
 * \details This is an item that
 * returns the list's parent element when selected.
 */
class ListItemBack : public ListItem {
public:
	ListItemBack(const sg_icon_t * icon = 0, ElementLinked * parent = 0);
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


/*! \brief Directory List Class
 * \details This class will create a list from directory
 * entries in the filesystem
 */
class ListDir : public List {
public:
	/*! \details Callback for actions on a list directory */
	typedef void (*list_dir_callback_t)(ListDir * list);

	/*! \details Contruct a new list */
	ListDir(const char * path, const sg_icon_t * icon = 0, ElementLinked * parent = 0, ElementLinked * child = 0);
	~ListDir();

	/*! \details Set the path for the directory */
	void set_path(const char * path);
	/*! \details Access the path */
	const var::String & path() const { return m_path; }

	/*! \details Return a pointer to the element in the specfied location in the
	 * list
	 * @param i The offset within the list
	 * @return A pointer to the object
	 */
	virtual ElementLinked & at(list_attr_size_t i);

	/*! \details Return the total entries in the list */
	inline list_attr_size_t size() const { return m_size; }

	/*! \details This method changes the visibility of the file suffix in the list */
	inline void set_suffix_visible(bool v = true){
		set_flag(FLAG_SUFFIX_VISIBLE, v);
	}

	/*! \details Returns true if the filename suffixes should be visible */
	bool is_suffix_visible() const { return flag(FLAG_SUFFIX_VISIBLE); }

	/*! \details Assign the callback for when an item in the list is selected */
	void set_callback(list_dir_callback_t callback){ m_callback = callback; }

	/*! \details Accces the sys::Dir object used to read the directory */
	sys::Dir & dir(){ return m_dir; }

	/*! \details Access the currently selected item in the list */
	ListItem & item(){ return m_item; }

	virtual Element * handle_event(const Event  & event, const draw::DrawingAttr & attr);

protected:

	enum {
		FLAG_SUFFIX_VISIBLE = Element::FLAG_ELEMENT_TOTAL
	};

	inline void set_size(size_t total){ m_size = total; }

private:
	sys::Dir m_dir;
	/*! \todo List inherits ListAttr so the total entries should already be stored somewhere */
	size_t m_size;
	ListItem m_item;
	void recount(void);
	list_dir_callback_t m_callback;
	var::String m_path;
};

};





#endif /* UI_LISTITEM_HPP_ */
