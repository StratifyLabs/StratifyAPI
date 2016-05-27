//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_DRAWING_HPP_
#define DRAW_DRAWING_HPP_

#include <mcu/types.h>
#include "../sgfx/Bitmap.hpp"

namespace draw {

typedef u16 drawing_size_t;
typedef i16 drawing_int_t;

typedef struct MCU_PACK {
	drawing_int_t x;
	drawing_int_t y;
} drawing_point_t;

typedef struct MCU_PACK {
	drawing_size_t w;
	drawing_size_t h;
} drawing_dim_t;


typedef struct MCU_PACK {
	sgfx::Bitmap * b;
	drawing_point_t p;
	drawing_dim_t d;
} drawing_attr_t;

typedef struct MCU_PACK {
	drawing_point_t start /*! \brief Animation start point */;
	drawing_dim_t dim /*! \brief Animate within these dimensions with start in the top left corner */;
	drawing_size_t motion_total /*! \brief Total amount of animation movement */;
} drawing_animation_attr_t;


typedef struct MCU_PACK {
	sgfx::Bitmap * b;
	sg_point_t p;
	sg_dim_t d;
} drawing_scaled_attr_t;


drawing_point_t drawing_point(drawing_int_t x, drawing_int_t y);
drawing_dim_t drawing_dim(drawing_size_t w, drawing_size_t h);


class DrawingAttr{
public:
	DrawingAttr();
	DrawingAttr(drawing_attr_t & attr);
	operator drawing_attr_t (){ return m_attr; }

	void set(sgfx::Bitmap * b, drawing_point_t p, drawing_dim_t d);
	void set_bitmap(sgfx::Bitmap * b){ m_attr.b = b; }
	void set_dim(drawing_dim_t d){ m_attr.d = d; }
	void set_dim(drawing_size_t w, drawing_size_t h){ m_attr.d.w = w; m_attr.d.h = h; }
	void set_point(drawing_point_t p){ m_attr.p = p; }
	void set_point(drawing_int_t x, drawing_int_t y){ m_attr.p.x = x; m_attr.p.y = y; }

	drawing_size_t w() const { return m_attr.d.w; }
	drawing_size_t h() const { return m_attr.d.h; }
	drawing_int_t x() const { return m_attr.p.x; }
	drawing_int_t y() const { return m_attr.p.y; }

	sgfx::Bitmap * b() const { return m_attr.b; }
	drawing_point_t p() const { return m_attr.p; }
	drawing_dim_t d() const { return m_attr.d; }
	drawing_attr_t & attr(){ return m_attr; }

	drawing_size_t w(drawing_size_t v) const;
	drawing_size_t h(drawing_size_t v) const;

	DrawingAttr operator+ (drawing_point_t d) const;
	DrawingAttr operator+ (drawing_dim_t d) const;

	drawing_dim_t square(drawing_size_t v) const;
	drawing_dim_t square_w(drawing_size_t v) const;
	drawing_dim_t square_h(drawing_size_t v) const;


private:
	drawing_attr_t m_attr;
};

class DrawingScaledAttr {
public:
	operator drawing_scaled_attr_t (){ return m_attr; }

	void set(sgfx::Bitmap * b, sg_point_t p, sg_dim_t d);
	void set_bitmap(sgfx::Bitmap * b){ m_attr.b = b; }
	void set_dim(sg_dim_t d){ m_attr.d = d; }
	void set_height(sg_size_t h){ m_attr.d.h = h; }
	void set_width(sg_size_t w){ m_attr.d.h = w; }
	void set_x(sg_int_t x){ m_attr.p.x = x; }
	void set_y(sg_int_t y){ m_attr.p.x = y; }
	void set_dim(sg_size_t w, sg_size_t h){ m_attr.d.w = w; m_attr.d.h = h; }
	void set_point(sg_point_t p){ m_attr.p = p; }

	sgfx::Bitmap * b() const { return m_attr.b; }
	sg_point_t p() const { return m_attr.p; }
	sg_dim_t d() const { return m_attr.d; }
	drawing_scaled_attr_t & attr(){ return m_attr; }

	sg_size_t w() const { return m_attr.d.w; }
	sg_size_t h() const { return m_attr.d.h; }
	sg_int_t x() const { return m_attr.p.x; }
	sg_int_t y() const { return m_attr.p.y; }

	DrawingScaledAttr operator+ (sg_point_t d) const;
	DrawingScaledAttr operator+ (sg_dim_t d) const;

	sg_size_t w(sg_size_t v) const;
	sg_size_t h(sg_size_t v) const;


private:
	drawing_scaled_attr_t m_attr;
};


class Drawing {
public:
	Drawing();

	sg_size_t w(sg_size_t scale, sg_dim_t d);
	sg_size_t h(sg_size_t scale, sg_dim_t d);


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
	virtual void draw(const DrawingAttr & attr);
	void draw(sgfx::Bitmap * b, drawing_int_t x, drawing_int_t y, drawing_size_t w, drawing_size_t h);

	static void set(const DrawingAttr & attr, sg_bmap_data_t v = 0xFF);
	static void clear(const DrawingAttr & attr, sg_bmap_data_t v = 0xFF);
	static void invert(const DrawingAttr & attr, sg_bmap_data_t v = 0xFF);

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
	static inline drawing_size_t scale(){ return 1000; }

	/*! \brief Sets the scale value (see Element::scale() for details) */
	static inline void set_scale(drawing_size_t s){ m_scale = s; }

	/*! \brief Draw the element on a bitmap
	 *
	 * @param b Bitmap to draw on
	 * @param x Actually x coord of bitmap
	 * @param y Actually y coord of bitmap
	 * @param d Dimensions on bitmap to draw
	 * @param padding Padding (in bitmap dimensions)
	 */
	virtual void draw_to_scale(const DrawingScaledAttr & attr);
	void draw_to_scale(sgfx::Bitmap * b, sg_int_t x, sg_int_t y, sg_size_t w, sg_size_t h);


	/*! \brief Return the dimensions (in pixels) if any Element is drawn on the specified bitmap */
	static sg_dim_t dim_on_bitmap(const DrawingAttr & attr);
	static sg_size_t height_on_bitmap(const DrawingAttr & attr);
	static sg_size_t width_on_bitmap(const DrawingAttr & attr);
	static sg_point_t point_on_bitmap(const DrawingAttr & attr);

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

	sg_point_t point_on_bitmap(sgfx::Bitmap * b, drawing_size_t x, drawing_size_t y, sg_dim_t d);
	sg_dim_t dim_on_bitmap(sgfx::Bitmap * b) const;

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
		FLAG_DRAWING_TOTAL
	};


	bool flag(int flag) const;
	void set_flag(int flag, bool v = true);

private:
	u32 m_flags;
	static drawing_size_t m_scale;


};

};

#endif /* DRAW_DRAWING_HPP_ */
