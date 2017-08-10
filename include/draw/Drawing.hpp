//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_DRAWING_HPP_
#define DRAW_DRAWING_HPP_

#include <mcu/types.h>
#include "../sgfx/Bitmap.hpp"
#include "../sgfx/Pen.hpp"

namespace draw {

/*! \details Drawing size (unsigned) */
typedef u16 drawing_size_t;
/*! \details Drawing position (signed) */
typedef s16 drawing_int_t;

/*! \brief Holds a coordinate (point) in the drawing system */
typedef struct MCU_PACK {
	drawing_int_t x /*! X position */;
	drawing_int_t y /*! Y position */;
} drawing_point_t;

/*! \brief Holds a dimension in the drawing system */
typedef struct MCU_PACK {
	drawing_size_t width /*! Width of the object */;
	drawing_size_t height /*! Height of the object */;
} drawing_dim_t;


/*! \brief Data used for drawing a draw::Drawing on a bitmap
 * \details This data structure holds a real bitmap but the point
 * and dimensions haven't been mapped to the bitmap.
 * The point \a p and dimension \a d are both in a universal coordinate
 * system from 0 to draw::Drawing::scale().  For example
 * if draw::Drawing::scale() is 1000 and p.x and p.y both are equal to 500,
 * the top left corner of any item drawn using these attibutes will be
 * in the center of the target bitmap (regardless of the target bitmap's
 * size).
 */
typedef struct MCU_PACK {
	sgfx::Bitmap * bitmap /*! A pointer to the target bitmap */;
	drawing_point_t point /*! The point on the bitmap where the draw::Drawing will be drawn */;
	drawing_dim_t dim /*! The size of the draw::Drawing on the target bitmap */;
	sgfx::Bitmap * scratch /*! A pointer to the scratch bitmap used for animations (0 if not available) */;
} drawing_attr_t;


/*! \brief Attributes for drawing directly on a bitmap using bitmap coordinates */
typedef struct MCU_PACK {
	sgfx::Bitmap * bitmap /*! The target bitmap */;
	sg_point_t point /*! The point on the bitmap */;
	sg_dim_t dim /*! The dimesions of where to fit the item within the bitmap */;
} drawing_scaled_attr_t;


/*! \details This returns a drawing_point_t populated with x and y */
drawing_point_t drawing_point(drawing_int_t x, drawing_int_t y);

/*! \details This returns a drawing_dim_t populated with the width and height.
 *
 * @param w Width of the returned data
 * @param h Height of the retured data
 * @return A drawing_dim_t with w and h populated as specified
 */
drawing_dim_t drawing_dim(drawing_size_t w, drawing_size_t h);

/*! \brief Drawing Attribute Class
 * \details This class contains the information needed to draw various
 * Drawing objects on a bitmap.
 *
 * This class is passed to Drawing::drawidth() to render graphics on a bitmap.
 *
 *
 */
class DrawingAttr{
public:

	/*! \details Construct an object */
	DrawingAttr();

	/*! \details Construct an object from a drawing_attr_t */
	DrawingAttr(const drawing_attr_t & attr);

	/*! \details Return the underlying drawing_attr_t */
	operator drawing_attr_t (){ return m_attr; }

	/*! \details Assign the bitmap point and dimensions */
	void set(sgfx::Bitmap & b, drawing_point_t p, drawing_dim_t d, sgfx::Bitmap * scratch = 0);

	/*! \details Set the bitmap */
	void set_bitmap(sgfx::Bitmap & b){ m_attr.bitmap = &b; }

	/*! \details Set the scratch bitmap */
	void set_scratch(sgfx::Bitmap * b){ m_attr.scratch = b; }

	/*! \details Set the dimensions.  Both width and height are from 0 to 1000. */
	void set_dim(drawing_dim_t d){ m_attr.dim = d; }

	/*! \details Set the dimensions.  Both width and height are from 0 to 1000. */
	void set_dim(drawing_size_t w, drawing_size_t h){ m_attr.dim.width = w; m_attr.dim.height = h; }

	/*! \details Set the location.  Both x and y are from 0 to 1000. */
	void set_point(drawing_point_t p){ m_attr.point = p; }

	/*! \details Set the location.  Both x and y are from 0 to 1000. */
	void set_point(drawing_int_t x, drawing_int_t y){ m_attr.point.x = x; m_attr.point.y = y; }

	/*! \details Return the width */
	drawing_size_t width() const { return m_attr.dim.width; }
	/*! \details Return the height */
	drawing_size_t height() const { return m_attr.dim.height; }
	/*! \details Return the x value */
	drawing_int_t x() const { return m_attr.point.x; }
	/*! \details Return the y value */
	drawing_int_t y() const { return m_attr.point.y; }

	/*! \details Access the bitmap */
	sgfx::Bitmap & bitmap() const { return *(m_attr.bitmap); }

	/*! \details Access the scratch bitmap */
	sgfx::Bitmap * scratcheight() const { return (m_attr.scratch); }

	/*! \details Access the position (point) */
	drawing_point_t point() const { return m_attr.point; }
	/*! \details Access the dimensions (dim) */
	drawing_dim_t dim() const { return m_attr.dim; }

	/*! \details Access the underlying attr object */
	drawing_attr_t & attr(){ return m_attr; }

	/*! \details Calculate the scaled width (width of object on the bitmap) */
	drawing_size_t calc_width(drawing_size_t v) const;

	/*! \details Calculate the scaled height (height of object on the bitmap) */
	drawing_size_t calc_height(drawing_size_t v) const;

	/*! \details Add a drawing_point_t offset */
	DrawingAttr operator+ (drawing_point_t d) const;
	/*! \details Update the dimension (must come after adding drawing_point_t) */
	DrawingAttr operator+ (drawing_dim_t d) const;

	/*! \details Calculate dimensions that will map to the bitmap as a square.
	 *
	 * @param v The maximum width or height
	 * @return Square dimensions
	 */
	drawing_dim_t calc_square(drawing_size_t v) const;

	/*! \details Calculate dimensions that will map to the bitmap as a square
	 * with the given width.
	 *
	 * @param v The width (height will be calculated)
	 * @return Square dimensions
	 */
	drawing_dim_t calc_square_w(drawing_size_t v) const;

	/*! \details Calculate dimensions that will map to the bitmap as a square
	 * with the given height.
	 *
	 * @param v The height (width will be calculated)
	 * @return Square dimensions
	 */
	drawing_dim_t calc_square_h(drawing_size_t v) const;


private:
	drawing_attr_t m_attr;
};

/*! \brief Scaled Drawing Attributes
 * \details This is similar to draw::DrawingAttr but the point
 * and dimensions have been scaled to fit in the target bitmap.
 */
class DrawingScaledAttr {
public:
	operator drawing_scaled_attr_t (){ return m_attr; }

	void set(sgfx::Bitmap & b, sg_point_t p, sg_dim_t d);

	/*! \details Assign a value to the bitmap pointer using a reference */
	void set_bitmap(sgfx::Bitmap & b){ m_attr.bitmap = &b; }
	/*! \details Assign dimensions */
	void set_dim(sg_dim_t d){ m_attr.dim = d; }
	/*! \details Set the height of the object */
	void set_height(sg_size_t h){ m_attr.dim.height = h; }
	/*! \details Set the width of the object */
	void set_width(sg_size_t w){ m_attr.dim.height = w; }
	/*! \details Set the x value of the object */
	void set_x(sg_int_t x){ m_attr.point.x = x; }
	/*! \details Set the y value of the object */
	void set_y(sg_int_t y){ m_attr.point.x = y; }
	/*! \details Assign dimensions using width and height parameters */
	void set_dim(sg_size_t w, sg_size_t h){ m_attr.dim.width = w; m_attr.dim.height = h; }

	/*! \details Assign the position */
	void set_point(sg_point_t p){ m_attr.point = p; }

	sgfx::Bitmap & bitmap() const { return *m_attr.bitmap; }
	sg_point_t point() const { return m_attr.point; }
	sg_dim_t dim() const { return m_attr.dim; }
	drawing_scaled_attr_t & attr(){ return m_attr; }

	/*! \details Return the width */
	sg_size_t width() const { return m_attr.dim.width; }
	/*! \details Return the height */
	sg_size_t height() const { return m_attr.dim.height; }
	/*! \details Return the x value */
	sg_int_t x() const { return m_attr.point.x; }
	/*! \details Return the y value */
	sg_int_t y() const { return m_attr.point.y; }

	/*! \details Add an sg_point_t */
	DrawingScaledAttr operator+ (sg_point_t d) const;
	/*! \details Add an sg_dim_t */
	DrawingScaledAttr operator+ (sg_dim_t d) const;

	/*! \details Calculate a width value
	 *
	 * @param v Unscaled drawing dimensions
	 * @return
	 */
	sg_size_t calc_width(drawing_size_t v) const;
	sg_size_t calc_height(drawing_size_t v) const;


private:
	drawing_scaled_attr_t m_attr;
};


/*! \brief Drawing Class
 * \details This is the base class for creating drawings.  A Drawing class
 * allows for nesting and positioning of graphics within a bitmap.
 *
 * \code
 *	void MyObject::draw(const DrawingAttr::attr){
 *		Icon my_icon;
 *		drawing_dim_t square;
 *
 *		square = attr.square_w(500);
 *
 *		//this will draw my icon centered in the bitmap specified by attr
 *		//adding drawing_point(250, 250) offset the icon by 25% of the bitmap in both x and y
 *		//adding a drawing_dim_t will then scale icon to fit in a square that is half the width of the bitmap (see square_width())
 *		//draw will call the underlying draw_to_scale() method unless Icon re-implements draw
 *		my_icon.draw(attr + drawing_point(250, 250) + square);
 *
 *	}
 *
 *
 * \endcode
 *
 */
class Drawing {
public:
	Drawing();

	sg_size_t w(sg_size_t scale, sg_dim_t d);
	sg_size_t h(sg_size_t scale, sg_dim_t d);


	/*! \details This method draws the object using the specified drawing attributes.
	 *
	 * The attributes specify which bitmap to draw on, what size to draw, and where to draw.
	 * The dimensions and position are scaled to fit on the bitmap.
	 *
	 */
	virtual void draw(const DrawingAttr & attr);
	virtual void draw_scratch(const DrawingAttr & attr);
	void draw(sgfx::Bitmap & b, drawing_int_t x, drawing_int_t y, drawing_size_t w, drawing_size_t h);

	/*! \details This method will set the pixels in the area of the bitmap
	 * specified.
	 *
	 * @param attr Specifies the bitmap and area
	 * @param v Specifies the fill pattern
	 */
	static void set(const DrawingAttr & attr, sg_bmap_data_t v = 0xFF);

	/*! \details This method will clear the pixels in the area of the bitmap
	 * specified.
	 *
	 * @param attr Specifies the bitmap and area
	 * @param v Specifies the fill pattern
	 */
	static void clear(const DrawingAttr & attr, sg_bmap_data_t v = 0xFF);

	/*! \details This method will invert the pixels in the area of the bitmap
	 * specified.
	 *
	 * @param attr Specifies the bitmap and area
	 * @param v Specifies the fill pattern
	 */
	static void invert(const DrawingAttr & attr, sg_bmap_data_t v = 0xFF);

	/*! \details This value determines how all objects are scaled.
	 * The default value is 1000. This means a value of 500 is half the target bitmap.
	 *
	 */
	static inline drawing_size_t scale(){ return 1000; }

	/*! \brief Sets the scale value (see Element::scale() for details) */
	static inline void set_scale(drawing_size_t s){ m_scale = s; }

	/*! \details This methods draws the drawing on the specified attributes.
	 *
	 * @param attr Specifies the bitmap, point and area to draw the drawing
	 */
	virtual void draw_to_scale(const DrawingScaledAttr & attr);
	void draw_to_scale(sgfx::Bitmap & b, sg_int_t x, sg_int_t y, sg_size_t w, sg_size_t h);


	/*! \details Return the dimensions (in pixels) if any Element is drawn on the specified bitmap */
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
	inline void set_align_center(bool v = true){
		if( v ){
			set_align_right(false);
			set_align_left(false);
		}
	}

	inline bool align_top() const { return flag(FLAG_ALIGN_TOP); }
	inline void set_align_top(bool v = true){ set_flag(FLAG_ALIGN_TOP, v); }
	inline bool align_bottom() const { return flag(FLAG_ALIGN_BOTTOM); }
	inline void set_align_bottom(bool v = true){ set_flag(FLAG_ALIGN_BOTTOM, v); }
	inline void set_align_middle(bool v = true){
		if( v ){
			set_align_top(false);
			set_align_bottom(false);
		}
	}

	inline bool dark() const { return flag(FLAG_DARK); }
	inline void set_dark(bool v = true){ set_flag(FLAG_DARK, v); }

	inline bool invert() const { return flag(FLAG_INVERT); }
	inline void set_invert(bool v = true){ set_flag(FLAG_INVERT, v); }



protected:

	sg_point_t point_on_bitmap(sgfx::Bitmap & b, drawing_size_t x, drawing_size_t y, sg_dim_t d);
	sg_dim_t dim_on_bitmap(sgfx::Bitmap & b) const;

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
