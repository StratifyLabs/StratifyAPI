/*
 * mg.c
 *
 *  Created on: Oct 27, 2015
 *      Author: tgil
 */



#include "mg.h"


void mg_set_data(mg_bmap_t * mg, mg_bitmap_t * mem, mg_size_t w, mg_size_t h){
	mg->dim.w = w;
	mg->dim.h = h;
	mg->columns = mg_calc_byte_width(w);
	mg->data = mem;
}

size_t mg_calc_size(mg_int_t w, mg_int_t h){
	return 0;
}

size_t mg_size(const mg_bmap_t * mg){
	return mg_calc_byte_width(mg->dim.w) * mg->dim.h;
}

size_t mg_byte_width(const mg_bmap_t * mg){
	return mg_calc_byte_width(mg->dim.w);
}

size_t mg_word_width(const mg_bmap_t * mg){
	return mg_calc_word_width(mg->dim.w);
}

size_t mg_calc_byte_width(mg_size_t w){
	return (w + 7) >> 3;
}

size_t mg_calc_word_width(mg_size_t w){
	return (w + 31) >> 5;
}

/*! \brief Change effective size without free/alloc sequence */
mg_bitmap_t * mg_data(const mg_bmap_t * mg, mg_point_t p){
	return mg->data + mg_offset(p, mg_calc_byte_width(mg->dim.w));
}

int mg_set_size(mg_bmap_t * mg, mg_size_t w, mg_size_t h, mg_size_t offset){
	return 0;
}

void mg_bound(const mg_bmap_t * mg, mg_point_t * p){
	mg_bound_x(mg, &(p->x));
	mg_bound_y(mg, &(p->y));
}

void mg_bound_x(const mg_bmap_t * mg, mg_int_t * x){
	mg_int_t t = *x;
	if( t < 0 ){
		t = 0;
	}

	if( t >= mg->dim.w ){
		t = mg->dim.w - 1 ;
	}

	*x = t;
}

void mg_bound_y(const mg_bmap_t * mg, mg_int_t * y){
	mg_int_t t = *y;
	if( t < 0 ){
		t = 0;
	}

	if( t >= mg->dim.h ){
		t = mg->dim.h - 1 ;
	}

	*y = t;
}



mg_int_t mg_x_max(const mg_bmap_t * mg){
	return mg->dim.w -1;
}

mg_int_t mg_y_max(const mg_bmap_t * mg){
	return mg->dim.h -1;
}


void mg_show(const mg_bmap_t * mg){

}
