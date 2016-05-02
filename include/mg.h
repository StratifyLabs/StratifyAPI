/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef MG_H_
#define MG_H_


#include <mcu/types.h>

#include "mg_types.h"

#if defined __cplusplus
extern "C" {
#endif



/*! \brief Change effective size without free/alloc sequence */
void mg_set_data(mg_bmap_t * mg, mg_bitmap_t * mem, mg_size_t w, mg_size_t h);
mg_bitmap_t * mg_data(const mg_bmap_t * mg, mg_point_t p);
int mg_set_size(mg_bmap_t * mg, mg_size_t w, mg_size_t h, mg_size_t offset);
size_t mg_calc_size(mg_int_t w, mg_int_t h);
size_t mg_size(const mg_bmap_t * mg);
size_t mg_byte_width(const mg_bmap_t * mg);
size_t mg_word_width(const mg_bmap_t * mg);
size_t mg_calc_byte_width(mg_size_t w);
size_t mg_calc_word_width(mg_size_t w);
mg_int_t mg_x_max(const mg_bmap_t * mg);
mg_int_t mg_y_max(const mg_bmap_t * mg);
void mg_show(const mg_bmap_t * mg);

void mg_bound(const mg_bmap_t * mg, mg_point_t * p);
void mg_bound_x(const mg_bmap_t * mg, mg_int_t * x);
void mg_bound_y(const mg_bmap_t * mg, mg_int_t * y);

static inline mg_size_t mg_margin_left(const mg_bmap_t * mg){ return mg->margin_top_left.w; }
static inline mg_size_t mg_margin_right(const mg_bmap_t * mg){ return mg->margin_bottom_right.w; }
static inline mg_size_t mg_margin_top(const mg_bmap_t * mg){ return mg->margin_top_left.h; }
static inline mg_size_t mg_margin_bottom(const mg_bmap_t * mg){ return mg->margin_bottom_right.h; }

static inline mg_size_t mg_h(mg_bmap_t * mg){ return mg->dim.h; }
static inline mg_size_t mg_w(mg_bmap_t * mg){ return mg->dim.w; }
static inline mg_size_t mg_cols(mg_bmap_t * mg){ return mg->columns; }

void mg_flip_x(mg_bmap_t * mg);
void mg_flip_y(mg_bmap_t * mg);
void mg_flip_xy(mg_bmap_t * mg);

void mg_shift_right(mg_bmap_t * mg, int count, mg_point_t start, mg_dim_t d);
void mg_shift_left(mg_bmap_t * mg, int count, mg_point_t start, mg_dim_t d);
void mg_shift_up(mg_bmap_t * mg, int count, mg_point_t start, mg_dim_t d);
void mg_shift_down(mg_bmap_t * mg, int count, mg_point_t start, mg_dim_t d);

int mg_animate(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation);
int mg_animate_init(mg_animation_t * animation,
		u8 type,
		u8 path,
		u8 step_total,
		mg_size_t motion_total,
		mg_point_t start,
		mg_dim_t dim);


/*! \brief Rotation values */
enum Rotation {
	MG_ROT0 /*! Zero degress */,
	MG_ROT90 /*! 90 degress */,
	MG_ROT180 /*! 180 degress */,
	MG_ROT270 /*! 270 degress */,
	MG_ROT360 /*! 360 degress */,
	MG_ROT_SCALE = 65536
};


mg_bitmap_t mg_op_set(mg_bitmap_t current, mg_bitmap_t input);
mg_bitmap_t mg_op_clr(mg_bitmap_t current, mg_bitmap_t input);
mg_bitmap_t mg_op_inv(mg_bitmap_t current, mg_bitmap_t input);
mg_bitmap_t mg_op_assign(mg_bitmap_t current, mg_bitmap_t input);

/*! \brief Set the pixels of a bitmap.
 * \details This method sets a bitmap on to the
 * current bitmap.  It only has bit level positioning but is
 * slower than the copy() method
 *
 * @param bitmap The bitmap to set
 * @param x The x location to start copying
 * @param y The y location to start copying
 * @param pos true to set pixels and false to clear them
 * @return Zero on success
 */
int mg_set_bitmap(mg_bmap_t * mg, const mg_bmap_t * bitmap, mg_point_t p);
int mg_set_bitmap_area(mg_bmap_t * mg, mg_point_t dest, const mg_bmap_t * bitmap, mg_point_t p_src, mg_dim_t d);
int mg_clr_bitmap(mg_bmap_t * mg, const mg_bmap_t * bitmap, mg_point_t p);
int mg_clr_bitmap_area(mg_bmap_t * mg, mg_point_t dest, const mg_bmap_t * bitmap, mg_point_t p_src, mg_dim_t d);
int mg_inv_bitmap(mg_bmap_t * mg, const mg_bmap_t * bitmap, mg_point_t p);
int mg_inv_bitmap_area(mg_bmap_t * mg, mg_point_t dest, const mg_bmap_t * bitmap, mg_point_t p_src, mg_dim_t d);
int mg_assign_bitmap(mg_bmap_t * mg, const mg_bmap_t * bitmap, mg_point_t p);
int mg_assign_bitmap_area(mg_bmap_t * mg, mg_point_t dest, const mg_bmap_t * bitmap, mg_point_t p_src, mg_dim_t d);


u8 mg_tst_hline(const mg_bmap_t * mg, mg_int_t xmin, mg_int_t xmax, mg_int_t y, mg_int_t * pos);
void mg_tst_hedge(const mg_bmap_t * mg, mg_point_t p, mg_int_t * xmin, mg_int_t * xmax);

void mg_set_vline(mg_bmap_t * mg, mg_int_t x, mg_int_t ymin, mg_int_t ymax, mg_size_t thickness);
void mg_set_hline(mg_bmap_t * mg, mg_int_t xmin, mg_int_t xmax, mg_int_t y, mg_size_t thickness);
void mg_clr_vline(mg_bmap_t * mg, mg_int_t x, mg_int_t ymin, mg_int_t ymax, mg_size_t thickness);
void mg_clr_hline(mg_bmap_t * mg, mg_int_t xmin, mg_int_t xmax, mg_int_t y, mg_size_t thickness);
void mg_inv_vline(mg_bmap_t * mg, mg_int_t x, mg_int_t ymin, mg_int_t ymax, mg_size_t thickness);
void mg_inv_hline(mg_bmap_t * mg, mg_int_t xmin, mg_int_t xmax, mg_int_t y, mg_size_t thickness);
void mg_set_line(mg_bmap_t * mg, mg_point_t p1, mg_point_t p2, mg_size_t thickness);
void mg_clr_line(mg_bmap_t * mg, mg_point_t p1, mg_point_t p2, mg_size_t thickness);
void mg_inv_line(mg_bmap_t * mg, mg_point_t p1, mg_point_t p2, mg_size_t thickness);

u8 mg_tst_pixel(const mg_bmap_t * mg, mg_point_t p);
u8 mg_tst_bounded_pixel(const mg_bmap_t * mg, mg_point_t p);

void mg_set_pixel(mg_bmap_t * mg, mg_point_t p);
void mg_set_bounded_pixel(mg_bmap_t * mg, mg_point_t p);
void mg_inv_pixel(mg_bmap_t * mg, mg_point_t p);
void mg_inv_bounded_pixel(mg_bmap_t * mg, mg_point_t p);
void mg_clr_pixel(mg_bmap_t * mg, mg_point_t p);
void mg_clr_bounded_pixel(mg_bmap_t * mg, mg_point_t p);

void mg_fill(mg_bmap_t * mg, mg_bitmap_t v, mg_int_t start, mg_size_t h);
void mg_set_area(mg_bmap_t * mg, mg_point_t p, mg_dim_t d, mg_bitmap_t v);
void mg_clr_area(mg_bmap_t * mg, mg_point_t p, mg_dim_t d, mg_bitmap_t v);
void mg_inv_area(mg_bmap_t * mg, mg_point_t p, mg_dim_t d, mg_bitmap_t v);

void mg_pour(mg_bmap_t * mg, mg_point_t p);

//drawing
void mg_draw(mg_bmap_t * bitmap, const mg_t * prim, const mg_map_t * map, mg_attr_t * attr);
void mg_draw_list(mg_bmap_t * bitmap, const mg_t prim_list[], unsigned int total, const mg_map_t * map, mg_attr_t * attr);
void mg_draw_icon(mg_bmap_t * bitmap, const mg_icon_t * icon, const mg_map_t * map, mg_attr_t * attr);

static inline mg_dim_t mg_draw_attr_dim(const mg_attr_t * attr){
	mg_dim_t d;
	d.w = attr->bottom_right.x - attr->top_left.x;
	d.h = attr->bottom_right.y - attr->top_left.y;
	return d;
}

static inline mg_point_t mg_draw_attr_center(const mg_attr_t * attr){
	mg_point_t p;
	p.x = (attr->bottom_right.x + attr->top_left.x)/2;
	p.y = (attr->bottom_right.y + attr->top_left.y)/2;
	return p;
}

static inline u8 mg_y_visible(const mg_bmap_t * mg, mg_int_t y){
	if( y < 0  ) return 0;
	if( y >= mg->dim.h) return 0;
	return 1;
}

static inline u8 mg_x_visible(const mg_bmap_t * mg, mg_int_t x){
	if( x < 0  ) return 0;
	if( x >= mg->dim.w) return 0;
	return 1;
}

static inline u8 mg_point_visible(const mg_bmap_t * mg, mg_point_t p){
	if( p.x < 0 ) return 0;
	if( p.y < 0 ) return 0;
	if( p.x >= mg->dim.w ) return 0;
	if( p.y >= mg->dim.h ) return 0;
	return 1;
}


static inline int mg_offset(mg_point_t p, mg_size_t columns) {
	return (p.x/8) + p.y*(columns);
}

static inline  mg_bitmap_t mg_mask(mg_int_t x){
	return ( 0x80 >> (x&0x07) );
}


//point operations
static inline mg_point_t mg_point(mg_int_t x, mg_int_t y){
	mg_point_t p;
	p.x = x;
	p.y = y;
	return p;
}

static inline mg_dim_t mg_dim(mg_size_t w, mg_size_t h){
	mg_dim_t d;
	d.w = w;
	d.h = h;
	return d;
}
void mg_point_set(mg_point_t * d, mg_point_t p);
void mg_point_map(mg_point_t * d, const mg_map_t * m);
mg_size_t mg_point_map_pixel_size(const mg_map_t * m);
void mg_point_add(mg_point_t * d, const mg_point_t * a);
void mg_point_subtract(mg_point_t * d, const mg_point_t * a);
void mg_point_arc(mg_point_t * d, mg_size_t rx, mg_size_t ry, i16 angle);
void mg_point_rotate(mg_point_t * d, i16 angle);
void mg_point_scale(mg_point_t * d, u16 a);
void mg_point_shift(mg_point_t * d, mg_point_t p);
void mg_point_shift_x(mg_point_t * d, mg_int_t a);
void mg_point_shift_y(mg_point_t * d, mg_int_t a);
static inline mg_point_t mg_point_origin(){ mg_point_t p; p.point = 0; return p; }


#if defined __cplusplus
}
#endif

#endif /* MG_H_ */
