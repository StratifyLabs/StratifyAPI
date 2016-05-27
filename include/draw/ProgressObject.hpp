/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_PROGRESSOBJECT_HPP_
#define DRAW_PROGRESSOBJECT_HPP_

#include "Drawing.hpp"

namespace draw {

typedef struct MCU_PACK {
	u16 value;
	u16 max;
} progress_t;


class ProgressAttr {
public:
	ProgressAttr(){ memset(&m_progress, 0, sizeof(progress_t)); }
	ProgressAttr(u16 value, u16 max){ set(value, max); }
	inline u16 value() const { return m_progress.value; }
	inline u16 max() const { return m_progress.max; }

	inline void set_value(u16 value){ m_progress.value = value; }
	inline void set_max(u16 max){ m_progress.max = max; }
	inline void set(u16 value, u16 max){ set_value(value); set_max(max); }
	inline void set(const ProgressAttr * progress){ set(progress->value(), progress->max()); }
	inline void set(const ProgressAttr & progress){ set(&progress); }
	inline void set(const progress_t * progress){ m_progress = *progress; }
	inline void set(const progress_t & progress){ m_progress = progress; }

private:
	progress_t m_progress;
};

/*! \brief Progress Class
 * \ingroup element
 * \details The Progress Class defines the top level object for progress bars etc.
 */
class ProgressObject : public Drawing {
public:
	ProgressObject(){}
	ProgressObject(u16 value, u16 max) : m_progress(value, max){}

	inline ProgressAttr & progress(){ return m_progress; }
	inline ProgressAttr & attr(){ return m_progress; }

	inline u16 value() const { return m_progress.value(); }
	inline u16 max() const { return m_progress.max(); }


protected:
	ProgressAttr m_progress;
};

};

#endif /* DRAW_PROGRESSOBJECT_HPP_ */
