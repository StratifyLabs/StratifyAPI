
#include "mg.h"


static void draw_line(const sg_icon_primitive_t * p, sg_bmap_t * bm, const sg_map_t * map, sg_bounds_t * attr);
static void draw_arc(const sg_icon_primitive_t * p, sg_bmap_t * bm, const sg_map_t * map, sg_bounds_t * attr);
static void draw_fill(const sg_icon_primitive_t * p, sg_bmap_t * bm, const sg_map_t * map, sg_bounds_t * attr);

static void (*draw_func [SG_TYPE_TOTAL])(const sg_icon_primitive_t * p, sg_bmap_t * bm, const sg_map_t * map, sg_bounds_t * attr) = {
		draw_line,
		draw_arc,
		draw_fill
};


void sg_draw(sg_bmap_t * bitmap, const sg_icon_primitive_t * prim, const sg_map_t * map, sg_bounds_t * attr){
	int type;
	type = prim->type & SG_TYPE_MASK;
	if( prim->type & SG_ENABLE_FLAG ){
		if( (type & SG_TYPE_MASK) < SG_TYPE_TOTAL ){
			draw_func[type](prim, bitmap, map, attr);
		}
	}
}

void sg_draw_icon(sg_bmap_t * bitmap, const sg_icon_t * icon, const sg_map_t * map, sg_bounds_t * attr){
	unsigned int total;
	if( map->o_thickness_fill == 0 ){
		total = icon->total;
	} else {
		total = icon->total - icon->fill_total;
	}
	sg_draw_list(bitmap, icon->elements, total, map, attr);
}

void sg_draw_list(sg_bmap_t * bitmap, const sg_icon_primitive_t prim_list[], unsigned int total, const sg_map_t * map, sg_bounds_t * attr){
	unsigned int i;

	if( attr ){
		attr->bottom_right.x = -SG_MAX;
		attr->bottom_right.y = -SG_MAX;
		attr->top_left.x = SG_MAX;
		attr->top_left.y = SG_MAX;
	}

	for(i=0; i < total; i++){
		sg_draw(bitmap, &(prim_list[i]), map, attr);
	}
}


void draw_line(const sg_icon_primitive_t * p, sg_bmap_t * bm, const sg_map_t * map, sg_bounds_t * attr){
	//draw a line from bottom left to top right
	sg_point_t p1;
	sg_point_t p2;

	p1 = p->shift;
	p2 = p->line.p;
	sg_size_t thickness;

	thickness = map->o_thickness_fill;
	if( thickness == 0 ){
		thickness = 1;
	}


	//apply bitmap space rotation
	sg_point_map(&p1, map);
	sg_point_map(&p2, map);

	if( attr ){
		if( p1.x < attr->top_left.x ){ attr->top_left.x = p1.x; }
		if( p1.y < attr->top_left.y ){ attr->top_left.y = p1.y; }
		if( p2.x < attr->top_left.x ){ attr->top_left.x = p2.x; }
		if( p2.y < attr->top_left.y ){ attr->top_left.y = p2.y; }

		if( p1.x > attr->bottom_right.x ){ attr->bottom_right.x = p1.x; }
		if( p1.y > attr->bottom_right.y ){ attr->bottom_right.y = p1.y; }
		if( p2.x > attr->bottom_right.x ){ attr->bottom_right.x = p2.x; }
		if( p2.y > attr->bottom_right.y ){ attr->bottom_right.y = p2.y; }
	}

	//add the option to invert the line

	//now draw a line between the two points
	if( map->op == SG_OP_CLR ){
		sg_clr_line(bm, p1, p2, thickness);
	} else if( map->op == SG_OP_INV ){
		sg_inv_line(bm, p1, p2, thickness);
	} else {
		sg_set_line(bm, p1, p2, thickness);
	}
}

void draw_arc(const sg_icon_primitive_t * p, sg_bmap_t * bm, const sg_map_t * map, sg_bounds_t * attr){
	int i;
	int points;
	int step;
	int radians = p->arc.stop - p->arc.start;
	sg_int_t t;
	sg_int_t r;
	sg_size_t unit;
	sg_int_t thick;

	sg_size_t half_thick;
	sg_size_t thickness;


	thickness = map->o_thickness_fill;
	if( thickness == 0 ){
		thickness = 1;
	}

	half_thick = thickness/2;

	sg_point_t pen;
	sg_point_t circum;
	circum.point = 0;

	unit = sg_point_map_pixel_size(map);

	if( p->arc.rx < p->arc.ry ){
		r = p->arc.ry;
	} else {
		r = p->arc.rx;
	}

	r += (thickness-1)*unit;

	sg_point_shift_x(&circum, r);
	sg_point_map(&circum, map);

	points = ((circum.x - map->shift.x) * 2 * 320) / 100 * (radians) / SG_TRIG_POINTS;
	if( points > SG_TRIG_POINTS ){
		points = SG_TRIG_POINTS;
	}

	if( points == 0 ){
		points = 1;
	}

	if( radians == 0 ){
		step = 1;
	} else {
		step = points / (radians);
	}

	if( step == 0 ){
		step = 1;
	}

	//! \todo implement thickness
	for(t=0; t < thickness; t++){
		thick = unit * (t - half_thick);
		for(i=p->arc.start; i < p->arc.stop; i+=step){
			pen.point = 0;
			sg_point_arc(&pen, p->arc.rx + thick, p->arc.ry + thick, i);
			sg_point_rotate(&pen, p->rotation);
			sg_point_shift(&pen, p->shift);
			sg_point_map(&pen, map);

			if( attr ){
				if( pen.x < attr->top_left.x ){ attr->top_left.x = pen.x; }
				if( pen.y < attr->top_left.y ){ attr->top_left.y = pen.y; }
				if( pen.x > attr->bottom_right.x ){ attr->bottom_right.x = pen.x; }
				if( pen.y > attr->bottom_right.y ){ attr->bottom_right.y = pen.y; }
			}

			if( map->op == SG_OP_CLR ){
				sg_clr_pixel(bm, pen);
			} else {
				sg_set_pixel(bm, pen);
			}
		}
	}

}

void draw_fill(const sg_icon_primitive_t * p, sg_bmap_t * bm, const sg_map_t * map, sg_bounds_t * attr){
	sg_point_t center;
	center.point = p->shift.point;
	sg_point_map(&center, map);
	sg_pour(bm, center);
}




