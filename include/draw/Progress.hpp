/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_PROGRESS_HPP_
#define DRAW_PROGRESS_HPP_

#include "Drawing.hpp"

namespace draw {

typedef struct MCU_PACK {
	u16 value;
	u16 max;
} progress_t;

/*! \brief Progress Attribute Class
 * \details This class defines the attributes of any
 * progress object.  This class is available as a minimal
 * way to store progress without having to inherit Drawing by using
 * Progress object.
 */
class ProgressAttr {
public:
	ProgressAttr() : m_pen(1,3,false) { memset(&m_progress, 0, sizeof(progress_t)); }
	ProgressAttr(u16 value, u16 max){ set_attr(value, max); }

	/*! \details The progress value */
	u16 value() const { return m_progress.value; }
	/*! \details The maxium value for progress */
	u16 max() const { return m_progress.max; }

	/*! \details Set the progress value */
	void set_value(u16 value){ m_progress.value = value; }

	/*! \details Set the maximum value */
	void set_max(u16 max){ m_progress.max = max; }

	/*! \details Set both the value and the max */
	void set_attr(u16 value, u16 max){ set_value(value); set_max(max); }

	operator progress_t(){ return m_progress; }

	void set_attr(const progress_t * progress){ m_progress = *progress; }
	void set_attr(const progress_t & progress){ m_progress = progress; }

	sgfx::Pen & pen(){ return m_pen; }

private:
	progress_t m_progress;
	sgfx::Pen m_pen;

};

/*! \brief Progress Class
 * \ingroup element
 * \details The Progress Class defines the top level object for progress bars etc.
 */
class Progress : public Drawing, public ProgressAttr {
public:
	Progress(){}

protected:

};

};

#endif /* DRAW_PROGRESS_HPP_ */
