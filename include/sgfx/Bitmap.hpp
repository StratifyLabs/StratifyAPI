/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SGFX_BITMAP_HPP_
#define SGFX_BITMAP_HPP_

#include <sapi/sg.h>

#include "../var/Data.hpp"
#include "Region.hpp"
#include "Pen.hpp"

namespace sgfx {

/*! \brief Bitmap Class
 * \details This class implements a bitmap and is
 * powered by the sgfx library.
 */
class Bitmap : public var::Data {
public:
	/*! \details Constructs an empty bitmap. */
	Bitmap();

	/*! \details Constructs a bitmap using an existing sg_bmap_header_t.
	 *
	 * @param hdr A pointer to the existing bitmap structure
	 * @param readonly True if the data is stored in read-only memory
	 */
	Bitmap(sg_bmap_header_t * hdr, bool readonly = false); //read/write bitmap

	/*! \details Constructs a bitmap using an existing memory buffer.
	 *
	 * @param mem A pointer to the memory buffer
	 * @param w The width of the bitmap that fits in the buffer
	 * @param h The height of the bitmap buffer
	 * @param readonly True if \a mem is in read-only memory
	 */
	Bitmap(sg_bmap_data_t * mem, sg_size_t w, sg_size_t h, bool readonly = false); //read/write bitmap

	/*! \details Constructs a new bitmap (dynamic memory allocation).
	 *
	 * @param w Width of the new bitmap
	 * @param h Height of the new bitmap
	 */
	Bitmap(sg_size_t w, sg_size_t h);

	/*! \details Constructs a new bitmap (dynamic memory allocation).
	 *
	 * @param d Dimensions of the bitmap
	 */
	Bitmap(sg_dim_t d);

	/*! \details Sets the color of the pen.
	 *
	 * @param color The color of the pen
	 *
	 * Valid colors value depend on the format of the underlying Stratify graphics library.
	 * The Stratify graphics library supports the following formats.
	 *  - 1bpp (bit per pixel)
	 *  - 2bpp
	 *  - 4bpp
	 *  - 8bpp
	 *
	 * For example, for 4bpp, there are 16 colors so \a color can be from 0 to 15 (inclusive).
	 * The actual color that displays on the screen depends on the DisplayPalette associated
	 * with the Display.
	 *
	 * \sa DisplayPalette, Display
	 *
	 */
	void set_pen_color(sg_color_t color){ m_bmap.pen.color = color; }

	/*! \details Sets the pen to \a pen. */
	void set_pen(const Pen & pen){ m_bmap.pen = pen; }

	/*! \details Returns a copy of the bitmap's pen. */
	Pen pen() const { return m_bmap.pen; }

	/*! \details Returns the pen color. */
	sg_color_t pen_color() const { return m_bmap.pen.color; }
	/*! \details Retursn the pen thickness. */
	sg_color_t pen_thickness() const { return m_bmap.pen.thickness; }
	u16 pen_flags() const { return m_bmap.pen.o_flags; }

	/*! \details Sets the thickness of the pen.
	 *
	 * @param thickness The thickness in pixels
	 */
	void set_pen_thickness(sg_size_t thickness){ m_bmap.pen.thickness = thickness; }

	virtual ~Bitmap();

	/*! \details Sets data pointer and size for bitmap */
	void set_data(sg_bmap_header_t * hdr, bool readonly = false);
	void set_data(sg_bmap_data_t * mem, sg_size_t w, sg_size_t h, bool readonly = false);
	void set_data(sg_bmap_data_t * mem, const Dim & dim){ set_data(mem, dim.width(), dim.height()); }
	void set_data(sg_bmap_data_t * mem, bool readonly = false){ set_data(mem, width(), height(), readonly); }

	/*! \details Changes effective size without free/alloc sequence */
	bool set_size(sg_size_t w, sg_size_t h, sg_size_t offset = 0);

	/*! \details Returns the number of bytes used to store a Bitmap of specified size
	 *
	 * @param w Width used for calculation
	 * @param h Height used for calculation
	 */
	static u32 calc_size(int w, int h){ return sg_api()->calc_bmap_size(sg_dim(w,h)); }

	static u16 calc_word_width(sg_size_t w){
		return sg_calc_word_width(w);
	}

	/*! \details Returns the number of bytes used to store the Bitmap. */
	u32 calc_size(){ return calc_size(width(), height()); }
	sg_point_t calc_center() const;

	/*! \details Returns the maximum x value. */
	sg_int_t x_max() const { return width()-1; }
	/*! \details Returns the maximum y value. */
	sg_int_t y_max() const { return height()-1; }

	static Dim load_dim(const char * path);

	/*! \details Loads a bitmap from a file.
	 *
	 * @param path The path to the bitmap file name
	 * @return Zero on success
	 */
	int load(const char * path);

	/*! \details Saves a bitmap to a file.
	 *
	 * @param path The path for the new file
	 * @return Zero on success
	 *
	 * If the file already exists, it will be overwritten.
	 *
	 */
	int save(const char * path) const;


	/*! \details Allocates memory for the bitmap data using the specified
	 * width and height.  If the bitmap already has a memory associated
	 * with it, it will be freed before the new memory is assigned.
	 */
	int alloc(sg_size_t w, sg_size_t h);
	int alloc(const Dim & d){ return alloc(d.width(), d.height()); }
	/*! \details Free memory associated with bitmap (auto freed on ~Bitmap) */
	void free();

	void clear(){ clear_rectangle(sg_point(0,0), dim()); }
	void invert(){ invert_rectangle(sg_point(0,0), dim()); }


	void transform_flip_x() const { sg_api()->transform_flip_x(bmap_const()); }
	void transform_flip_y() const { sg_api()->transform_flip_y(bmap_const()); }
	void transform_flip_xy() const { sg_api()->transform_flip_xy(bmap_const()); }
	void transform_shift(sg_point_t shift, sg_point_t p, sg_dim_t d) const { sg_api()->transform_shift(bmap_const(), shift, p, d); }


	/*! \details Gets the value of the pixel at the specified point.
	 *
	 * @param p The point to get the pixel color
	 * @return The color of the pixel at \a p
	 */
	sg_color_t get_pixel(sg_point_t p) const { return sg_api()->get_pixel(bmap_const(), p); }

	/*! \details Draws a pixel at the specified point.
	 *
	 * @param p The point where to draw the pixel
	 *
	 * The color of the pixel is determined by the pen settings.
	 *
	 * \sa set_pen_color()
	 */
	void draw_pixel(sg_point_t p) const { sg_api()->draw_pixel(bmap_const(), p); }

	/*! \details Draws a line on the bitmap.
	 *
	 * @param p1 Starting point
	 * @param p2 Ending point
	 *
	 * The bitmap's pen will determine the color, thickness, and drawing mode.
	 *
	 */
	void draw_line(sg_point_t p1, sg_point_t p2) const { sg_api()->draw_line(bmap_const(), p1, p2); }
	void draw_quadtratic_bezier(sg_point_t p1, sg_point_t p2, sg_point_t p3) const { sg_api()->draw_quadtratic_bezier(bmap_const(), p1, p2, p3); }
	void draw_cubic_bezier(sg_point_t p1, sg_point_t p2, sg_point_t p3, sg_point_t p4) const { sg_api()->draw_cubic_bezier(bmap_const(), p1, p2, p3, p4); }

	/*! \details Draws a rectangle on the bitmap.
	 *
	 * @param p The top left corner of the rectangle
	 * @param d The dimensions of the rectangle
	 *
	 * The bitmap's pen color and drawing mode will affect how the rectangle is drawn.
	 */
	void draw_rectangle(sg_point_t p, sg_dim_t d) const { sg_api()->draw_rectangle(bmap_const(), p, d); }

	//invert and clear rectangle should not be needed (can use INVERT and ERASE pen modes)
	void invert_rectangle(sg_point_t p, sg_dim_t d) const { sg_api()->invert_rectangle(bmap_const(), p, d); }
	void clear_rectangle(sg_point_t p, sg_dim_t d) const { sg_api()->clear_rectangle(bmap_const(), p, d); }

	void draw_pour(sg_point_t p) const { sg_api()->draw_pour(bmap_const(), p); }

	/*! \details This function sets the pixels in a bitmap
	 * based on the pixels of the source bitmap
	 *
	 * @param p_dest The point in the destination bitmap of the top left corner of \a bitmap
	 * @param src The source bitmap
	 * @return Zero on success
	 */
	void draw_bitmap(sg_point_t p_dest, const Bitmap & src) const {
		sg_api()->draw_bitmap(bmap_const(), p_dest, src.bmap_const());
	}


	/*! \details This function draws a pattern on the bitmap.
	 *
	 * @param p The top-left point to start the pattern
	 * @param d The dimensions to draw the pattern on
	 * @param odd_pattern The odd pattern as a 1bpp bitmask (e.g. 0xAA)
	 * @param even_pattern The even pattern as a 1bpp bitmask (e.g. 0x55)
	 * @param pattern_height The pixel height of alternating pixels
	 */
	void draw_pattern(sg_point_t p, sg_dim_t d, sg_bmap_data_t odd_pattern, sg_bmap_data_t even_pattern, sg_size_t pattern_height) const {
		sg_api()->draw_pattern(bmap_const(), p, d, odd_pattern, even_pattern, pattern_height);
	}

	/*! \details This function draws a subset of
	 * the source bitmap on the destination bitmap.
	 *
	 * @param p_dest The point in the destination bitmap to start setting pixels
	 * @param src The source bitmap
	 * @param p_src The top left corner of the source bitmap to copy
	 * @param d_src The dimensions of the area to copy
	 * @return Zero on success
	 */
	void draw_sub_bitmap(sg_point_t p_dest, const Bitmap & src, sg_point_t p_src, sg_dim_t d_src) const {
		sg_api()->draw_sub_bitmap(bmap_const(), p_dest, src.bmap_const(), p_src, d_src);
	}


	/*! \details This method is designated as an interface
	 * for classes that inherit Bitmap to copy the bitmap to a physical
	 * device (such as hal::DisplayDev).  The implementation in this class is simple
	 * an empty method.  Here is an example:
	 *
	 * \code
	 * DisplayDev display; //this class inherits bitmap and re-implments refresh()
	 *
	 * display.clear(); //clear the bitmap in memory
	 * display.refresh(); //copy the memory to the physical device (or notify the underlying driver that it is time to copy)
	 * \endcode
	 *
	 */
	virtual void refresh() const{}

	/*! \details This method is designated as an interface for classes
	 * that inherit Bitmap and use refresh() to copy the bitmap memory to a physical
	 * device. The default implementation always returns false.
	 *
	 * The bitmap should not be modified while a refresh is in progress to prevent a frame from
	 * being partially copied.
	 *
	 * See refresh() for an example.
	 *
	 * @return True if the refresh() is still in progress, false if the bitmap can be modified again
	 */
	virtual bool busy() const { return false; }

	/*! \details This method will block until the refresh operation is complete */
	virtual void wait(u16 resolution) const {}


	sg_size_t height() const { return m_bmap.dim.height; }
	sg_size_t width() const { return m_bmap.dim.width; }
	const Dim dim() const {
		Dim d(m_bmap.dim);
		return d;
	}

	inline sg_size_t columns() const { return m_bmap.columns; }

	inline sg_size_t margin_left() const { return m_bmap.margin_top_left.width; }
	inline sg_size_t margin_right() const { return m_bmap.margin_bottom_right.width; }
	inline sg_size_t margin_top() const { return m_bmap.margin_top_left.height; }
	inline sg_size_t margin_bottom() const { return m_bmap.margin_bottom_right.height; }

	inline void set_margin_left(sg_size_t v) { m_bmap.margin_top_left.width = v; }
	inline void set_margin_right(sg_size_t v) { m_bmap.margin_bottom_right.width = v; }
	inline void set_margin_top(sg_size_t v) { m_bmap.margin_top_left.height = v; }
	inline void set_margin_bottom(sg_size_t v) { m_bmap.margin_bottom_right.height = v; }


	void show() const;

	sg_bmap_data_t * data() const{ return (sg_bmap_data_t *)Data::data(); }
	const sg_bmap_data_t * data_const() const { return (const sg_bmap_data_t *)Data::data_const(); }

	sg_bmap_data_t * data(sg_point_t p) const;
	sg_bmap_data_t * data(sg_int_t x, sg_int_t y) const;
	const sg_bmap_data_t * data_const(sg_point_t p) const;


	sg_bmap_t * bmap() MCU_ALWAYS_INLINE { return &m_bmap; }
	const sg_bmap_t * bmap_const() const  MCU_ALWAYS_INLINE { return &m_bmap; }


protected:

	/*! \brief Return a pointer to the bitmap memory */



	/* Transform and bound the x and y values */
	virtual void transform(sg_int_t & x, sg_int_t & y) const {}


	//routines for calculating pixel memory locations and masks
	inline const sg_bmap_data_t * target_const(sg_int_t x, sg_int_t y) const {
		return data_const() + (x/8) + y*(m_bmap.columns);
	}
	inline sg_bmap_data_t * target(sg_int_t x, sg_int_t y) const {
		return data() + (x/8) + y*(m_bmap.columns);
	}
	inline static sg_bmap_data_t mask(sg_int_t x){
		return ( 0x80 >> (x&0x07) );
	}


private:

	sg_bmap_t m_bmap;
	void init_members();
	void calc_members(sg_size_t w, sg_size_t h);

};

};

#endif /* SGFX_BITMAP_HPP_ */
