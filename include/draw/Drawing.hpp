/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
//Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#ifndef SAPI_DRAW_DRAWING_HPP_
#define SAPI_DRAW_DRAWING_HPP_

#include <mcu/types.h>
#include "../sgfx/Bitmap.hpp"
#include "../sgfx/Pen.hpp"
#include "../api/DrawObject.hpp"

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
} drawing_area_t;

typedef struct MCU_PACK {
	drawing_point_t point /*! Point in the region */;
	drawing_area_t area /*! Height of the object */;
} drawing_region_t;


sg_color_t color_transparent();

/*! \brief Data used for drawing a draw::Drawing on a bitmap
 * \details This data structure holds a real bitmap but the point
 * and dimensions haven't been mapped to the bitmap.
 * The point \a p and dimension \a d are both in a universal coordinate
 * system from 0 to draw::DrawingAttributes::scale().  For example
 * if draw::DrawingAttributes::scale() is 1000 and p.x and p.y both are equal to 500,
 * the top left corner of any item drawn using these attibutes will be
 * in the center of the target bitmap (regardless of the target bitmap's
 * size).
 */
typedef struct MCU_PACK {
	sgfx::Bitmap * bitmap /*! A pointer to the target bitmap */;
	drawing_region_t region /*! The region on the bitmap where draw::Drawing will be drawn */;
	sgfx::Bitmap * scratch /*! A pointer to the scratch bitmap used for animations (0 if not available) */;
	drawing_point_t offset /*! An offset that applies a shift to any objects drawn on the bitmap */;
} drawing_attr_t;


/*! \brief Attributes for drawing directly on a bitmap using bitmap coordinates */
typedef struct MCU_PACK {
	sgfx::Bitmap * bitmap /*! The target bitmap */;
	sg_region_t region /*! The region on the bitmap where draw::DrawingScaled will be drawn */;
} drawing_scaled_attr_t;


/*! \details This returns a drawing_point_t populated with x and y */
drawing_point_t drawing_point(drawing_int_t x, drawing_int_t y);

/*! \details This returns a drawing_area_t populated with the width and height.
 *
 * @param w Width of the returned data
 * @param h Height of the retured data
 * @return A drawing_area_t with w and h populated as specified
 */
drawing_area_t drawing_area(drawing_size_t w, drawing_size_t h);
drawing_area_t drawing_dim(drawing_size_t w, drawing_size_t h);


class DrawingArea : public api::InfoObject {
public:

	using Width = arg::Argument<drawing_size_t, struct DrawingAreaWidthTag>;
	using Height = arg::Argument<drawing_size_t, struct DrawingAreaHeightTag>;

	DrawingArea(){
		m_area.height = 0;
		m_area.width = 0;
	}

	DrawingArea(drawing_size_t w, drawing_size_t h){
		m_area.height = h;
		m_area.width = w;
	}

	DrawingArea(Width w, Height h){
		m_area.height = h.argument();
		m_area.width = w.argument();
	}

	DrawingArea(const drawing_area_t & area){
		m_area = area;
	}

	bool is_valid() const {
		return m_area.width * m_area.height;
	}

	static DrawingArea maximum();

	drawing_size_t width() const { return m_area.width; }
	drawing_size_t height() const { return m_area.height; }

	DrawingArea & set_width(drawing_size_t value){ m_area.width = value; return *this; }
	DrawingArea & set_height(drawing_size_t value){ m_area.height = value; return *this; }

	const drawing_area_t & area() const { return m_area; }
	drawing_area_t & area(){ return m_area; }

	operator	const drawing_area_t & () const { return m_area; }


private:
	drawing_area_t m_area;
};

class DrawingPoint {
public:

	using X = arg::Argument<drawing_int_t, struct DrawingPointXTag>;
	using Y = arg::Argument<drawing_int_t, struct DrawingPointYTag>;


	DrawingPoint(){ m_point.x = 0; m_point.y = 0; }
	DrawingPoint(X x, Y y){
		m_point.x = x.argument();
		m_point.y = y.argument();
	}

	DrawingPoint(drawing_int_t x, drawing_int_t y){
		m_point.x = x;
		m_point.y = y;
	}

	DrawingPoint(const drawing_point_t & point){
		m_point = point;
	}

	static DrawingPoint origin(){
		return DrawingPoint(0,0);
	}

	drawing_int_t x() const { return m_point.x; }
	void set_x(drawing_int_t value){ m_point.x = value; }

	drawing_int_t y() const { return m_point.y; }
	void set_y(drawing_int_t value){ m_point.y = value; }

	const drawing_point_t & point() const { return m_point; }
	drawing_point_t & point(){ return m_point; }

	operator	const drawing_point_t & () const { return m_point; }


private:
	drawing_point_t m_point;
};

class DrawingRegion : public api::InfoObject {

public:
	DrawingRegion(){
		m_region.point = DrawingPoint(0,0);
	}

	DrawingRegion(const drawing_region_t & region){
		m_region = region;
	}

	bool is_valid() const {
		return DrawingArea(m_region.area).is_valid();
	}

	DrawingRegion(const DrawingPoint & point, const DrawingArea & area){
		m_region.point = point;
		m_region.area = area;
	}

	DrawingRegion & operator << (const DrawingArea & area){
		m_region.area = area; return *this;
	}

	DrawingRegion & operator << (const DrawingPoint & point){
		m_region.point = point; return *this;
	}

	DrawingArea area() const { return m_region.area; }
	DrawingPoint point() const { return m_region.point; }

	operator	const drawing_region_t & () const { return m_region; }

private:
	drawing_region_t m_region;
};

/*! \brief Drawing Attribute Class
 * \details This class contains the information needed to draw various
 * Drawing objects on a bitmap.
 *
 * This class is passed to Drawing::draw() to render graphics on a bitmap.
 *
 * The following code will draw an rectangle in the middle of a 64x64
 * pixel bitmap.
 *
 * ```
 * #include <sapi/sgfx.hpp>
 * #include <sapi/draw.hpp>
 *
 * Bitmap bitmap(64,64); //64x64 pixel bitmap
 * DrawingAttributes attributes;
 *
 * attributes << bitmap; //apply bitmap to attributes
 *
 * Rectangle().draw(attributes + DrawingPoint(250,250) + DrawingArea(500,500));
 * ```
 *
 * A DrawingAttributes object is always passed to the Drawing::draw() method.
 *
 * The + operator when adding a DrawingPoint will create a new
 * DrawingAttributes object that starts at the specified point. The coordinates
 * are the width of the bitmap / 1000.
 *
 * The + operator when adding a DrawingArea will scale the region of the
 * DrawingAttributes based on the size of the bitmap where 1000 represents
 * full width and height.
 *
 * So in the example above, the top left corner of the rectangle is mapped
 * to 16,16 (64*250/1000=16). The width and height of the rectangle are both
 * 32 (64*500/1000=32).
 *
 * These operators allow the cascading of drawing objects as the region scales.
 * For example, if we create a DrawingAttributes object that is already
 * region limited, the + operators will operate within the existing region.
 *
 * ```
 * Bitmap bitmap(64,64); //64x64 pixel bitmap
 * DrawingAttributes attributes;
 *
 * DrawingRegion region(DrawingPoint(0,0), DrawingArea(500,500));
 * attributes << bitmap << region; //apply bitmap and region to attributes
 *
 * //rectangle will be 500/1000 of 500/1000 of the area of the bitmap
 * Rectangle().draw(attributes + DrawingArea(500,500));
 * ```
 *
 * In this second example, because the DrawingAttributes region was already set
 * to a subregion of the bitmap, the + operator with the DrawingArea scales
 * the region down by 500/1000. So the rectangle is 16,16 (64*500/1000 * 500/1000).
 *
 * Using these operators to cascade is great for creating compound
 * drawing objects like a button.
 *
 * ```
 * class Button: public Drawing {
 * public:
 *  void draw(const DrawingAttributes & attributes){
 *    RoundedRectangle().draw(attributes); //files entire attributes region
 *
 *    //The Icon is scaled down to half the width/height and centered
 *    Icon().set_name("OK").draw(attributes + DrawingPoint(250,250) + DrawingArea(500,500));
 *
 *  }
 * };
 * ```
 *
 */
class DrawingAttributes {
public:

	/*! \details Construct an object */
	DrawingAttributes();

	/*! \details Construct an object from a drawing_attr_t */
	DrawingAttributes(const drawing_attr_t & attr);

	/*! \details Return the underlying drawing_attr_t */
	operator drawing_attr_t & (){ return m_attr; }
	operator const drawing_attr_t & () const { return m_attr; }

	/*! \details Access the underlying attr object */
	drawing_attr_t & attributes(){ return m_attr; }
	const drawing_attr_t & attributes() const { return m_attr; }
	drawing_attr_t & attr(){ return m_attr; }

	DrawingAttributes(sgfx::Bitmap & bitmap);
	DrawingAttributes(sgfx::Bitmap & bitmap, const DrawingRegion & region);

	DrawingAttributes & operator << (sgfx::Bitmap & bitmap);
	DrawingAttributes & operator << (const DrawingRegion & region);
	DrawingAttributes & operator << (const DrawingPoint & point);
	DrawingAttributes & operator << (const DrawingArea & area);

	bool is_valid() const { return m_attr.bitmap != 0; }


	/*! \details Assign the bitmap point and dimensions */
	void set(sgfx::Bitmap & b, drawing_point_t p, drawing_area_t d, sgfx::Bitmap * scratch = 0);

	/*! \details Set the bitmap */
	DrawingAttributes & set_bitmap(sgfx::Bitmap & b){ m_attr.bitmap = &b; return *this; }

	/*! \details Set the scratch bitmap */
	DrawingAttributes & set_scratch(sgfx::Bitmap * b){ m_attr.scratch = b; return *this; }

	/*! \details Set the dimensions.  Both width and height are from 0 to 1000. */
	DrawingAttributes & set_area(const DrawingArea & value){ m_attr.region.area = value; return *this; }

	/*! \details Set the location.  Both x and y are from 0 to 1000. */
	DrawingAttributes & set_point(const DrawingPoint & p){ m_attr.region.point = p; return *this; }

	/*! \details Set the location.  Both x and y are from 0 to 1000. */
	//void set_point(drawing_int_t x, drawing_int_t y){ m_attr.region.point.x = x; m_attr.region.point.y = y; }

	/*! \details Return the width */
	drawing_size_t width() const { return m_attr.region.area.width; }
	/*! \details Return the height */
	drawing_size_t height() const { return m_attr.region.area.height; }
	/*! \details Return the x value */
	drawing_int_t x() const { return m_attr.region.point.x; }
	/*! \details Return the y value */
	drawing_int_t y() const { return m_attr.region.point.y; }

	/*! \details Checks to see if bitmap is available. */
	bool is_bitmap_available() const { return m_attr.bitmap != 0; }

	/*! \details Access the bitmap */
	sgfx::Bitmap & bitmap() const { return *(m_attr.bitmap); }

	/*! \details Access the scratch bitmap */
	sgfx::Bitmap * scratch() const { return (m_attr.scratch); }


	/*! \details Returns a copy of the region. */
	DrawingRegion region() const { return m_attr.region; }
	/*! \details Returns a copy of the position. */
	DrawingPoint point() const { return m_attr.region.point; }
	/*! \details Returns a copy of the dimensions. */
	DrawingArea area() const { return m_attr.region.area; }



	/*! \details Calculate the scaled height relative to the height of the DrawingAttributes object.
	 *
	 * @param v The value to calculate
	 *
	 * This method will calculate a height relative to the heigh of the object
	 * and the scale() value.  For example, if the current object maps to a region of
	 * a bitmap height of 500 pixels, this method will return a drawing height that will
	 * be from 0 to 500 pixels with \a v being from 0 to DrawingAttributes::scale().
	 *
	 */
	drawing_size_t calculate_height(drawing_size_t v) const;

	/*! \details Calculate the scaled width (width of object on the bitmap) */
	drawing_size_t calculate_width(drawing_size_t v) const;

	/*! \details Add a drawing_point_t offset
	 *
	 * This operated can be used to create a subregion within the object
	 * when used with operation+ (drawing_area_t).
	 *
	 * For example:
	 * \code
	 * DrawingAttributes attr0;
	 * DrawingAttributes attr1;
	 * attr0 = attr1 + DrawingPoint(250,250) + DrawingArea(500,500);
	 * \endcode
	 *
	 * When mapped to the same bitmap, attr0 will be half the height and half
	 * the width of attr1. attr0 and attr1 will have the same center point.
	 *
	 * These operators can be used to draw sub-drawings on drawings. The following
	 * example draws a rectangles recursively in an object.
	 *
	 * ```
	 * void draw_rect_recursive(const DrawingAttributes & attr){
	 *  static sg_color_t color = 0;
	 *  Rectangle().set_color(color++ % 1).draw(attr + DrawingPoint(250,250) + DrawingArea(500,500);
	 *  if( attr.width() > 100 ){
	 *  	draw_rect_recursive(attr);
	 *	 }
	 * }
	 * ```
	 *
	 */
	DrawingAttributes operator+ (const drawing_point_t & d) const;
	/*! \details Update the dimension (must come after adding drawing_point_t) */
	DrawingAttributes operator+ (const drawing_area_t & d) const;

	DrawingAttributes operator+ (const DrawingPoint & d) const;
	DrawingAttributes operator+ (const DrawingArea & d) const;

	DrawingAttributes operator+ (DrawingRegion region) const{
		return *this + region.point() + region.area();
	}

	/*! \details Calculate dimensions that will map to the bitmap as a square.
	 *
	 * @param v The maximum width or height
	 * @return Square dimensions
	 */
	drawing_area_t calculate_square(drawing_size_t v) const;

	/*! \details Calculate dimensions that will map to the bitmap as a square
	 * with the given width.
	 *
	 * @param v The width (height will be calculated)
	 * @return Square dimensions
	 */
	drawing_area_t calculate_square_width(drawing_size_t v) const;


	/*! \details Calculate dimensions that will map to the bitmap as a square
	 * with the given height.
	 *
	 * @param v The height (width will be calculated)
	 * @return Square dimensions
	 */
	drawing_area_t calculate_square_height(drawing_size_t v) const;


	static sg_area_t calculate_area_on_bitmap(const DrawingAttributes & attr);
	static sg_size_t calculate_height_on_bitmap(const DrawingAttributes & attr);
	static sg_size_t calculate_height_on_bitmap(const DrawingAttributes & attr, drawing_size_t value);
	static sg_size_t calculate_width_on_bitmap(const DrawingAttributes & attr);
	static sg_size_t calculate_width_on_bitmap(const DrawingAttributes & attr, drawing_size_t value);
	static sg_point_t calculate_point_on_bitmap(const DrawingAttributes & attr);
	static sg_region_t calculate_region_on_bitmap(const DrawingAttributes & attr);

	sg_area_t calculate_area_on_bitmap() const { return calculate_area_on_bitmap(*this); }
	sg_size_t calculate_height_on_bitmap() const { return calculate_height_on_bitmap(*this); }
	sg_size_t calculate_height_on_bitmap(drawing_size_t value) const { return calculate_height_on_bitmap(*this, value); }
	sg_size_t calculate_width_on_bitmap() const { return calculate_width_on_bitmap(*this); }
	sg_size_t calculate_width_on_bitmap(drawing_size_t value) const { return calculate_width_on_bitmap(*this, value); }
	sg_point_t calculate_point_on_bitmap() const { return calculate_point_on_bitmap(*this); }
	sg_region_t calculate_region_on_bitmap() const { return calculate_region_on_bitmap(*this); }


	/*! \details This value determines how all objects are scaled.
	 *
	 * The default value is 1000. This means a value of 500 is half the target bitmap.
	 *
	 * sg_size_t * drawing_size_t / scale() = sg_size_t (drawing_size_t cancels with scale())
	 *
	 */
	static drawing_size_t scale(){ return 1000; }


private:
	/*! \cond */
	drawing_attr_t m_attr;
	/*! \endcond */
};


typedef DrawingAttributes DrawingAttr;

class DrawingWindow {
public:

   DrawingWindow(){}


   DrawingWindow(
         const DrawingAttributes & parent_attributes,
         const DrawingRegion & window_region
         );

   DrawingWindow& update(
         const DrawingAttributes & parent_attributes,
         const DrawingRegion & window_region
         );

   /*! \details Returns a reference to the
    * newly created drawing attributes.
    *
    * The bitmap contained in the attributes
    * should be copied or drawn on the master
    * bitmap in the window defined by region().
    *
    */
   DrawingAttributes & attributes();

   /*! \details Returns the region where
    * the window maps to the master bitmap.
    */
   sgfx::Region & region();

private:
   DrawingAttributes m_attributes;
   sgfx::Region m_region;
   sgfx::Bitmap m_bitmap;

};


/*! \brief Scaled Drawing Attributes
 * \details This is similar to draw::DrawingAttributes but the point
 * and area have been scaled to fit in the target bitmap.
 */
class DrawingScaledAttributes : public api::InfoObject {
public:

	DrawingScaledAttributes(){}
	DrawingScaledAttributes(const DrawingAttributes & attr){
		set(attr.bitmap(), attr.calculate_point_on_bitmap(), attr.calculate_area_on_bitmap());
	}

	operator drawing_scaled_attr_t (){ return m_attr; }

	void set(sgfx::Bitmap & b, sg_point_t p, sg_area_t d);

	/*! \details Assign a value to the bitmap pointer using a reference. */
	DrawingScaledAttributes & set_bitmap(sgfx::Bitmap & b){ m_attr.bitmap = &b; return *this; }
	/*! \details Assigns area. */
	DrawingScaledAttributes & set_area(const sgfx::Area & area){ m_attr.region.area = area.area(); return *this; }
	/*! \details Sets the height of the object */
	DrawingScaledAttributes & set_height(sg_size_t h){ m_attr.region.area.height = h; return *this; }
	/*! \details Sets the width of the object. */
	DrawingScaledAttributes & set_width(sg_size_t w){ m_attr.region.area.height = w; return *this; }
	/*! \details Sets the x value of the object. */
	DrawingScaledAttributes & set_x(sg_int_t x){ m_attr.region.point.x = x; return *this; }
	/*! \details Sets the y value of the object. */
	DrawingScaledAttributes & set_y(sg_int_t y){ m_attr.region.point.y = y; return *this; }

	/*! \details Assign the position */
	void set_point(sg_point_t p){ m_attr.region.point = p; }

	sgfx::Bitmap & bitmap() const { return *m_attr.bitmap; }

	sgfx::Region region() const { return m_attr.region; }
	sgfx::Point point() const { return m_attr.region.point; }
	sgfx::Area area() const { return m_attr.region.area; }
	drawing_scaled_attr_t & attr(){ return m_attr; }

	/*! \details Return the width */
	sg_size_t width() const { return m_attr.region.area.width; }
	/*! \details Return the height */
	sg_size_t height() const { return m_attr.region.area.height; }
	/*! \details Return the x value */
	sg_int_t x() const { return m_attr.region.point.x; }
	/*! \details Return the y value */
	sg_int_t y() const { return m_attr.region.point.y; }

	/*! \details Add an sg_point_t */
	DrawingScaledAttributes operator+ (sg_point_t d) const;
	/*! \details Add an sg_area_t */
	DrawingScaledAttributes operator+ (sg_area_t d) const;

	/*! \details Calculate a width value
	 *
	 * @param v Unscaled drawing dimensions
	 * @return
	 */
	sg_size_t calculate_width(drawing_size_t v) const;
	sg_size_t calculate_height(drawing_size_t v) const;


private:
	drawing_scaled_attr_t m_attr;
};

typedef DrawingScaledAttributes DrawingScaledAttr;



/*! \brief Drawing Class
 * \details This is the base class for creating drawings.  A Drawing class
 * allows for nesting and positioning of graphics within a bitmap.
 *
 * ```
 *	void MyObject::draw(const DrawingAttributes::attr){
 *		Icon my_icon;
 *		drawing_area_t square;
 *
 *		square = attr.square_w(500);
 *
 *		//this will draw my icon centered in the bitmap specified by attr
 *		//adding drawing_point(250, 250) offset the icon by 25% of the bitmap in both x and y
 *		//adding a drawing_area_t will then scale icon to fit in a square that is half the width of the bitmap (see square_width())
 *		//draw will call the underlying draw_to_scale() method unless Icon re-implements draw
 *		my_icon.draw(attr + DrawingPoint(250, 250) + square);
 *
 *	}
 * ```
 *
 */
class Drawing : public virtual api::WorkObject {
public:
	Drawing();



	static sg_size_t width(sg_size_t scale, sg_area_t d);
	static sg_size_t height(sg_size_t scale, sg_area_t d);

	static sg_size_t w(sg_size_t scale, sg_area_t d){ return width(scale, d); }
	static sg_size_t h(sg_size_t scale, sg_area_t d){ return height(scale, d); }

	/*! \details This method draws the object using the specified drawing attributes.
	 *
	 * The attributes specify which bitmap to draw on, what size to draw, and where to draw.
	 * The dimensions and position are scaled to fit on the bitmap.
	 *
	 */
	virtual void draw(const DrawingAttributes & attr);

	void draw(const DrawingAttributes & attr, const DrawingPoint & point, const DrawingArea & area){
		draw(attr + point + area);
	}

	virtual void draw_scratch(const DrawingAttributes & attr);
	void draw(sgfx::Bitmap & b, drawing_int_t x, drawing_int_t y, drawing_size_t w, drawing_size_t h);

	/*! \details This method will set the pixels in the area of the bitmap
	 * specified.
	 *
	 * @param attr Specifies the bitmap and area
	 * @param color Specifies the fill pattern
	 */
	static void set(const DrawingAttributes & attr, sg_color_t color = 0xffff);

	static void draw_pattern(const DrawingAttributes & attr, sg_bmap_data_t odd_pattern, sg_bmap_data_t even_pattern, sg_size_t pattern_height);
	static void draw_checkerboard(const DrawingAttributes & attr, sg_size_t pattern_height = 1);

	/*! \details This method will clear the pixels in the area of the bitmap
	 * specified.
	 *
	 * @param attr Specifies the bitmap and area
	 * @param v Specifies the fill pattern
	 */
	static void clear(const DrawingAttributes & attr);

	/*! \details This method will invert the pixels in the area of the bitmap
	 * specified.
	 *
	 * @param attr Specifies the bitmap and area
	 * @param v Specifies the fill pattern
	 */
	static void invert(const DrawingAttributes & attr, sg_bmap_data_t v = 0xFF);

	/*! \brief Sets the scale value (see Element::scale() for details). */
	static void set_scale(drawing_size_t s){ m_scale = s; }

	/*! \details This methods draws the drawing on the specified attributes.
	 *
	 * @param attr Specifies the bitmap, point and area to draw the drawing
	 */
	virtual void draw_to_scale(const DrawingScaledAttr & attr);
	void draw_to_scale(sgfx::Bitmap & b, sg_int_t x, sg_int_t y, sg_size_t w, sg_size_t h);

	sg_color_t default_color(){ return m_default_color; }
	void set_default_color(sg_color_t value){ m_default_color = value; }

	sg_color_t color(){ return m_color; }
	void set_color(sg_color_t value){ m_color = value; }

	/*! \brief Returns true if element is visible */
	bool is_visible() const { return flag(FLAG_VISIBLE); }
	void set_visible(bool v = true){ set_flag(FLAG_VISIBLE, v); }

	bool is_align_left() const { return flag(FLAG_ALIGN_LEFT); }
	void set_align_left(bool v = true){ set_flag(FLAG_ALIGN_LEFT, v); }
	bool is_align_right() const { return flag(FLAG_ALIGN_RIGHT); }
	void set_align_right(bool v = true){ set_flag(FLAG_ALIGN_RIGHT, v); }
	void set_align_center(bool v = true){
		if( v ){
			set_align_right(false);
			set_align_left(false);
		}
	}

	bool is_align_center() const { return flag(FLAG_ALIGN_LEFT|FLAG_ALIGN_RIGHT) == 0; }

	bool is_align_top() const { return flag(FLAG_ALIGN_TOP); }
	void set_align_top(bool v = true){ set_flag(FLAG_ALIGN_TOP, v); }
	bool is_align_bottom() const { return flag(FLAG_ALIGN_BOTTOM); }
	void set_align_bottom(bool v = true){ set_flag(FLAG_ALIGN_BOTTOM, v); }
	void set_align_middle(bool v = true){
		if( v ){
			set_align_top(false);
			set_align_bottom(false);
		}
	}
	bool is_align_middle() const { return flag(FLAG_ALIGN_TOP|FLAG_ALIGN_BOTTOM) == 0; }


	bool dark() const { return flag(FLAG_DARK); }
	void set_dark(bool v = true){ set_flag(FLAG_DARK, v); }

	bool invert() const { return flag(FLAG_INVERT); }
	void set_invert(bool v = true){ set_flag(FLAG_INVERT, v); }

protected:

	/*! \cond */

	sg_point_t point_on_bitmap(sgfx::Bitmap & b, drawing_size_t x, drawing_size_t y, sg_area_t d);
	sg_area_t dim_on_bitmap(sgfx::Bitmap & b) const;

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


	bool flag(u32 flag) const;
	void set_flag(u32 flag, bool v = true);

private:

	static drawing_size_t m_scale;
	static sg_color_t m_default_color;
	u32 m_flags;

	sg_color_t m_color;


	static void draw_rectangle(const DrawingAttributes & attr, const sgfx::Pen & pen);
	/*! \endcond */




};

}

namespace sys {
class Printer;
Printer& operator << (Printer& printer, const draw::DrawingPoint& a);
Printer& operator << (Printer& printer, const draw::DrawingArea& a);
Printer& operator << (Printer& printer, const draw::DrawingRegion& a);
}

#endif /* SAPI_DRAW_DRAWING_HPP_ */
