/*
 * sg_icon_primitive_transform.c
 *
 *  Created on: Oct 27, 2015
 *      Author: tgil
 */


#include <mcu/mcu.h>
#include <unistd.h>

#if defined __armv7em
#define __FPU_PRESENT 1
#define SysTick_IRQn 0
#define __NVIC_PRIO_BITS 4
typedef int IRQn_Type;
#include <mcu/arch/cmsis/core_cm4.h>
#elif defined __armv7m
#define SysTick_IRQn 0
#define __NVIC_PRIO_BITS 4
typedef int IRQn_Type;
#include <mcu/arch/cmsis/core_cm3.h>
#endif

#include "mg.h"

static int sg_animate_push_right(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation);
static int sg_animate_push_left(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation);
static int sg_animate_push_up(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation);
static int sg_animate_push_down(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation);
static int sg_animate_slide_right(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation);
static int sg_animate_undo_slide_right(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation);
static int sg_animate_slide_left(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation);
static int sg_animate_undo_slide_left(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation);
static int sg_animate_slide_up(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation);
static int sg_animate_undo_slide_up(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation);
static int sg_animate_slide_down(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation);
static int sg_animate_undo_slide_down(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation);
static int sg_animate_none(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation);
static int sg_animate_bounce_up(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation);
static int sg_animate_bounce_down(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation);
static int sg_animate_bounce_left(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation);
static int sg_animate_bounce_right(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation);


void sg_flip_xy(sg_bmap_t * mg){
#ifndef __link
	size_t i;
	uint32_t tmp;
	uint32_t * ptr = (uint32_t*)sg_data(mg,sg_point_origin());
	size_t s = sg_size(mg) / 4;
	uint32_t rev;

	for(i=0; i < s/2; i++){
		tmp = ptr[i];
		ptr[i] = ptr[s - i - 1];
		rev = __RBIT(tmp);
		ptr[s-i-1] = rev;
		rev = __RBIT(ptr[i]);
		ptr[i] = rev;
	}
#endif
}

void sg_flip_x(sg_bmap_t * mg){

}

void sg_flip_y(sg_bmap_t * mg){

}

void sg_shift_right(sg_bmap_t * mg, int count, sg_point_t start, sg_dim_t d){
	uint32_t tmp0, tmp1;
	sg_int_t i, j;
	uint32_t * ptr;
	size_t w = sg_byte_width(mg)/4 - 1;

	int page_size;
	int bytes_shifted = 0;

	while( bytes_shifted < count ){

		page_size = count - bytes_shifted;
		if( page_size > 32 ){
			page_size = 32;
		}

		for(i=start.y; i < start.y+d.h; i++){
			if( sg_y_visible(mg, i) ){
				ptr = (uint32_t *)sg_data(mg, sg_point_origin()) + i*(w+1); //point to beg of each row
				for(j = w; j > 0; j--){
					//need to do a byte reversal
					tmp0 = __REV(ptr[j]);
					tmp1 = __REV(ptr[j-1]);
					ptr[j] = __REV((tmp0 >> page_size) | ((tmp1) << (32-page_size)));
				}
				ptr[0] = __REV(__REV(ptr[0]) >> page_size);
			}
		}

		bytes_shifted += page_size;


	}
}

void sg_shift_left(sg_bmap_t * mg, int count, sg_point_t start, sg_dim_t d){
	uint32_t tmp0, tmp1;
	sg_int_t i, j;
	uint32_t * ptr;
	ssize_t w = sg_byte_width(mg)/4 - 1;
	int page_size;
	int bytes_shifted = 0;

	while( bytes_shifted < count ){

		page_size = count - bytes_shifted;
		if( page_size > 32 ){
			page_size = 32;
		}

		for(i=start.y; i < start.y+d.h; i++){
			if( sg_y_visible(mg, i) ){
				ptr = (uint32_t *)sg_data(mg, sg_point_origin()) + i*(w+1); //point to beg of each row
				for(j = 0; j < w; j++){
					//need to do a byte reversal
					tmp0 = __REV(ptr[j]);
					tmp1 = __REV(ptr[j+1]);
					ptr[j] = __REV((tmp0 << page_size) | ((tmp1) >> (32-page_size)));
				}
				ptr[w] = __REV(__REV(ptr[w]) << page_size);
			}
		}

		bytes_shifted += page_size;
	}
}

void sg_shift_up(sg_bmap_t * mg, int count, sg_point_t start, sg_dim_t d){
	sg_point_t dest;
	sg_point_t src;
	sg_size_t rows_shifted;
	sg_dim_t shift_dim;

	if( count > mg->dim.h ){
		count = mg->dim.h;
	}

	if( count == 0 ){
		return;
	}

	if( d.h + start.y > mg->dim.h ){
		d.h = mg->dim.h - start.y;
	}

	//clear the top area
	dest.x = start.x;
	dest.y = start.y;
	shift_dim.w = d.w;
	shift_dim.h = count;

	src.x = start.x;
	rows_shifted = 0;

	while( rows_shifted < (d.h - count) ){
		dest.y = start.y + rows_shifted;
		shift_dim.h = count;
		src.y = start.y + rows_shifted + count;
		if( rows_shifted + shift_dim.h > mg->dim.h ){
			shift_dim.h = d.h - rows_shifted;
		}

		sg_assign_bitmap_area(mg, dest, mg, src, shift_dim);
		rows_shifted += shift_dim.h;

		if( rows_shifted < (d.h - count) ){
			//sg_clr_area(mg, src, shift_dim, 0xFF);
		}

	}
}

void sg_shift_down(sg_bmap_t * mg, int count, sg_point_t start, sg_dim_t d){
	sg_point_t dest;
	sg_point_t src;
	sg_dim_t shift_dim;
	sg_size_t rows_shifted;

	if( count > mg->dim.h ){
		count = mg->dim.h;
	}

	if( count == 0 ){
		return;
	}

	if( d.h + start.y > mg->dim.h ){
		d.h = mg->dim.h - start.y;
	}

	//clear the top area
	dest.x = start.x;
	dest.y = start.y + d.h - count;
	shift_dim.w = d.w;
	shift_dim.h = count;

	src.x = start.x;
	rows_shifted = 0;

	while( rows_shifted < (d.h - count) ){

		if( mg->dim.h > (2*count + rows_shifted) ){
			shift_dim.h = count;
			dest.y = start.y + d.h - rows_shifted - count;
			src.y = dest.y - count;
		} else {
			shift_dim.h = d.h - rows_shifted - count;
			src.y = 0;
			dest.y = rows_shifted - shift_dim.h;
		}

		sg_assign_bitmap_area(mg, dest, mg, src, shift_dim);
		rows_shifted += shift_dim.h;

		if( rows_shifted < (d.h - count) ){
			//sg_clr_area(mg, src, shift_dim, 0xFF);
		}

	}
}


static u32 sum_of_squares(u32 x){
	u32 i;
	u32 sum = 0;
	for(i=1; i <= x; i++){
		sum += i*i;
	}
	return sum;
}

static u32 animation_step_size(sg_animation_t * animation, u32 i){
	u32 step_size;
	u32 m;

	step_size = 4;

	switch(animation->path.type){
	case ANIMATION_PATH_LINEAR:
		step_size = animation->path.motion_total / animation->path.step_total;
		break;
	case ANIMATION_PATH_SQUARED_UNDO:
		m = i+1;
		step_size = (animation->path.motion_total * m*m  + animation->path.sum_of_squares/2) / animation->path.sum_of_squares;
		break;
	case ANIMATION_PATH_SQUARED:
		m = animation->path.step_total - i;
		step_size = (animation->path.motion_total *  m*m + animation->path.sum_of_squares/2) / animation->path.sum_of_squares;
		break;
	}

	return step_size;
}

static u32 sum_of_steps(sg_animation_t * animation){
	u32 i;
	u32 steps = 0;
	for(i=0; i < animation->path.step_total; i++){
		steps += animation_step_size(animation, i);
	}
	return steps;
}

static sg_size_t sg_animate_calc_count(sg_animation_t * animation){
	sg_size_t count;
	u32 sum;
	u16 step = animation->path.step & ~SG_ANIMATION_STEP_FLAG;
	count = animation_step_size(animation, step);
	if( step == 0 ){
		sum = sum_of_steps(animation);
		count = count + (animation->path.motion_total-sum);
	}
	animation->path.step++; //must come sg_animate_calc_count calc_count
	return count;
}

int sg_animate_push_right(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation){
	sg_point_t dest;
	sg_point_t src;
	sg_dim_t shift_dim;
	sg_size_t count;
	sg_point_t start;
	sg_dim_t d;

	if( animation->path.step < animation->path.step_total ){
		count = sg_animate_calc_count(animation);
		//no work to be performed
		if( count == 0 ){
			return 1;
		}

		start = animation->start;
		d = animation->dim;

		animation->path.motion += count;

		src.x = start.x + animation->dim.w - animation->path.motion;
		src.y = start.y;

		shift_dim.w = count;
		shift_dim.h = d.h;

		dest.x = start.x;
		dest.y = start.y;

		sg_shift_right(mg, count, start, d);
		sg_clr_area(mg, dest, shift_dim, 0xFF);
		sg_set_bitmap_area(mg, dest, bitmap, src, shift_dim);


		return 1;
	}

	return 0;

}

int sg_animate_push_left(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation){
	sg_point_t dest;
	sg_point_t src;
	sg_dim_t shift_dim;
	sg_point_t start;
	sg_dim_t d;
	sg_size_t count;

	if( animation->path.step < animation->path.step_total ){
		count = sg_animate_calc_count(animation);

		if( count == 0 ){
			return 1;
		}

		d = animation->dim;
		start = animation->start;

		dest.x = start.x + animation->dim.w - count;
		dest.y = start.y;

		shift_dim.w = count;
		shift_dim.h = d.h;

		src.x = start.x + animation->path.motion;
		src.y = start.y;

		sg_shift_left(mg, count, start, d);
		sg_clr_area(mg, dest, shift_dim, 0xFF);
		sg_set_bitmap_area(mg, dest, bitmap, src, shift_dim);

		animation->path.motion += count;

		return 1;
	}
	return 0;
}

int sg_animate_push_up(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation){
	sg_point_t dest;
	sg_point_t src;
	sg_dim_t d;
	sg_size_t count;

	sg_point_t start;

	if( animation->path.step < animation->path.step_total ){
		start = animation->start;
		count = sg_animate_calc_count(animation);

		src.x = start.x;
		src.y = start.y + animation->path.motion;

		d.w = animation->dim.w;
		d.h = count;

		dest.x = start.x;
		dest.y = start.y + animation->dim.h - d.h;

		sg_shift_up(mg, count, start, animation->dim);
		sg_assign_bitmap_area(mg, dest, bitmap, src, d);

		animation->path.motion += count;

		return 1;
	}
	return 0;
}

int sg_animate_push_down(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation){
	sg_point_t src;
	sg_dim_t d;
	sg_size_t count;

	sg_point_t start;

	if( animation->path.step < animation->path.step_total ){
		start = animation->start;
		count = sg_animate_calc_count(animation);
		animation->path.motion += count;

		src.x = start.x;
		src.y = start.y + animation->dim.h - animation->path.motion;

		d.w = animation->dim.w;
		d.h = count;

		sg_shift_down(mg, count, start, animation->dim);
		sg_assign_bitmap_area(mg, start, bitmap, src, d);


		return 1;
	}
	return 0;
}

int sg_animate_slide_right(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation){
	//this could be used for drawers
	return 0;
}

int sg_animate_undo_slide_right(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation){
	//this could be used for drawers
	return 0;
}

int sg_animate_slide_left(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation){
	//this could be used for drawers
	return 0;
}

int sg_animate_undo_slide_left(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation){
	//this could be used for drawers
	return 0;
}

int sg_animate_slide_up(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation){
	sg_point_t dest;
	sg_point_t src;
	sg_dim_t d;

	sg_point_t start;

	if( animation->path.step < animation->path.step_total ){
		start = animation->start;
		animation->path.motion += sg_animate_calc_count(animation);

		src.x = start.x;
		src.y = start.y;

		dest.x = start.x;
		dest.y = start.y + animation->dim.h - animation->path.motion;

		d.w = animation->dim.w;
		d.h = animation->path.motion;

		sg_clr_area(mg, dest, d, 0xFF);
		sg_set_bitmap_area(mg, dest, bitmap, src, d);


		return 1;
	}
	return 0;
}

int sg_animate_undo_slide_up(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation){
	sg_point_t dest;
	sg_point_t src;
	sg_dim_t d;
	sg_size_t count;

	sg_point_t start;

	if( animation->path.step < animation->path.step_total ){
		start = animation->start;
		count = sg_animate_calc_count(animation);
		animation->path.motion += count;

		src.x = start.x;
		src.y = start.y;

		dest.x = start.x;
		dest.y = start.y;

		d.w = animation->dim.w;
		d.h = animation->path.motion;

		sg_shift_down(mg, count, start, animation->dim);

		sg_clr_area(mg, dest, d, 0xFF);
		sg_set_bitmap_area(mg, dest, bitmap, src, d);

		return 1;
	}
	return 0;
}

int sg_animate_slide_down(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation){
	sg_point_t src;
	sg_point_t dest;
	sg_dim_t d;

	sg_point_t start;

	if( animation->path.step < animation->path.step_total ){
		start = animation->start;
		animation->path.motion += sg_animate_calc_count(animation);
		dest.x = start.x;
		dest.y = start.y;

		src.x = start.x;
		src.y = start.y + animation->dim.h - animation->path.motion;

		d.w = animation->dim.w;
		d.h = animation->path.motion;

		sg_clr_area(mg, start, d, 0xFF);
		sg_set_bitmap_area(mg, dest, bitmap, src, d);
		return 1;
	}
	return 0;
}

int sg_animate_undo_slide_down(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation){
	sg_point_t dest;
	sg_point_t src;
	sg_dim_t d;
	sg_size_t count;

	sg_point_t start;

	if( animation->path.step < animation->path.step_total ){
		start = animation->start;
		count = sg_animate_calc_count(animation);

		animation->path.motion += count;

		src.x = start.x;
		src.y = start.y + animation->dim.h - animation->path.motion;

		dest = src;

		d.w = animation->dim.w;
		d.h = animation->dim.h - animation->path.motion + count;

		sg_shift_up(mg, count, start, d);

		d.h = count;

		sg_clr_area(mg, dest, d, 0xFF);
		sg_set_bitmap_area(mg, dest, bitmap, src, d);

		return 1;
	}
	return 0;

}

int sg_animate_none(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation){
	if( animation->path.step < animation->path.step_total ){
		animation->path.motion = sg_animate_calc_count(animation);
		return 1;
	}
	return 0;
}

int sg_animate_bounce_up(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation){
	sg_point_t dest;
	sg_point_t src;
	sg_dim_t d;
	sg_size_t count;
	u16 step;

	sg_point_t start;

	step = animation->path.step & ~(SG_ANIMATION_STEP_FLAG);

	if( step < animation->path.step_total ){
		start = animation->start;
		count = sg_animate_calc_count(animation);

		if( animation->path.step & (SG_ANIMATION_STEP_FLAG) ){
			//bounce back phase

			dest.x = animation->start.x;
			dest.y = animation->start.y + animation->path.motion_total - animation->path.motion;

			d.w = animation->dim.w;
			d.h = animation->dim.h - animation->path.motion_total + animation->path.motion;

			sg_assign_bitmap_area(mg, dest, bitmap, start, d);

		} else {
			//bounce phase
			if( step == 0 ){
				sg_assign_bitmap_area(mg, animation->start, bitmap, animation->start, animation->dim);
			}

			src.x = start.x;
			src.y = start.y + animation->path.motion;

			d.w = animation->dim.w;
			d.h = animation->dim.h - animation->path.motion;

			dest.x = start.x;
			dest.y = start.y;

			sg_shift_down(mg, count, src, d);

			d.h = animation->path.motion + count;

			sg_clr_area(mg, dest, d, 0xFF);
			sg_set_area(mg, dest, d, 0xAA);

			if( animation->path.step == animation->path.step_total ){
				animation->path.step = SG_ANIMATION_STEP_FLAG;
				animation->path.motion = 0;
			}
		}

		animation->path.motion += count;

		return 1;
	}
	return 0;
}

static int sg_animate_bounce_down(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation){
	sg_point_t dest;
	sg_point_t src;
	sg_dim_t d;
	sg_size_t count;
	u16 step;

	sg_point_t start;

	step = animation->path.step & ~(SG_ANIMATION_STEP_FLAG);

	if( step < animation->path.step_total ){
		start = animation->start;
		count = sg_animate_calc_count(animation);

		if( animation->path.step & (SG_ANIMATION_STEP_FLAG) ){
			//bounce back phase

			src.x = start.x;
			src.y = start.y + animation->path.motion_total - animation->path.motion;

			dest.x = animation->start.x;
			dest.y = animation->start.y;

			d.w = animation->dim.w;
			d.h = animation->dim.h - animation->path.motion_total + animation->path.motion;

			sg_assign_bitmap_area(mg, dest, bitmap, src, d);

		} else {
			//bounce phase

			if( step == 0 ){
				//copy the drawing to the scratch area
				sg_assign_bitmap_area(mg, animation->start, bitmap, animation->start, animation->dim);
			}

			src.x = start.x;
			src.y = start.y;

			d.w = animation->dim.w;
			d.h = animation->dim.h - animation->path.motion;

			sg_shift_up(mg, count, src, d);

			d.h = animation->path.motion + count;
			dest.x = start.x;
			dest.y = start.y + animation->dim.h - d.h;

			sg_clr_area(mg, dest, d, 0xFF);
			sg_set_area(mg, dest, d, 0xAA);

			if( animation->path.step == animation->path.step_total ){
				animation->path.step = SG_ANIMATION_STEP_FLAG;
				animation->path.motion = 0;
			}
		}

		animation->path.motion += count;

		return 1;
	}
	return 0;
}

static int sg_animate_bounce_left(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation){
	sg_point_t dest;
	sg_point_t src;
	sg_dim_t d;
	sg_size_t count;
	u16 step;

	sg_point_t start;

	step = animation->path.step & ~(SG_ANIMATION_STEP_FLAG);

	if( step < animation->path.step_total ){
		start = animation->start;
		count = sg_animate_calc_count(animation);

		if( animation->path.step & (SG_ANIMATION_STEP_FLAG) ){
			//bounce back phase

			dest.x = animation->start.x + animation->path.motion_total - animation->path.motion;
			dest.y = animation->start.y;

			d.w = animation->dim.w - animation->path.motion_total + animation->path.motion;
			d.h = animation->dim.h;

			sg_assign_bitmap_area(mg, dest, bitmap, start, d);

		} else {
			//bounce phase
			if( step == 0 ){
				sg_assign_bitmap_area(mg, animation->start, bitmap, animation->start, animation->dim);
			}

			src.x = start.x + animation->path.motion;
			src.y = start.y;

			d.w = animation->dim.w - animation->path.motion;
			d.h = animation->dim.h;

			dest.x = start.x;
			dest.y = start.y;

			sg_shift_right(mg, count, src, d);

			d.w = animation->path.motion + count;

			sg_clr_area(mg, dest, d, 0xFF);
			sg_set_area(mg, dest, d, 0xAA);

			if( animation->path.step == animation->path.step_total ){
				animation->path.step = SG_ANIMATION_STEP_FLAG;
				animation->path.motion = 0;
			}
		}

		animation->path.motion += count;

		return 1;
	}
	return 0;
}

static int sg_animate_bounce_right(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation){
	sg_point_t dest;
	sg_point_t src;
	sg_dim_t d;
	sg_size_t count;
	u16 step;

	sg_point_t start;

	step = animation->path.step & ~(SG_ANIMATION_STEP_FLAG);

	if( step < animation->path.step_total ){
		start = animation->start;
		count = sg_animate_calc_count(animation);

		if( animation->path.step & (SG_ANIMATION_STEP_FLAG) ){
			//bounce back phase

			src.x = start.x + animation->path.motion_total - animation->path.motion;
			src.y = start.y;

			dest.x = animation->start.x;
			dest.y = animation->start.y;

			d.w = animation->dim.w - animation->path.motion_total + animation->path.motion;
			d.h = animation->dim.h;

			sg_assign_bitmap_area(mg, dest, bitmap, src, d);

		} else {
			//bounce phase

			if( step == 0 ){
				//copy the drawing to the scratch area
				sg_assign_bitmap_area(mg, animation->start, bitmap, animation->start, animation->dim);
			}

			src.x = start.x;
			src.y = start.y;

			d.w = animation->dim.w - animation->path.motion;
			d.h = animation->dim.h;

			sg_shift_left(mg, count, src, d);

			d.w = animation->path.motion + count;
			dest.x = start.x + animation->dim.w - d.w;
			dest.y = start.y;

			sg_clr_area(mg, dest, d, 0xFF);
			sg_set_area(mg, dest, d, 0xAA);

			if( animation->path.step == animation->path.step_total ){
				animation->path.step = SG_ANIMATION_STEP_FLAG;
				animation->path.motion = 0;
			}
		}

		animation->path.motion += count;

		return 1;
	}
	return 0;}

int (* const animations[ANIMATION_TYPE_TOTAL])(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation) = {
		sg_animate_push_left,
		sg_animate_push_right,
		sg_animate_push_up,
		sg_animate_push_down,
		sg_animate_slide_left,
		sg_animate_undo_slide_left,
		sg_animate_slide_right,
		sg_animate_undo_slide_right,
		sg_animate_slide_up,
		sg_animate_undo_slide_up,
		sg_animate_slide_down,
		sg_animate_undo_slide_down,
		sg_animate_none,
		sg_animate_bounce_up,
		sg_animate_bounce_down,
		sg_animate_bounce_left,
		sg_animate_bounce_right
};

int sg_animate(sg_bmap_t * mg, sg_bmap_t * bitmap, sg_animation_t * animation){
	if( animation->type < ANIMATION_TYPE_TOTAL ){
		return animations[animation->type](mg, bitmap, animation);
	}
	return -1;
}

int sg_animate_init(sg_animation_t * animation,
		u8 type,
		u8 path,
		u8 step_total,
		sg_size_t motion_total,
		sg_point_t start,
		sg_dim_t dim){
	animation->type = type;
	animation->path.type = path;
	animation->path.step = 0;
	animation->start = start;
	animation->dim = dim;
	animation->path.step_total = step_total;
	animation->path.motion = 0;
	animation->path.motion_total = motion_total;
	animation->path.sum_of_squares = sum_of_squares(step_total);
	return 0;

}

