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
 * ProgressObject.
 */
class ProgressAttr {
public:
	ProgressAttr(){ memset(&m_progress, 0, sizeof(progress_t)); }
	ProgressAttr(u16 value, u16 max){ set(value, max); }

	/*! \details The progress value */
	u16 value() const { return m_progress.value; }
	/*! \details The maxium value for progress */
	u16 max() const { return m_progress.max; }

	/*! \details Set the progress value */
	void set_value(u16 value){ m_progress.value = value; }

	/*! \details Set the maximum value */
	void set_max(u16 max){ m_progress.max = max; }

	/*! \details Set both the value and the max */
	void set(u16 value, u16 max){ set_value(value); set_max(max); }

	void set(const ProgressAttr * progress){ set(progress->value(), progress->max()); }
	void set(const ProgressAttr & progress){ set(&progress); }
	void set(const progress_t * progress){ m_progress = *progress; }
	void set(const progress_t & progress){ m_progress = progress; }

private:
	progress_t m_progress;
};

/*! \brief Progress Class
 * \ingroup element
 * \details The Progress Class defines the top level object for progress bars etc.
 */
class Progress : public Drawing {
public:
	Progress(){}

	ProgressAttr & progress(){ return m_progress; }

	/*! \details Access the progress attributes */
	ProgressAttr & attr(){ return m_progress; }

	/*! \details Shortcut to access attribute value */
	u16 value() const { return m_progress.value(); }

	/*! \details Shortcut to access attribute max */
	u16 max() const { return m_progress.max(); }


protected:
	ProgressAttr m_progress;
};

};

#endif /* DRAW_PROGRESS_HPP_ */
