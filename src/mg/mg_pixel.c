/*
 * mb.c
 *
 *  Created on: Oct 27, 2015
 *      Author: tgil
 */


#include "mg.h"

static int mg_op_bitmap(mg_bmap_t * mg, mg_point_t p_dest, const mg_bmap_t * bitmap, mg_point_t p_src, mg_dim_t d, mg_bitmap_t (*op)(mg_bitmap_t current, mg_bitmap_t input));
static int mg_op_area(mg_bmap_t * mg, mg_point_t p, mg_dim_t d, mg_bitmap_t v, mg_bitmap_t (*op)(mg_bitmap_t current, mg_bitmap_t input));


static mg_bitmap_t calc_row_mask(mg_bitmap_t v, mg_size_t row){
	switch(v){
	case 0xAA: return (v >> (row & 0x01));
	case 0x55: return (0xAA >> (1 - (row & 0x01)));
	case 0xCC:
		if( (row & 0x03) > 1 ){
			return 0x33;
		}
		break;
	case 0x33:
		if( (row & 0x03) > 1 ){
			return 0xCC;
		}
		break;
	case 0x0F:
		if( (row & 0x0F) > 7 ){
			return 0xF0;
		}
		break;
	case 0xF0:
		if( (row & 0x0F) > 7 ){
			return 0x0F;
		}
		break;
	}
	return v;
}

static u8 calc_mask(mg_size_t x){
	u8 mask;
	u16 x_mod;
	//divide by 8
	x_mod = x & 0x07;
	x_mod = (1<<(8 - x_mod))-1; //can be a 9-bit calculation
	mask = x_mod;
	return mask;
}

static mg_size_t calc_col_span(mg_int_t x, mg_size_t w){
	mg_size_t col_span;
	u8 left_bits = x & 0x07;
	col_span = ((w + left_bits - 1) >> 3) + 1;
	return col_span;
}

void mg_fill(mg_bmap_t * mg, mg_bitmap_t v, mg_int_t start, mg_size_t h){
	mg_bitmap_t * p = mg_data(mg, mg_point(0, start));
	size_t size = h * mg_cols(mg);
	memset(p, v, size);
}


void mg_pour(mg_bmap_t * mg, mg_point_t p){
	mg_int_t xmin, xmax;
	mg_point_t above;
	mg_point_t below;
	u8 is_above, is_below;

	if( !mg_x_visible(mg, p.x) ){
		return;
	}

	mg_tst_hedge(mg, p, &xmin, &xmax); //find the bounding points xmin and xmax
	is_above = !mg_tst_hline(mg, xmin, xmax, p.y+1, &(above.x)); //see if anywhere above the bounded region is blank
	is_below = !mg_tst_hline(mg, xmin, xmax, p.y-1, &(below.x)); //see if anywhere below the bounded region is blank
	mg_set_hline(mg, xmin, xmax, p.y, 1);
	if( is_above ){
		above.y = p.y+1;
		mg_pour(mg, above); //if the above line has a blank spot -- fill it
	}
	if( is_below ){
		below.y = p.y-1;
		mg_pour(mg, below); //if the below line has a blank spot -- fill it
	}
}

mg_bitmap_t mg_op_set(mg_bitmap_t current, mg_bitmap_t input){
	return current | input;
}

mg_bitmap_t mg_op_clr(mg_bitmap_t current, mg_bitmap_t input){
	return current & ~input;
}

mg_bitmap_t mg_op_inv(mg_bitmap_t current, mg_bitmap_t input){
	return current ^ input;
}

mg_bitmap_t mg_op_assign(mg_bitmap_t current, mg_bitmap_t input){
	return input;
}


int mg_set_bitmap(mg_bmap_t * mg, const mg_bmap_t * bitmap, mg_point_t p){
	return mg_op_bitmap(mg, p, bitmap, mg_point(0,0), bitmap->dim, mg_op_set);
}

int mg_set_bitmap_area(mg_bmap_t * mg, mg_point_t dest, const mg_bmap_t * bitmap, mg_point_t p_src,  mg_dim_t d){
	return mg_op_bitmap(mg, dest, bitmap, p_src, d, mg_op_set);
}

int mg_clr_bitmap(mg_bmap_t * mg, const mg_bmap_t * bitmap, mg_point_t p){
	return mg_op_bitmap(mg, p, bitmap, mg_point(0,0), bitmap->dim, mg_op_clr);
}

int mg_clr_bitmap_area(mg_bmap_t * mg, mg_point_t dest, const mg_bmap_t * bitmap, mg_point_t p_src,  mg_dim_t d){
	return mg_op_bitmap(mg, dest, bitmap, p_src, d, mg_op_clr);
}

int mg_inv_bitmap(mg_bmap_t * mg, const mg_bmap_t * bitmap, mg_point_t p){
	return mg_op_bitmap(mg, p, bitmap, mg_point(0,0), bitmap->dim, mg_op_inv);
}

int mg_inv_bitmap_area(mg_bmap_t * mg, mg_point_t dest, const mg_bmap_t * bitmap, mg_point_t p_src,  mg_dim_t d){
	return mg_op_bitmap(mg, dest, bitmap, p_src, d, mg_op_inv);
}

int mg_assign_bitmap(mg_bmap_t * mg, const mg_bmap_t * bitmap, mg_point_t p){
	return mg_op_bitmap(mg, p, bitmap, mg_point(0,0), bitmap->dim, mg_op_assign);

}

int mg_assign_bitmap_area(mg_bmap_t * mg, mg_point_t dest, const mg_bmap_t * bitmap, mg_point_t p_src, mg_dim_t d){
	mg_clr_area(mg, dest, d, 0xFF);
	return mg_set_bitmap_area(mg, dest, bitmap, p_src, d);
}

u8 mg_tst_hline(const mg_bmap_t * mg, mg_int_t xmin, mg_int_t xmax, mg_int_t y, mg_int_t * pos){
	mg_point_t p;
	if( !mg_y_visible(mg, y) ){
		return 1;
	}
	mg_bound_x(mg, &xmin);
	mg_bound_x(mg, &xmax);
	p.y = y;
	for(p.x = xmin; p.x < xmax; p.x++){
		if( mg_tst_bounded_pixel(mg,p) == 0 ){
			*pos = p.x;
			return false;
		}
	}
	return 1;
}

void mg_tst_hedge(const mg_bmap_t * mg, mg_point_t p, mg_int_t * xmin, mg_int_t * xmax){
	mg_point_t min;
	mg_point_t max;
	mg_bound(mg, &p);
	min.point = p.point;
	max.point = p.point;
	while( mg_tst_bounded_pixel(mg, min) == false ){ min.x--; }
	while( mg_tst_bounded_pixel(mg, max) == false ){ max.x++; }
	*xmin = min.x+1;
	*xmax = max.x;
	return;
}

int mg_op_area(mg_bmap_t * mg, mg_point_t p, mg_dim_t d, mg_bitmap_t v, mg_bitmap_t (*op)(mg_bitmap_t current, mg_bitmap_t input)){
	mg_int_t i, j;
	u8 byte;
	u8 left_mask;
	u8 right_mask;
	mg_size_t col_span;
	u8 row_mask;

	mg_size_t mg_cols = mg_byte_width(mg);
	mg_bitmap_t * dest;

	left_mask = calc_mask(p.x);
	right_mask = ~calc_mask(p.x + d.w);
	if( right_mask == 0 ){ right_mask = ~right_mask; }
	col_span = calc_col_span(p.x, d.w);

	for(i=0; i < d.h; i++){
		if( (mg_y_visible(mg, p.y+i)) ){ //if p.y is not visible don't do this row
			mg_point_t tp;
			tp.x = p.x;
			tp.y = p.y+i;
			dest = mg->data + mg_offset(tp, mg_cols);
			row_mask = calc_row_mask(v, i);

			for(j=0; j < col_span; j++){

				if( (((p.x>>3) + j) >= mg_cols) || ((((p.x>>3) + j) < 0)) ){ //stop if the rest is off the screen
					break;
				}

				byte = row_mask;

				if( (j == (col_span - 1)) ){
					byte &= right_mask;
				}

				if( j == 0 ){
					byte &= left_mask;
				}

				dest[0] = op(dest[0], byte);
				dest++;
			}
		} else {
			//if writing is past the bottom then writing is done
			if( p.y+i >= mg->dim.h ){
				return 0;
			}
		}
	}

	return 0;
}

int mg_op_bitmap(mg_bmap_t * mg, mg_point_t p_dest, const mg_bmap_t * bitmap, mg_point_t p_src, mg_dim_t d, mg_bitmap_t (*op)(mg_bitmap_t current, mg_bitmap_t input)){
	mg_int_t i, j;
	u8 shift = p_dest.x & 0x07;
	u8 not_shift =  8 - shift;
	u8 byte;
	u8 next_byte;
	u8 width_mask;
	u8 src_shift;
	u8 src_not_shift;
	mg_size_t col_span;
	mg_point_t tp;
	mg_size_t mg_cols = mg_byte_width(mg);
	mg_size_t bitmap_cols = mg_byte_width(bitmap);

	mg_bitmap_t * dest;
	const mg_bitmap_t * src;


	src_shift = p_src.x & 0x07;
	src_not_shift = 8 - src_shift;
	col_span = calc_col_span(p_src.x, d.w);

	width_mask = ~calc_mask(d.w);
	if( width_mask == 0 ){ width_mask = ~width_mask; }

	for(i=0; i < d.h; i++){

		if( (mg_y_visible(mg, p_dest.y+i)) && (mg_y_visible(bitmap, p_src.y+i)) ){ //if p.y is not visible don't do this row

			tp.x = p_dest.x;
			tp.y = p_dest.y+i;
			dest = mg->data + mg_offset(tp, mg_cols);
			tp.x = p_src.x;
			tp.y = p_src.y + i;
			src = bitmap->data + mg_offset(tp, bitmap_cols);
			if( src == 0 ){
				return -1;
			}

			for(j=0; j < col_span; j++){

				if( (((p_dest.x>>3) + j) >= mg_cols) || ((p_dest.x>>3) < 0) ){ //stop if the rest is off the screen
					break;
				}

				if( ((p_src.x>>3) + j) >= bitmap_cols ){ //stop if the rest is off the screen
					break;
				}

				byte = *src << src_shift;

				if( src_shift != 0 ){
					next_byte = *(src+1) >> src_not_shift;
					byte |= next_byte;
				}

				if( j == col_span -1 ){
					byte &= width_mask;
				}

				dest[0] = op(dest[0], byte>>shift);
				if( shift != 0 ){
					dest[1] = op(dest[1], byte << not_shift);
				}

				dest++;
				src++;

			}
		} else {
			//if writing is past the bottom then writing is done
			if( p_src.y+i >= mg->dim.h ){
				return 0;
			}
		}
	}

	return 0;
}


u8 mg_tst_bounded_pixel(const mg_bmap_t * mg, mg_point_t p){
	mg_bitmap_t * target = mg->data + mg_offset(p,mg->columns);
	if( *target & mg_mask(p.x) ){
		return 1;
	}
	return 0;
}


u8 mg_tst_pixel(const mg_bmap_t * mg, mg_point_t p){
	if( mg_point_visible(mg, p) ){
		return mg_tst_bounded_pixel(mg,p);
	}
	return 1;
}

void mg_set_pixel(mg_bmap_t * mg, mg_point_t p){
	if( mg_point_visible(mg, p) ){
		mg_set_bounded_pixel(mg, p);
	}
}

void mg_set_bounded_pixel(mg_bmap_t * mg, mg_point_t p){
	mg_bitmap_t * target = mg->data + mg_offset(p,mg->columns);
	*target |= mg_mask(p.x);
}


void mg_inv_pixel(mg_bmap_t * mg, mg_point_t p){
	if( mg_point_visible(mg, p) ){
		mg_inv_bounded_pixel(mg, p);
	}
}

void mg_inv_bounded_pixel(mg_bmap_t * mg, mg_point_t p){
	mg_bitmap_t * target = mg->data + mg_offset(p,mg->columns);
	*target ^= mg_mask(p.x);
}

void mg_clr_pixel(mg_bmap_t * mg, mg_point_t p){
	if( mg_point_visible(mg, p) ){
		mg_clr_bounded_pixel(mg, p);
	}
}

void mg_clr_bounded_pixel(mg_bmap_t * mg, mg_point_t p){
	mg_bitmap_t * target = mg->data + mg_offset(p,mg->columns);
	*target &= ~mg_mask(p.x);
}

void mg_set_area(mg_bmap_t * mg, mg_point_t p, mg_dim_t d, mg_bitmap_t v){
	mg_op_area(mg, p, d, v, mg_op_set);
}

void mg_clr_area(mg_bmap_t * mg, mg_point_t p, mg_dim_t d, mg_bitmap_t v){
	mg_op_area(mg, p, d, 0xFF, mg_op_clr);
}

void mg_inv_area(mg_bmap_t * mg, mg_point_t p, mg_dim_t d, mg_bitmap_t v){
	mg_op_area(mg, p, d, 0xFF, mg_op_inv);
}

void mg_inv_rows(mg_bmap_t * mg, mg_int_t start, mg_int_t stop){
	int i;
	mg_int_t sr;
	mg_int_t er;
	if( start < stop ){
		sr = start;
		er = stop;
	} else {
		sr = stop;
		er = start;
	}
	mg_bitmap_t * data = mg_data(mg, mg_point(0, sr));
	size_t size = mg_byte_width(mg)*(er - sr);
	for(i=0; i < size; i++){
		data[i] = ~data[i];
	}
}

static inline int abs_value(int x){
	if( x < 0 ){ return x*-1; }
	return x;
}

static void mg_op_vline(mg_bmap_t * mg,
		mg_int_t x,
		mg_int_t ymin,
		mg_int_t ymax,
		mg_size_t thickness,
		void (*op)(mg_bmap_t *, mg_point_t)){
	mg_point_t p;
	mg_size_t half_thick = thickness/2;
	mg_size_t it;
	//for(i=ymin-half_thick; i <= ymax+half_thick; i++){
	for(p.y=ymin; p.y <= ymax; p.y++){
		for(it=0; it < thickness; it++){
			//! \todo Use byte level operations for thickness
			p.x = x - half_thick + it;
			op(mg, p);
		}
	}
}

static void mg_op_hline(mg_bmap_t * mg,
		mg_int_t xmin,
		mg_int_t xmax,
		mg_int_t y,
		mg_size_t thickness,
		void (*op)(mg_bmap_t *, mg_point_t)){
	mg_point_t p;
	mg_size_t half_thick = thickness/2;
	mg_size_t it;
	//for(i=xmin-half_thick; i <= xmax+half_thick; i++){
	for(p.x=xmin; p.x <= xmax; p.x++){
		for(it=0; it < thickness; it++){
			//! \todo Use byte level operations here
			p.y = y - half_thick + it;
			op(mg, p);
		}
	}
}


static void mg_op_line(mg_bmap_t * mg,
		mg_point_t p1,
		mg_point_t p2,
		mg_size_t thickness,
		void (*op)(mg_bmap_t *, mg_point_t)){
	int dx, dy;
	int adx, ady;
	int rise, run;
	int i;
	mg_point_t p;
	mg_point_t tmp;
	mg_size_t half_thick;

	if( thickness == 0 ){
		thickness = 1;
	}

	if( p2.y == p1.y ){
		mg_op_hline(mg, p1.x < p2.x ? p1.x : p2.x, p1.x > p2.x ? p1.x : p2.x, p1.y, thickness, op);
		return;
	}

	if( p2.x == p1.x ){
		mg_op_vline(mg, p1.x, p1.y < p2.y ? p1.y : p2.y, p1.y > p2.y ? p1.y : p2.y, thickness, op);
		return;
	}

	half_thick = thickness/2;

	if( p2.y > p1.y ){
		dy = 1;
	} else {
		dy = -1;
	}

	if( p2.x > p1.x ){
		dx = 1;
	} else {
		dx = -1;
	}

	adx = abs_value(p2.x - p1.x);
	ady = abs_value(p2.y - p1.y);
	rise = (p2.y - p1.y);
	run = (p2.x - p1.x);

	p.x = p1.x;
	p.y = p1.y;
	if( adx > ady ){

		while(p.x != p2.x ){
			for(i=0; i < thickness; i++){
				tmp.point = p.point;
				tmp.y = p.y - half_thick + i;
				op(mg,tmp);
			}
			p.x += dx;
			p.y = ((p.x - p1.x) * rise + dy*run/2) / run + p1.y;
		}
	} else {
		while(p.y != p2.y ){
			for(i=0; i < thickness; i++){
				tmp.point = p.point;
				tmp.x = p.x - half_thick + i;
				op(mg,tmp);
			}
			p.y += dy;
			p.x = ((p.y - p1.y) * run + dx*rise/2) / rise + p1.x;
		}
	}

	if( adx <= ady ){
		for(i=0; i < thickness; i++){
			tmp.point = p2.point;
			tmp.x = p2.x - half_thick + i;
			op(mg, tmp);
		}
	} else {
		for(i=0; i < thickness; i++){
			tmp.point = p2.point;
			tmp.y = p2.y - half_thick + i;
			op(mg, tmp);
		}
	}
	//op(mg, p2);
}

void mg_set_vline(mg_bmap_t * mg, mg_int_t x, mg_int_t ymin, mg_int_t ymax, mg_size_t thickness){
	mg_op_vline(mg, x, ymin, ymax, thickness, mg_set_pixel);
}

void mg_clr_vline(mg_bmap_t * mg, mg_int_t x, mg_int_t ymin, mg_int_t ymax, mg_size_t thickness){
	mg_op_vline(mg, x, ymin, ymax, thickness, mg_clr_pixel);
}

void mg_inv_vline(mg_bmap_t * mg, mg_int_t x, mg_int_t ymin, mg_int_t ymax, mg_size_t thickness){
	mg_op_vline(mg, x, ymin, ymax, thickness, mg_inv_pixel);
}

void mg_set_hline(mg_bmap_t * mg, mg_int_t xmin, mg_int_t xmax, mg_int_t y, mg_size_t thickness){
	mg_op_hline(mg, xmin, xmax, y, thickness, mg_set_pixel);
}

void mg_clr_hline(mg_bmap_t * mg, mg_int_t xmin, mg_int_t xmax, mg_int_t y, mg_size_t thickness){
	mg_op_hline(mg, xmin, xmax, y, thickness, mg_clr_pixel);
}

void mg_inv_hline(mg_bmap_t * mg, mg_int_t xmin, mg_int_t xmax, mg_int_t y, mg_size_t thickness){
	mg_op_hline(mg, xmin, xmax, y, thickness, mg_inv_pixel);
}


void mg_set_line(mg_bmap_t * mg, mg_point_t p1, mg_point_t p2, mg_size_t thickness){
	mg_op_line(mg, p1, p2, thickness, mg_set_pixel);
}

void mg_clr_line(mg_bmap_t * mg, mg_point_t p1, mg_point_t p2, mg_size_t thickness){
	mg_op_line(mg, p1, p2, thickness, mg_clr_pixel);
}

void mg_inv_line(mg_bmap_t * mg, mg_point_t p1, mg_point_t p2, mg_size_t thickness){
	mg_op_line(mg, p1, p2, thickness, mg_inv_pixel);
}

