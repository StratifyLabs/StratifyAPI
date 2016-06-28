/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SGFX_BITMAP_HPP_
#define SGFX_BITMAP_HPP_

#include <sgfx/sg.h>

#include "../var/Data.hpp"
#include "Region.hpp"

namespace sgfx {

/*! \brief Bitmap Class
 * \details This class implements a bitmap and is
 * powered by the sgfx library.
 */
class Bitmap : public var::Data {
public:
	/*! \details Construct an empty bitmap */
	Bitmap();
	/*! \details Construct a bitmap using an existing sg_bitmap_hdr_t
	 *
	 * @param hdr A pointer to the existing bitmap structure
	 * @param readonly True if the data is stored in read-only memory
	 */
	Bitmap(sg_bitmap_hdr_t * hdr, bool readonly = false); //read/write bitmap

	/*! \details Construct a bitmap using an existing memory buffer
	 *
	 * @param mem A pointer to the memory buffer
	 * @param w The width of the bitmap that fits in the buffer
	 * @param h The height of the bitmap buffer
	 * @param readonly True if \a mem is in read-only memory
	 */
	Bitmap(sg_bmap_data_t * mem, sg_size_t w, sg_size_t h, bool readonly = false); //read/write bitmap

	/*! \details Construct a new bitmap (dynamic memory allocation)
	 *
	 * @param w Width of the new bitmap
	 * @param h Height of the new bitmap
	 */
	Bitmap(sg_size_t w, sg_size_t h);
	/*! \details Construct a new bitmap (dynamic memory allocation)
	 *
	 * @param d Dimensions of the bitmap
	 */
	Bitmap(sg_dim_t d);


	virtual ~Bitmap();

	static int calc_byte_width(int w);
	static int calc_word_width(int w);

	/*! \details Set data pointer and size for bitmap */
	void set_data(sg_bitmap_hdr_t * hdr, bool readonly = false);
	void set_data(sg_bmap_data_t * mem, sg_size_t w, sg_size_t h, bool readonly = false);
	inline void set_data(sg_bmap_data_t * mem, const Dim & dim){
		set_data(mem, dim.w(), dim.h());
	}
	inline void set_data(sg_bmap_data_t * mem, bool readonly = false){
		set_data(mem, w(), h(), readonly);
	}

	static Dim load_dim(const char * path);

	int load(const char * path, sg_point_t p);
	int load(const char * path);
	int save(const char * path) const;


	/*! \details Allocate memory for the bitmap data using the specified
	 * width and height.  If the bitmap already has a memory associated
	 * with it, it will be freed before the new memory is assigned.
	 */
	int alloc(sg_size_t w, sg_size_t h);
	inline int alloc(const Dim & d){ return alloc(d.w(), d.h()); }
	/*! \details Free memory associated with bitmap (auto freed on ~Bitmap) */
	void free();


	void flip_x();
	void flip_y();
	void flip_xy();


	sg_point_t calc_center() const;
	void invert();
	void invert(sg_point_t p, sg_dim_t d, sg_bmap_data_t v = 0xFF);

	void set(sg_point_t p, sg_dim_t d, sg_bmap_data_t v = 0xFF){ sg_set_area(bmap(), p, d, v); }
	void clear(sg_point_t p, sg_dim_t d, sg_bmap_data_t v = 0xFF){ sg_clr_area(bmap(), p, d, v); }

	/*! \details Rotation values */
	enum Rotation {
		ROT0 /*! \brief Zero degress */,
		ROT90 /*! \brief 90 degress */,
		ROT180 /*! \brief 180 degress */,
		ROT270 /*! \brief 270 degress */,
		ROT360 /*! \brief 360 degress */,
		ROT_SCALE = 65536
	};



	/*! \details This method sets a bitmap on to the
	 * current bitmap.  It only has bit level positioning but is
	 * slower than the copy() method
	 *
	 * @param bitmap The bitmap to set
	 * @param x The x location to start copying
	 * @param y The y location to start copying
	 * @param pos true to set pixels and false to clear them
	 * @return Zero on success
	 */
	int set_bitmap(const Bitmap & bitmap, sg_point_t p){ return sg_set_bitmap(bmap(), bitmap.bmap_const(), p); }

	int set_bitmap_column(const Bitmap & bitmap, sg_point_t p, sg_int_t col, sg_size_t h);
	int set_bitmap_column(const Bitmap & bitmap, sg_point_t p, sg_int_t col);

	int clr_bitmap(const Bitmap & bitmap, sg_point_t p){ return sg_clr_bitmap(bmap(), bitmap.bmap_const(), p); }
	int clear_bitmap(const Bitmap & bitmap, sg_point_t p){ return sg_clr_bitmap(bmap(), bitmap.bmap_const(), p); }

	/*! \details Change effective size without free/alloc sequence */
	int set_size(sg_size_t w, sg_size_t h, sg_size_t offset = 0);


	/*! \details Return the size of a bitmap of specified size */
	static size_t calc_size(int w, int h){ return h*calc_byte_width(w); }

	/*! \details size() is the current size of the bitmap.  capacity() will
	 * return total memory allocated for the size of the object
	 * @return The effective memory size of the bitmap
	 */
	size_t calc_size() const { return sg_size(&m_bmap); }


	/*! \details Maximum x value */
	inline sg_int_t x_max() const { return w()-1; }
	/*! \details Maximum y value */
	inline sg_int_t y_max() const { return h()-1; }


	void set_vline(sg_int_t x, sg_int_t ymin, sg_int_t ymax, sg_size_t thickness = 1);
	void set_hline(sg_int_t xmin, sg_int_t xmax, sg_int_t y, sg_size_t thickness = 1);

	void clr_vline(sg_int_t x, sg_int_t ymin, sg_int_t ymax, sg_size_t thickness = 1);
	void clr_hline(sg_int_t xmin, sg_int_t xmax, sg_int_t y, sg_size_t thickness = 1);

	/*! \details Set the pixels of a line */
	virtual void set_line(sg_point_t p1, sg_point_t p2, sg_size_t thickness = 1);

	/*! \details clear the pixels in a line */
	virtual void clr_line(sg_point_t p1, sg_point_t p2, sg_size_t thickness = 1);

	/*! \details This method is designated as an interface
	 * for classes that inherit Bitmap to copy the bitmap to a physical
	 * device (such as an LCD).  The implementation in this class is simple
	 * an empty method.  Here is an example:
	 *
	 * \code
	 * LcdDev lcd; //this class inherits bitmap and re-implments refresh()
	 *
	 * lcd.clear(); //clear the bitmap in memory
	 * lcd.refresh(); //copy the memory to the physical device (or notify the underlying driver that it is time to copy)
	 * \endcode
	 *
	 */
	virtual void refresh() const {}

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


	inline bool is_empty() const { return calc_size() == 0; }
	inline sg_size_t h() const { return m_bmap.dim.h; }
	inline sg_size_t w() const { return m_bmap.dim.w; }
	inline const Dim dim() const {
		Dim d(m_bmap.dim);
		return d;
	}

	inline sg_size_t calc_byte_width() const { return sg_byte_width(&m_bmap); }

	inline sg_size_t columns() const { return m_bmap.columns; }

	/*! \brief Function for filling a bounded area in local LCD memory */
	virtual void pour(sg_point_t p);

	inline sg_size_t margin_left() const { return m_bmap.margin_top_left.w; }
	inline sg_size_t margin_right() const { return m_bmap.margin_bottom_right.w; }
	inline sg_size_t margin_top() const { return m_bmap.margin_top_left.h; }
	inline sg_size_t margin_bottom() const { return m_bmap.margin_bottom_right.h; }

	inline void set_margin_left(sg_size_t v) { m_bmap.margin_top_left.w = v; }
	inline void set_margin_right(sg_size_t v) { m_bmap.margin_bottom_right.w = v; }
	inline void set_margin_top(sg_size_t v) { m_bmap.margin_top_left.h = v; }
	inline void set_margin_bottom(sg_size_t v) { m_bmap.margin_bottom_right.h = v; }

	virtual void shift_right(int count);
	virtual void shift_right(int count, sg_size_t h);
	virtual void shift_left(int count);
	virtual void shift_left(int count, sg_size_t h);

	bool tst_pixel(sg_point_t p) const { return sg_tst_pixel(bmap_const(), p) != 0; }
	void set_pixel(sg_point_t p){ sg_set_pixel(bmap(), p); }
	void inv_pixel(sg_point_t p){ sg_inv_pixel(bmap(), p); }
	void clr_pixel(sg_point_t p){ sg_clr_pixel(bmap(), p); }

	virtual void fill(sg_bmap_data_t v);
	virtual void fill(sg_bmap_data_t v, sg_int_t start, sg_size_t h);

	virtual inline void clear(){
		fill(0x00);
	}



	void show() const;

	inline sg_bmap_data_t * data() const __attribute__((always_inline)) { return (sg_bmap_data_t *)Data::data(); }
	inline const sg_bmap_data_t * data_const() const { return (const sg_bmap_data_t *)Data::data_const(); }

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
