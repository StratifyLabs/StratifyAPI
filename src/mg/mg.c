/*
 * mg.c
 *
 *  Created on: Oct 27, 2015
 *      Author: tgil
 */



#include "mg.h"


void sg_set_data(sg_bmap_t * mg, sg_bmap_data_t * mem, sg_size_t w, sg_size_t h){
	mg->dim.w = w;
	mg->dim.h = h;
	mg->columns = sg_calc_byte_width(w);
	mg->data = mem;
}

size_t sg_calc_size(sg_int_t w, sg_int_t h){
	return 0;
}

size_t sg_size(const sg_bmap_t * mg){
	return sg_calc_byte_width(mg->dim.w) * mg->dim.h;
}

size_t sg_byte_width(const sg_bmap_t * mg){
	return sg_calc_byte_width(mg->dim.w);
}

size_t sg_word_width(const sg_bmap_t * mg){
	return sg_calc_word_width(mg->dim.w);
}

size_t sg_calc_byte_width(sg_size_t w){
	return (w + 7) >> 3;
}

size_t sg_calc_word_width(sg_size_t w){
	return (w + 31) >> 5;
}

/*! \brief Change effective size without free/alloc sequence */
sg_bmap_data_t * sg_data(const sg_bmap_t * mg, sg_point_t p){
	return mg->data + sg_offset(p, sg_calc_byte_width(mg->dim.w));
}

int sg_set_size(sg_bmap_t * mg, sg_size_t w, sg_size_t h, sg_size_t offset){
	return 0;
}

void sg_bound(const sg_bmap_t * mg, sg_point_t * p){
	sg_bound_x(mg, &(p->x));
	sg_bound_y(mg, &(p->y));
}

void sg_bound_x(const sg_bmap_t * mg, sg_int_t * x){
	sg_int_t t = *x;
	if( t < 0 ){
		t = 0;
	}

	if( t >= mg->dim.w ){
		t = mg->dim.w - 1 ;
	}

	*x = t;
}

void sg_bound_y(const sg_bmap_t * mg, sg_int_t * y){
	sg_int_t t = *y;
	if( t < 0 ){
		t = 0;
	}

	if( t >= mg->dim.h ){
		t = mg->dim.h - 1 ;
	}

	*y = t;
}



sg_int_t sg_x_max(const sg_bmap_t * mg){
	return mg->dim.w -1;
}

sg_int_t sg_y_max(const sg_bmap_t * mg){
	return mg->dim.h -1;
}


void sg_show(const sg_bmap_t * mg){

}
