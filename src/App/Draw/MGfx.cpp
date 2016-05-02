//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include "Draw/MGfx.hpp"
using namespace Draw;



void MGfxMap::set_dim(const MDim & dim){
	data()->size.h = dim.h();
	data()->size.w = dim.w();
}

void MGfxMap::set_shift(const MPoint & p){
	data()->shift.x = p.x();
	data()->shift.y = p.y();
}

void MGfxMap::set_dim(mg_size_t w, mg_size_t h){
	data()->size.w = w;
	data()->size.h = h;
}

void MGfxMap::set_shift(mg_int_t x, mg_int_t y){
	data()->shift.x = x;
	data()->shift.y = y;
}

MGfxMap::MGfxMap(const MBitmap * bitmap){
	data()->rotation = 0;
	data()->shift.x = bitmap->w()/2;
	data()->shift.y = bitmap->h()/2;
	data()->size.w = bitmap->w()*2;
	data()->size.h = bitmap->h()*2;
}

void MGfx::draw(MBitmap * bitmap, const mg_icon_t * icon, const mg_map_t & map, mg_attr_t * attr){
	mg_draw_icon(bitmap->bmap(), icon, &map, attr);
}

mg_t MGfx::line(int16_t x1, int16_t y1, int16_t x2, int16_t y2){
	mg_t ret;
	memset(&ret, 0, sizeof(ret));
	ret.type = MG_LINE | MG_ENABLE_FLAG;
	ret.shift.x = x1;
	ret.shift.y = y1;
	ret.line.p.x = x2;
	ret.line.p.y = y2;
	ret.rotation = 0;
	return ret;
}


mg_t MGfx::circle(int16_t x, int16_t y, uint16_t r){
	mg_t ret;
	memset(&ret, 0, sizeof(ret));
	ret.type = MG_ARC | MG_ENABLE_FLAG;
	ret.shift.x = x;
	ret.shift.y = y;
	ret.arc.rx = r;
	ret.arc.ry = r;
	ret.arc.start = 0;
	ret.arc.stop = MG_TRIG_POINTS;
	return ret;
}

mg_t MGfx::arc(int16_t x, int16_t y, mg_size_t rx, mg_size_t ry, int16_t start, int16_t stop, int16_t rotation){
	mg_t ret;
	memset(&ret, 0, sizeof(ret));
	ret.type = MG_ARC | MG_ENABLE_FLAG;
	ret.shift.x = x;
	ret.shift.y = y;
	ret.arc.rx = rx;
	ret.arc.ry = ry;
	ret.arc.start = start;
	ret.arc.stop = stop;
	ret.rotation = rotation;
	return ret;
}

mg_t MGfx::fill(int16_t x1, int16_t y1){
	mg_t ret;
	memset(&ret, 0, sizeof(ret));
	ret.type = MG_FILL | MG_ENABLE_FLAG;
	ret.shift.x = x1;
	ret.shift.y = y1;
	ret.rotation = 0;

	return ret;
}


void MGfx::show(const mg_t & mg){
	size_t i;
	const unsigned char * p = (const unsigned char *)(&mg);
	printf("{ ");
	for(i=0; i < sizeof(mg_t); i++){
		printf("0x%X", p[i]);
		if( i < (sizeof(mg_t)-1) ){
			printf(",");
		}
		printf(" ");
	}
	printf("}");
}

void MGfx::show(const mg_icon_t * icon){
	u8 i;
	for(i=0; i < icon->total; i++){
		show(icon->elements[i]);
		if( i < (icon->total-1) ){
			printf(",");
		}
		printf("\n");
	}
}





void MGfx::draw_remove(MBitmap * bitmap, const MGfxMap & map, bool show){

	int total = 2;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = line(-MG_MAX/2, -MG_MAX/2, MG_MAX/2, MG_MAX/2);
	objs[1] = line(-MG_MAX/2, MG_MAX/2, MG_MAX/2, -MG_MAX/2);

	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}
}

void MGfx::draw_ok(MBitmap * bitmap, const MGfxMap & map, bool show){
	int total = 2;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = line(-MG_MAX/2, MG_MAX/4, -MG_MAX/4, MG_MAX/2);
	objs[1] = line(-MG_MAX/4, MG_MAX/2, MG_MAX/2, -MG_MAX/4);

	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}
}

void MGfx::draw_bars(MBitmap * bitmap, const MGfxMap & map, bool show){
	int total = 3;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = MGfx::line(MG_MIN/2, MG_MIN/2, MG_MAX/2, MG_MIN/2);
	objs[1] = MGfx::line(MG_MIN/2, 0, MG_MAX/2, 0);
	objs[2] = MGfx::line(MG_MIN/2, MG_MAX/2, MG_MAX/2, MG_MAX/2);

	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}
}

void MGfx::draw_circle(MBitmap * bitmap, const MGfxMap & map, bool show){
	int total = 1;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = circle(0, 0, MG_MAX/2);
	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}
}

void MGfx::draw_circle_fill(MBitmap * bitmap, const MGfxMap & map, bool show){
	int total = 2;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = circle(0, 0, MG_MAX/2);
	objs[1] = fill(0, 0);
	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}
}

void MGfx::draw_toggle_off(MBitmap * bitmap, const MGfxMap & map, bool show){
	int total = 5;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = line(-MG_MAX/4, -MG_MAX/4, MG_MAX/4, -MG_MAX/4);
	objs[1] = line(-MG_MAX/4, MG_MAX/4, MG_MAX/4, MG_MAX/4);
	objs[2] = arc(-MG_MAX/4, 0, MG_MAX/4, MG_MAX/4, MG_TRIG_POINTS/4, MG_TRIG_POINTS*3/4);
	objs[3] = arc(MG_MAX/4, 0, MG_MAX/4, MG_MAX/4, MG_TRIG_POINTS*3/4, MG_TRIG_POINTS*5/4);
	objs[4] = circle(-MG_MAX/4, 0, MG_MAX/4);

	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}
}

void MGfx::draw_toggle_on(MBitmap * bitmap, const MGfxMap & map, bool show){
	int total = 6;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = line(-MG_MAX/4, -MG_MAX/4, MG_MAX/4, -MG_MAX/4);
	objs[1] = line(-MG_MAX/4, MG_MAX/4, MG_MAX/4, MG_MAX/4);
	objs[2] = arc(-MG_MAX/4, 0, MG_MAX/4, MG_MAX/4, MG_TRIG_POINTS/4, MG_TRIG_POINTS*3/4);
	objs[3] = arc(MG_MAX/4, 0, MG_MAX/4, MG_MAX/4, MG_TRIG_POINTS*3/4, MG_TRIG_POINTS*5/4);
	objs[4] = circle(MG_MAX/4, 0, MG_MAX/4);
	objs[5] = fill(-MG_MAX/4, 0);

	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}
}

void MGfx::draw_power(MBitmap * bitmap, const MGfxMap & map, bool show){
	int total = 2;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = line(0, 0, 0, -MG_MAX/2);
	objs[1] = arc(0, 0, MG_MAX/2, MG_MAX/2, MG_TRIG_POINTS*7/8, MG_TRIG_POINTS*7/8 + MG_TRIG_POINTS*3/4);

	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}
}

void MGfx::draw_chevron(MBitmap * bitmap, const MGfxMap & map, bool show){
	int total = 2;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = line(-MG_MAX/8, -MG_MAX/2, MG_MAX/8, 0);
	objs[1] = line(MG_MAX/8, 0, -MG_MAX/8, MG_MAX/2);

	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}
}

void MGfx::draw_arrow(MBitmap * bitmap, const MGfxMap & map, bool show){
	int total = 3;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = line(-MG_MAX/2, 0, MG_MAX/2, 0);
	objs[1] = line(0, MG_MAX/2, MG_MAX/2, 0);
	objs[2] = line(0, -MG_MAX/2, MG_MAX/2, 0);

	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}
}

void MGfx::draw_zoom(MBitmap * bitmap, const MGfxMap & map, bool show){
	int total = 2;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = circle(MG_MAX/6, -MG_MAX/6, MG_MAX*3/10);
	objs[1] = line(-MG_MAX/20, MG_MAX/20, -MG_MAX/2, MG_MAX/2);

	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}
}

void MGfx::draw_reset(MBitmap * bitmap, const MGfxMap & map, bool show){
	int total = 3;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = line(MG_MAX*3/10, 0, MG_MAX*5/10, -MG_MAX*2/20);
	objs[1] = line(MG_MAX*3/10, 0, MG_MAX*1/10, -MG_MAX*2/20);
	objs[2] = arc(-MG_MAX/40, 0, MG_MAX*3/10 + MG_MAX*3/40, MG_MAX*3/10 + MG_MAX*3/40, MG_TRIG_POINTS/12, MG_TRIG_POINTS);

	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}
}

void MGfx::draw_lightning(MBitmap * bitmap, const MGfxMap & map, bool show){
	int total = 3;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = line(0, -MG_MAX/2, -MG_MAX/4, MG_MAX/10);
	objs[1] = line(-MG_MAX/4, MG_MAX/10, MG_MAX/4, -MG_MAX/10);
	objs[2] = line(MG_MAX/4, -MG_MAX/10, 0, MG_MAX/2);

	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}
}

void MGfx::draw_play(MBitmap * bitmap, const MGfxMap & map, bool show){
	int total = 4;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = line(-MG_MAX/2, -MG_MAX/2, -MG_MAX/2, MG_MAX/2);
	objs[1] = line(-MG_MAX/2, MG_MAX/2, MG_MAX/2, 0);
	objs[2] = line(MG_MAX/2, 0, -MG_MAX/2, -MG_MAX/2);
	objs[3] = fill(0, 0);

	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}
}


void MGfx::draw_pause(MBitmap * bitmap, const MGfxMap & map, bool show){
	int total = 2;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = line(-MG_MAX/4, -MG_MAX/2, -MG_MAX/4, MG_MAX/2);
	objs[1] = line(MG_MAX/4, -MG_MAX/2, MG_MAX/4, MG_MAX/2);

	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}
}

void MGfx::draw_button_bar(MBitmap * bitmap, const MGfxMap & map, bool show){
	int total = 7;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;
	objs[0] = MGfx::line(MG_MIN/2, 0, MG_MAX/2, 0);
	objs[1] = MGfx::line(MG_MIN/2+MG_MAX/32, MG_MAX/4, MG_MAX/2-MG_MAX/32, MG_MAX/4);
	objs[2] = MGfx::line(MG_MIN/2, 0, MG_MIN/2, MG_MAX/4 - MG_MAX/32);
	objs[3] = MGfx::line(MG_MAX/2, 0, MG_MAX/2, MG_MAX/4 - MG_MAX/32);
	objs[4] = MGfx::arc(MG_MAX/2-MG_MAX/32, MG_MAX/4-MG_MAX/32, MG_MAX/32, MG_MAX/32, 0, MG_TRIG_POINTS/4);
	objs[5] = MGfx::arc(MG_MIN/2+ MG_MAX/32, MG_MAX/4-MG_MAX/32, MG_MAX/32, MG_MAX/32, MG_TRIG_POINTS/4, MG_TRIG_POINTS*2/4);
	objs[6] = MGfx::fill(0, MG_MAX/8);

	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}
}

void MGfx::draw_mic(MBitmap * bitmap, const MGfxMap & map, bool show){
	int total = 8;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;


	objs[0] = MGfx::arc(0, -MG_MAX/6, MG_MAX/6, MG_MAX/6, MG_TRIG_POINTS/2, MG_TRIG_POINTS);
	objs[1] = MGfx::line(-MG_MAX/6, -MG_MAX/6, -MG_MAX/6, 0);
	objs[2] = MGfx::line(MG_MAX/6, -MG_MAX/6, MG_MAX/6, 0);
	objs[3] = MGfx::arc(0, 0, MG_MAX/6, MG_MAX/6, 0, MG_TRIG_POINTS/2);
	objs[4] = MGfx::arc(0, 0, MG_MAX/3, MG_MAX/3, 0, MG_TRIG_POINTS/2);
	objs[5] = MGfx::line(0, MG_MAX/3, 0, MG_MAX/2);
	objs[6] = MGfx::line(-MG_MAX/5, MG_MAX/2, MG_MAX/5, MG_MAX/2);
	objs[7] = MGfx::fill(0, -MG_MAX/8);

	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}
}

void MGfx::draw_clock(MBitmap * bitmap, const MGfxMap & map, bool show){
	int total = 3;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;


	objs[0] = MGfx::circle(0, 0, MG_MAX/2);
	objs[1] = MGfx::line(0, 0, MG_MAX/4, 0);
	objs[2] = MGfx::line(0, 0, 0, -MG_MAX/3);

	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}
}

void MGfx::draw_heart(MBitmap * bitmap, const MGfxMap & map, bool show){
	int total = 5;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;


	objs[0] = MGfx::arc(-MG_MAX/4, 0, MG_MAX/4, MG_MAX/4, MG_TRIG_POINTS*3/8, MG_TRIG_POINTS);
	objs[1] = MGfx::arc(MG_MAX/4, 0, MG_MAX/4, MG_MAX/4, MG_TRIG_POINTS/2, MG_TRIG_POINTS + MG_TRIG_POINTS/8);
	objs[2] = MGfx::line(-MG_MAX/2 + MG_MAX*1/20, MG_MAX*3/10 - MG_MAX*3/20, 0, MG_MAX/2);
	objs[3] = MGfx::line(MG_MAX/2 - MG_MAX*1/20, MG_MAX*3/10 - MG_MAX*3/20, 0, MG_MAX/2);
	objs[4] = MGfx::fill(MG_MAX/4, 0);

	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}
}

void MGfx::draw_plot(MBitmap * bitmap, const MGfxMap & map, bool show){
	int total = 6;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;


	objs[0] = MGfx::line(-MG_MAX/2, -MG_MAX/2, -MG_MAX/2, MG_MAX/2);
	objs[1] = MGfx::line(-MG_MAX/2, MG_MAX/2, MG_MAX/2, MG_MAX/2);
	objs[2] = MGfx::line(-MG_MAX/2, MG_MAX/2, -MG_MAX/4, -MG_MAX/3);
	objs[3] = MGfx::line(-MG_MAX/4, -MG_MAX/3, 0, 0);
	objs[4] = MGfx::line(0, 0, MG_MAX/4, -MG_MAX/4);
	objs[5] = MGfx::line(MG_MAX/4, -MG_MAX/4, MG_MAX/2, MG_MAX/4);

	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}
}

void MGfx::draw_bike(MBitmap * bitmap, const MGfxMap & map, bool show){
	int total = 11;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;

	objs[0] = MGfx::circle(-MG_MAX/4, 0, MG_MAX/6);
	objs[1] = MGfx::circle(MG_MAX/4, 0, MG_MAX/6);
	objs[2] = MGfx::line(-MG_MAX/4, 0, 0, 0);
	objs[3] = MGfx::line(-MG_MAX/4, 0, -MG_MAX/8, -MG_MAX/4);
	objs[4] = MGfx::line(0, 0, MG_MAX*1/8, -MG_MAX/4);
	objs[5] = MGfx::line(-MG_MAX/8, -MG_MAX/4, MG_MAX*1/8, -MG_MAX/4);
	objs[6] = MGfx::line(MG_MAX/4, 0, MG_MAX*1/8, -MG_MAX/4);
	objs[7] = MGfx::line(MG_MAX*1/8, -MG_MAX*3/8, MG_MAX*1/8, -MG_MAX/4);
	objs[8] = MGfx::line(-MG_MAX*3/16, -MG_MAX*5/16, -MG_MAX*1/8, -MG_MAX/4);
	objs[9] = MGfx::line(MG_MAX*1/16, -MG_MAX*3/8, MG_MAX*3/16, -MG_MAX*3/8);
	objs[10] = MGfx::line(-MG_MAX*4/16, -MG_MAX*5/16, -MG_MAX*2/16, -MG_MAX*5/16);

	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}
}

void MGfx::draw_mountain(MBitmap * bitmap, const MGfxMap & map, bool show){
	int total = 6;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;

	objs[0] = MGfx::line(-MG_MAX/2, MG_MAX/2, -MG_MAX/4, -MG_MAX/3);
	objs[1] = MGfx::line(-MG_MAX/4, -MG_MAX/3, 0, 0);
	objs[2] = MGfx::line(0, 0, MG_MAX/4, -MG_MAX/4);
	objs[3] = MGfx::line(MG_MAX/4, -MG_MAX/4, MG_MAX/2, MG_MAX/2);
	objs[4] = MGfx::line(-MG_MAX/2, MG_MAX/2, MG_MAX/2, MG_MAX/2);
	objs[5] = MGfx::fill(MG_MAX/8, 0);

	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}
}

void MGfx::draw_compass(MBitmap * bitmap, const MGfxMap & map, bool show){
	int total = 6;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;


	objs[0] = MGfx::line(-MG_MAX/8, 0, MG_MAX/8, 0);
	objs[1] = MGfx::line(-MG_MAX/8, 0, 0, -MG_MAX/4);
	objs[2] = MGfx::line(MG_MAX/8, 0, 0, -MG_MAX/4);
	objs[3] = MGfx::line(-MG_MAX/8, 0, 0, MG_MAX/4);
	objs[4] = MGfx::line(MG_MAX/8, 0, 0, MG_MAX/4);
	objs[5] = MGfx::fill(0, -MG_MAX/8);

	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}
}

void MGfx::draw_compass_outer(MBitmap * bitmap, const MGfxMap & map, bool show){
	int total = 9;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;


	objs[0] = MGfx::circle(0, 0, MG_MAX*4/10);
	objs[1] = MGfx::line(MG_MAX*3/10, 0, MG_MAX*5/10, 0);
	objs[2] = MGfx::line(-MG_MAX*3/10, 0, -MG_MAX*5/10, 0);
	objs[3] = MGfx::line(0, -MG_MAX*3/10, 0, -MG_MAX*5/10);
	objs[4] = MGfx::line(0, MG_MAX*3/10, 0, MG_MAX*5/10);
	objs[5] = MGfx::line(MG_MAX*5/20, MG_MAX*5/20, MG_MAX*7/20, MG_MAX*7/20);
	objs[6] = MGfx::line(-MG_MAX*5/20, MG_MAX*5/20, -MG_MAX*7/20, MG_MAX*7/20);
	objs[7] = MGfx::line(MG_MAX*5/20, -MG_MAX*5/20, MG_MAX*7/20, -MG_MAX*7/20);
	objs[8] = MGfx::line(-MG_MAX*5/20, -MG_MAX*5/20, -MG_MAX*7/20, -MG_MAX*7/20);

	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}
}

void MGfx::draw_panel(MBitmap * bitmap, const MGfxMap & map, bool show){
	int total = 8;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;


	objs[0] = MGfx::arc(-MG_MAX/2+MG_MAX/16, -MG_MAX/2+MG_MAX/16, MG_MAX/16, MG_MAX/16, MG_TRIG_POINTS/2, MG_TRIG_POINTS*3/4);
	objs[1] = MGfx::arc(-MG_MAX/2+MG_MAX/16, MG_MAX/2-MG_MAX/16, MG_MAX/16, MG_MAX/16, MG_TRIG_POINTS/4, MG_TRIG_POINTS/2);
	objs[2] = MGfx::arc(MG_MAX/2-MG_MAX/16, -MG_MAX/2+MG_MAX/16, MG_MAX/16, MG_MAX/16, MG_TRIG_POINTS*3/4, MG_TRIG_POINTS);
	objs[3] = MGfx::arc(MG_MAX/2-MG_MAX/16, MG_MAX/2-MG_MAX/16, MG_MAX/16, MG_MAX/16, 0, MG_TRIG_POINTS/4);
	objs[4] = MGfx::line(-MG_MAX/2+MG_MAX/16, -MG_MAX/2, MG_MAX/2-MG_MAX/16, -MG_MAX/2);
	objs[5] = MGfx::line(-MG_MAX/2+MG_MAX/16, MG_MAX/2, MG_MAX/2-MG_MAX/16, MG_MAX/2);
	objs[6] = MGfx::line(-MG_MAX/2, -MG_MAX/4+MG_MAX/16, -MG_MAX/2, MG_MAX/4-MG_MAX/16);
	objs[7] = MGfx::line(MG_MAX/2, -MG_MAX/4+MG_MAX/16, MG_MAX/2, MG_MAX/4-MG_MAX/16);

	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}
}

void MGfx::draw_panel_fill(MBitmap * bitmap, const MGfxMap & map, bool show){
	int total = 9;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;


	objs[0] = MGfx::arc(-MG_MAX/2+MG_MAX/16, -MG_MAX/2+MG_MAX/16, MG_MAX/16, MG_MAX/16, MG_TRIG_POINTS/2, MG_TRIG_POINTS*3/4);
	objs[1] = MGfx::arc(-MG_MAX/2+MG_MAX/16, MG_MAX/2-MG_MAX/16, MG_MAX/16, MG_MAX/16, MG_TRIG_POINTS/4, MG_TRIG_POINTS/2);
	objs[2] = MGfx::arc(MG_MAX/2-MG_MAX/16, -MG_MAX/2+MG_MAX/16, MG_MAX/16, MG_MAX/16, MG_TRIG_POINTS*3/4, MG_TRIG_POINTS);
	objs[3] = MGfx::arc(MG_MAX/2-MG_MAX/16, MG_MAX/2-MG_MAX/16, MG_MAX/16, MG_MAX/16, 0, MG_TRIG_POINTS/4);
	objs[4] = MGfx::line(-MG_MAX/2+MG_MAX/16, -MG_MAX/2, MG_MAX/2-MG_MAX/16, -MG_MAX/2);
	objs[5] = MGfx::line(-MG_MAX/2+MG_MAX/16, MG_MAX/2, MG_MAX/2-MG_MAX/16, MG_MAX/2);
	objs[6] = MGfx::line(-MG_MAX/2, -MG_MAX/4+MG_MAX/16, -MG_MAX/2, MG_MAX/4-MG_MAX/16);
	objs[7] = MGfx::line(MG_MAX/2, -MG_MAX/4+MG_MAX/16, MG_MAX/2, MG_MAX/4-MG_MAX/16);
	objs[8] = MGfx::fill(0, 0);

	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}
}

void MGfx::draw_sun(MBitmap * bitmap, const MGfxMap & map, bool show){
	int total = 9;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;


	objs[0] = MGfx::circle(0, 0, MG_MAX*2/10);
	objs[1] = MGfx::line(MG_MAX*3/10, 0, MG_MAX*5/10, 0);
	objs[2] = MGfx::line(-MG_MAX*3/10, 0, -MG_MAX*5/10, 0);
	objs[3] = MGfx::line(0, -MG_MAX*3/10, 0, -MG_MAX*5/10);
	objs[4] = MGfx::line(0, MG_MAX*3/10, 0, MG_MAX*5/10);
	objs[5] = MGfx::line(MG_MAX*5/20, MG_MAX*5/20, MG_MAX*7/20, MG_MAX*7/20);
	objs[6] = MGfx::line(-MG_MAX*5/20, MG_MAX*5/20, -MG_MAX*7/20, MG_MAX*7/20);
	objs[7] = MGfx::line(MG_MAX*5/20, -MG_MAX*5/20, MG_MAX*7/20, -MG_MAX*7/20);
	objs[8] = MGfx::line(-MG_MAX*5/20, -MG_MAX*5/20, -MG_MAX*7/20, -MG_MAX*7/20);

	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}
}

void MGfx::draw_battery(MBitmap * bitmap, const MGfxMap & map, bool show){
	int total = 7;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;


	objs[0] = MGfx::line(-MG_MAX/2, -MG_MAX/4, -MG_MAX/2, MG_MAX/4);
	objs[1] = MGfx::line(-MG_MAX/2, -MG_MAX/4, MG_MAX*4/10, -MG_MAX/4);
	objs[2] = MGfx::line(-MG_MAX/2, MG_MAX/4, MG_MAX*4/10, MG_MAX/4);
	objs[3] = MGfx::line(MG_MAX*4/10, -MG_MAX/4, MG_MAX*4/10, MG_MAX/4);
	objs[4] = MGfx::line(MG_MAX*4/10, MG_MAX*1/10, MG_MAX*5/10, MG_MAX*1/10);
	objs[5] = MGfx::line(MG_MAX*4/10, -MG_MAX*1/10, MG_MAX*5/10, -MG_MAX*1/10);
	objs[6] = MGfx::line(MG_MAX*5/10, -MG_MAX*1/10, MG_MAX*5/10, MG_MAX*1/10);

	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}
}

void MGfx::draw_arrowhead(MBitmap * bitmap, const MGfxMap & map, bool show){
	int total = 4;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;


	objs[0] = MGfx::line(0, -MG_MAX/2, MG_MAX/4, MG_MAX/2);
	objs[1] = MGfx::line(0, -MG_MAX/2, -MG_MAX/4, MG_MAX/2);
	objs[2] = MGfx::line(MG_MAX/4, MG_MAX/2, 0, MG_MAX/4);
	objs[3] = MGfx::line(-MG_MAX/4, MG_MAX/2, 0, MG_MAX/4);

	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}
}

void MGfx::draw_dot(MBitmap * bitmap, const MGfxMap & map, bool show){
	int total = 1;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;

	objs[0] = MGfx::circle(0, 0, MG_MAX*1/10);

	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}
}

void MGfx::draw_dash(MBitmap * bitmap, const MGfxMap & map, bool show){
	int total = 1;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;

	objs[0] = MGfx::line(-MG_MAX/4, 0, MG_MAX/4, 0);

	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}

}

void MGfx::draw_message(MBitmap * bitmap, const MGfxMap & map, bool show){
	int total = 6;
	mg_t objs[total];
	mg_icon_t icon;
	icon.elements = objs;
	icon.total = total;
	icon.fill_total = 0;

	objs[0] = MGfx::line(-MG_MAX/2, -MG_MAX/4, MG_MAX/2, -MG_MAX/4);
	objs[1] = MGfx::line(-MG_MAX/2, MG_MAX/4, MG_MAX/2, MG_MAX/4);
	objs[2] = MGfx::line(MG_MAX/2, -MG_MAX/4, MG_MAX/2, MG_MAX/4);
	objs[3] = MGfx::line(-MG_MAX/2, -MG_MAX/4, -MG_MAX/2, MG_MAX/4);
	objs[4] = MGfx::line(-MG_MAX/2, -MG_MAX/4, 0, 0);
	objs[5] = MGfx::line(0, 0, MG_MAX/2, -MG_MAX/4);

	draw(bitmap, &icon, map.item());
	if( show ){
		MGfx::show(&icon);
	}

}

void MGfx::draw_ALPHA(MBitmap * bitmap, const MGfxMap & map, bool show){

	mg_t objs[6];
	mg_icon_t icon;
	icon.elements = objs;
	icon.fill_total = 0;

	//A
	icon.total = 3;
	objs[0] = MGfx::line(-MG_MAX/4, MG_MAX/2, 0, -MG_MAX/2);
	objs[1] = MGfx::line(MG_MAX/4, MG_MAX/2, 0, -MG_MAX/2);
	objs[2] = MGfx::line(-MG_MAX/8, 0, MG_MAX/8, 0);
	//draw(bitmap, &icon, map.item());
	if( show ){ MGfx::show(&icon); }

	//B
	icon.total = 6;

	//C
	icon.total = 3;
	objs[0] = MGfx::line(-MG_MAX/4, -MG_MAX/4, -MG_MAX/4, MG_MAX/4);
	objs[1] = MGfx::arc(0,MG_MAX/4,MG_MAX/4,0,0,MG_TRIG_POINTS/2);
	objs[2] = MGfx::arc(0,-MG_MAX/4,MG_MAX/4,MG_MAX/4,MG_TRIG_POINTS/2,MG_TRIG_POINTS);
	//draw(bitmap, &icon, map.item());
	if( show ){ MGfx::show(&icon); }

	//D
	icon.total = 2;

	//E
	icon.total = 4;
	objs[0] = MGfx::line(-MG_MAX/4, MG_MAX/2, MG_MAX/4, MG_MAX/2);
	objs[1] = MGfx::line(-MG_MAX/4, 0, 0, 0);
	objs[2] = MGfx::line(-MG_MAX/4, -MG_MAX/2, MG_MAX/4, -MG_MAX/2);
	objs[3] = MGfx::line(-MG_MAX/4, -MG_MAX/2, -MG_MAX/4, MG_MAX/2);
	//draw(bitmap, &icon, map.item());
	if( show ){ MGfx::show(&icon); }

	//F
	icon.total = 3;
	objs[0] = MGfx::line(-MG_MAX/4, -MG_MAX/2, MG_MAX/4, -MG_MAX/2);
	objs[1] = MGfx::line(-MG_MAX/4, 0, 0, 0);
	objs[2] = MGfx::line(-MG_MAX/4, -MG_MAX/2, -MG_MAX/4, MG_MAX/2);
	//draw(bitmap, &icon, map.item());
	if( show ){ MGfx::show(&icon); }

	//G
	icon.total = 5;

	//H
	icon.total = 3;
	objs[0] = MGfx::line(-MG_MAX/4, 0, MG_MAX/4, 0);
	objs[1] = MGfx::line(MG_MAX/4, -MG_MAX/2, MG_MAX/4, MG_MAX/2);
	objs[2] = MGfx::line(-MG_MAX/4, -MG_MAX/2, -MG_MAX/4, MG_MAX/2);
	//draw(bitmap, &icon, map.item());
	if( show ){ MGfx::show(&icon); }

	//I
	icon.total = 3;
	objs[0] = MGfx::line(-MG_MAX/4, -MG_MAX/2, MG_MAX/4, -MG_MAX/2);
	objs[1] = MGfx::line(-MG_MAX/4, MG_MAX/2, MG_MAX/4, MG_MAX/2);
	objs[2] = MGfx::line(0, -MG_MAX/2, 0, MG_MAX/2);
	//draw(bitmap, &icon, map.item());
	if( show ){ MGfx::show(&icon); }

	//J
	icon.total = 2;

	//K
	icon.total = 3;
	objs[0] = MGfx::line(-MG_MAX/4, -MG_MAX/2, -MG_MAX/4, MG_MAX/2);
	objs[1] = MGfx::line(-MG_MAX/4, 0, MG_MAX/4, MG_MAX/2);
	objs[2] = MGfx::line(-MG_MAX/4, 0, MG_MAX/4, -MG_MAX/2);
	//draw(bitmap, &icon, map.item());
	if( show ){ MGfx::show(&icon); }

	//L
	icon.total = 2;
	objs[0] = MGfx::line(-MG_MAX/4, -MG_MAX/2, -MG_MAX/4, MG_MAX/2);
	objs[1] = MGfx::line(-MG_MAX/4, MG_MAX/2, MG_MAX/4, MG_MAX/2);
	//draw(bitmap, &icon, map.item());
	if( show ){ MGfx::show(&icon); }

	//M
	icon.total = 4;
	objs[0] = MGfx::line(-MG_MAX/4, MG_MAX/2, -MG_MAX/4, -MG_MAX/2);
	objs[1] = MGfx::line(MG_MAX/4, MG_MAX/2, MG_MAX/4, -MG_MAX/2);
	objs[2] = MGfx::line(-MG_MAX/4, -MG_MAX/2, 0, MG_MAX/4);
	objs[3] = MGfx::line(MG_MAX/4, -MG_MAX/2, 0, MG_MAX/4);
	//draw(bitmap, &icon, map.item());
	if( show ){ MGfx::show(&icon); }

	//N
	icon.total = 3;
	objs[0] = MGfx::line(-MG_MAX/4, MG_MAX/2, -MG_MAX/4, -MG_MAX/2);
	objs[1] = MGfx::line(MG_MAX/4, MG_MAX/2, MG_MAX/4, -MG_MAX/2);
	objs[2] = MGfx::line(-MG_MAX/4, -MG_MAX/2, MG_MAX/4, MG_MAX/2);
	//draw(bitmap, &icon, map.item());
	if( show ){ MGfx::show(&icon); }

	//O
	icon.total = 4;
	objs[0] = MGfx::line(-MG_MAX/4, -MG_MAX/4, -MG_MAX/4, MG_MAX/4);
	objs[1] = MGfx::line(MG_MAX/4, -MG_MAX/4, MG_MAX/4, MG_MAX/4);
	objs[2] = MGfx::arc(0,MG_MAX/4,MG_MAX/4,MG_MAX/4,0,MG_TRIG_POINTS/2);
	objs[3] = MGfx::arc(0,-MG_MAX/4,MG_MAX/4,MG_MAX/4,MG_TRIG_POINTS/2,MG_TRIG_POINTS);
	//draw(bitmap, &icon, map.item());
	if( show ){ MGfx::show(&icon); }

	//P
	icon.total = 4;
	objs[0] = MGfx::line(-MG_MAX/4, -MG_MAX/2, -MG_MAX/4, MG_MAX/2);
	objs[1] = MGfx::line(-MG_MAX/4, -MG_MAX/2, 0, -MG_MAX/2);
	objs[2] = MGfx::line(-MG_MAX/4, 0, 0, 0);
	objs[3] = MGfx::arc(0,-MG_MAX/4,MG_MAX/4,MG_MAX/4,MG_TRIG_POINTS*3/4,MG_TRIG_POINTS*5/4);
	//draw(bitmap, &icon, map.item());
	if( show ){ MGfx::show(&icon); }

	//Q
	icon.total = 5;
	objs[0] = MGfx::line(-MG_MAX/4, -MG_MAX/4, -MG_MAX/4, MG_MAX/4);
	objs[1] = MGfx::line(MG_MAX/4, -MG_MAX/4, MG_MAX/4, MG_MAX/4);
	objs[2] = MGfx::arc(0,MG_MAX/4,MG_MAX/4,MG_MAX/4,0,MG_TRIG_POINTS/2);
	objs[3] = MGfx::arc(0,-MG_MAX/4,MG_MAX/4,MG_MAX/4,MG_TRIG_POINTS/2,MG_TRIG_POINTS);
	objs[4] = MGfx::line(MG_MAX/4, MG_MAX/4, MG_MAX/4, MG_MAX/2);
	//draw(bitmap, &icon, map.item());
	if( show ){ MGfx::show(&icon); }

	//R
	icon.total = 5;
	objs[0] = MGfx::line(-MG_MAX/4, -MG_MAX/2, -MG_MAX/4, MG_MAX/2);
	objs[1] = MGfx::line(-MG_MAX/4, -MG_MAX/2, 0, -MG_MAX/2);
	objs[2] = MGfx::line(-MG_MAX/4, 0, 0, 0);
	objs[3] = MGfx::arc(0,-MG_MAX/4,MG_MAX/4,MG_MAX/4,MG_TRIG_POINTS*3/4,MG_TRIG_POINTS*5/4);
	objs[4] = MGfx::line(0, 0, MG_MAX/4, MG_MAX/2);
	//draw(bitmap, &icon, map.item());
	if( show ){ MGfx::show(&icon); }


	//S
	icon.total = 2;
	objs[0] = MGfx::arc(0,-MG_MAX/4,MG_MAX/4,MG_MAX/4,MG_TRIG_POINTS/4,MG_TRIG_POINTS);
	objs[1] = MGfx::arc(0,MG_MAX/4,MG_MAX/4,MG_MAX/4,MG_TRIG_POINTS*3/4,MG_TRIG_POINTS*3/2);
	draw(bitmap, &icon, map.item());
	if( show ){ MGfx::show(&icon); }

	return;

	//T
	icon.total = 2;
	objs[0] = MGfx::line(-MG_MAX/4, MG_MAX/2, MG_MAX/4, MG_MAX/2);
	objs[1] = MGfx::line(0, MG_MAX/2, 0, -MG_MAX/2);
	draw(bitmap, &icon, map.item());
	if( show ){ MGfx::show(&icon); }

	//U
	icon.total = 3;
	objs[0] = MGfx::line(-MG_MAX/4, -MG_MAX/4, -MG_MAX/4, MG_MAX/2);
	objs[1] = MGfx::line(MG_MAX/4, -MG_MAX/4, MG_MAX/4, MG_MAX/2);
	objs[2] = MGfx::arc(0,-MG_MAX/4,0,0,MG_TRIG_POINTS/2,MG_TRIG_POINTS);
	draw(bitmap, &icon, map.item());
	if( show ){ MGfx::show(&icon); }

	//V
	icon.total = 2;
	objs[0] = MGfx::line(-MG_MAX/4, MG_MAX/2, 0, -MG_MAX/2);
	objs[1] = MGfx::line(MG_MAX/4, MG_MAX/2, 0, -MG_MAX/2);
	draw(bitmap, &icon, map.item());
	if( show ){ MGfx::show(&icon); }

	//W
	icon.total = 4;
	objs[0] = MGfx::line(-MG_MAX/4, -MG_MAX/2, -MG_MAX/4, MG_MAX/2);
	objs[1] = MGfx::line(MG_MAX/4, -MG_MAX/2, MG_MAX/4, MG_MAX/2);
	objs[2] = MGfx::line(-MG_MAX/4, -MG_MAX/2, 0, MG_MAX/4);
	objs[3] = MGfx::line(MG_MAX/4, -MG_MAX/2, 0, MG_MAX/4);
	draw(bitmap, &icon, map.item());
	if( show ){ MGfx::show(&icon); }

	//X
	icon.total = 2;
	objs[0] = MGfx::line(-MG_MAX/4, -MG_MAX/2, MG_MAX/4, MG_MAX/2);
	objs[1] = MGfx::line(-MG_MAX/4, MG_MAX/2, MG_MAX/4, -MG_MAX/2);
	draw(bitmap, &icon, map.item());
	if( show ){ MGfx::show(&icon); }

	//Y
	icon.total = 3;
	objs[0] = MGfx::line(-MG_MAX/4, -MG_MAX/2, 0, 0);
	objs[1] = MGfx::line(MG_MAX/4, MG_MAX/2, 0, 0);
	objs[2] = MGfx::line(0, 0, 0, -MG_MAX/2);
	draw(bitmap, &icon, map.item());
	if( show ){ MGfx::show(&icon); }

	//Z
	icon.total= 3;
	objs[0] = MGfx::line(-MG_MAX/4, -MG_MAX/2, MG_MAX/4, -MG_MAX/2);
	objs[1] = MGfx::line(-MG_MAX/4, MG_MAX/2, MG_MAX/4, MG_MAX/2);
	objs[2] = MGfx::line(-MG_MAX/4, -MG_MAX/2, MG_MAX/4, MG_MAX/2);
	draw(bitmap, &icon, map.item());
	if( show ){ MGfx::show(&icon); }
}







