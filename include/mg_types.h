/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef MG_TYPES_H_
#define MG_TYPES_H_

#include <mcu/types.h>
#include <stdint.h>
#include <sys/types.h>

#define MG_MAX (32767)
#define MG_MIN (-32767)
#define MG_TRIG_POINTS 512

#define MG_MAP_MAX (MG_MAX/2)


#define MG_CENTER 0
#define MG_LEFT (-MG_MAP_MAX)
#define MG_RIGHT (MG_MAP_MAX)
#define MG_TOP (-MG_MAP_MAX)
#define MG_BOT (MG_MAP_MAX)

#define MG_ANIMATION_STEP_FLAG (1<<15)



enum {
	MG_LINE,
	MG_ARC,
	MG_FILL,
	MG_TYPE_TOTAL
};

enum {
	MG_OP_SET,
	MG_OP_CLR,
	MG_OP_INV
};

#define MG_TYPE_MASK (0xFF)
#define MG_ENABLE_FLAG (1<<15)
#define MG_FILL_FLAG (1<<14)


typedef i16 mg_int_t;
typedef u16 mg_uint_t;
typedef mg_uint_t mg_size_t;
typedef u8 mg_bitmap_t;
typedef u32 mg_uni_t;

typedef union MCU_PACK {
	struct {
		mg_int_t x;
		mg_int_t y;
	};
	mg_uni_t point;
} mg_point_t;

typedef union MCU_PACK {
	struct {
		mg_size_t w;
		mg_size_t h;
	};
	mg_uni_t dim;
} mg_dim_t;


typedef struct MCU_PACK {
	mg_bitmap_t * data;
	mg_dim_t dim;
	mg_dim_t margin_top_left;
	mg_dim_t margin_bottom_right;
	mg_size_t columns;
} mg_bmap_t;


typedef struct MCU_PACK {
	mg_point_t p[2];
} mg_triangle_t;

typedef struct MCU_PACK {
	mg_size_t rx; //x radius
	mg_size_t ry; //y radius
	i16 start, stop; //start/stop angles
} mg_arc_t;

typedef struct MCU_PACK {
	//list of points
	mg_size_t r; //inner/outer radii
} mg_circle_t;

typedef struct MCU_PACK {
	mg_point_t p;
} mg_line_t;

typedef struct MCU_PACK {
	mg_dim_t dim;
} mg_rect_t;

typedef struct MCU_PACK {
	mg_size_t w;
	mg_size_t h;
	size_t size;
	//this must be 4 byte aligned
} mg_bitmap_hdr_t;

typedef struct MCU_PACK {
	u16 type; //type of primitive object
	i16 rotation; //vector space rotation
	mg_point_t shift;
	union {
		mg_arc_t arc;
		mg_triangle_t triangle;
		mg_line_t line;
		mg_rect_t rect;
		mg_circle_t circle;
	};
} mg_t;

typedef struct MCU_PACK {
	mg_point_t top_left;
	mg_point_t bottom_right;
} mg_attr_t;

typedef struct HPWL_PACK {
	mg_point_t point;
	mg_dim_t dim;
} mg_object_t;

typedef struct MCU_PACK {
	mg_point_t shift; //shift within screen (absolute)
	i16 rotation; //rotate within screen (absolute)
	mg_dim_t size; //scaling
	u8 thickness_fill; //execute fill items
	u8 op;
} mg_map_t;

typedef struct MCU_PACK {
	u16 total;
	u16 fill_total;
	const mg_t * elements;
} mg_icon_t;


enum {
	ANIMATION_TYPE_PUSH_LEFT,
	ANIMATION_TYPE_PUSH_RIGHT,
	ANIMATION_TYPE_PUSH_UP,
	ANIMATION_TYPE_PUSH_DOWN,
	ANIMATION_TYPE_SLIDE_LEFT,
	ANIMATION_TYPE_UNDO_SLIDE_LEFT,
	ANIMATION_TYPE_SLIDE_RIGHT,
	ANIMATION_TYPE_UNDO_SLIDE_RIGHT,
	ANIMATION_TYPE_SLIDE_UP,
	ANIMATION_TYPE_UNDO_SLIDE_UP,
	ANIMATION_TYPE_SLIDE_DOWN,
	ANIMATION_TYPE_UNDO_SLIDE_DOWN,
	ANIMATION_TYPE_NONE,
	ANIMATION_TYPE_BOUNCE_UP,
	ANIMATION_TYPE_BOUNCE_DOWN,
	ANIMATION_TYPE_BOUNCE_LEFT,
	ANIMATION_TYPE_BOUNCE_RIGHT,
	ANIMATION_TYPE_TOTAL
};


enum {
	ANIMATION_PATH_LINEAR,
	ANIMATION_PATH_SQUARED,
	ANIMATION_PATH_SQUARED_UNDO
};

typedef struct MCU_PACK {
	u8 type;
	u16 step;
	u16 step_total;
	u32 sum_of_squares;
	mg_size_t motion;
	mg_size_t motion_total /*! \brief Total amount of animation movement */;
} mg_animation_path_t;

typedef struct MCU_PACK {
	u8 type /*! \brief Animation type */;
	mg_point_t start /*! \brief Animation start point */;
	mg_dim_t dim /*! \brief Animate within these dimensions with start in the top left corner */;
	mg_animation_path_t path;
} mg_animation_t;


#endif /* MG_TYPES_H_ */
