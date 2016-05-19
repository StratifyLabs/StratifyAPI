/*
 * mg_transform.c
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

static int mg_animate_push_right(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation);
static int mg_animate_push_left(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation);
static int mg_animate_push_up(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation);
static int mg_animate_push_down(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation);
static int mg_animate_slide_right(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation);
static int mg_animate_undo_slide_right(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation);
static int mg_animate_slide_left(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation);
static int mg_animate_undo_slide_left(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation);
static int mg_animate_slide_up(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation);
static int mg_animate_undo_slide_up(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation);
static int mg_animate_slide_down(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation);
static int mg_animate_undo_slide_down(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation);
static int mg_animate_none(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation);
static int mg_animate_bounce_up(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation);
static int mg_animate_bounce_down(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation);
static int mg_animate_bounce_left(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation);
static int mg_animate_bounce_right(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation);


void mg_flip_xy(mg_bmap_t * mg){
#ifndef __link
	size_t i;
	uint32_t tmp;
	uint32_t * ptr = (uint32_t*)mg_data(mg,mg_point_origin());
	size_t s = mg_size(mg) / 4;
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

void mg_flip_x(mg_bmap_t * mg){

}

void mg_flip_y(mg_bmap_t * mg){

}

void mg_shift_right(mg_bmap_t * mg, int count, mg_point_t start, mg_dim_t d){
	uint32_t tmp0, tmp1;
	mg_int_t i, j;
	uint32_t * ptr;
	size_t w = mg_byte_width(mg)/4 - 1;

	int page_size;
	int bytes_shifted = 0;

	while( bytes_shifted < count ){

		page_size = count - bytes_shifted;
		if( page_size > 32 ){
			page_size = 32;
		}

		for(i=start.y; i < start.y+d.h; i++){
			if( mg_y_visible(mg, i) ){
				ptr = (uint32_t *)mg_data(mg, mg_point_origin()) + i*(w+1); //point to beg of each row
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

void mg_shift_left(mg_bmap_t * mg, int count, mg_point_t start, mg_dim_t d){
	uint32_t tmp0, tmp1;
	mg_int_t i, j;
	uint32_t * ptr;
	ssize_t w = mg_byte_width(mg)/4 - 1;
	int page_size;
	int bytes_shifted = 0;

	while( bytes_shifted < count ){

		page_size = count - bytes_shifted;
		if( page_size > 32 ){
			page_size = 32;
		}

		for(i=start.y; i < start.y+d.h; i++){
			if( mg_y_visible(mg, i) ){
				ptr = (uint32_t *)mg_data(mg, mg_point_origin()) + i*(w+1); //point to beg of each row
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

void mg_shift_up(mg_bmap_t * mg, int count, mg_point_t start, mg_dim_t d){
	mg_point_t dest;
	mg_point_t src;
	mg_size_t rows_shifted;
	mg_dim_t shift_dim;

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

		mg_assign_bitmap_area(mg, dest, mg, src, shift_dim);
		rows_shifted += shift_dim.h;

		if( rows_shifted < (d.h - count) ){
			//mg_clr_area(mg, src, shift_dim, 0xFF);
		}

	}
}

void mg_shift_down(mg_bmap_t * mg, int count, mg_point_t start, mg_dim_t d){
	mg_point_t dest;
	mg_point_t src;
	mg_dim_t shift_dim;
	mg_size_t rows_shifted;

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

		mg_assign_bitmap_area(mg, dest, mg, src, shift_dim);
		rows_shifted += shift_dim.h;

		if( rows_shifted < (d.h - count) ){
			//mg_clr_area(mg, src, shift_dim, 0xFF);
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

static u32 animation_step_size(mg_animation_t * animation, u32 i){
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

static u32 sum_of_steps(mg_animation_t * animation){
	u32 i;
	u32 steps = 0;
	for(i=0; i < animation->path.step_total; i++){
		steps += animation_step_size(animation, i);
	}
	return steps;
}

static mg_size_t mg_animate_calc_count(mg_animation_t * animation){
	mg_size_t count;
	u32 sum;
	u16 step = animation->path.step & ~MG_ANIMATION_STEP_FLAG;
	count = animation_step_size(animation, step);
	if( step == 0 ){
		sum = sum_of_steps(animation);
		count = count + (animation->path.motion_total-sum);
	}
	animation->path.step++; //must come mg_animate_calc_count calc_count
	return count;
}

int mg_animate_push_right(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation){
	mg_point_t dest;
	mg_point_t src;
	mg_dim_t shift_dim;
	mg_size_t count;
	mg_point_t start;
	mg_dim_t d;

	if( animation->path.step < animation->path.step_total ){
		count = mg_animate_calc_count(animation);
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

		mg_shift_right(mg, count, start, d);
		mg_clr_area(mg, dest, shift_dim, 0xFF);
		mg_set_bitmap_area(mg, dest, bitmap, src, shift_dim);


		return 1;
	}

	return 0;

}

int mg_animate_push_left(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation){
	mg_point_t dest;
	mg_point_t src;
	mg_dim_t shift_dim;
	mg_point_t start;
	mg_dim_t d;
	mg_size_t count;

	if( animation->path.step < animation->path.step_total ){
		count = mg_animate_calc_count(animation);

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

		mg_shift_left(mg, count, start, d);
		mg_clr_area(mg, dest, shift_dim, 0xFF);
		mg_set_bitmap_area(mg, dest, bitmap, src, shift_dim);

		animation->path.motion += count;

		return 1;
	}
	return 0;
}

int mg_animate_push_up(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation){
	mg_point_t dest;
	mg_point_t src;
	mg_dim_t d;
	mg_size_t count;

	mg_point_t start;

	if( animation->path.step < animation->path.step_total ){
		start = animation->start;
		count = mg_animate_calc_count(animation);

		src.x = start.x;
		src.y = start.y + animation->path.motion;

		d.w = animation->dim.w;
		d.h = count;

		dest.x = start.x;
		dest.y = start.y + animation->dim.h - d.h;

		mg_shift_up(mg, count, start, animation->dim);
		mg_assign_bitmap_area(mg, dest, bitmap, src, d);

		animation->path.motion += count;

		return 1;
	}
	return 0;
}

int mg_animate_push_down(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation){
	mg_point_t src;
	mg_dim_t d;
	mg_size_t count;

	mg_point_t start;

	if( animation->path.step < animation->path.step_total ){
		start = animation->start;
		count = mg_animate_calc_count(animation);
		animation->path.motion += count;

		src.x = start.x;
		src.y = start.y + animation->dim.h - animation->path.motion;

		d.w = animation->dim.w;
		d.h = count;

		mg_shift_down(mg, count, start, animation->dim);
		mg_assign_bitmap_area(mg, start, bitmap, src, d);


		return 1;
	}
	return 0;
}

int mg_animate_slide_right(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation){
	//this could be used for drawers
	return 0;
}

int mg_animate_undo_slide_right(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation){
	//this could be used for drawers
	return 0;
}

int mg_animate_slide_left(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation){
	//this could be used for drawers
	return 0;
}

int mg_animate_undo_slide_left(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation){
	//this could be used for drawers
	return 0;
}

int mg_animate_slide_up(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation){
	mg_point_t dest;
	mg_point_t src;
	mg_dim_t d;

	mg_point_t start;

	if( animation->path.step < animation->path.step_total ){
		start = animation->start;
		animation->path.motion += mg_animate_calc_count(animation);

		src.x = start.x;
		src.y = start.y;

		dest.x = start.x;
		dest.y = start.y + animation->dim.h - animation->path.motion;

		d.w = animation->dim.w;
		d.h = animation->path.motion;

		mg_clr_area(mg, dest, d, 0xFF);
		mg_set_bitmap_area(mg, dest, bitmap, src, d);


		return 1;
	}
	return 0;
}

int mg_animate_undo_slide_up(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation){
	mg_point_t dest;
	mg_point_t src;
	mg_dim_t d;
	mg_size_t count;

	mg_point_t start;

	if( animation->path.step < animation->path.step_total ){
		start = animation->start;
		count = mg_animate_calc_count(animation);
		animation->path.motion += count;

		src.x = start.x;
		src.y = start.y;

		dest.x = start.x;
		dest.y = start.y;

		d.w = animation->dim.w;
		d.h = animation->path.motion;

		mg_shift_down(mg, count, start, animation->dim);

		mg_clr_area(mg, dest, d, 0xFF);
		mg_set_bitmap_area(mg, dest, bitmap, src, d);

		return 1;
	}
	return 0;
}

int mg_animate_slide_down(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation){
	mg_point_t src;
	mg_point_t dest;
	mg_dim_t d;

	mg_point_t start;

	if( animation->path.step < animation->path.step_total ){
		start = animation->start;
		animation->path.motion += mg_animate_calc_count(animation);
		dest.x = start.x;
		dest.y = start.y;

		src.x = start.x;
		src.y = start.y + animation->dim.h - animation->path.motion;

		d.w = animation->dim.w;
		d.h = animation->path.motion;

		mg_clr_area(mg, start, d, 0xFF);
		mg_set_bitmap_area(mg, dest, bitmap, src, d);
		return 1;
	}
	return 0;
}

int mg_animate_undo_slide_down(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation){
	mg_point_t dest;
	mg_point_t src;
	mg_dim_t d;
	mg_size_t count;

	mg_point_t start;

	if( animation->path.step < animation->path.step_total ){
		start = animation->start;
		count = mg_animate_calc_count(animation);

		animation->path.motion += count;

		src.x = start.x;
		src.y = start.y + animation->dim.h - animation->path.motion;

		dest = src;

		d.w = animation->dim.w;
		d.h = animation->dim.h - animation->path.motion + count;

		mg_shift_up(mg, count, start, d);

		d.h = count;

		mg_clr_area(mg, dest, d, 0xFF);
		mg_set_bitmap_area(mg, dest, bitmap, src, d);

		return 1;
	}
	return 0;

}

int mg_animate_none(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation){
	if( animation->path.step < animation->path.step_total ){
		animation->path.motion = mg_animate_calc_count(animation);
		return 1;
	}
	return 0;
}

int mg_animate_bounce_up(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation){
	mg_point_t dest;
	mg_point_t src;
	mg_dim_t d;
	mg_size_t count;
	u16 step;

	mg_point_t start;

	step = animation->path.step & ~(MG_ANIMATION_STEP_FLAG);

	if( step < animation->path.step_total ){
		start = animation->start;
		count = mg_animate_calc_count(animation);

		if( animation->path.step & (MG_ANIMATION_STEP_FLAG) ){
			//bounce back phase

			dest.x = animation->start.x;
			dest.y = animation->start.y + animation->path.motion_total - animation->path.motion;

			d.w = animation->dim.w;
			d.h = animation->dim.h - animation->path.motion_total + animation->path.motion;

			mg_assign_bitmap_area(mg, dest, bitmap, start, d);

		} else {
			//bounce phase
			if( step == 0 ){
				mg_assign_bitmap_area(mg, animation->start, bitmap, animation->start, animation->dim);
			}

			src.x = start.x;
			src.y = start.y + animation->path.motion;

			d.w = animation->dim.w;
			d.h = animation->dim.h - animation->path.motion;

			dest.x = start.x;
			dest.y = start.y;

			mg_shift_down(mg, count, src, d);

			d.h = animation->path.motion + count;

			mg_clr_area(mg, dest, d, 0xFF);
			mg_set_area(mg, dest, d, 0xAA);

			if( animation->path.step == animation->path.step_total ){
				animation->path.step = MG_ANIMATION_STEP_FLAG;
				animation->path.motion = 0;
			}
		}

		animation->path.motion += count;

		return 1;
	}
	return 0;
}

static int mg_animate_bounce_down(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation){
	mg_point_t dest;
	mg_point_t src;
	mg_dim_t d;
	mg_size_t count;
	u16 step;

	mg_point_t start;

	step = animation->path.step & ~(MG_ANIMATION_STEP_FLAG);

	if( step < animation->path.step_total ){
		start = animation->start;
		count = mg_animate_calc_count(animation);

		if( animation->path.step & (MG_ANIMATION_STEP_FLAG) ){
			//bounce back phase

			src.x = start.x;
			src.y = start.y + animation->path.motion_total - animation->path.motion;

			dest.x = animation->start.x;
			dest.y = animation->start.y;

			d.w = animation->dim.w;
			d.h = animation->dim.h - animation->path.motion_total + animation->path.motion;

			mg_assign_bitmap_area(mg, dest, bitmap, src, d);

		} else {
			//bounce phase

			if( step == 0 ){
				//copy the drawing to the scratch area
				mg_assign_bitmap_area(mg, animation->start, bitmap, animation->start, animation->dim);
			}

			src.x = start.x;
			src.y = start.y;

			d.w = animation->dim.w;
			d.h = animation->dim.h - animation->path.motion;

			mg_shift_up(mg, count, src, d);

			d.h = animation->path.motion + count;
			dest.x = start.x;
			dest.y = start.y + animation->dim.h - d.h;

			mg_clr_area(mg, dest, d, 0xFF);
			mg_set_area(mg, dest, d, 0xAA);

			if( animation->path.step == animation->path.step_total ){
				animation->path.step = MG_ANIMATION_STEP_FLAG;
				animation->path.motion = 0;
			}
		}

		animation->path.motion += count;

		return 1;
	}
	return 0;
}

static int mg_animate_bounce_left(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation){
	mg_point_t dest;
	mg_point_t src;
	mg_dim_t d;
	mg_size_t count;
	u16 step;

	mg_point_t start;

	step = animation->path.step & ~(MG_ANIMATION_STEP_FLAG);

	if( step < animation->path.step_total ){
		start = animation->start;
		count = mg_animate_calc_count(animation);

		if( animation->path.step & (MG_ANIMATION_STEP_FLAG) ){
			//bounce back phase

			dest.x = animation->start.x + animation->path.motion_total - animation->path.motion;
			dest.y = animation->start.y;

			d.w = animation->dim.w - animation->path.motion_total + animation->path.motion;
			d.h = animation->dim.h;

			mg_assign_bitmap_area(mg, dest, bitmap, start, d);

		} else {
			//bounce phase
			if( step == 0 ){
				mg_assign_bitmap_area(mg, animation->start, bitmap, animation->start, animation->dim);
			}

			src.x = start.x + animation->path.motion;
			src.y = start.y;

			d.w = animation->dim.w - animation->path.motion;
			d.h = animation->dim.h;

			dest.x = start.x;
			dest.y = start.y;

			mg_shift_right(mg, count, src, d);

			d.w = animation->path.motion + count;

			mg_clr_area(mg, dest, d, 0xFF);
			mg_set_area(mg, dest, d, 0xAA);

			if( animation->path.step == animation->path.step_total ){
				animation->path.step = MG_ANIMATION_STEP_FLAG;
				animation->path.motion = 0;
			}
		}

		animation->path.motion += count;

		return 1;
	}
	return 0;
}

static int mg_animate_bounce_right(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation){
	mg_point_t dest;
	mg_point_t src;
	mg_dim_t d;
	mg_size_t count;
	u16 step;

	mg_point_t start;

	step = animation->path.step & ~(MG_ANIMATION_STEP_FLAG);

	if( step < animation->path.step_total ){
		start = animation->start;
		count = mg_animate_calc_count(animation);

		if( animation->path.step & (MG_ANIMATION_STEP_FLAG) ){
			//bounce back phase

			src.x = start.x + animation->path.motion_total - animation->path.motion;
			src.y = start.y;

			dest.x = animation->start.x;
			dest.y = animation->start.y;

			d.w = animation->dim.w - animation->path.motion_total + animation->path.motion;
			d.h = animation->dim.h;

			mg_assign_bitmap_area(mg, dest, bitmap, src, d);

		} else {
			//bounce phase

			if( step == 0 ){
				//copy the drawing to the scratch area
				mg_assign_bitmap_area(mg, animation->start, bitmap, animation->start, animation->dim);
			}

			src.x = start.x;
			src.y = start.y;

			d.w = animation->dim.w - animation->path.motion;
			d.h = animation->dim.h;

			mg_shift_left(mg, count, src, d);

			d.w = animation->path.motion + count;
			dest.x = start.x + animation->dim.w - d.w;
			dest.y = start.y;

			mg_clr_area(mg, dest, d, 0xFF);
			mg_set_area(mg, dest, d, 0xAA);

			if( animation->path.step == animation->path.step_total ){
				animation->path.step = MG_ANIMATION_STEP_FLAG;
				animation->path.motion = 0;
			}
		}

		animation->path.motion += count;

		return 1;
	}
	return 0;}

int (* const animations[ANIMATION_TYPE_TOTAL])(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation) = {
		mg_animate_push_left,
		mg_animate_push_right,
		mg_animate_push_up,
		mg_animate_push_down,
		mg_animate_slide_left,
		mg_animate_undo_slide_left,
		mg_animate_slide_right,
		mg_animate_undo_slide_right,
		mg_animate_slide_up,
		mg_animate_undo_slide_up,
		mg_animate_slide_down,
		mg_animate_undo_slide_down,
		mg_animate_none,
		mg_animate_bounce_up,
		mg_animate_bounce_down,
		mg_animate_bounce_left,
		mg_animate_bounce_right
};

int mg_animate(mg_bmap_t * mg, mg_bmap_t * bitmap, mg_animation_t * animation){
	if( animation->type < ANIMATION_TYPE_TOTAL ){
		return animations[animation->type](mg, bitmap, animation);
	}
	return -1;
}

int mg_animate_init(mg_animation_t * animation,
		u8 type,
		u8 path,
		u8 step_total,
		mg_size_t motion_total,
		mg_point_t start,
		mg_dim_t dim){
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

