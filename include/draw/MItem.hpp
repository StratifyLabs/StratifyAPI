//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_MITEM_HPP_
#define DRAW_MITEM_HPP_


#include "FSys.hpp"

typedef u16 mitem_size_t;
typedef i16 mitem_int_t;

typedef struct MCU_PACK {
	mitem_int_t x;
	mitem_int_t y;
} mitem_point_t;

typedef struct MCU_PACK {
	mitem_size_t w;
	mitem_size_t h;
} mitem_dim_t;


typedef struct MCU_PACK {
	draw::MBitmap * b;
	mitem_point_t p;
	mitem_dim_t d;
} mitem_attr_t;

typedef struct MCU_PACK {
	mitem_point_t start /*! \brief Animation start point */;
	mitem_dim_t dim /*! \brief Animate within these dimensions with start in the top left corner */;
	mitem_size_t motion_total /*! \brief Total amount of animation movement */;
} mitem_animation_attr_t;


typedef struct MCU_PACK {
	draw::MBitmap * b;
	mg_point_t p;
	mg_dim_t d;
} mitem_scaled_attr_t;


mitem_point_t mitem_point(mitem_int_t x, mitem_int_t y);
mitem_dim_t mitem_dim(mitem_size_t w, mitem_size_t h);


class MItemAttr{
public:
	MItemAttr();
	MItemAttr(mitem_attr_t & attr);
	operator mitem_attr_t (){ return m_attr; }

	void set(draw::MBitmap * b, mitem_point_t p, mitem_dim_t d);
	void set_bitmap(draw::MBitmap * b){ m_attr.b = b; }
	void set_dim(mitem_dim_t d){ m_attr.d = d; }
	void set_dim(mitem_size_t w, mitem_size_t h){ m_attr.d.w = w; m_attr.d.h = h; }
	void set_point(mitem_point_t p){ m_attr.p = p; }
	void set_point(mitem_int_t x, mitem_int_t y){ m_attr.p.x = x; m_attr.p.y = y; }

	mitem_size_t w() const { return m_attr.d.w; }
	mitem_size_t h() const { return m_attr.d.h; }
	mitem_int_t x() const { return m_attr.p.x; }
	mitem_int_t y() const { return m_attr.p.y; }

	draw::MBitmap * b() const { return m_attr.b; }
	mitem_point_t p() const { return m_attr.p; }
	mitem_dim_t d() const { return m_attr.d; }
	mitem_attr_t & attr(){ return m_attr; }

	mitem_size_t w(mitem_size_t v) const;
	mitem_size_t h(mitem_size_t v) const;

	MItemAttr operator+ (mitem_point_t d) const;
	MItemAttr operator+ (mitem_dim_t d) const;

	mitem_dim_t square(mitem_size_t v) const;
	mitem_dim_t square_w(mitem_size_t v) const;
	mitem_dim_t square_h(mitem_size_t v) const;


private:
	mitem_attr_t m_attr;
};

class MItemScaledAttr {
public:
	operator mitem_scaled_attr_t (){ return m_attr; }

	void set(draw::MBitmap * b, mg_point_t p, mg_dim_t d);
	void set_bitmap(draw::MBitmap * b){ m_attr.b = b; }
	void set_dim(mg_dim_t d){ m_attr.d = d; }
	void set_height(mg_size_t h){ m_attr.d.h = h; }
	void set_width(mg_size_t w){ m_attr.d.h = w; }
	void set_x(mg_int_t x){ m_attr.p.x = x; }
	void set_y(mg_int_t y){ m_attr.p.x = y; }
	void set_dim(mg_size_t w, mg_size_t h){ m_attr.d.w = w; m_attr.d.h = h; }
	void set_point(mg_point_t p){ m_attr.p = p; }

	draw::MBitmap * b() const { return m_attr.b; }
	mg_point_t p() const { return m_attr.p; }
	mg_dim_t d() const { return m_attr.d; }
	mitem_scaled_attr_t & attr(){ return m_attr; }

	mg_size_t w() const { return m_attr.d.w; }
	mg_size_t h() const { return m_attr.d.h; }
	mg_int_t x() const { return m_attr.p.x; }
	mg_int_t y() const { return m_attr.p.y; }

	MItemScaledAttr operator+ (mg_point_t d) const;
	MItemScaledAttr operator+ (mg_dim_t d) const;

	mg_size_t w(mg_size_t v) const;
	mg_size_t h(mg_size_t v) const;


private:
	mitem_scaled_attr_t m_attr;
};


class MItem {
public:
	MItem();

	mg_size_t w(mg_size_t scale, mg_dim_t d);
	mg_size_t h(mg_size_t scale, mg_dim_t d);


	/*! \brief Draw the element on the specified bitmap
	 * \details This method draws the element on the specified bitmap.  It uses
	 * the elements dimensions (see set_dim()) as a percentage of the bitmap size.
	 * So if an element's dimensions are set using set_dim(50,50), the element
	 * will occupy half the width and half the height of the destination bitmap.
	 *
	 * Here is an example of drawing a labeled graphic:
	 *
	 * \code
	 * 	Label label;
	 * 	Gfx gfx;
	 *
	 * 	label.set_text("Hello");
	 * 	label.set_dim(100, 20); //can occupy entire width and 20% of height
	 * 	label.set_center_align();
	 * 	label.draw(b, 0, 5); //draw centered near top of bitmap
	 *
	 * 	gfx.set(GfxAttr::MIC);
	 * 	gfx.set_dim(50, 50);
	 * 	gfx.draw(b, 25, 30);  //draw centered horizontal and below label
	 *
	 * \endcode
	 *
	 * Since label has a height of 20 and is drawn at 5, the bottom of label will
	 * be at about 25.  gfx has a width of 50 and is drawn at 25 which means it will
	 * span from 25 to 75 with the center at 50.
	 *
	 * @param b A pointer to the destination bitmap
	 * @param x X offset in element coordinates (percentage of screen width; see Element::scale)
	 * @param y Y offset in element coordinates (percentage of screen height; see Element::scale)
	 * @param center If true object will be centered at \a x
	 * @return The screen dimensions where the element was drawn
	 */
	virtual void draw(const MItemAttr & attr);
	void draw(draw::MBitmap * b, mitem_int_t x, mitem_int_t y, mitem_size_t w, mitem_size_t h);

	static void set(const MItemAttr & attr, mg_bitmap_t v = 0xFF);
	static void clear(const MItemAttr & attr, mg_bitmap_t v = 0xFF);
	static void invert(const MItemAttr & attr, mg_bitmap_t v = 0xFF);

	/*! \brief Scale value
	 * \details This value determines how all objects are scaled.
	 * The default value is 100. This means a value of 50 is half the target bitmap.
	 * For example, by default the following code sets the element dimensions to be half
	 * the size of the target bitmap.
	 * \code
	 * #include <fogo/Ui/Label.hpp>
	 *
	 * Label l;
	 * l.set_dim(50,50);
	 * \endcode
	 *
	 * To get better resolution, the scale value can be increased.  For example:
	 *
	 * \code
	 * #include <fogo/Ui/Label.hpp>
	 * Element::set_scale(1000);  //this will apply to all elements in the application
	 *
	 * Label l;
	 * l.set_dim(500,500); //sets dimensions to half the size of the target bitmap
	 * \endcode
	 *
	 *
	 */
	static inline mitem_size_t scale(){ return 1000; }

	/*! \brief Sets the scale value (see Element::scale() for details) */
	static inline void set_scale(mitem_size_t s){ m_scale = s; }

	/*! \brief Draw the element on a bitmap
	 *
	 * @param b Bitmap to draw on
	 * @param x Actually x coord of bitmap
	 * @param y Actually y coord of bitmap
	 * @param d Dimensions on bitmap to draw
	 * @param padding Padding (in bitmap dimensions)
	 */
	virtual void draw_to_scale(const MItemScaledAttr & attr);
	void draw_to_scale(draw::MBitmap * b, mg_int_t x, mg_int_t y, mg_size_t w, mg_size_t h);


	/*! \brief Return the dimensions (in pixels) if any Element is drawn on the specified bitmap */
	static mg_dim_t dim_on_bitmap(const MItemAttr & attr);
	static mg_size_t height_on_bitmap(const MItemAttr & attr);
	static mg_size_t width_on_bitmap(const MItemAttr & attr);
	static mg_point_t point_on_bitmap(const MItemAttr & attr);

	/*! \brief Returns true if element is visible */
	inline bool visible() const { return flag(FLAG_VISIBLE); }
	inline void set_visible(bool v = true){ set_flag(FLAG_VISIBLE, v); }

	inline bool align_left() const { return flag(FLAG_ALIGN_LEFT); }
	inline void set_align_left(bool v = true){ set_flag(FLAG_ALIGN_LEFT, v); }
	inline bool align_right() const { return flag(FLAG_ALIGN_RIGHT); }
	inline void set_align_right(bool v = true){ set_flag(FLAG_ALIGN_RIGHT, v); }
	inline bool align_top() const { return flag(FLAG_ALIGN_TOP); }
	inline void set_align_top(bool v = true){ set_flag(FLAG_ALIGN_TOP, v); }
	inline bool align_bottom() const { return flag(FLAG_ALIGN_BOTTOM); }
	inline void set_align_bottom(bool v = true){ set_flag(FLAG_ALIGN_BOTTOM, v); }
	inline bool font_bold() const { return flag(FLAG_FONT_BOLD); }
	inline void set_font_bold(bool v = true){ set_flag(FLAG_FONT_BOLD, v); }

	inline bool dark() const { return flag(FLAG_DARK); }
	inline void set_dark(bool v = true){ set_flag(FLAG_DARK, v); }

	inline bool invert() const { return flag(FLAG_INVERT); }
	inline void set_invert(bool v = true){ set_flag(FLAG_INVERT, v); }



protected:

	mg_point_t point_on_bitmap(draw::MBitmap * b, mitem_size_t x, mitem_size_t y, mg_dim_t d);
	mg_dim_t dim_on_bitmap(draw::MBitmap * b) const;

	enum {
		FLAG_VISIBLE,
		FLAG_ENABLED,
		FLAG_DRAW_CLEAR,
		FLAG_EXCLUSIVE_CHECKLIST,
		FLAG_SCROLL_VISIBLE,
		FLAG_ALIGN_LEFT,
		FLAG_ALIGN_RIGHT,
		FLAG_ALIGN_TOP,
		FLAG_ALIGN_BOTTOM,
		FLAG_FONT_BOLD,
		FLAG_BUSY,
		FLAG_CANCELLED,
		FLAG_DARK,
		FLAG_INVERT,
		FLAG_mitem_TOTAL
	};


	bool flag(int flag) const;
	void set_flag(int flag, bool v = true);

private:
	u32 m_flags;
	static mitem_size_t m_scale;



};

#endif /* DRAW_MITEM_HPP_ */
