
#include "mg.h"


static void draw_line(const mg_t * p, mg_bmap_t * bm, const mg_map_t * map, mg_attr_t * attr);
static void draw_arc(const mg_t * p, mg_bmap_t * bm, const mg_map_t * map, mg_attr_t * attr);
static void draw_fill(const mg_t * p, mg_bmap_t * bm, const mg_map_t * map, mg_attr_t * attr);

static void (*draw_func [MG_TYPE_TOTAL])(const mg_t * p, mg_bmap_t * bm, const mg_map_t * map, mg_attr_t * attr) = {
		draw_line,
		draw_arc,
		draw_fill
};


void mg_draw(mg_bmap_t * bitmap, const mg_t * prim, const mg_map_t * map, mg_attr_t * attr){
	int type;
	type = prim->type & MG_TYPE_MASK;
	if( prim->type & MG_ENABLE_FLAG ){
		if( (type & MG_TYPE_MASK) < MG_TYPE_TOTAL ){
			draw_func[type](prim, bitmap, map, attr);
		}
	}
}

void mg_draw_icon(mg_bmap_t * bitmap, const mg_icon_t * icon, const mg_map_t * map, mg_attr_t * attr){
	unsigned int total;
	if( map->thickness_fill == 0 ){
		total = icon->total;
	} else {
		total = icon->total - icon->fill_total;
	}
	mg_draw_list(bitmap, icon->elements, total, map, attr);
}

void mg_draw_list(mg_bmap_t * bitmap, const mg_t prim_list[], unsigned int total, const mg_map_t * map, mg_attr_t * attr){
	unsigned int i;

	if( attr ){
		attr->bottom_right.x = -MG_MAX;
		attr->bottom_right.y = -MG_MAX;
		attr->top_left.x = MG_MAX;
		attr->top_left.y = MG_MAX;
	}

	for(i=0; i < total; i++){
		mg_draw(bitmap, &(prim_list[i]), map, attr);
	}
}


void draw_line(const mg_t * p, mg_bmap_t * bm, const mg_map_t * map, mg_attr_t * attr){
	//draw a line from bottom left to top right
	mg_point_t p1;
	mg_point_t p2;

	p1 = p->shift;
	p2 = p->line.p;
	mg_size_t thickness;

	thickness = map->thickness_fill;
	if( thickness == 0 ){
		thickness = 1;
	}


	//apply bitmap space rotation
	mg_point_map(&p1, map);
	mg_point_map(&p2, map);

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
	if( map->op == MG_OP_CLR ){
		mg_clr_line(bm, p1, p2, thickness);
	} else if( map->op == MG_OP_INV ){
		mg_inv_line(bm, p1, p2, thickness);
	} else {
		mg_set_line(bm, p1, p2, thickness);
	}
}

void draw_arc(const mg_t * p, mg_bmap_t * bm, const mg_map_t * map, mg_attr_t * attr){
	int i;
	int points;
	int step;
	int radians = p->arc.stop - p->arc.start;
	mg_int_t t;
	mg_int_t r;
	mg_size_t unit;
	mg_int_t thick;

	mg_size_t half_thick;
	mg_size_t thickness;


	thickness = map->thickness_fill;
	if( thickness == 0 ){
		thickness = 1;
	}

	half_thick = thickness/2;

	mg_point_t pen;
	mg_point_t circum;
	circum.point = 0;

	unit = mg_point_map_pixel_size(map);

	if( p->arc.rx < p->arc.ry ){
		r = p->arc.ry;
	} else {
		r = p->arc.rx;
	}

	r += (thickness-1)*unit;

	mg_point_shift_x(&circum, r);
	mg_point_map(&circum, map);

	points = ((circum.x - map->shift.x) * 2 * 320) / 100 * (radians) / MG_TRIG_POINTS;
	if( points > MG_TRIG_POINTS ){
		points = MG_TRIG_POINTS;
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
			mg_point_arc(&pen, p->arc.rx + thick, p->arc.ry + thick, i);
			mg_point_rotate(&pen, p->rotation);
			mg_point_shift(&pen, p->shift);
			mg_point_map(&pen, map);

			if( attr ){
				if( pen.x < attr->top_left.x ){ attr->top_left.x = pen.x; }
				if( pen.y < attr->top_left.y ){ attr->top_left.y = pen.y; }
				if( pen.x > attr->bottom_right.x ){ attr->bottom_right.x = pen.x; }
				if( pen.y > attr->bottom_right.y ){ attr->bottom_right.y = pen.y; }
			}

			if( map->op == MG_OP_CLR ){
				mg_clr_pixel(bm, pen);
			} else {
				mg_set_pixel(bm, pen);
			}
		}
	}

}

void draw_fill(const mg_t * p, mg_bmap_t * bm, const mg_map_t * map, mg_attr_t * attr){
	mg_point_t center;
	center.point = p->shift.point;
	mg_point_map(&center, map);
	mg_pour(bm, center);
}




