/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef MBITMAP_HPP_
#define MBITMAP_HPP_

#include <stdint.h>
#include <string.h>
#include <stfy/mg.h>
#include "../Var/Data.hpp"
#include "MPoint.hpp"

namespace Draw {

class MBitmap : public Var::Data {
public:
	MBitmap();
	MBitmap(mg_bitmap_hdr_t * hdr, bool readonly = false); //read/write bitmap
	MBitmap(mg_bitmap_t * mem, mg_size_t w, mg_size_t h, bool readonly = false); //read/write bitmap
	MBitmap(mg_size_t w, mg_size_t h);
	virtual ~MBitmap();

	static int byte_width(int w);
	static int word_width(int w);

	/*! \details Set data pointer and size for bitmap */
	void set_data(mg_bitmap_hdr_t * hdr, bool readonly = false);
	void set_data(mg_bitmap_t * mem, mg_size_t w, mg_size_t h, bool readonly = false);
	inline void set_data(mg_bitmap_t * mem, const MDim & dim){
		set_data(mem, dim.w(), dim.h());
	}
	inline void set_data(mg_bitmap_t * mem, bool readonly = false){
		set_data(mem, w(), h(), readonly);
	}

	static MDim load_dim(const char * path);

	int load(const char * path, mg_point_t p);
	int load(const char * path);
	int save(const char * path) const;


	/*! \details Allocate memory for the bitmap data */
	int alloc(mg_size_t w, mg_size_t h);
	inline int alloc(const MDim & d){ return alloc(d.w(), d.h()); }
	/*! \details Free memory associated with bitmap (auto freed on ~MBitmap) */
	void free(void);


	void flipx(void);
	void flipy(void);
	void flipxy(void);


	mg_point_t center() const;
	void invert();
	void invert(mg_point_t p, mg_dim_t d, mg_bitmap_t v = 0xFF);

	void set(mg_point_t p, mg_dim_t d, mg_bitmap_t v = 0xFF){ mg_set_area(bmap(), p, d, v); }
	void clear(mg_point_t p, mg_dim_t d, mg_bitmap_t v = 0xFF){ mg_clr_area(bmap(), p, d, v); }

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
	int set_bitmap(const MBitmap * bitmap, mg_point_t p){ return mg_set_bitmap(bmap(), bitmap->bmap_const(), p); }

	int set_bitmap_column(const MBitmap * bitmap, mg_point_t p, mg_int_t col, mg_size_t h);
	int set_bitmap_column(const MBitmap * bitmap, mg_point_t p, mg_int_t col);

	int clr_bitmap(const MBitmap * bitmap, mg_point_t p){ return mg_clr_bitmap(bmap(), bitmap->bmap_const(), p); }

	/*! \details Change effective size without free/alloc sequence */
	int setsize(mg_size_t w, mg_size_t h, mg_size_t offset = 0);
	inline int set_size(mg_size_t w, mg_size_t h, mg_size_t offset = 0){
		return setsize(w,h,offset);
	}


	/*! \details Return the size of a bitmap of specified size */
	static size_t size(int w, int h){ return h*byte_width(w); }

	/*! \details size() is the current size of the bitmap.  capacity() will
	 * return total memory allocated for the size of the object
	 * @return The effective memory size of the bitmap
	 */
	size_t size() const { return mg_size(&_bmap); }


	/*! \details Maximum x value */
	inline mg_int_t xmax() const { return w()-1; }
	/*! \details Maximum y value */
	inline mg_int_t ymax() const { return h()-1; }


	void set_vline(mg_int_t x, mg_int_t ymin, mg_int_t ymax, mg_size_t thickness = 1);
	void set_hline(mg_int_t xmin, mg_int_t xmax, mg_int_t y, mg_size_t thickness = 1);

	void clr_vline(mg_int_t x, mg_int_t ymin, mg_int_t ymax, mg_size_t thickness = 1);
	void clr_hline(mg_int_t xmin, mg_int_t xmax, mg_int_t y, mg_size_t thickness = 1);

	/*! \details Set the pixels of a line */
	virtual void set_line(mg_point_t p1, mg_point_t p2, mg_size_t thickness = 1);

	/*! \details clear the pixels in a line */
	virtual void clr_line(mg_point_t p1, mg_point_t p2, mg_size_t thickness = 1);


	inline bool is_empty(void) const { return size() == 0; }
	inline mg_size_t h() const { return _bmap.dim.h; }
	inline mg_size_t w() const { return _bmap.dim.w; }
	inline const MDim dim() const {
		MDim d(_bmap.dim);
		return d;
	}

	inline mg_size_t byte_width() const { return mg_byte_width(&_bmap); }

	inline mg_size_t cols(void) const { return _bmap.columns; }

	/*! \brief Function for filling a bounded area in local LCD memory */
	virtual void pour(mg_point_t p);

	inline mg_size_t margin_left() const { return _bmap.margin_top_left.w; }
	inline mg_size_t margin_right() const { return _bmap.margin_bottom_right.w; }
	inline mg_size_t margin_top() const { return _bmap.margin_top_left.h; }
	inline mg_size_t margin_bottom() const { return _bmap.margin_bottom_right.h; }

	inline void set_margin_left(mg_size_t v) { _bmap.margin_top_left.w = v; }
	inline void set_margin_right(mg_size_t v) { _bmap.margin_bottom_right.w = v; }
	inline void set_margin_top(mg_size_t v) { _bmap.margin_top_left.h = v; }
	inline void set_margin_bottom(mg_size_t v) { _bmap.margin_bottom_right.h = v; }

	virtual void shift_right(int count);
	virtual void shift_right(int count, mg_size_t h);
	virtual void shift_left(int count);
	virtual void shift_left(int count, mg_size_t h);

	bool tst_pixel(mg_point_t p) const { return mg_tst_pixel(bmap_const(), p) != 0; }
	void set_pixel(mg_point_t p){ mg_set_pixel(bmap(), p); }
	void inv_pixel(mg_point_t p){ mg_inv_pixel(bmap(), p); }
	void clr_pixel(mg_point_t p){ mg_clr_pixel(bmap(), p); }

	virtual void fill(mg_bitmap_t v);
	virtual void fill(mg_bitmap_t v, mg_int_t start, mg_size_t h);

	virtual inline void clear(){
		fill(0x00);
	}



	void show(void) const;

	inline mg_bitmap_t * data() const __attribute__((always_inline)) { return (mg_bitmap_t *)Data::data(); }
	inline const mg_bitmap_t * data_const() const { return (const mg_bitmap_t *)Data::data_const(); }

	mg_bitmap_t * data(mg_point_t p) const;
	mg_bitmap_t * data(mg_int_t x, mg_int_t y) const;
	const mg_bitmap_t * data_const(mg_point_t p) const;


	mg_bmap_t * bmap() MCU_ALWAYS_INLINE { return &_bmap; }
	const mg_bmap_t * bmap_const() const  MCU_ALWAYS_INLINE { return &_bmap; }


protected:

	/*! \brief Return a pointer to the bitmap memory */



	/* Transform and bound the x and y values */
	virtual void transform(mg_int_t & x, mg_int_t & y) const {}


	//routines for calculating pixel memory locations and masks
	inline const mg_bitmap_t * target_const(mg_int_t x, mg_int_t y) const {
		return data_const() + (x/8) + y*(_bmap.columns);
	}
	inline mg_bitmap_t * target(mg_int_t x, mg_int_t y) const {
		return data() + (x/8) + y*(_bmap.columns);
	}
	inline static mg_bitmap_t mask(mg_int_t x){
		return ( 0x80 >> (x&0x07) );
	}


private:

	mg_bmap_t _bmap;

	void init_members();
	void calc_members(mg_size_t w, mg_size_t h);
	//const MBFont * font_;
	//mg_size_t columns;
	//MDim _dim;

};
};

#endif /* MBITMAP_HPP_ */
